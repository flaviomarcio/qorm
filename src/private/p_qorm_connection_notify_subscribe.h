#pragma once

#include "../qorm_connection_manager.h"
#include "../qorm_connection_notify.h"
#include "../qorm_connection_pool.h"
#include <QTimer>
#include <QThread>
#include <QObject>
#include <QVariant>
#include <QSqlDatabase>
#include <QSqlDriver>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
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
    explicit ConnectionNotifySubscribe(ConnectionNotify*parent = nullptr):QThread(nullptr), notify(parent), subscribeToNotification(parent->subscribeToNotification()){
        this->moveToThread(this);
    }
    Q_INVOKABLE ~ConnectionNotifySubscribe(){
    }
public:

    void run() override{
        auto connect=[this](){
            if(!this->queueStart())
                this->quit();
        };
        QTimer::singleShot(10, this, connect);
        this->exec();
        this->queueCloseConnection();
    }

    virtual bool start(const ConnectionPool&pool){
        if(!this->isRunning()){
            this->pool.from(pool.setting());
            QThread::start();
            while(this->eventDispatcher()!=nullptr)
                QThread::msleep(1);
        }
        return this->isRunning();
    }

    Q_INVOKABLE virtual bool start(const QSqlDatabase&connection){
        if(!this->isRunning()){
            //TODO FLAVIO, VERIFICAR PORQUE NAO LER A CONFIGURACAO DO POOL.
            this->pool.from(connection);
            QThread::start();
            while(this->eventDispatcher()!=nullptr)
                QThread::msleep(1);
        }
        return this->isRunning();
    }

    QSqlDriver::DbmsType dbmsType()const{
        return this->connectedDriver==nullptr?QSqlDriver::UnknownDbms:this->connectedDriver->dbmsType();
    }

    void queueCloseConnection(){
        this->pool.finish(this->_localDb);
        this->_localDb=QSqlDatabase();
        this->connectedDriver=nullptr;
    }

    bool queueCheckConnection(){
        QSqlDatabase db;
        if(!this->pool.get(db))
            return false;

        if(db.isValid() & db.isOpen())
            return false;

        auto dbmsType=db.driver()->dbmsType();
        if(listDbmsType.contains(dbmsType))
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

    bool queueStop(){
        auto&sqlDriver=this->connectedDriver;
        if(sqlDriver!=nullptr){
            QObject::disconnect(sqlDriver, QOverload<const QString&, QSqlDriver::NotificationSource, const QVariant &>::of(&QSqlDriver::notification), this, &ConnectionNotifySubscribe::notification_receive);
            this->queueCloseConnection();
        }
        return (this->connectedDriver==nullptr);
    }

    bool queueStart(){
        bool __return=false;
        if(!this->queueCheckConnection())
            return (this->connectedDriver!=nullptr);

        if(listDbmsType.contains(this->dbmsType()))
            return (this->connectedDriver!=nullptr);

        for(auto&v:this->subscribeToNotification){
            if(!this->connectedDriver->subscribeToNotification(v)){
                sWarning()<<tr("Invalid channel(%1) for subscribeToNotification").arg(v);
                continue;
            }
            QObject::connect(this->connectedDriver, QOverload<const QString &, QSqlDriver::NotificationSource, const QVariant &>::of(&QSqlDriver::notification), this, &ConnectionNotifySubscribe::notification_receive);
            __return=true;
        }
        if(!__return)
            this->queueStop();

        return (this->connectedDriver!=nullptr);
    }

    bool queueStarted(){
        return (this->connectedDriver!=nullptr);
    }

    bool queueSend(const QString &channel, const QVariant &payload){

        if(!payload.isValid())
            return false;

        if(this->connectedDriver==nullptr)
            return false;

        QString payloadBytes;

        auto channelList=channel.trimmed().isEmpty()?this->subscribeToNotification:QStringList{channel.trimmed()};

        auto&v=payload;
        switch (qTypeId(v)) {
            case QMetaType_QVariantHash:
                payloadBytes=QJsonDocument::fromVariant(payload).toJson(QJsonDocument::Compact).trimmed().toHex();
                break;
            case QMetaType_QVariantMap:
                payloadBytes=QJsonDocument::fromVariant(payload).toJson(QJsonDocument::Compact).trimmed().toHex();
                break;
            case QMetaType_QVariantList:
                payloadBytes=QJsonDocument::fromVariant(payload).toJson(QJsonDocument::Compact).trimmed().toHex();
                break;
            case QMetaType_QStringList:
                payloadBytes=QJsonDocument::fromVariant(payload).toJson(QJsonDocument::Compact).trimmed().toHex();
                break;
            case QMetaType_QUuid:
                payloadBytes=v.toUuid().toString();
                break;
            case QMetaType_QUrl:
                payloadBytes=v.toUrl().toString();
                break;
        default:
                payloadBytes=payload.toByteArray().trimmed().toHex();
        }

        QSqlDatabase localConnection;

        if(payloadBytes.isEmpty()){
            sWarning()<<tr("payload is empty");
            return false;
        }

        if(!this->pool.get(localConnection)){
            sWarning()<<tr("invalid connection on subscriber");
            return false;
        }

        QStringList commandList;
        const auto dbmsType = this->dbmsType();
        if(dbmsType==QSqlDriver::PostgreSQL){
            for(auto&channel:channelList)
                commandList<<qsl("select pg_notify('%1', '%2');").arg(channel, payloadBytes);
        }

        auto __return=false;
        for(auto&command:commandList){
            auto q=localConnection.exec(command);
            if(q.lastError().type()!=QSqlError::NoError){
                sWarning()<<tr("invalid execute %1").arg(q.lastError().text());
                break;
            }

            q.finish();
            q.clear();
            __return=true;
        }

        this->pool.finish(localConnection);
        return __return;
    }

public slots:
    void notification_receive(const QString &channel, QSqlDriver::NotificationSource, const QVariant &payload){
        auto bytes=payload.toByteArray().trimmed();
        if(!bytes.isEmpty()){
            VariantUtil vu;
            bytes=vu.isHex(bytes)?QByteArray::fromHex(bytes):bytes;
            emit this->notification(channel, bytes);
        }
    }

    void notification_send(const QString &name, const QVariant &payload){
        this->queueSend(name, payload);
    }
private:
    void*p=nullptr;
signals:
    void threadFinish(QOrm::ConnectionNotifySubscribe*thread);
signals:
    void notification(const QString &name, const QVariant &payload);
};

}
