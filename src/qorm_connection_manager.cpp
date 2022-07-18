#include "./qorm_connection_manager.h"
#include "./private/p_qorm_connection_manager.h"
#include "./qorm_macro.h"
#include <QMetaProperty>

namespace QOrm {

ConnectionManager::ConnectionManager(QObject *parent) : QOrm::Object(nullptr)
{
    this->p = new ConnectionManagerPvt{this};
    if (parent == nullptr)
        return;
    if (parent->thread() != this->thread() || parent->thread() != QThread::currentThread())
        oWarning() << "Invalid parent";
    else
        this->setParent(parent);

    p->parentParent = parent;
    p->load(p->parentParent);
}

ConnectionManager::ConnectionManager(ConnectionManager &manager, QObject *parent)
    : QStm::Object(nullptr)
{
    this->p = new ConnectionManagerPvt{this};
    if (parent == nullptr)
        return;

    if (parent->thread() != this->thread() || parent->thread() != QThread::currentThread())
        oWarning() << "Invalid parent";
    else
        this->setParent(parent);

    p->parentParent = parent;
    p->load(manager.toHash());
}

ConnectionManager::ConnectionManager(const QVariant &setting, QObject *parent)
    : QStm::Object(nullptr)
{
    this->p = new ConnectionManagerPvt{this};
    if (parent == nullptr)
        return;
    if (parent->thread() != this->thread() || parent->thread() != QThread::currentThread())
        oWarning() << "Invalid parent";
    else
        this->setParent(parent);

    p->parentParent = parent;
    p->v_load(setting);
}

void ConnectionManager::clear()
{
    p->clear();
}

QByteArray ConnectionManager::enviroment() const
{
    return p->enviroment;
}

void ConnectionManager::setEnviroment(const QByteArray &value)
{
    p->enviroment = value;
}

QByteArray ConnectionManager::secretKey() const
{
    return p->secret;
}

void ConnectionManager::setSecretKey(const QByteArray &value)
{
    p->secret = value;
}

QVariantHash ConnectionManager::paramaters() const
{
    QVariantHash paramaters;
    QHashIterator<QString, ConnectionSetting *> i(p->settings);
    while (i.hasNext()) {
        i.next();
        const auto k = i.key().trimmed().toLower();
        const auto &v = i.value();
        if (k.trimmed().isEmpty())
            continue;

        if (v->isValid())
            paramaters.insert(k, v->toMap());
    }
    return paramaters;
}

void ConnectionManager::setParamaters(const QVariantHash &value)
{
    auto lst = p->settings.values();
    qDeleteAll(lst);
    p->settings.clear();
    for (auto &v : value) {
        switch (v.typeId()) {
        case QMetaType::QVariantHash:
        case QMetaType::QVariantMap:
            this->insert(v.toHash());
        default:
            break;
        }
    }
}

ConnectionManager &ConnectionManager::insert(ConnectionSetting &value)
{
    return p->insert(value.toHash());
}

ConnectionManager &ConnectionManager::insert(const QVariantHash &value)
{
    return p->insert(value);
}

ConnectionSetting &ConnectionManager::detail()
{
    return this->detail(p->enviroment);
}

ConnectionSetting &ConnectionManager::detail(const QByteArray &value)
{
    auto name = value;
    return p->detailGetCheck(name);
}

ConnectionPool &ConnectionManager::pool()
{
    return p->pool(p->enviroment);
}

ConnectionPool &ConnectionManager::pool(const QByteArray &value)
{
    return p->pool(value);
}

QVariantMap ConnectionManager::toMap() const
{
    if (this->isEmpty() || !this->isLoaded())
        return {};

    QVariantMap RETURN;
    for (int row = 0; row < this->metaObject()->propertyCount(); ++row) {
        auto property = this->metaObject()->property(row);
        if (QByteArray{property.name()} == QT_STRINGIFY2(objectName))
            continue;

        const auto key = property.name();
        const auto value = property.read(this);
        if (!value.isNull())
            RETURN.insert(key, value);
    }
    return RETURN;
}

QVariantHash ConnectionManager::toHash() const
{
    if (this->isEmpty() || !this->isLoaded())
        return {};

    QVariantHash RETURN;
    for (int row = 0; row < this->metaObject()->propertyCount(); ++row) {
        auto property = this->metaObject()->property(row);
        if (QByteArray{property.name()} == QT_STRINGIFY2(objectName))
            continue;

        const auto key = property.name();
        const auto value = property.read(this);
        if (!value.isNull())
            RETURN.insert(key, value);
    }
    return RETURN;
}

bool ConnectionManager::isEmpty() const
{
    return p->isEmpty();
}

bool ConnectionManager::isLoaded() const
{
    return p->isLoaded();
}

bool ConnectionManager::load(const QVariant &settings)
{
    return p->v_load(settings);
}

bool ConnectionManager::load(QObject *settingsObject)
{
    return p->load(settingsObject);
}

bool ConnectionManager::load(const ConnectionManager &manager)
{
    return p->load(manager.toHash());
}

QVariant ConnectionManager::settingsFileName()
{
    return p->settingsFileName;
}

bool ConnectionManager::setSettingsFileName(const QString &fileName)
{
    return p->load(fileName);
}

ConnectionNotify &ConnectionManager::notify()
{
    return p->notify;
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
