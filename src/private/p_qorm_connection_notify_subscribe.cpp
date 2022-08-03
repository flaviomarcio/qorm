#include "./p_qorm_connection_notify_subscribe.h"
#include "../qorm_macro.h"
#include "../qorm_connection_manager.h"
#include "../../../qstm/src/qstm_util_variant.h"
#include <QSqlError>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QSqlQuery>


namespace QOrm {

typedef QList<QSqlDriver::DbmsType> DbmsTypeList;
Q_GLOBAL_STATIC_WITH_ARGS(DbmsTypeList, listDbmsType, ({QSqlDriver::PostgreSQL}))

ConnectionNotifySubscribe::ConnectionNotifySubscribe(ConnectionNotify *parent):QThread{nullptr}, notify(parent), subscribeToNotification(parent->subscribeToNotification())
{
    this->moveToThread(this);
}

ConnectionNotifySubscribe::~ConnectionNotifySubscribe()
{
}

void ConnectionNotifySubscribe::run()
{
    auto connect=[this](){
        if(!this->queueStart())
            this->quit();
    };
    QTimer::singleShot(10, this, connect);
    this->exec();
    this->queueCloseConnection();
}

bool ConnectionNotifySubscribe::start(const ConnectionPool &pool)
{
    if(!this->isRunning()){
        this->pool.from(pool.setting());
        QThread::start();
        while(this->eventDispatcher()!=nullptr)
            QThread::msleep(1);
    }
    return this->isRunning();
}

bool ConnectionNotifySubscribe::start(const QSqlDatabase &connection)
{
    if(!this->isRunning()){
        this->pool.from(connection);
        QThread::start();
        while(this->eventDispatcher()!=nullptr)
            QThread::msleep(1);
    }
    return this->isRunning();
}

QSqlDriver::DbmsType ConnectionNotifySubscribe::dbmsType() const
{
    return this->connectedDriver==nullptr?QSqlDriver::UnknownDbms:this->connectedDriver->dbmsType();
}

void ConnectionNotifySubscribe::queueCloseConnection()
{
    this->pool.finish(this->_localDb);
    this->_localDb=QSqlDatabase();
    this->connectedDriver=nullptr;
}

bool ConnectionNotifySubscribe::queueCheckConnection()
{
    QSqlDatabase db;
    if(!this->pool.get(db))
        return false;

    if(db.isValid() && db.isOpen())
        return false;

    auto dbmsType=db.driver()->dbmsType();
    if(listDbmsType->contains(dbmsType))
        return false;

    auto cnnMng=dynamic_cast<ConnectionManager*>(this->parent());
    if(cnnMng==nullptr)
        return false;

    if(!this->pool.get(db))
        return false;

    if(!this->notify->setConnection(db))
        return false;

    this->connectedDriver=db.driver();
    return this->connectedDriver!=nullptr;
}

bool ConnectionNotifySubscribe::queueStop()
{
    auto &sqlDriver=this->connectedDriver;
    if(sqlDriver!=nullptr){
        QObject::disconnect(sqlDriver, QOverload<const QString &, QSqlDriver::NotificationSource, const QVariant &>::of(&QSqlDriver::notification), this, &ConnectionNotifySubscribe::onNotificationReceive);
        this->queueCloseConnection();
    }
    return (this->connectedDriver==nullptr);
}

bool ConnectionNotifySubscribe::queueStart()
{
    bool __return=false;
    if(!this->queueCheckConnection())
        return (this->connectedDriver!=nullptr);

    if(listDbmsType->contains(this->dbmsType()))
        return (this->connectedDriver!=nullptr);

    for(auto &v:this->subscribeToNotification){
        if(!this->connectedDriver->subscribeToNotification(v)){
            oWarning()<<tr("Invalid channel(%1) for subscribeToNotification").arg(v);
            continue;
        }
        QObject::connect(this->connectedDriver, QOverload<const QString &, QSqlDriver::NotificationSource, const QVariant &>::of(&QSqlDriver::notification), this, &ConnectionNotifySubscribe::onNotificationReceive);
        __return=true;
    }
    if(!__return)
        this->queueStop();

    return (this->connectedDriver!=nullptr);
}

bool ConnectionNotifySubscribe::queueStarted()
{
    return (this->connectedDriver!=nullptr);
}

bool ConnectionNotifySubscribe::queueSend(const QString &channel, const QVariant &payload)
{

    if(!payload.isValid())
        return false;

    if(this->connectedDriver==nullptr)
        return false;

    QString payloadBytes;

    auto channelList=channel.trimmed().isEmpty()?this->subscribeToNotification:QStringList{channel.trimmed()};

    auto &v=payload;
    switch (v.typeId()) {
    case QMetaType::QVariantHash:
    case QMetaType::QVariantMap:
    case QMetaType::QVariantList:
    case QMetaType::QStringList:
        payloadBytes=QJsonDocument::fromVariant(payload).toJson(QJsonDocument::Compact).trimmed().toHex();
        break;
    case QMetaType::QUuid:
        payloadBytes=v.toUuid().toString();
        break;
    case QMetaType::QUrl:
        payloadBytes=v.toUrl().toString();
        break;
    default:
        payloadBytes=payload.toByteArray().trimmed().toHex();
    }

    QSqlDatabase localConnection;

    if(payloadBytes.isEmpty()){
        oWarning()<<tr("payload is empty");
        return false;
    }

    if(!this->pool.get(localConnection)){
        oWarning()<<tr("invalid connection on subscriber");
        return false;
    }

    QStringList commandList;
    const auto dbmsType = this->dbmsType();
    if(dbmsType==QSqlDriver::PostgreSQL){
        for(auto &channel:channelList)
            commandList.append(QStringLiteral("select pg_notify('%1', '%2');").arg(channel, payloadBytes));
    }

    auto __return=false;
    for(auto &command:commandList){
        auto q=localConnection.exec(command);
        if(q.lastError().type()!=QSqlError::NoError){
            oWarning()<<tr("invalid execute %1").arg(q.lastError().text());
            break;
        }

        q.finish();
        q.clear();
        __return=true;
    }

    this->pool.finish(localConnection);
    return __return;
}

void ConnectionNotifySubscribe::onNotificationReceive(const QString &channel, QSqlDriver::NotificationSource, const QVariant &payload)
{
    auto bytes=payload.toByteArray().trimmed();
    if(!bytes.isEmpty()){
        VariantUtil vu;
        bytes=vu.isHex(bytes)?QByteArray::fromHex(bytes):bytes;
        emit this->notification(channel, bytes);
    }
}

void ConnectionNotifySubscribe::onNotificationSend(const QString &name, const QVariant &payload)
{
    this->queueSend(name, payload);
}

}
