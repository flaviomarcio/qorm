#pragma once

#include "./qorm_global.h"
#include <QSqlDatabase>

namespace QOrm {
class ConnectionNotifyPvt;
//!
//! \brief The ConnectionNotify class
//!
class Q_ORM_EXPORT ConnectionNotify : public QObject
{
    Q_OBJECT
public:
    //!
    //! \brief ConnectionNotify
    //! \param parent
    //!
    Q_INVOKABLE explicit ConnectionNotify(QObject *parent = nullptr);

public:
    //!
    //! \brief notify_send
    //! \param payload
    //! \return
    //!
    virtual bool notify_send(const QVariant &payload);

    //!
    //! \brief notify_send
    //! \param channel
    //! \param payload
    //! \return
    //!
    virtual bool notify_send(const QString &channel, const QVariant &payload);

    //!
    //! \brief subscribeToNotification
    //! \return
    //!
    virtual QStringList &subscribeToNotification();

    //!
    //! \brief start
    //! \return
    //!
    virtual bool start();

    //!
    //! \brief stop
    //! \return
    //!
    virtual bool stop();

    //!
    //! \brief started
    //! \return
    //!
    virtual bool started();

    //!
    //! \brief setConnection
    //! \param connection
    //! \return
    //!
    virtual bool setConnection(const QSqlDatabase &connection);
signals:
    //!
    //! \brief notification
    //! \param channel
    //! \param payload
    //!
    void notification(const QString &channel, const QVariant &payload);

private:
    ConnectionNotifyPvt *p = nullptr;
};

} // namespace QOrm
