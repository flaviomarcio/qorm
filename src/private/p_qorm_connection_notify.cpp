#include "./p_qorm_connection_notify.h"

namespace QOrm {

ConnectionNotify *ConnectionNotifyPvt::notify()
{
    return dynamic_cast<ConnectionNotify*>(this->parent());
}

ConnectionNotifyPvt::ConnectionNotifyPvt(QObject *parent):QObject(parent)
{
}

ConnectionNotifyPvt::~ConnectionNotifyPvt()
{
    this->queueStop();
}

bool ConnectionNotifyPvt::queueStop()
{
    QMutexLocker locker(&this->mutexLock);
    if(this->notifySubscribe!=nullptr){
        QObject::disconnect(this->notifySubscribe, &ConnectionNotifySubscribe::threadFinish, this, &ConnectionNotifyPvt::threadFinish);
        QObject::disconnect(this, &ConnectionNotifyPvt::notification_send, this->notifySubscribe, &ConnectionNotifySubscribe::onNotificationSend);
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

bool ConnectionNotifyPvt::queueStart()
{
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

bool ConnectionNotifyPvt::queueStarted()
{
    return (this->notifySubscribe!=nullptr);
}

bool ConnectionNotifyPvt::queueSend(const QString &channel, const QVariant &payload)
{
    if(this->notifySubscribe!=nullptr){
        emit notification_send(channel, payload);
    }
    return true;
}

void ConnectionNotifyPvt::threadFinish(ConnectionNotifySubscribe *v)
{
    Q_UNUSED(v)
    this->queueStop();
}

}
