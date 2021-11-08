#include "./qorm_connection_manager.h"
#include "./private/p_qorm_connection_manager.h"

namespace QOrm {

ConnectionManager::ConnectionManager(QObject *parent) : QStm::Object(nullptr)
{
    this->p = new ConnectionManagerPrv(this);
    if(parent!=nullptr){
        if(parent->thread()!=this->thread() || parent->thread()!=QThread::currentThread())
            sWarning()<<"Invalid parent";
        else
            this->setParent(parent);
        dPvt();
        p.parentParent = parent;
        p.load(p.parentParent);
    }
}

ConnectionManager::ConnectionManager(ConnectionManager &manager, QObject *parent):QStm::Object(nullptr)
{
    this->p = new ConnectionManagerPrv(this);
    if(parent!=nullptr){
        if(parent->thread()!=this->thread() || parent->thread()!=QThread::currentThread())
            sWarning()<<"Invalid parent";
        else
            this->setParent(parent);
        dPvt();
        p.parentParent = parent;
        p.load(manager.toHash());
    }
}

ConnectionManager::ConnectionManager(const QVariant&setting, QObject *parent):QStm::Object(nullptr)
{
    this->p = new ConnectionManagerPrv(this);
    if(parent!=nullptr){
        if(parent->thread()!=this->thread() || parent->thread()!=QThread::currentThread())
            sWarning()<<"Invalid parent";
        else
            this->setParent(parent);
        dPvt();
        p.parentParent = parent;
        p.v_load(setting);
    }
}

ConnectionManager::~ConnectionManager()
{
    dPvt();delete&p;
}

void ConnectionManager::clear()
{
    dPvt();
    p.clear();
}

QByteArray ConnectionManager::enviroment() const
{
    dPvt();
    return p.enviroment;
}

void ConnectionManager::setEnviroment(const QByteArray &value)
{
    dPvt();
    p.enviroment = value;
}

QByteArray ConnectionManager::secretKey() const
{
    dPvt();
    return p.secret;
}

void ConnectionManager::setSecretKey(const QByteArray &value)
{
    dPvt();
    p.secret = value;
}

QVariantHash ConnectionManager::paramaters() const
{
    dPvt();
    QVariantHash paramaters;
    QHashIterator<QString, ConnectionSetting*> i(p.settings);
    while (i.hasNext()) {
        i.next();
        const auto k=i.key().trimmed().toLower();
        const auto&v=i.value();
        if(k.trimmed().isEmpty())
            continue;
        else if(v->isValid())
            paramaters.insert(k, v->toMap());
    }
    return paramaters;
}

void ConnectionManager::setParamaters(const QVariantHash &value)
{
    dPvt();
    auto lst=p.settings.values();
    qDeleteAll(lst);
    p.settings.clear();
    for(auto&v:value){
        if(v.typeId()==QMetaType::QVariantMap || v.typeId()==QMetaType::QVariantHash)
            this->insert(v.toHash());
    }
}

ConnectionManager &ConnectionManager::insert(ConnectionSetting &value)
{
    dPvt();
    return p.insert(value.toHash());
}

ConnectionManager &ConnectionManager::insert(const QVariantHash &value)
{
    dPvt();
    return p.insert(value);
}

ConnectionSetting&ConnectionManager::detail()
{
    dPvt();
    return this->detail(p.enviroment);
}

ConnectionSetting&ConnectionManager::detail(const QByteArray&value)
{
    dPvt();
    auto name=value;
    return p.detailGetCheck(name);
}

ConnectionPool &ConnectionManager::pool()
{
    dPvt();
    return p.pool(p.enviroment);
}

ConnectionPool &ConnectionManager::pool(const QByteArray &value)
{
    dPvt();
    return p.pool(value);
}

QVariantMap ConnectionManager::toMap() const
{
    if(this->isEmpty() || !this->isLoaded())
        return QVariantMap();
    else{
        QVariantMap RETURN;
        for (int row = 0; row < this->metaObject()->propertyCount(); ++row) {
            auto property=this->metaObject()->property(row);
            if(QByteArray(property.name())==QT_STRINGIFY2(objectName))
                continue;
            else{
                const auto key=property.name();
                const auto value = property.read(this);
                if(!value.isNull())
                    RETURN.insert(key, value);
            }
        }
        return RETURN;
    }
}

QVariantHash ConnectionManager::toHash() const
{
    if(this->isEmpty() || !this->isLoaded())
        return {};
    else{
        QVariantHash RETURN;
        for (int row = 0; row < this->metaObject()->propertyCount(); ++row) {
            auto property=this->metaObject()->property(row);
            if(QByteArray(property.name())==QT_STRINGIFY2(objectName))
                continue;
            else{
                const auto key=property.name();
                const auto value = property.read(this);
                if(!value.isNull())
                    RETURN.insert(key, value);
            }
        }
        return RETURN;
    }
}

bool ConnectionManager::isEmpty() const
{
    dPvt();
    return p.isEmpty();
}

bool ConnectionManager::isLoaded() const
{
    dPvt();
    return p.isLoaded();
}

bool ConnectionManager::load(const QVariant &settings)
{
    dPvt();
    return p.v_load(settings);
}

bool ConnectionManager::load(QObject *settingsObject)
{
    dPvt();
    return p.load(settingsObject);
}

bool ConnectionManager::load(const ConnectionManager &manager)
{
    dPvt();
    return p.load(manager.toHash());
}

QVariant ConnectionManager::settingsFileName()
{
    dPvt();
    return p.settingsFileName;
}

ConnectionNotify &ConnectionManager::notify()
{
    dPvt();
    return p.notify;
}

ConnectionManager &ConnectionManager::operator<<(ConnectionSetting &value)
{
    return this->insert(value);
}

ConnectionManager &ConnectionManager::operator<<(const QVariantHash &value)
{
    return this->insert(value);
}

}
