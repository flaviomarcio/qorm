#pragma once

#include "../qorm_connection_manager.h"
#include "../qorm_connection_notify.h"
#include "../qorm_connection_pool.h"
#include <QTimer>
#include <QThread>
#include <QObject>
#include <QVariant>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlDriver>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

namespace QOrm {

//!
//! \brief listDbmsType
//!
static const auto listDbmsType=QList<QSqlDriver::DbmsType>{QSqlDriver::PostgreSQL};

//!
//! \brief The ConnectionNotifySubscribe class
//!
class Q_ORM_EXPORT ConnectionNotifySubscribe : public QThread
{
    Q_OBJECT
public:
    QSqlDatabase _localDb;
    QSqlDriver*connectedDriver=nullptr;
    ConnectionPool pool;
    ConnectionNotify*notify=nullptr;
    QStringList subscribeToNotification;
    explicit ConnectionNotifySubscribe(ConnectionNotify*parent = nullptr);
    ~ConnectionNotifySubscribe();
public:
    //!
    //! \brief run
    //!
    void run() override;

    //!
    //! \brief start
    //! \param pool
    //! \return
    //!
    virtual bool start(const ConnectionPool &pool);

    //!
    //! \brief start
    //! \param connection
    //! \return
    //!
    virtual bool start(const QSqlDatabase&connection);

    //!
    //! \brief dbmsType
    //! \return
    //!
    QSqlDriver::DbmsType dbmsType()const;

    //!
    //! \brief queueCloseConnection
    //!
    void queueCloseConnection();

    //!
    //! \brief queueCheckConnection
    //! \return
    //!
    bool queueCheckConnection();

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

private:
    void*p=nullptr;
public slots:
    //!
    //! \brief onNotificationReceive
    //! \param channel
    //! \param payload
    //!
    void onNotificationReceive(const QString &channel, QSqlDriver::NotificationSource, const QVariant &payload);

    //!
    //! \brief onNotificationSend
    //! \param name
    //! \param payload
    //!
    void onNotificationSend(const QString &name, const QVariant &payload);
signals:
    void threadFinish(QOrm::ConnectionNotifySubscribe*thread);
    void notification(const QString &name, const QVariant &payload);
};

}
