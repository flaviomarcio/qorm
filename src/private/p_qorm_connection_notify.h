#pragma once

#include "./p_qorm_connection_notify_subscribe.h"
#include "../qorm_connection_notify.h"
#include <QMutex>
#include <QVariant>

namespace QOrm {
//!
//! \brief The ConnectionNotifyPvt class
//!
class Q_ORM_EXPORT ConnectionNotifyPvt:public QObject
{
    Q_OBJECT
public:
    ConnectionNotifySubscribe*notifySubscribe=nullptr;
    QStringList subscribeToNotification;
    ConnectionNotify*notify();
    QMutex mutexLock;

    //!
    //! \brief ConnectionNotifyPvt
    //! \param parent
    //!
    Q_INVOKABLE explicit ConnectionNotifyPvt(QObject*parent=nullptr);

    //!
    //!
    ~ConnectionNotifyPvt();

    //!
    //! \brief queueStop
    //! \return
    //!
    bool queueStop();

    //!
    //! \brief queueStart
    //! \return
    //!
    bool queueStart();

    //!
    //! \brief queueStarted
    //! \return
    //!
    bool queueStarted();

    //!
    //! \brief queueSend
    //! \param channel
    //! \param payload
    //! \return
    //!
    bool queueSend(const QString &channel, const QVariant &payload);

private slots:
    //!
    //! \brief threadFinish
    //! \param v
    //!
    void threadFinish(QOrm::ConnectionNotifySubscribe*v);
signals:
    void notification_send(const QString &channel, const QVariant &payload);
};

}

