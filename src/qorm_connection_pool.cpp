#include "./qorm_connection_pool.h"
#include "./private/p_qorm_sql_suitable_parser_keywork.h"
#include "./qorm_sql_suitable_types.h"
#include "./qorm_connection_setting.h"
#include "./qorm_macro.h"
#include "./qorm_const.h"
#include <QCoreApplication>
#include <QDebug>
#include <QMutex>
#include <QSettings>
#include <QThread>
#include <QSqlError>
#include <QSqlQuery>

namespace QOrm {

//static const auto driver_QMYSQL="QMYSQL";
static const auto driver_QODBC="QODBC";
//static const auto driver_QOIC="QOIC";
//static const auto driver_QPSQL="QPSQL";
static const auto driver_QSQLITE="QSQLITE";

static const auto __arg_1="%1";

static qlonglong connectionCount = 0;
Q_GLOBAL_STATIC(QStringList, static_dbcDrivers)

static void init()
{
    QSettings setting(QStringLiteral("/etc/odbcinst.ini"), QSettings::IniFormat);
    for (auto &line : setting.childGroups())
        static_dbcDrivers->append(line);
}

Q_COREAPP_STARTUP_FUNCTION(init);

class ConnectionPoolPvt:public QObject
{
public:
    QString baseName;
    ConnectionSetting connectionSetting;
    QStringList connectionList;
    QObject *parent = nullptr;

    QSqlError lastError;
    explicit ConnectionPoolPvt(QObject *parent, const ConnectionSetting &cnnSetting)
        : QObject{parent}, connectionSetting(cnnSetting, parent)
    {
        this->parent = parent;
        this->baseName
                = ((this->parent != nullptr) ? this->parent->objectName().trimmed() : "").left(50);
        if (!this->baseName.isEmpty())
            return;
        auto thread = QThread::currentThread();
        if (thread == nullptr)
            return;

        this->baseName = thread->objectName().trimmed().left(50);
        if (!this->baseName.isEmpty())
            return;
        auto threadId = QString::number(qlonglong(QThread::currentThreadId()), 16);
        this->baseName = QStringLiteral("pool%1").arg(threadId).left(50);
    }

    virtual ~ConnectionPoolPvt()
    {
        this->finish();
    }

    void finish()
    {
        for (auto &connectionName : this->connectionList){
            auto connection=QSqlDatabase::database(connectionName);
            finish(connection);
        }
    }

    bool finish(QSqlDatabase &connection)
    {
        if (!connection.isValid())
            return true;

        auto connectionName = connection.connectionName();
        this->connectionList.removeAll(connectionName);
        connection.close();
        connection = {};
        QSqlDatabase::removeDatabase(connectionName);
        return !connection.isValid();
    }

    virtual bool from(ConnectionPool &pool)
    {
        return connectionSetting.fromSetting(pool.setting()).isValid();
    }

    virtual bool from(const ConnectionSetting &setting)
    {
        return connectionSetting.fromSetting(setting).isValid();
    }

    virtual bool from(const QVariant &setting)
    {
        return connectionSetting.fromMap(setting.toHash()).isValid();
    }

    virtual bool from(const QSqlDatabase &db)
    {
        return connectionSetting.fromConnection(db).isValid();
    }

    static QString urlMaker(const QSqlDatabase &connection)
    {
        static const auto __formatDefault=QStringLiteral("connection: %1://%2");
        if(connection.driverName()==driver_QSQLITE)
            return __formatDefault.arg(connection.driverName(), connection.databaseName());

        if(connection.driverName()==driver_QODBC)
            return __formatDefault.arg(connection.driverName(), connection.connectOptions());

        if(connection.hostName().isEmpty() && !connection.connectOptions().isEmpty())
            return __formatDefault.arg(connection.driverName(), connection.connectOptions());

#ifdef QT_DEBUG
        static const auto __format=QStringLiteral("connection: %1://%2:%3/%4?user=%5&password=%6");
        return __format
                .arg(connection.driverName(),connection.hostName())
                .arg(connection.port())
                .arg(connection.databaseName(), connection.userName(), connection.password());

#else
        static const auto __format=QStringLiteral("connection: %1://%2:%3/%4?user=%5");
        return __format
                .arg(__connection.driverName(),__connection.hostName())
                .arg(__connection.port())
                .arg(__connection.databaseName(), __connection.userName());
#endif

    }

    bool get(QSqlDatabase &connection, bool readOnly)
    {
        Q_UNUSED(readOnly)
        this->finish(connection);
        this->lastError = {};
        auto driver = this->connectionSetting.driver().trimmed();

#if Q_ORM_LOG_VERBOSE
        oWarning() << QStringLiteral("avaliable drivers %1").arg(QSqlDatabase::drivers().join(QStringLiteral(",")));
#endif

        if (driver.isEmpty()) {
#if Q_ORM_LOG
            static const auto drivers = QSqlDatabase::drivers();
            auto msg = QStringLiteral("driver is empty, avaliable drivers, %1").arg(drivers.join(QStringLiteral(",")));
            this->lastError = QSqlError(QStringLiteral("NoDriver"), msg);
            oWarning() << msg;
#endif
            return {};
        }

        if (!QSqlDatabase::isDriverAvailable(driver)) {
#if Q_ORM_LOG
            auto msg = QStringLiteral("no avaliable driver: ") + driver;
            this->lastError = QSqlError(QStringLiteral("NoDriver"), msg);
            oWarning() << msg;
#endif
            return {};
        }

        auto connectionName = QStringLiteral("%1_%2").arg(this->baseName.left(55)).arg(++connectionCount);
        auto __connection = QSqlDatabase::addDatabase(driver, connectionName);
        if (!__connection.isValid()) {
#if Q_ORM_LOG
            auto msg = QStringLiteral("invalid QSqlDatabase connection settings");
            this->lastError = QSqlError(driver, msg);
            oWarning() << msg;
#endif
            this->finish(__connection);
            return {};
        }

        if (!__connection.lastError().text().isEmpty()) {
#if Q_ORM_LOG
            auto msg = QStringLiteral("QSqlDatabase error: ") + __connection.lastError().text();
            this->lastError = QSqlError(driver, msg);
            oWarning() << msg;
#endif
            this->finish(__connection);
            return {};
        }

        QString hostName = this->connectionSetting.hostName().trimmed();
        QString userName = this->connectionSetting.userName().trimmed();
        QString password = this->connectionSetting.password().trimmed();
        QString dataBaseName = this->connectionSetting.dataBaseName().trimmed();
        auto port = this->connectionSetting.port();
        QString connectOptions = this->connectionSetting.connectOptions().trimmed();
        auto schameNames = this->connectionSetting.schemaNames();

        if (__connection.driverName() == driver_QODBC) {
            QString odbcDriver;
            const auto &odbcDriverList = *static_dbcDrivers;
            if (odbcDriverList.isEmpty())
                odbcDriver.clear();
            else if (odbcDriverList.contains(QStringLiteral("freetds")))
                odbcDriver = QStringLiteral("freetds");
            else if (odbcDriverList.contains(QStringLiteral("ODBC Driver 17 for SQL Server")))
                odbcDriver = QStringLiteral("ODBC Driver 17 for SQL Server");
            else
                odbcDriver = odbcDriverList.first();

            if (odbcDriver.isEmpty()) {
                auto name = __connection.connectionName();
                __connection = {};
                QSqlDatabase::removeDatabase(name);
            }
            else {
                QStringList params={QStringLiteral("Driver=%1").arg(odbcDriver)};

                if (!hostName.isEmpty())
                    params.append(QStringLiteral("Server=%1").arg(hostName));
                if (!dataBaseName.isEmpty())
                    params.append(QStringLiteral("Database=%1").arg(dataBaseName));
                if (port > 0)
                    params.append(QStringLiteral("Port=%1").arg(port));
                if (!userName.isEmpty())
                    params.append(QStringLiteral("Uid=%1").arg(userName));
                if (!password.isEmpty())
                    params.append(QStringLiteral("Pwd=%1").arg(password));

                if (readOnly)
                    params.append(QStringLiteral("ApplicationIntent=ReadOnly"));
                params.append(QStringLiteral("Application Name=%1").arg(qAppName()));
                auto __db = params.join(QStringLiteral(";"));
                __connection.setDatabaseName(__db);
            }

        }
        else {
            __connection.setHostName(hostName);
            __connection.setUserName(userName);
            __connection.setPassword(password);
            __connection.setDatabaseName(dataBaseName);
            __connection.setPort(port);
            __connection.setConnectOptions(connectOptions);
        }


        if (!__connection.isValid()) {
            this->lastError = __connection.lastError();
            this->finish(__connection);
#if Q_ORM_LOG
            static const auto __format=QStringLiteral("%1, invalid connection");
            QString url=this->urlMaker(__connection);
            oWarning() << __format.arg(url);
#endif
            return {};
        }

        if (!__connection.open()) {
            this->lastError = __connection.lastError();
            this->finish(__connection);
#if Q_ORM_LOG
            static const auto __format=QStringLiteral("%1, error=%2");
            QString url=this->urlMaker(__connection);
            oWarning() << __format.arg(url, __connection.lastError().text());
#endif
            return {};
        }
#if Q_ORM_LOG_VERBOSE
        else{
            static const auto __format=QString("%1, successful");
            QString url=this->urlMaker(__connection);
            oWarning() << __format.arg(url);
        }
#endif

        auto &keyWord = SqlSuitableKeyWord::parser(__connection);
        if (!keyWord.isValid())
            return {};

        QStringList listCmd;
        QString cmd;

        //connectionName
        cmd = keyWord.parserCommand(kgcSetApplicationName);
        if (cmd.contains(__arg_1))
            listCmd.append(cmd.arg(this->baseName));

        if (!schameNames.isEmpty()) {
            cmd = keyWord.parserCommand(kgcSetSearchPath);
            if (cmd.contains(__arg_1))
                listCmd.append(cmd.arg(schameNames.join(QStringLiteral(","))));
        }

        {
            //transaction readonly
            auto transaction = readOnly ? kgcSetTransactionReadOnlyOn
                                        : kgcSetTransactionReadOnlyOff;
            auto cmd = keyWord.parserCommand(transaction).trimmed();
            if (!cmd.isEmpty())
                listCmd.append(cmd);
        }

        if (!listCmd.isEmpty()) {
            cmd = listCmd.join(QStringLiteral(";"));
            QSqlQuery query{__connection};
            if (!query.exec(cmd)) {
                this->lastError = __connection.lastError();
                oWarning() << QStringLiteral("%1:%2 error:%3")
                              .arg(__connection.driverName(),
                                   __connection.connectionName(),
                                   query.lastError().text());
            }
            query.clear();
            query.finish();
        }

        connection = __connection;
        this->connectionList.append(__connection.connectionName());
        return connection.isValid() && connection.isOpen();
    }
};

ConnectionPool::ConnectionPool(QObject *parent):QObject{parent}
{
    ConnectionSetting cnnSetting;
    this->p = new ConnectionPoolPvt{parent, cnnSetting};
}

ConnectionPool::ConnectionPool(const ConnectionSetting &connectionSetting, QObject *parent):QObject{parent}
{
    this->p = new ConnectionPoolPvt{parent, connectionSetting};
}

ConnectionPool::ConnectionPool(const QVariant &connection, QObject *parent):QObject{parent}
{
    QOrm::ConnectionSetting connectionSetting(QByteArray(), connection.toHash(), nullptr);
    this->p = new ConnectionPoolPvt{parent, connectionSetting};
}

ConnectionPool::ConnectionPool(const ConnectionPool &pool, QObject *parent):QObject{parent}
{
    this->p = new ConnectionPoolPvt{parent, pool.setting()};
}

ConnectionSetting &ConnectionPool::setting() const
{
    return p->connectionSetting;
}

bool ConnectionPool::isValid()
{
    return p->connectionSetting.isValid();
}

bool ConnectionPool::from(ConnectionPool &pool)
{
    return p->from(pool);
}

bool ConnectionPool::from(const ConnectionSetting &setting)
{
    return p->from(setting);
}

bool ConnectionPool::from(const QVariant &connection)
{
    return p->from(connection);
}

bool ConnectionPool::from(const QSqlDatabase &connection)
{
    return p->from(connection);
}

QSqlDatabase ConnectionPool::get()
{
    QSqlDatabase connection;
    if (p->get(connection, false))
        return connection;
    return {};
}

bool ConnectionPool::get(QSqlDatabase &connection)
{
    if (p->get(connection, false))
        return connection.isValid() && connection.isOpen();
    return {};
}

QSqlDatabase ConnectionPool::getReadOnly()
{
    QSqlDatabase connection;
    if (p->get(connection, true))
        return connection;
    return {};
}

bool ConnectionPool::getReadOnly(QSqlDatabase &connection)
{
    if (p->get(connection, true))
        return connection.isValid() && connection.isOpen();
    return {};
}

ConnectionPool &ConnectionPool::finish()
{
    p->finish();
    return *this;
}

bool ConnectionPool::finish(QSqlDatabase &connection)
{
    if (!connection.isValid())
        return true;


    connection.close();
    auto connectionName = connection.connectionName();
    p->connectionList.removeAll(connectionName);
    connection = {};
    if (!connectionName.isEmpty())
        QSqlDatabase::removeDatabase(connectionName);

    return !connection.isValid();
}

bool ConnectionPool::finish(QString connection)
{
    auto connectionName = QSqlDatabase::database(connection);
    return this->finish(connectionName);
}

QSqlError &ConnectionPool::lastError()
{
    return p->lastError;
}

} // namespace QOrm
