#include <QCoreApplication>
#include <QSettings>
#include <QThread>
#include <QMutex>
#include <QDebug>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QMutex>
#include "./qorm_connection_pool.h"
#include "./qorm_connection_setting.h"
#include "./qorm_macro.h"
#include "./private/p_qorm_sql_suitable_parser_keywork.h"

namespace QOrm {

#define dPvt()\
    auto&p = *reinterpret_cast<ConnectionPoolPvt*>(this->p)

static qlonglong connectionCount=0;
Q_GLOBAL_STATIC(QStringList, static_dbcDrivers)

static void init()
{
    QSettings setting(qsl("/etc/odbcinst.ini"), QSettings::IniFormat);
    for(auto&line:setting.childGroups())
        static_dbcDrivers->append(line);
}

Q_COREAPP_STARTUP_FUNCTION(init)

class ConnectionPoolPvt{
public:
    QString baseName;
    ConnectionSetting connectionSetting;
    QStringList connectionList;
    QObject*parent=nullptr;

    QSqlError lastError;
    explicit ConnectionPoolPvt(QObject*parent, const ConnectionSetting&cnnSetting) : connectionSetting(cnnSetting, parent)
    {
        this->parent=parent;
        this->baseName=((this->parent!=nullptr)?this->parent->objectName().trimmed():qsl_null).left(50);
        if(!this->baseName.isEmpty())
            return;
        auto thread=QThread::currentThread();
        if(thread==nullptr)
            return;

        this->baseName=thread->objectName().trimmed().left(50);
        if(!this->baseName.isEmpty())
            return;
        auto threadId=QString::number(qlonglong(QThread::currentThreadId()), 16);
        this->baseName=qsl("pool%1").arg(threadId).left(50);

    }
    virtual ~ConnectionPoolPvt()
    {
    }

    bool finish(QSqlDatabase&connection)
    {
        auto&p=*this;
        if(!connection.isValid())
            return true;

        auto connectionName=connection.connectionName();
        p.connectionList.removeAll(connectionName);
        connection.close();
        connection={};
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

    bool get(QSqlDatabase &connection, bool readOnly)
    {
        Q_UNUSED(readOnly)
        auto&p=*this;
        p.finish(connection);
        p.lastError=QSqlError();
        auto driver=p.connectionSetting.driver().trimmed();

#if Q_ORM_LOG_VERBOSE
        sWarning()<<qsl("avaliable drivers %1").arg(QSqlDatabase::drivers().join(qsl(",")));
#endif

        if(driver.isEmpty()){
#if Q_ORM_LOG
            static const auto drivers=QSqlDatabase::drivers();
            auto msg=qsl("driver is empty, avaliable drivers, %1").arg(drivers.join(qsl(",")));
            this->lastError=QSqlError(qsl("NoDriver"), msg);
            sWarning()<<msg;
#endif
            return {};
        }

        if(!QSqlDatabase::isDriverAvailable(driver)){
#if Q_ORM_LOG
            auto msg=qsl("no avaliable driver: ")+driver;
            this->lastError=QSqlError(qsl("NoDriver"), msg);
            sWarning()<<msg;
#endif
            return {};
        }

        auto connectionName=qsl("%1_%2").arg(p.baseName.left(55)).arg(++connectionCount);
        auto __connection=QSqlDatabase::addDatabase(driver, connectionName);
        if(!__connection.isValid()){
#if Q_ORM_LOG
            auto msg=qsl("invalid QSqlDatabase connection settings");
            this->lastError=QSqlError(driver, msg);
            sWarning()<<msg;
#endif
            this->finish(__connection);
            return {};
        }

        if(!__connection.lastError().text().isEmpty()){
#if Q_ORM_LOG
            auto msg=qsl("QSqlDatabase error: ") + __connection.lastError().text();
            this->lastError=QSqlError(driver, msg);
            sWarning()<<msg;
#endif
            this->finish(__connection);
            return {};
        }

        QString hostName=p.connectionSetting.hostName().trimmed();
        QString userName=p.connectionSetting.userName().trimmed();
        QString password=p.connectionSetting.password().trimmed();
        QString dataBaseName=p.connectionSetting.dataBaseName().trimmed();
        auto port=p.connectionSetting.port();
        QString connectOptions=p.connectionSetting.connectOptions().trimmed();
        auto schameNames=p.connectionSetting.schemaNames();

        if(__connection.driverName()==qsl("QODBC")){
            QString odbcDriver;
            const auto&odbcDriverList=*static_dbcDrivers;
            if(odbcDriverList.isEmpty())
                odbcDriver.clear();
            else if(odbcDriverList.contains(qsl("freetds")))
                odbcDriver=qsl("freetds");
            else if(odbcDriverList.contains(qsl("ODBC Driver 17 for SQL Server")))
                odbcDriver=qsl("ODBC Driver 17 for SQL Server");
            else
                odbcDriver=odbcDriverList.first();

            if(odbcDriver.isEmpty()){
                auto name=__connection.connectionName();
                __connection={};
                QSqlDatabase::removeDatabase(name);
            }
            else{
                QStringList params;
                params<<qsl("Driver=%1").arg(odbcDriver);
                if(!hostName.isEmpty())
                    params<<qsl("Server=%1").arg(hostName);
                if(!dataBaseName.isEmpty())
                    params<<qsl("Database=%1").arg(dataBaseName);
                if(port>0)
                    params<<qsl("Port=%1").arg(port);
                if(!userName.isEmpty())
                    params<<qsl("Uid=%1").arg(userName);
                if(!password.isEmpty())
                    params<<qsl("Pwd=%1").arg(password);

                if(readOnly)
                    params<<qsl("ApplicationIntent=ReadOnly");
                params<<qsl("Application Name=%1").arg(qAppName());
                auto __db = params.join(qsl(";"));
                __connection.setDatabaseName(__db);
            }

        }
        else{
            __connection.setHostName(hostName);
            __connection.setUserName(userName);
            __connection.setPassword(password);
            __connection.setDatabaseName(dataBaseName);
            __connection.setPort(port);
            __connection.setConnectOptions(connectOptions);
        }

        if(!__connection.isValid()){
#if Q_ORM_LOG
            this->lastError=__connection.lastError();
            sWarning()<<__connection.lastError().text();
#endif
            this->finish(__connection);
            return {};
        }

        if(!__connection.open()){
#if Q_ORM_LOG
            this->lastError=__connection.lastError();
            sWarning()<<__connection.lastError().text();
            if(__connection.driverName()==qsl("QODBC")){
#ifndef QT_DEBUG
                __connection.setPassword(qsl_null);
#endif
                sWarning()<<__connection.databaseName();
            }
#endif
            this->finish(__connection);
            return {};
        }

#if Q_ORM_LOG_VERBOSE
        sWarning()<<tr("%1:%2 is connected").arg(db.driverName(), db.connectionName());
#endif
        auto&keyWord=SqlSuitableKeyWord::parser(__connection);
        if(!keyWord.isValid())
            return {};

        QStringList listCmd;
        QString cmd;

        //connectionName
        cmd=keyWord.parserCommand(kgcSetApplicationName);
        if(cmd.contains(qsl("%1")))
            listCmd<<cmd.arg(p.baseName);

        if(!schameNames.isEmpty()){
            cmd=keyWord.parserCommand(kgcSetSearchPath);
            if(cmd.contains(qsl("%1")))
                listCmd<<cmd.arg(schameNames.join(qsl(",")));
        }

        {
            //transaction readonly
            auto transaction=readOnly?kgcSetTransactionReadOnlyOn:kgcSetTransactionReadOnlyOff;
            auto cmd=keyWord.parserCommand(transaction).trimmed();
            if(!cmd.isEmpty())
                listCmd<<cmd;
        }


        if(!listCmd.isEmpty()){
            cmd=listCmd.join(qsl(";"));
            QSqlQuery query(__connection);
            if(!query.exec(cmd)){
                this->lastError=__connection.lastError();
                sWarning()<<qsl("%1:%2 error:%3").arg(__connection.driverName(), __connection.connectionName(), query.lastError().text());
            }
            query.clear();
            query.finish();
        }

        connection=__connection;
        p.connectionList<<__connection.connectionName();
        return connection.isValid() && connection.isOpen();
    }

};

ConnectionPool::ConnectionPool(QObject *parent)
{
    ConnectionSetting cnnSetting;
    this->p = new ConnectionPoolPvt(parent, cnnSetting);
}

ConnectionPool::ConnectionPool(const ConnectionSetting&connectionSetting, QObject *parent)
{
    this->p = new ConnectionPoolPvt(parent, connectionSetting);
}

ConnectionPool::ConnectionPool(const QVariant &connection, QObject *parent)
{
    QOrm::ConnectionSetting connectionSetting(QByteArray(), connection.toHash(), nullptr);
    this->p = new ConnectionPoolPvt(parent, connectionSetting);
}

ConnectionPool::ConnectionPool(const ConnectionPool &pool, QObject *parent)
{
    this->p = new ConnectionPoolPvt(parent, pool.setting());
}

ConnectionPool::~ConnectionPool()
{
    this->finish();
    dPvt();delete&p;
}

ConnectionSetting &ConnectionPool::setting() const
{
    dPvt();
    return p.connectionSetting;
}

bool ConnectionPool::isValid()
{
    dPvt();
    return p.connectionSetting.isValid();
}

bool ConnectionPool::from(ConnectionPool &pool)
{
    dPvt();
    return p.from(pool);
}

bool ConnectionPool::from(const ConnectionSetting &setting)
{
    dPvt();
    return p.from(setting);
}

bool ConnectionPool::from(const QVariant &connection)
{
    dPvt();
    return p.from(connection);
}

bool ConnectionPool::from(const QSqlDatabase &connection)
{
    dPvt();
    return p.from(connection);
}


QSqlDatabase ConnectionPool::get()
{
    dPvt();
    QSqlDatabase connection;
    if(p.get(connection, false))
        return connection;
    return {};
}

bool ConnectionPool::get(QSqlDatabase &connection)
{
    dPvt();
    if(p.get(connection, false))
        return connection.isValid() && connection.isOpen();
    return {};
}

QSqlDatabase ConnectionPool::getReadOnly()
{
    dPvt();
    QSqlDatabase connection;
    if(p.get(connection, true))
        return connection;
    return {};
}

bool ConnectionPool::getReadOnly(QSqlDatabase &connection)
{
    dPvt();
    if(p.get(connection, true))
        return connection.isValid() && connection.isOpen();
    return {};
}

ConnectionPool &ConnectionPool::finish()
{
    dPvt();
    for(auto&connection:p.connectionList)
        this->finish(connection);
    return *this;
}

bool ConnectionPool::finish(QSqlDatabase&connection)
{
    if(!connection.isValid())
        return true;

    dPvt();
    connection.close();
    auto connectionName=connection.connectionName();
    p.connectionList.removeAll(connectionName);
    connection={};
    if(!connectionName.isEmpty())
        QSqlDatabase::removeDatabase(connectionName);

    return !connection.isValid();
}

bool ConnectionPool::finish(QString connection)
{
    auto connectionName=QSqlDatabase::database(connection);
    return this->finish(connectionName);
}

QSqlError &ConnectionPool::lastError()
{
    dPvt();
    return p.lastError;

}

}
