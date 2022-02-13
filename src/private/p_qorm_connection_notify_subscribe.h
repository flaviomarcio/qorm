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

static const auto listDbmsType=QList<QSqlDriver::DbmsType>()<<QSqlDriver::PostgreSQL;

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

    void run() override;

    virtual bool start(const ConnectionPool&pool);

    virtual bool start(const QSqlDatabase&connection);

    QSqlDriver::DbmsType dbmsType()const;

    void queueCloseConnection();

    bool queueCheckConnection();

    bool queueStop();

    bool queueStart();

    bool queueStarted();

    bool queueSend(const QString &channel, const QVariant &payload);

public slots:
    void onNotificationReceive(const QString &channel, QSqlDriver::NotificationSource, const QVariant &payload);

    void onNotificationSend(const QString &name, const QVariant &payload);
signals:
    void threadFinish(QOrm::ConnectionNotifySubscribe*thread);
    void notification(const QString &name, const QVariant &payload);
private:
    void*p=nullptr;
};

}
