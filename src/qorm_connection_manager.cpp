#include "./qorm_connection_manager.h"
#include "./qorm_macro.h"
#include "../../qstm/src/qstm_envs.h"
#include <QMetaProperty>
#include <QJsonDocument>

namespace QOrm {

static const auto __secretKey="secretKey";
static const auto __environment="environment";
static const auto __paramaters="paramaters";
static const auto __settings="settings";

//static const auto __host="host";
//static const auto __user="user";
static const auto __driver="driver";
static const auto __hostName="hostName";
static const auto __userName="userName";
static const auto __password="password";
static const auto __dataBaseName="dataBaseName";
static const auto __connectOptions="connectOptions";
static const auto __schemaNames="schemaNames";
static const auto __port="port";
static const auto __name="name";

static const auto __DB_ENVIRONMENT="DB_ENVIRONMENT";
static const auto __DB_DRIVER="DB_DRIVER";
static const auto __DB_HOST="DB_HOST";
static const auto __DB_USER="DB_USER";
static const auto __DB_PASSWORD="DB_PASSWORD";
static const auto __DB_DATABASE="DB_DATABASE";
static const auto __DB_OPTION="DB_OPTION";
static const auto __DB_PORT="DB_PORT";
static const auto __DB_SCHEMA="DB_SCHEMA";


//!
//! \brief The ConnectionManagerPvt class
//!
class ConnectionManagerPvt:public QObject
{
public:
    QVariantHash toHash;
    QByteArray enviroment, enviromentParser;
    QByteArray secretKey, secretKeyParser;
    QVariantHash settings;
    QHash<QString, ConnectionSetting*> settingsHash;
    QMap<QString, ConnectionPool*> pools;
    ConnectionManager *parent=nullptr;
    ConnectionPool defaultPool;
    QStm::Envs envs;
    //!
    //! \brief ConnectionManagerPvt
    //! \param parent
    //!
    explicit ConnectionManagerPvt(ConnectionManager *parent):QObject{parent}, parent{parent}, defaultPool{parent}, envs{this}
    {
    }

    explicit ConnectionManagerPvt(const ConnectionManager &managerParent, ConnectionManager *parent):QObject{parent}, parent{parent}, defaultPool{parent}
    {
        load(managerParent.toHash());
    }

    //!
    //! \brief pool
    //! \param value
    //! \return
    //!
    ConnectionPool &pool(const QByteArray &value)
    {
        auto &p=*this;

        auto detail=p.settingsHash.value(value);
        if(detail==nullptr){
            if(p.pools.isEmpty())
                return defaultPool;
            auto pool=p.pools.first();
            return *pool;
        }

        if(!p.pools.contains(value))
            p.pools.insert(value, new ConnectionPool(*detail));
        auto setting=value.trimmed().toLower();
        auto &pool=*p.pools.value(setting);
        return pool;
    }

    //!
    //! \brief isLoaded
    //! \return
    //!
    bool isLoaded()const
    {
        QHashIterator<QString, ConnectionSetting*> i(this->settingsHash);
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

    //!
    //! \brief isEmpty
    //! \return
    //!
    bool isEmpty()const
    {
        QHashIterator<QString, ConnectionSetting*> i(this->settingsHash);
        while (i.hasNext()) {
            i.next();
            auto &v=i.value();
            if(v->isValid())
                return false;
        }
        return true;
    }

    //!
    //! \brief clear
    //!
    void clear()
    {
        this->toHash={};
        this->enviroment={};
        this->enviromentParser={};
        this->secretKey={};
        this->secretKeyParser={};
        this->settings={};

        auto _detail=this->settingsHash;
        qDeleteAll(_detail);
        this->enviroment.clear();
        this->secretKey.clear();
        this->pools.clear();
        this->settingsHash.clear();

        this->envs.reset();
        this->envs.customEnvs(__environment   , envs.value(__DB_ENVIRONMENT   ));
        this->envs.customEnvs(__driver        , envs.value(__DB_DRIVER        ));
        this->envs.customEnvs(__hostName      , envs.value(__DB_HOST          ));
        this->envs.customEnvs(__userName      , envs.value(__DB_USER          ));
        this->envs.customEnvs(__password      , envs.value(__DB_PASSWORD      ));
        this->envs.customEnvs(__dataBaseName  , envs.value(__DB_DATABASE      ));
        this->envs.customEnvs(__connectOptions, envs.value(__DB_OPTION        ));
        this->envs.customEnvs(__port          , envs.value(__DB_PORT          ));
        this->envs.customEnvs(__schemaNames   , envs.value(__DB_SCHEMA        ));
    }

    //!
    //! \brief settingNameAdjust
    //! \param settingName
    //! \return
    //!
    QByteArray settingNameAdjust(const QByteArray &settingName)
    {
        auto setting=settingName.trimmed().isEmpty()? this->enviroment : settingName.trimmed();
        if(!this->settingsHash.contains(setting))
            setting=this->enviroment;
        return setting;
    }

    ConnectionManager &insert(const QSqlDatabase &value)
    {
        auto vHash=QVariantHash{
                                  {__name, value.connectionName()},
                                  {__driver, value.driverName()},
                                  {__dataBaseName, value.databaseName()},
                                  {__userName, value.userName()},
                                  {__password, value.password()},
                                  {__hostName, value.hostName()},
                                  {__port, value.port()},
                                  {__connectOptions, value.connectOptions()},
                                  };
        return this->insert(vHash);
    }

    ConnectionManager &insert(const QVariantHash &value)
    {
        auto &p=*this;
        if(value.isEmpty())
            return *this->parent;
        auto name=value.value(__name).toByteArray().trimmed();
        if(name.isEmpty())
            return *this->parent;

        auto drivers=QSqlDatabase::drivers();
        auto setting=p.settingsHash.value(name);
        if(setting==nullptr){
            setting=new ConnectionSetting(this);
            setting->from(value);
            setting->setName(name);
            p.settingsHash.insert(setting->name(), setting);
        }
        else{
            setting->from(value);
        }

        if(!drivers.contains(setting->driver())){
            oWarning()<<QStringLiteral("invalid database driver == ")<<setting->driver();
            oWarning()<<QStringLiteral("avaliable drivers == ")<<drivers.join(',');
        }
        return *this->parent;
    }

    //!
    //! \brief load
    //! \param vSettings
    //! \return
    //!
    bool load(const QVariantHash &vSetting)
    {
        if(vSetting.isEmpty()){
            this->clear();
            return false;
        }

        this->secretKey = vSetting.value(secretKey).toByteArray().trimmed();
        this->enviroment = vSetting.value(__environment).toByteArray().trimmed();

        auto settings = (vSetting.contains(__settings)?vSetting.value(__settings):vSetting.value(__paramaters)).toHash();

        if(settings.isEmpty())
            return {};

        settings=envs.parser(settings).toHash();
        QHashIterator<QString, QVariant> i(settings);
        while (i.hasNext()) {
            i.next();
            auto name=i.key().trimmed();
            if(name.trimmed().isEmpty())
                continue;

            auto value=i.value().toHash();
            if(value.isEmpty())
                continue;

            value.insert(__name, name);
            this->insert(value);
        }
        return true;
    }

    void setSettings(const QVariant &value)
    {
        QVariant values;
        QVariantList valuesList;
        switch (value.typeId()) {
        case QMetaType::QString:
        case QMetaType::QByteArray:
            values=QJsonDocument::fromJson(value.toByteArray()).toVariant();
            break;
        default:
            values=value;
        }

        switch (values.typeId()) {
        case QMetaType::QVariantHash:
        case QMetaType::QVariantMap:
        case QMetaType::QVariantPair:
        {
            auto vHash=values.toHash();
            if(!vHash.isEmpty())
                valuesList.append(vHash);
            break;
        }
        case QMetaType::QVariantList:
        case QMetaType::QStringList:
        {
            auto vList=values.toList();
            for (auto &v : vList) {
                if(!v.isValid() || v.isNull())
                    continue;

                switch (v.typeId()) {
                case QMetaType::QString:
                case QMetaType::QByteArray:
                {
                    auto vHash=QJsonDocument::fromJson(v.toByteArray()).toVariant().toHash();
                    if(!vHash.isEmpty())
                        valuesList.append(vHash);
                    break;
                }
                case QMetaType::QVariantHash:
                case QMetaType::QVariantMap:
                case QMetaType::QVariantPair:
                {
                    auto vHash=v.toHash();
                    if(!vHash.isEmpty())
                        valuesList.append(vHash);
                    break;
                }
                default:
                    break;
                }
            }
            break;
        }
        default:
            break;
        }

        auto lst = this->settingsHash.values();
        this->settingsHash.clear();
        qDeleteAll(lst);
        for(auto&v:valuesList)
            this->insert(v.toHash());
    }

};

ConnectionManager::ConnectionManager(QObject *parent)
    :
    QObject{parent},
    p{new ConnectionManagerPvt{this}}
{
}

ConnectionManager::ConnectionManager(const ConnectionManager &managerParent, QObject *parent)
    :
    QObject{parent},
    p{new ConnectionManagerPvt{managerParent, this}}
{

}

ConnectionManager &ConnectionManager::operator=(const ConnectionSetting &value)
{
    p->clear();
    p->load(value.toHash());
    return *this;
}

ConnectionManager &ConnectionManager::operator=(const QVariant &value)
{
    p->clear();
    p->setSettings(value.toHash());
    return *this;
}

ConnectionManager &ConnectionManager::operator=(const QSqlDatabase &value)
{
    p->clear();
    p->insert(value);
    return *this;
}

ConnectionManager &ConnectionManager::operator<<(const ConnectionSetting &value)
{
    return p->insert(value.toHash());
}

const QVariantHash &ConnectionManager::toHash() const
{
    p->toHash={
            {__environment, this->enviroment()},
            {__secretKey, this->secretKey()},
            {__settings, this->settings()},
            };
    return p->toHash;
}

ConnectionManager &ConnectionManager::operator<<(const QVariantHash &value)
{
    return p->insert(value);
}

ConnectionManager &ConnectionManager::operator<<(const QSqlDatabase &value)
{
    return p->insert(value);
}

ConnectionManager::ConnectionManager(const QVariantHash &setting, QObject *parent): QObject{parent}, p{new ConnectionManagerPvt{this}}
{
    p->load(setting);
}

const ConnectionManager &ConnectionManager::clear()
{
    p->clear();
    return *this;
}

const QByteArray &ConnectionManager::enviroment() const
{
    return p->enviromentParser=p->envs.parser(p->enviroment).toByteArray();
}

ConnectionManager &ConnectionManager::setEnviroment(const QByteArray &value)
{
    p->enviroment = value.trimmed();
    emit enviromentChanged();
    return *this;
}

const QByteArray &ConnectionManager::secretKey() const
{
    return p->secretKeyParser=p->envs.parser(p->secretKey).toByteArray();
}

ConnectionManager &ConnectionManager::setSecretKey(const QByteArray &value)
{
    p->secretKey = value.trimmed();
    emit settingsChanged();
    return *this;
}

const QVariantHash &ConnectionManager::settings() const
{
    p->settings.clear();
    QHashIterator<QString, ConnectionSetting *> i(p->settingsHash);
    while (i.hasNext()) {
        i.next();
        const auto k = i.key().trimmed().toLower();
        const auto &v = i.value();
        if (k.trimmed().isEmpty())
            continue;

        if (v->isValid())
            p->settings.insert(k, v->toHash());
    }
    return p->settings;
}

ConnectionManager &ConnectionManager::setSettings(const QVariant &value)
{
    p->setSettings(value);
    emit settingsChanged();
    return *this;
}

ConnectionManager &ConnectionManager::insert(const QSqlDatabase &value)
{
    return p->insert(value);
}

ConnectionManager &ConnectionManager::insert(const ConnectionSetting &value)
{
    return p->insert(value.toHash());
}

ConnectionManager &ConnectionManager::insert(const QVariantHash &value)
{
    return p->insert(value);
}

ConnectionPool &ConnectionManager::pool()
{
    return p->pool(p->enviroment);
}

ConnectionPool &ConnectionManager::pool(const QByteArray &value)
{
    return p->pool(value);
}

bool ConnectionManager::isEmpty() const
{
    return p->isEmpty();
}

bool ConnectionManager::isLoaded() const
{
    return p->isLoaded();
}

bool ConnectionManager::load(const QVariantHash &settings)
{
    return p->load(settings);
}

bool ConnectionManager::load(const ConnectionManager &manager)
{
    return p->load(manager.toHash());
}


} // namespace QOrm
