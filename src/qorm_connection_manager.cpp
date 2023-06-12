#include "./qorm_connection_manager.h"
#include "./qorm_macro.h"
#include "../../../qstm/src/qstm_envs.h"
#include <QMetaProperty>

namespace QOrm {

static const auto __secret="secret";
static const auto __environment="environment";
static const auto __paramaters="paramaters";

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
    QByteArray enviroment;
    QByteArray secret;
    QVariantHash parameters;
    QHash<QString, ConnectionSetting*> settings;
    QMap<QString, ConnectionPool*> pools;
    ConnectionManager *parent=nullptr;
    ConnectionPool defaultPool;
    //!
    //! \brief ConnectionManagerPvt
    //! \param parent
    //!
    explicit ConnectionManagerPvt(ConnectionManager *parent):QObject{parent}, parent{parent}, defaultPool{parent}
    {
    }

    //!
    //! \brief pool
    //! \param value
    //! \return
    //!
    ConnectionPool &pool(const QByteArray &value)
    {
        auto &p=*this;

        auto detail=p.settings.value(value);
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

    //!
    //! \brief isEmpty
    //! \return
    //!
    bool isEmpty()const
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

    //!
    //! \brief clear
    //!
    void clear()
    {
        auto _detail=this->settings;
        qDeleteAll(_detail);
        this->enviroment.clear();
        this->secret.clear();
        this->pools.clear();
        this->settings.clear();
    }

    //!
    //! \brief settingNameAdjust
    //! \param settingName
    //! \return
    //!
    QByteArray settingNameAdjust(const QByteArray &settingName)
    {
        auto setting=settingName.trimmed().isEmpty()? this->enviroment : settingName.trimmed();
        if(!this->settings.contains(setting))
            setting=this->enviroment;
        return setting;
    }

    //!
    //! \brief insert
    //! \param value
    //! \return
    //!
    ConnectionManager &insert(const QVariantHash &value)
    {
        auto &p=*this;
        if(value.isEmpty())
            return *this->parent;
        auto name=value.value(__name).toByteArray().trimmed();
        if(name.isEmpty())
            return *this->parent;

        auto drivers=QSqlDatabase::drivers();
        auto setting=p.settings.value(name);
        if(setting==nullptr){
            setting=new ConnectionSetting(this);
            setting->fromHash(value);
            setting->setName(name);
            p.settings.insert(setting->name(), setting);
        }
        else{
            setting->fromHash(value);
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
        bool __return=false;

        if(vSetting.isEmpty()){
            this->clear();
            return false;
        }

//        if(vSetting.contains(__host) && vSetting.contains(__user)){
//            this->insert(vSetting);
//            return true;
//        }

        QByteArray enviroment = getenv(__DB_ENVIRONMENT);

        QStm::Envs envs;
        envs.customEnvs(__driver        , envs.value(__DB_DRIVER  ));
        envs.customEnvs(__hostName      , envs.value(__DB_HOST    ));
        envs.customEnvs(__userName      , envs.value(__DB_USER    ));
        envs.customEnvs(__password      , envs.value(__DB_PASSWORD));
        envs.customEnvs(__dataBaseName  , envs.value(__DB_DATABASE));
        envs.customEnvs(__connectOptions, envs.value(__DB_OPTION  ));
        envs.customEnvs(__port          , envs.value(__DB_PORT    ));
        envs.customEnvs(__schemaNames   , envs.value(__DB_SCHEMA  ));

        this->secret = vSetting.value(__secret).toByteArray().trimmed();
        this->enviroment = vSetting.value(__environment).toByteArray().trimmed();
        this->enviroment=enviroment.isEmpty()?this->enviroment:enviroment.trimmed();
        auto paramaters = vSetting.value(__paramaters).toHash();
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

            value.insert(__name, name);
            this->insert(value);
            __return=true;
        }
        return __return;
    }

};

ConnectionManager::ConnectionManager(QObject *parent) : QOrm::Object{parent}, p{new ConnectionManagerPvt{this}}
{
}

ConnectionManager::ConnectionManager(const ConnectionManager &manager, QObject *parent)
    : QStm::Object{parent}, p{new ConnectionManagerPvt{this}}
{
    p->load(manager.toHash());
}

ConnectionManager::ConnectionManager(const QVariantHash &setting, QObject *parent): QStm::Object{parent}, p{new ConnectionManagerPvt{this}}
{
    p->load(setting);
}

ConnectionManager &ConnectionManager::clear()
{
    p->clear();
    return *this;
}

const QByteArray &ConnectionManager::enviroment() const
{
    return p->enviroment;
}

ConnectionManager &ConnectionManager::setEnviroment(const QByteArray &value)
{
    p->enviroment = value.trimmed();
    return *this;
}

const QByteArray &ConnectionManager::secretKey() const
{
    return p->secret;
}

ConnectionManager &ConnectionManager::setSecretKey(const QByteArray &value)
{
    p->secret = value.trimmed();
    return *this;
}

const QVariantHash &ConnectionManager::paramaters() const
{
    p->parameters.clear();
    QHashIterator<QString, ConnectionSetting *> i(p->settings);
    while (i.hasNext()) {
        i.next();
        const auto k = i.key().trimmed().toLower();
        const auto &v = i.value();
        if (k.trimmed().isEmpty())
            continue;

        if (v->isValid())
            p->parameters.insert(k, v->toHash());
    }
    return p->parameters;
}

ConnectionManager &ConnectionManager::setParamaters(const QVariantHash &value)
{
    auto lst = p->settings.values();
    p->settings.clear();
    qDeleteAll(lst);
    for (auto &v : value) {
        switch (v.typeId()) {
        case QMetaType::QVariantHash:
        case QMetaType::QVariantMap:
            this->insert(v.toHash());
        default:
            break;
        }
    }
    return *this;
}

ConnectionManager &ConnectionManager::insert(ConnectionSetting &value)
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

ConnectionManager &ConnectionManager::operator<<(ConnectionSetting &value)
{
    return this->insert(value);
}

ConnectionManager &ConnectionManager::operator<<(const QVariantHash &value)
{
    return this->insert(value);
}

} // namespace QOrm
