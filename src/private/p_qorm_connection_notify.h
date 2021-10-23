#pragma once

#include "./p_qorm_connection_notify_subscribe.h"
#include "../qorm_connection_notify.h"
#include <QMutex>
#include <QVariant>

namespace QOrm {

#define dPvt()\
    auto&p = *reinterpret_cast<ConnectionNotifyPvt*>(this->p)

class Q_ORM_EXPORT ConnectionNotifyPvt:public QObject{
    Q_OBJECT
public:
    ConnectionNotifySubscribe*notifySubscribe=nullptr;
    QStringList subscribeToNotification;
    ConnectionNotify*notify(){
        return dynamic_cast<ConnectionNotify*>(this->parent());
    }

    QMutex mutexLock;

    Q_INVOKABLE explicit ConnectionNotifyPvt(QObject*parent=nullptr):QObject(parent){
    }

    Q_INVOKABLE virtual ~ConnectionNotifyPvt(){
        this->queueStop();
    }

    bool queueStop(){
        QMutexLocker locker(&this->mutexLock);
        if(this->notifySubscribe!=nullptr){
            QObject::disconnect(this->notifySubscribe, &ConnectionNotifySubscribe::threadFinish, this, &ConnectionNotifyPvt::threadFinish);
            QObject::disconnect(this, &ConnectionNotifyPvt::notification_send, this->notifySubscribe, &ConnectionNotifySubscribe::notification_send);
            QObject::disconnect(this->notify(), &ConnectionNotify::notification, this->notifySubscribe, &ConnectionNotifySubscribe::notification);
            this->notifySubscribe->quit();
            if(this->notifySubscribe->wait(1000))
                delete this->notifySubscribe;
            else
                this->notifySubscribe->deleteLater();
            this->notifySubscribe=nullptr;
        }
        return (this->notifySubscribe==nullptr);
    }

    bool queueStart(){
//        QMutexLocker locker(&this->mutexLock);
//        if(this->notifySubscribe==nullptr){
//            auto notify=this->notify();
//            this->notifySubscribe=new ConnectionNotifySubscribe(notify);
//            QObject::connect(this->notifySubscribe, &ConnectionNotifySubscribe::threadFinish, this, &ConnectionNotifyPvt::threadFinish);
//            QObject::connect(this, &ConnectionNotifyPvt::notification_send, this->notifySubscribe, &ConnectionNotifySubscribe::notification_send);
//            QObject::connect(this->notify(), &ConnectionNotify::notification, this->notifySubscribe, &ConnectionNotifySubscribe::notification);
//            ObjectDb objectdb(this);
//            this->notifySubscribe->start(objectdb.connection());
//        }
        return (this->notifySubscribe!=nullptr);
    }

    bool queueStarted(){
        return (this->notifySubscribe!=nullptr);
    }

    bool queueSend(const QString &channel, const QVariant &payload){
        if(this->notifySubscribe!=nullptr){
            emit notification_send(channel, payload);
        }
        return true;
    }

private slots:
    void threadFinish(QOrm::ConnectionNotifySubscribe*v){
        Q_UNUSED(v)
        this->queueStop();
    }
signals:
    void notification_send(const QString &channel, const QVariant &payload);
};

} // namespace QOrm

