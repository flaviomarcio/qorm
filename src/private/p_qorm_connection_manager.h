#pragma once

#include "../qorm_connection_manager.h"
#include <QFile>
#include <QDir>
#include <QSettings>
#include <QStringList>
#include <QVariantHash>
#include <QVariantList>
#include <QVariantHash>
#include <QMetaMethod>
#include <QMetaProperty>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QGenericArgument>

namespace QOrm {

#define dPvt()\
    auto&p = *reinterpret_cast<ConnectionManagerPrv*>(this->p)

class ConnectionManagerPrv{
public:
    ConnectionNotify notify;
    QVariant settingsFileName;
    QByteArray enviroment;
    QByteArray secret;
    QHash<QString, ConnectionSetting*> settings;
    QMap<QString, ConnectionPool*> pools;
    ConnectionManager*parent=nullptr;
    QObject*parentParent=nullptr;
    ConnectionPool defaultPool;
    explicit ConnectionManagerPrv(ConnectionManager*parent):notify(parent),defaultPool(parent){
        this->parent=parent;
        this->init();
    }
    virtual ~ConnectionManagerPrv(){
        this->clear();
    }

    void init(){
        this->detailGetCheck(this->enviroment);
    }

    ConnectionPool&pool(const QByteArray &value){
        auto&p=*this;
        if(p.parentParent!=nullptr){
            if(!p.isLoaded())
                p.load(p.parentParent);
        }

        auto detail=p.settings.value(value);
        if(detail==nullptr){
            if(p.pools.isEmpty()){
#if Q_ORM_LOG
                sWarning()<<qsl("invalid pool: ")<<value<<qsl(", keys==")<<p.pools.keys();
#endif
                return defaultPool;
            }

            auto pool=p.pools.first();
            return*pool;
        }

        if(!p.pools.contains(value))
            p.pools.insert(value, new ConnectionPool(*detail));
        auto setting=p.settingNameAdjust(value);
        auto&pool=*p.pools.value(setting);
        return pool;
    }

    bool isLoaded()const{
        QHashIterator<QString, ConnectionSetting*> i(this->settings);
        while (i.hasNext()) {
            i.next();
            if(i.key().trimmed().isEmpty())
                continue;

            auto&v=i.value();
            if(v->isValid())
                return true;
        }
        return false;
    }

    bool isEmpty()const{
        QHashIterator<QString, ConnectionSetting*> i(this->settings);
        while (i.hasNext()) {
            i.next();
            auto&v=i.value();
            if(v->isValid())
                return false;
        }
        return true;
    }

    void clear(){
        auto _detail=this->settings;
        qDeleteAll(_detail);
        this->enviroment.clear();
        this->secret.clear();
        this->pools.clear();
        this->settings.clear();
    }

    QByteArray settingNameAdjust(QByteArray settingName){
        auto setting=settingName.trimmed().isEmpty()? this->enviroment : settingName.trimmed();
        if(!this->settings.contains(setting))
            setting=this->enviroment;
        return setting;
    }

    ConnectionSetting&detailGetCheck(QByteArray&settingName){
        auto RETURN=this->settingNameAdjust(settingName);
        if(!settings.contains(RETURN))
            settings.insert(RETURN, new ConnectionSetting(parent));
        return*settings.value(RETURN);
    }

    ConnectionManager &insert(const QVariantHash &value)
    {
        auto&p=*this;
        if(!value.isEmpty()){
            auto name=value.value(qsl("name")).toByteArray().trimmed();
            if(!name.isEmpty()){
                auto drivers=QSqlDatabase::drivers();
                auto setting=p.settings.value(name);
                if(setting==nullptr){
                    setting=new ConnectionSetting(nullptr);
                    setting->fromMap(value);
                    setting->setName(name);
                    p.settings.insert(setting->name(), setting);
                }
                else{
                    setting->fromMap(value);
                }
                if(!drivers.contains(setting->driver())){
                    sWarning()<<qsl("invalid database driver == ")<<setting->driver();
                    sWarning()<<qsl("avaliable drivers == ")<<drivers.join(',');
                }
            }
        }
        return*this->parent;
    }

    bool v_load(const QVariant &v){
        auto typeId=qTypeId(v);
        if(QStmTypesVariantList.contains(typeId))
            return this->load(v.toStringList());

        if(QStmTypesVariantDictionary.contains(typeId))
            return this->load(v.toHash());

        return this->load(v.toString());
    }

    bool load(const QVariantHash &vSettings)
    {
        auto&p=*this;
        bool RETURN=false;
        auto settings=vSettings;

        if(vSettings.contains(qsl("connection")))
            settings=vSettings[qsl("connection")].toHash();

        if(vSettings.isEmpty()){
            p.clear();
            return false;
        }

        if(settings.contains(qsl("host")) && settings.contains(qsl("user"))){
            this->insert(settings);
            return true;
        }

        auto enviroment = QByteArray(getenv(qbl("DATABASE_ENVIROMENT"))).trimmed();

        QVariantHash defaultVariables;
        QVariantHash defaultValues;
        defaultVariables.insert(qsl("driver"        )  , qsl("DATABASE_DRIVER"  )    );
        defaultVariables.insert(qsl("hostName"      )  , qsl("DATABASE_HOST"    )    );
        defaultVariables.insert(qsl("userName"      )  , qsl("DATABASE_USERNAME")    );
        defaultVariables.insert(qsl("password"      )  , qsl("DATABASE_PASSWORD")    );
        defaultVariables.insert(qsl("dataBaseName"  )  , qsl("DATABASE_NAME"    )    );
        defaultVariables.insert(qsl("connectOptions")  , qsl("DATABASE_OPTION"  )    );
        defaultVariables.insert(qsl("port"          )  , qsl("DATABASE_PORT"    )    );

        if(!defaultVariables.isEmpty()){
            QHashIterator<QString, QVariant> i(defaultVariables);
            while (i.hasNext()) {
                i.next();
                auto env=i.value().toByteArray().trimmed();
                auto v = QByteArray(getenv(env)).trimmed();
                if(v.isEmpty())
                    v = QByteArray(getenv(env.toLower())).trimmed();
                if(!v.isEmpty())
                    defaultValues.insert(i.key(),v);
            }
        }
        p.secret = settings.value(qsl("secret")).toByteArray();
        p.enviroment = settings.value(qsl("enviroment")).toByteArray();
        p.enviroment=enviroment.isEmpty()?p.enviroment:enviroment;
        auto paramaters = settings.value(qsl("paramaters")).toHash();
        QHashIterator<QString, QVariant> i(paramaters);
        while (i.hasNext()) {
            i.next();
            auto name=i.key().trimmed();
            if(name.trimmed().isEmpty())
                continue;

            auto value = paramaters.value(name).toHash();

            if(p.enviroment.toLower()==i.key().toLower()){
                QHashIterator<QString, QVariant> i(defaultValues);
                while (i.hasNext()) {
                    i.next();
                    auto v0=value.value(i.key()).toString();
                    sWarning()<<qsl("replace [%1==%2] to [%1==%3]").arg(i.key(), v0, i.value().toString());
                    value.insert(i.key(), i.value());
                }
            }

            if(!value.isEmpty()){
                value.insert(qsl("name"), name);
                p.insert(value);
                RETURN=true;
            }
        }
        return RETURN;
    }

    bool load(const QString &settingsFileName)
    {
        auto&p=*this;
        QFile file(settingsFileName);
        if(settingsFileName.trimmed().isEmpty()){
            sWarning()<<qsl("not file settings");
            return false;
        }
        if(!file.exists()){
            sWarning()<<qsl("file not exists %1").arg(file.fileName());
            return false;
        }
        if(!file.open(QFile::ReadOnly)){
            sWarning()<<qsl("%1, %2").arg(file.errorString(), file.fileName());
            return false;
        }



        auto bytes=file.readAll();
        QJsonParseError*error=nullptr;
        auto doc=QJsonDocument::fromJson(bytes, error);
        if(error!=nullptr){
            sWarning()<<qsl("%1, %2").arg(file.fileName(), error->errorString());
            return false;
        }

        if(doc.object().isEmpty()){
            sWarning()<<qsl("object is empty, %1").arg(file.fileName());
            return false;
        }

        auto settings=doc.object().toVariantHash();
        if(!settings.contains(qsl("connection"))){
            sWarning()<<qsl("tag connection not exists, %1").arg(file.fileName());
            return false;
        }

        if(!p.load(settings)){
            this->settingsFileName.clear();
            return false;
        }

        this->settingsFileName=settingsFileName;
        return true;
    }

    bool load(const QStringList &settingsFileName)
    {
        QVariantList vList;
        auto&p=*this;
        for(auto&fileName:settingsFileName){
            QFile file(fileName);
            if(fileName.isEmpty())
                continue;
            if(!file.exists()){
#if Q_ORM_LOG_DEBUG
                sWarning()<<tr("file not exists %1").arg(file.fileName());
#endif
                continue;
            }

            if(!file.open(QFile::ReadOnly)){
#if Q_ORM_LOG_DEBUG
                sWarning()<<tr("%1, %2").arg(file.fileName(), file.errorString());
#endif
                continue;
            }

            auto bytes=file.readAll();
            file.close();
            QJsonParseError*error=nullptr;
            auto doc=QJsonDocument::fromJson(bytes, error);
            if(error!=nullptr){
#if Q_ORM_LOG
                sWarning()<<qsl("%1, %2").arg(file.fileName(), error->errorString());
#endif
                continue;

            }
            if(doc.object().isEmpty()){
#if Q_ORM_LOG
                sWarning()<<qsl("object is empty, %1").arg(file.fileName());
#endif
                continue;
            }
            auto map=doc.object().toVariantHash();
            if(!map.isEmpty())
                vList<<map;
        }
        Q_DECLARE_VU;
        auto vMap=vu.vMerge(vList).toHash();
        if(p.load(vMap))
            p.settingsFileName=settingsFileName;
        else
            p.settingsFileName.clear();
        return p.isLoaded();
    }

    bool load(QObject *settingsObject)
    {
        static auto ignoreMethods=QVector<QByteArray>{"destroyed","objectNameChanged","deleteLater","_q_reregisterTimers"};
        static auto staticNames=QVector<QByteArray>{qbl("settingsfilename"), qbl("settings_server"), qbl("settingsserver")};
        auto&p=*this;
        if(settingsObject==nullptr)
            return false;
        auto metaObject=settingsObject->metaObject();
        for(int methodIndex = 0; methodIndex < metaObject->methodCount(); ++methodIndex) {
            auto metaMethod = metaObject->method(methodIndex);
            if(metaMethod.parameterCount()>0)
                continue;

            if(ignoreMethods.contains(metaMethod.name()))
                continue;

            auto methodName=QByteArray(metaMethod.name()).toLower();
            if(!staticNames.contains(methodName))
                continue;

            QVariant invokeReturn;
            auto argReturn=Q_RETURN_ARG(QVariant, invokeReturn);
            if(!metaMethod.invoke(settingsObject, argReturn))
                continue;

            if(!p.v_load(invokeReturn))
                continue;
            return true;
        }
        return false;
    }

};

}
