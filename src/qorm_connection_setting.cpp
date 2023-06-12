#include "./qorm_connection_setting.h"
#include "./qorm_macro.h"
#include "../../qstm/src/qstm_envs.h"
#include <QDebug>
#include <QFile>
#include <QJsonDocument>
#include <QMetaMethod>
#include <QMetaProperty>
#include <QSettings>
#include <QStringList>
#include <QStandardPaths>
#include <QCoreApplication>
#include <QProcess>

namespace QOrm {

class ConnectionSettingPvt:public QObject
{
public:
    QObject *parent = nullptr;
    QByteArray _driver;
    QByteArray _name;
    QByteArray _hostName;
    QByteArray _userName;
    QByteArray _password;
    QByteArray _dataBaseName;
    QVariant _port = -1;
    QStringList _schemaNames;
    QByteArray _connectOptions;
    QStringList _commandBeforeOpen;
    QStringList _commandAfterClose;

    QStm::Envs envs;

    explicit ConnectionSettingPvt(QObject *parent):QObject{parent} { this->parent = parent; }

    QByteArray replaceVar(const QString &value) const
    {
        return envs.parser(value).toByteArray();
    }
};

ConnectionSetting::ConnectionSetting(QObject *parent)
    : QObject{parent}, p{new ConnectionSettingPvt{this}}
{
}

ConnectionSetting::ConnectionSetting(const QSqlDatabase &connection, QObject *parent)
    : QObject{parent}, p{new ConnectionSettingPvt{this}}
{
    p->_name = connection.connectionName().toUtf8();
    this->fromConnection(connection);
}

ConnectionSetting::ConnectionSetting(const ConnectionSetting &setting, QObject *parent)
    : QObject{parent}, p{new ConnectionSettingPvt{this}}
{
    if(setting.isValid()){
        p->_name = setting.name();
        this->fromSetting(setting);
    }
}

ConnectionSetting::ConnectionSetting(const QByteArray &name,
                                     const QVariantHash &detailMap,
                                     QObject *parent)
    : QObject{parent}
{
    this->p = new ConnectionSettingPvt{this};

    auto _name = name.trimmed();

    if (_name.isEmpty()) {
        srand(time(NULL));
        auto r = rand() % 100000000 + 1000;
        _name = QStringLiteral("connection%1").arg(r).toUtf8();
    }

    p->_name = _name;
    auto metaObject = ConnectionSetting::staticMetaObject;
    for (int row = 0; row < metaObject.propertyCount(); ++row) {
        auto property = metaObject.property(row);
        auto vGet = detailMap.value(property.name());
        if (vGet.isValid())
            property.write(this, vGet);

    }
}

ConnectionSetting &ConnectionSetting::printLog()
{
    QStringList lst;
    int len = 0;
    for (int row = 0; row < this->metaObject()->propertyCount(); ++row) {
        auto p = this->metaObject()->property(row);
        auto v = p.read(this);
        auto s = QStringLiteral("%1:%2").arg(QString::fromUtf8(p.name()), v.toString()).trimmed();
        lst << s;
        len = s.length() > len ? s.length() : len;
    }
    len += 10;
    oWarning() << QStringLiteral("=").leftJustified(len, '=');
    for (auto &s : lst) {
        oWarning() << QStringLiteral("+%1+").arg(s.leftJustified(len - 2, ' '));
    }
    oWarning() << QStringLiteral("=").leftJustified(len, '=');
    return *this;
}

bool ConnectionSetting::isValid() const
{
    if(this->name().trimmed().isEmpty())
        return {};
    for (int row = 0; row < this->metaObject()->propertyCount(); ++row) {
        auto property = this->metaObject()->property(row);
        if (QByteArray{property.name()} == QT_STRINGIFY2(objectName))
            continue;

        auto vGet = property.read(this);
        if (!vGet.isValid())
            continue;
        //TODO switch () {}
        auto t = vGet.typeId();
        if ((t == QMetaType::QString || t == QMetaType::QByteArray)
            && vGet.toString().trimmed().isEmpty())
            continue;
        if ((t == QMetaType::Double || t == QMetaType::Int || t == QMetaType::UInt
             || t == QMetaType::LongLong || t == QMetaType::ULongLong)
            && vGet.toLongLong() > 0)
            continue;
        if ((t == QMetaType::QVariantHash || t == QMetaType::QVariantMap) && vGet.toHash().isEmpty())
            continue;
        if ((t == QMetaType::QVariantList || t == QMetaType::QStringList) && vGet.toList().isEmpty())
            continue;

        return true;
    }
    return false;
}

ConnectionSetting &ConnectionSetting::fromSetting(const ConnectionSetting &setting)
{
    return this->fromHash(setting.toHash());
}

QVariantHash ConnectionSetting::toHash() const
{
    if (!this->isValid())
        return {};

    QVariantHash __return;
    for (int row = 0; row < this->metaObject()->propertyCount(); ++row) {
        auto property = this->metaObject()->property(row);
        if (QByteArray{property.name()} == QT_STRINGIFY2(objectName))
            continue;

        const auto key = property.name();
        const auto value = property.read(this);
        if (!value.isNull())
            __return.insert(key, value);
    }
    return __return;
}

ConnectionSetting &ConnectionSetting::fromHash(const QVariantHash &map)
{
    QVariantHash vMap;
    QHashIterator<QString, QVariant> i(map);
    while (i.hasNext()) {
        i.next();
        vMap.insert(i.key().toLower(), i.value());
    }

    auto &metaObject = *this->metaObject();
    for (int row = 0; row < metaObject.propertyCount(); ++row) {
        auto property = metaObject.property(row);
        QString key = property.name();
        auto value = vMap.value(key.toLower());

        if (value.isNull() || !value.isValid())
            continue;

        if (property.write(this, value))
            continue;

        switch (property.typeId()) {
        case QMetaType::Int:
            property.write(this, value.toInt());
            break;
        case QMetaType::UInt:
            property.write(this, value.toInt());
            break;
        case QMetaType::LongLong:
            property.write(this, value.toLongLong());
            break;
        case QMetaType::ULongLong:
            property.write(this, value.toLongLong());
            break;
        case QMetaType::Double:
            property.write(this, value.toDouble());
            break;
        case QMetaType::QVariantHash:{
            auto v = QJsonDocument::fromJson(value.toByteArray()).toVariant();
            property.write(this, v.toHash());
            break;
        }
        case QMetaType::QVariantMap: {
            auto v = QJsonDocument::fromJson(value.toByteArray()).toVariant();
            property.write(this, v.toMap());
            break;
        }
        case QMetaType::QVariantList:{
            auto v = QJsonDocument::fromJson(value.toByteArray()).toVariant().toList();
            property.write(this, v);
            break;
        }
        case QMetaType::QStringList: {
            auto v = QJsonDocument::fromJson(value.toByteArray()).toVariant().toStringList();
            property.write(this, v);
            break;
        }
        default:
            break;
        }
    }
    return *this;
}

ConnectionSetting &ConnectionSetting::fromConnection(const QSqlDatabase &connection)
{
    QVariantHash map;
    map[QStringLiteral("driver")] = connection.driverName();
    map[QStringLiteral("dataBaseName")] = connection.databaseName();
    map[QStringLiteral("userName")] = connection.userName();
    map[QStringLiteral("password")] = connection.password();
    map[QStringLiteral("hostName")] = connection.hostName();
    map[QStringLiteral("port")] = connection.port();
    map[QStringLiteral("connectOptions")] = connection.connectOptions();
    return this->fromHash(map);
}

ConnectionSetting &ConnectionSetting::operator=(const QVariant &value)
{
    return this->fromHash(value.toHash());
}

QVariantHash ConnectionSetting::variables() const
{
    return p->envs.customEnvs();
}

void ConnectionSetting::setVariables(const QVariantHash &value)
{
    p->envs.customEnvs(value);
}

QByteArray ConnectionSetting::driver() const
{
    return p->replaceVar(p->_driver);
}

void ConnectionSetting::setDriver(const QByteArray &value)
{
    p->_driver = value;
}

QByteArray ConnectionSetting::name() const
{
    return p->replaceVar(p->_name);
}

void ConnectionSetting::setName(const QByteArray &value)
{
    p->_name = value.trimmed();
}

QByteArray ConnectionSetting::hostName() const
{
    return p->replaceVar(p->_hostName.trimmed());
}

void ConnectionSetting::setHostName(const QByteArray &value)
{
    p->_hostName = value.trimmed();
}

QByteArray ConnectionSetting::userName() const
{
    return p->replaceVar(p->_userName.trimmed());
}

void ConnectionSetting::setUserName(const QByteArray &value)
{
    p->_userName = value.trimmed();
}

QByteArray ConnectionSetting::password() const
{
    return p->replaceVar(p->_password);
}

void ConnectionSetting::setPassword(const QByteArray &value)
{
    p->_password = value.trimmed();
}

int ConnectionSetting::port() const
{
    QVariant v=p->replaceVar(p->_port.toString());
    if(!v.isValid())
        return -1;
    return v.toInt();
}

void ConnectionSetting::setPort(const QVariant &value)
{
    p->_port = value;
}

QByteArray ConnectionSetting::dataBaseName() const
{
    return p->replaceVar(p->_dataBaseName);
}

void ConnectionSetting::setDataBaseName(const QByteArray &value)
{
    p->_dataBaseName = value;
}

QStringList ConnectionSetting::schemaNames() const
{
    QString v=p->replaceVar(p->_schemaNames.join(":"));
    return v.split(":");
}

void ConnectionSetting::setSchemaNames(const QStringList &value)
{
    p->_schemaNames = value;
}

QByteArray ConnectionSetting::connectOptions() const
{
    return p->replaceVar(p->_connectOptions);
}

void ConnectionSetting::setConnectOptions(const QByteArray &value)
{
    p->_connectOptions = value;
}

QStringList ConnectionSetting::commandBeforeOpen() const
{
    return p->_commandBeforeOpen;
}

void ConnectionSetting::setCommandBeforeOpen(const QStringList &value)
{
    p->_commandBeforeOpen = value;
}

QStringList ConnectionSetting::commandAfterClose() const
{
    return p->_commandAfterClose;
}

void ConnectionSetting::setCommandAfterClose(const QStringList &value)
{
    p->_commandAfterClose = value;
}

} // namespace QOrm
