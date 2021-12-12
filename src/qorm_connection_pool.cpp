#include <QCoreApplication>
#include <QSqlQuery>
#include <QSettings>
#include <QThread>
#include <QMutex>
#include <QDebug>
#include <QSqlError>
#include <QMutex>
#include "./qorm_connection_pool.h"
#include "./qorm_connection_setting.h"
#include "./private/p_qorm_sql_suitable_parser_keywork.h"
#include "./qorm_macro.h"

namespace QOrm {

#define dPvt()\
    auto&p = *reinterpret_cast<ConnectionPoolPvt*>(this->p)

static qlonglong connectionCount=0;

static const QStringList&initOdbc(){
    static QStringList odbcDrivers;
    static bool ___initOdbc=false;
    if(!___initOdbc){
        static QMutex ___mutexOdbc;
        QMutexLocker locker(&___mutexOdbc);
        if(!___initOdbc){
            ___initOdbc=true;
            QSettings setting(qsl("/etc/odbcinst.ini"), QSettings::IniFormat);
            odbcDrivers=setting.childGroups();
        }
    }
    return odbcDrivers;
}

class ConnectionPoolPvt{
public:
    QMutex locker;
    QString baseName;
    ConnectionSetting connectionSetting;
    QStringList connectionList;
    QObject*parent=nullptr;
    QStringList odbcDrivers;
    QSqlError lastError;
    explicit ConnectionPoolPvt(QObject*parent, const ConnectionSetting&cnnSetting) : connectionSetting(cnnSetting, parent){
        this->odbcDrivers=initOdbc();
        this->parent=parent;
        this->baseName=(this->parent!=nullptr)?this->parent->objectName().trimmed():"";
        if(!this->baseName.isEmpty())
            return;
        auto thread=QThread::currentThread();
        if(thread==nullptr)
            return;

        this->baseName=thread->objectName().trimmed();
        if(!this->baseName.isEmpty())
            return;
        auto threadId=QString::number(qlonglong(QThread::currentThreadId()), 16);
        this->baseName=qsl("pool%1").arg(threadId);

    }
    virtual ~ConnectionPoolPvt(){
    }

    bool finish(QSqlDatabase&connection)
    {
        auto&p=*this;
        if(connection.isValid()){
            QMutexLocker locker(&p.locker);
            auto connectionName=connection.connectionName();
            p.connectionList.removeAll(connectionName);
            connection.close();
            connection=QSqlDatabase();
            QSqlDatabase::removeDatabase(connectionName);
        }
        return !connection.isValid();
    }

    virtual bool from(ConnectionPool&pool){
        return connectionSetting.fromSetting(pool.setting()).isValid();
    }

    virtual bool from(const ConnectionSetting &setting){
        return connectionSetting.fromSetting(setting).isValid();
    }

    virtual bool from(const QVariant &setting){
        return connectionSetting.fromMap(setting.toHash()).isValid();
    }

    virtual bool from(const QSqlDatabase &db){
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
            sWarning()<<"avaliable drivers "<<QSqlDatabase::drivers().join(qsl(","));
#endif

        if(driver.isEmpty()){
#if Q_ORM_LOG
            auto msg=qsl("driver is empty, avaliable drivers, ")+QSqlDatabase::drivers().join(qsl(","));
            this->lastError=QSqlError(qsl("NoDriver"),msg);
            sWarning()<<msg;
#endif
            return false;
        }

        if(!QSqlDatabase::isDriverAvailable(driver)){
#if Q_ORM_LOG
            auto msg=qsl("no avaliable driver: ")+driver;
            this->lastError=QSqlError(qsl("NoDriver"), msg);
            sWarning()<<msg;
#endif
            return false;
        }

        auto connectionName=qsl("%1_%2").arg(p.baseName).arg(++connectionCount);
        auto __connection=QSqlDatabase::addDatabase(driver, connectionName);
        if(!__connection.isValid()){
#if Q_ORM_LOG
            auto msg=qsl("invalid QSqlDatabase connection settings");
            this->lastError=QSqlError(driver, msg);
            sWarning()<<msg;
#endif
            this->finish(__connection);
            return false;
        }

        if(!__connection.lastError().text().isEmpty()){
#if Q_ORM_LOG
            auto msg=qsl("QSqlDatabase error: ") + __connection.lastError().text();
            this->lastError=QSqlError(driver, msg);
            sWarning()<<msg;
#endif
            this->finish(__connection);
            return false;
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
            if(p.odbcDrivers.isEmpty())
                odbcDriver.clear();
            else if(p.odbcDrivers.contains(qsl("freetds")))
                odbcDriver=qsl("freetds");
            else if(p.odbcDrivers.contains(qsl("ODBC Driver 17 for SQL Server")))
                odbcDriver=qsl("ODBC Driver 17 for SQL Server");
            else
                odbcDriver=p.odbcDrivers.first();

            if(odbcDriver.isEmpty()){
                auto name=__connection.connectionName();
                __connection=QSqlDatabase();
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
            return false;
        }
        else if(!__connection.open()){
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
            return false;
        }

#if Q_ORM_LOG_VERBOSE
        sWarning()<<tr("%1:%2 is connected").arg(db.driverName(), db.connectionName());
#endif
        auto&keyWord=SqlSuitableKeyWord::parser(__connection);
        if(!keyWord.isValid())
            return false;

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
        locker.lock();
        p.connectionList<<__connection.connectionName();
        locker.unlock();

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
    return QSqlDatabase();
}

bool ConnectionPool::get(QSqlDatabase &connection)
{
    dPvt();
    if(p.get(connection, false))
        return connection.isValid() && connection.isOpen();
    return false;
}

QSqlDatabase ConnectionPool::getReadOnly()
{
    dPvt();
    QSqlDatabase connection;
    if(p.get(connection, true))
        return connection;
    return QSqlDatabase();
}

bool ConnectionPool::getReadOnly(QSqlDatabase &connection)
{
    dPvt();
    if(p.get(connection, true))
        return connection.isValid() && connection.isOpen();
    return false;
}

void ConnectionPool::finish()
{
    dPvt();
    for(auto&connection:p.connectionList)
        this->finish(connection);
}

bool ConnectionPool::finish(QSqlDatabase&connection)
{
    if(connection.isValid()){
        dPvt();
        QMutexLocker locker(&p.locker);
        auto connectionName=connection.connectionName();
        p.connectionList.removeAll(connectionName);
        connection.close();
        connection=QSqlDatabase();
        if(!connectionName.isEmpty())
            QSqlDatabase::removeDatabase(connectionName);
    }
    return !connection.isValid();
}

bool ConnectionPool::finish(QString connection)
{
    auto _connection=QSqlDatabase::database(connection);
    return this->finish(_connection);

}

QSqlError &ConnectionPool::lastError()
{
    dPvt();
    return p.lastError;

}
//set default_transaction_read_only = on;
}
