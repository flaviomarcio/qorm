#pragma once

#include "./qorm_object_db.h"

namespace QOrm {

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

    //!
    //! \brief ~ConnectionNotify
    //!
    Q_INVOKABLE ~ConnectionNotify();
public:
    //!
    //! \brief notify_send
    //! \param payload
    //! \return
    //!
    Q_INVOKABLE virtual bool notify_send(const QVariant &payload);

    //!
    //! \brief notify_send
    //! \param channel
    //! \param payload
    //! \return
    //!
    Q_INVOKABLE virtual bool notify_send(const QString &channel, const QVariant &payload);

    //!
    //! \brief subscribeToNotification
    //! \return
    //!
    Q_INVOKABLE virtual QStringList &subscribeToNotification();

    //!
    //! \brief start
    //! \return
    //!
    Q_INVOKABLE virtual bool start();

    //!
    //! \brief stop
    //! \return
    //!
    Q_INVOKABLE virtual bool stop();

    //!
    //! \brief started
    //! \return
    //!
    Q_INVOKABLE virtual bool started();

    //!
    //! \brief setConnection
    //! \param connection
    //! \return
    //!
    Q_INVOKABLE virtual bool setConnection(const QSqlDatabase &connection);
signals:
    //!
    //! \brief notification
    //! \param channel
    //! \param payload
    //!
    void notification(const QString &channel, const QVariant &payload);
private:
    void*p=nullptr;
};

}
