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
        if(this->pool.get(db)){
            if(!db.isValid() || !db.isOpen()){
                auto dbmsType=db.driver()->dbmsType();
                if(!listDbmsType.contains(dbmsType)){
                    auto cnnMng=dynamic_cast<ConnectionManager*>(this->parent());
                    if(cnnMng!=nullptr){
                        if(this->pool.get(db)){
                            if(this->notify->setConnection(db)){
                                this->connectedDriver=db.driver();
                                return this->connectedDriver!=nullptr;
                            }
                        }
                    }
                }
            }
        }
        return false;
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
        if(this->queueCheckConnection()){
            if(!listDbmsType.contains(this->dbmsType())){
                for(auto&v:this->subscribeToNotification){
                    if(!this->connectedDriver->subscribeToNotification(v))
                        sWarning()<<tr("Invalid channel(%1) for subscribeToNotification").arg(v);
                    else{
                        QObject::connect(this->connectedDriver, QOverload<const QString &, QSqlDriver::NotificationSource, const QVariant &>::of(&QSqlDriver::notification), this, &ConnectionNotifySubscribe::notification_receive);
                        __return=true;
                    }
                }
                if(!__return)
                    this->queueStop();
            }
        }
        return (this->connectedDriver!=nullptr);
    }

    bool queueStarted(){
        return (this->connectedDriver!=nullptr);
    }

    bool queueSend(const QString &channel, const QVariant &payload){

        if(!payload.isValid())
            return false;
        else if(this->connectedDriver==nullptr)
            return false;
        else{
            bool __return=false;
            QString payloadBytes;

            auto channelList=channel.trimmed().isEmpty()?this->subscribeToNotification:QStringList()<<channel.trimmed();

            auto&v=payload;
            if(v.canConvert(QVariant::Map) || v.canConvert(QVariant::Hash) || v.canConvert(QVariant::List) || v.canConvert(QVariant::StringList))
                payloadBytes=QJsonDocument::fromVariant(payload).toJson(QJsonDocument::Compact).trimmed().toHex();
            else if(v.type()==QVariant::Uuid)
                payloadBytes=v.toUuid().toString();
            else if(v.type()==QVariant::Url)
                payloadBytes=v.toUrl().toString();
            else
                payloadBytes=payload.toByteArray().trimmed().toHex();

            QSqlDatabase localConnection;

            if(payloadBytes.isEmpty()){
                sWarning()<<tr("payload is empty");
                __return=false;
            }
            else if(!this->pool.get(localConnection)){
                sWarning()<<tr("invalid connection on subscriber");
                return false;
            }
            else{
                QStringList commandList;
                const auto dbmsType = this->dbmsType();
                if(dbmsType==QSqlDriver::PostgreSQL){
                    for(auto&channel:channelList){
                        commandList<<qsl("select pg_notify('%1', '%2');").arg(channel, payloadBytes);
                    }
                }
                else{
                    sWarning()<<tr("Invalid database to notification");
                }

                __return=channelList.isEmpty();
                for(auto&command:commandList){
                    auto q=localConnection.exec(command);
                    if(q.lastError().type()!=QSqlError::NoError){
                        sWarning()<<tr("invalid execute %1").arg(q.lastError().text());
                        break;
                    }
                    else{
                        q.finish();
                        q.clear();
                        __return=true;
                    }
                }

                this->pool.finish(localConnection);
            }
            return __return;
        }
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
