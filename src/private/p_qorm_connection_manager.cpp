#include "./p_qorm_connection_manager.h"
#include "../qorm_macro.h"
#include "../qorm_const.h"
#include "../../qstm/src/qstm_util_variant.h"
#include "../../qstm/src/qstm_envs.h"
#include <QFile>
#include <QDir>
#include <QJsonDocument>
#include <QMetaMethod>
#include <QJsonObject>
#include <QJsonParseError>

namespace QOrm {

ConnectionManagerPvt::ConnectionManagerPvt(ConnectionManager *parent):QObject{parent}, notify(parent),defaultPool(parent)
{
    this->parent=parent;
    this->init();
}

ConnectionManagerPvt::~ConnectionManagerPvt()
{
    this->clear();
}

void ConnectionManagerPvt::init()
{
    this->detailGetCheck(this->enviroment);
}

ConnectionPool &ConnectionManagerPvt::pool(const QByteArray &value)
{
    auto &p=*this;
    if(p.parentParent!=nullptr){
        if(!p.isLoaded())
            p.load(p.parentParent);
    }

    auto detail=p.settings.value(value);
    if(detail==nullptr){
        if(p.pools.isEmpty()){
#if Q_ORM_LOG
            oWarning()<<QStringLiteral("invalid pool: ")<<value<<QStringLiteral(", keys==")<<p.pools.keys();
#endif
            return defaultPool;
        }

        auto pool=p.pools.first();
        return*pool;
    }

    if(!p.pools.contains(value))
        p.pools.insert(value, new ConnectionPool(*detail));
    auto setting=p.settingNameAdjust(value);
    auto &pool=*p.pools.value(setting);
    return pool;
}

bool ConnectionManagerPvt::isLoaded() const
{
    QHashIterator<QString, ConnectionSetting*> i(this->settings);
    while (i.hasNext()) {
        i.next();
        if(i.key().trimmed().isEmpty())
            continue;

        auto &v=i.value();
        if(v->isValid())
            return true;
    }
    return false;
}

bool ConnectionManagerPvt::isEmpty() const
{
    QHashIterator<QString, ConnectionSetting*> i(this->settings);
    while (i.hasNext()) {
        i.next();
        auto &v=i.value();
        if(v->isValid())
            return false;
    }
    return true;
}

void ConnectionManagerPvt::clear()
{
    auto _detail=this->settings;
    qDeleteAll(_detail);
    this->enviroment.clear();
    this->secret.clear();
    this->pools.clear();
    this->settings.clear();
}

QByteArray ConnectionManagerPvt::settingNameAdjust(QByteArray settingName)
{
    auto setting=settingName.trimmed().isEmpty()? this->enviroment : settingName.trimmed();
    if(!this->settings.contains(setting))
        setting=this->enviroment;
    return setting;
}

ConnectionSetting &ConnectionManagerPvt::detailGetCheck(QByteArray &settingName)
{
    auto RETURN=this->settingNameAdjust(settingName);
    if(!settings.contains(RETURN))
        settings.insert(RETURN, new ConnectionSetting(parent));
    return*settings.value(RETURN);
}

ConnectionManager &ConnectionManagerPvt::insert(const QVariantHash &value)
{
    auto &p=*this;
    if(!value.isEmpty()){
        auto name=value.value(QStringLiteral("name")).toByteArray().trimmed();
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
                oWarning()<<QStringLiteral("invalid database driver == ")<<setting->driver();
                oWarning()<<QStringLiteral("avaliable drivers == ")<<drivers.join(',');
            }
        }
    }
    return*this->parent;
}

bool ConnectionManagerPvt::v_load(const QVariant &v)
{
    switch (v.typeId()) {
    case QMetaType::QVariantHash:
    case QMetaType::QVariantMap:
        return this->load(v.toStringList());
    default:
        break;
    }

    switch (v.typeId()) {
    case QMetaType::QVariantHash:
    case QMetaType::QVariantMap:
        return this->load(v.toHash());
    default:
        return this->load(v.toString());
    }

    return {};
}

bool ConnectionManagerPvt::load(const QVariantHash &vSettings)
{
    auto &p=*this;
    bool RETURN=false;
    auto settings=vSettings;

    if(vSettings.contains(QStringLiteral("connection")))
        settings=vSettings.value(QStringLiteral("connection")).toHash();

    if(vSettings.isEmpty()){
        p.clear();
        return false;
    }

    if(settings.contains(QStringLiteral("host")) && settings.contains(QStringLiteral("user"))){
        this->insert(settings);
        return true;
    }

    auto enviroment = QByteArray(getenv(QByteArrayLiteral("DB_ENVIROMENT"))).trimmed();

    QStm::Envs envs;
    envs.customEnvs(QStringLiteral("driver"        )  , QStringLiteral("DB_DRIVER"  ));
    envs.customEnvs(QStringLiteral("hostName"      )  , QStringLiteral("DB_HOST"    ));
    envs.customEnvs(QStringLiteral("userName"      )  , QStringLiteral("DB_USER"    ));
    envs.customEnvs(QStringLiteral("password"      )  , QStringLiteral("DB_PASSWORD"    ));
    envs.customEnvs(QStringLiteral("dataBaseName"  )  , QStringLiteral("DB_DATABASE"));
    envs.customEnvs(QStringLiteral("connectOptions")  , QStringLiteral("DB_OPTION"  ));
    envs.customEnvs(QStringLiteral("port"          )  , QStringLiteral("DB_PORT"    ));
    envs.customEnvs(QStringLiteral("schemaNames"   )  , QStringLiteral("DB_SCHEMA"  ));

    p.secret = settings.value(QStringLiteral("secret")).toByteArray();
    p.enviroment = settings.value(QStringLiteral("enviroment")).toByteArray();
    p.enviroment=enviroment.isEmpty()?p.enviroment:enviroment;
    auto paramaters = settings.value(QStringLiteral("paramaters")).toHash();
    paramaters=envs.parser(paramaters).toHash();
    QHashIterator<QString, QVariant> i(paramaters);
    while (i.hasNext()) {
        i.next();
        auto name=i.key().trimmed();
        if(name.trimmed().isEmpty())
            continue;

        auto value=i.value().toHash();
        if(value.isEmpty())
            continue;

        value.insert(QStringLiteral("name"), name);
        p.insert(value);
        RETURN=true;
    }
    return RETURN;
}

bool ConnectionManagerPvt::load(const QString &settingsFileName)
{
    auto &p=*this;
    QFile file(settingsFileName);
    if(settingsFileName.trimmed().isEmpty()){
        oWarning()<<QStringLiteral("not file settings");
        return false;
    }
    if(!file.exists()){
        oWarning()<<QStringLiteral("file not exists %1").arg(file.fileName());
        return false;
    }
    if(!file.open(QFile::ReadOnly)){
        oWarning()<<QStringLiteral("%1, %2").arg(file.errorString(), file.fileName());
        return false;
    }
    auto bytes=file.readAll();
    QJsonParseError*error=nullptr;
    auto doc=QJsonDocument::fromJson(bytes, error);
    if(error!=nullptr){
        oWarning()<<QStringLiteral("%1, %2").arg(file.fileName(), error->errorString());
        return false;
    }

    if(doc.object().isEmpty()){
        oWarning()<<QStringLiteral("object is empty, %1").arg(file.fileName());
        return false;
    }

    auto settings=doc.object().toVariantHash();
    if(!settings.contains(QStringLiteral("connection"))){
        oWarning()<<QStringLiteral("tag connection not exists, %1").arg(file.fileName());
        return false;
    }

    if(!p.load(settings)){
        this->settingsFileName.clear();
        return false;
    }

    this->settingsFileName=settingsFileName;
    return true;
}

bool ConnectionManagerPvt::load(const QStringList &settingsFileName)
{
    QVariantList vList;
    auto &p=*this;
    for(auto &fileName:settingsFileName){
        QFile file(fileName);
        if(fileName.isEmpty())
            continue;
        if(!file.exists()){
#if Q_ORM_LOG_DEBUG
            oWarning()<<tr("file not exists %1").arg(file.fileName());
#endif
            continue;
        }

        if(!file.open(QFile::ReadOnly)){
#if Q_ORM_LOG_DEBUG
            oWarning()<<tr("%1, %2").arg(file.fileName(), file.errorString());
#endif
            continue;
        }

        auto bytes=file.readAll();
        file.close();
        QJsonParseError*error=nullptr;
        auto doc=QJsonDocument::fromJson(bytes, error);
        if(error!=nullptr){
#if Q_ORM_LOG
            oWarning()<<QStringLiteral("%1, %2").arg(file.fileName(), error->errorString());
#endif
            continue;

        }
        if(doc.object().isEmpty()){
#if Q_ORM_LOG
            oWarning()<<QStringLiteral("object is empty, %1").arg(file.fileName());
#endif
            continue;
        }
        auto map=doc.object().toVariantHash();
        if(!map.isEmpty())
            vList.append(map);
    }
    Q_DECLARE_VU;
    auto vMap=vu.vMerge(vList).toHash();
    if(p.load(vMap))
        p.settingsFileName=settingsFileName;
    else
        p.settingsFileName.clear();
    return p.isLoaded();
}

bool ConnectionManagerPvt::load(QObject *settingsObject)
{
    static auto ignoreMethods=QVector<QByteArray>{"destroyed","objectNameChanged","deleteLater","_q_reregisterTimers"};
    static auto staticNames=QVector<QByteArray>{"resourcesettings"};
    auto &p=*this;
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

}
