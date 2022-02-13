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
    ConnectionNotify*notify();

    QMutex mutexLock;

    Q_INVOKABLE explicit ConnectionNotifyPvt(QObject*parent=nullptr);

    ~ConnectionNotifyPvt();

    bool queueStop();

    bool queueStart();

    bool queueStarted();

    bool queueSend(const QString &channel, const QVariant &payload);

private slots:
    void threadFinish(QOrm::ConnectionNotifySubscribe*v);
signals:
    void notification_send(const QString &channel, const QVariant &payload);
};

}

