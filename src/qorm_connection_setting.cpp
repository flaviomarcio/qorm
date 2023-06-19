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

static const auto __driver="driver";
static const auto __dataBaseName="dataBaseName";
static const auto __userName="userName";
static const auto __password="password";
static const auto __hostName="hostName";
static const auto __port="port";
static const auto __connectOptions="connectOptions";

class ConnectionSettingPvt:public QObject
{
public:
    QObject *parent = nullptr;
    QByteArray driver;
    QByteArray name;
    QByteArray hostName;
    QByteArray userName;
    QByteArray password;
    QByteArray dataBaseName;
    QVariant port = -1;
    QStringList schemaNames;
    QByteArray connectOptions;
    QStringList commandBeforeOpen;
    QStringList commandAfterClose;
    QStm::Envs envs;
    explicit ConnectionSettingPvt(QObject *parent):QObject{parent}, parent{parent}, envs{parent} {}

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
    p->name = connection.connectionName().toUtf8();
    this->fromConnection(connection);
}

ConnectionSetting::ConnectionSetting(const ConnectionSetting &setting, QObject *parent)
    : QObject{parent}, p{new ConnectionSettingPvt{this}}
{
    if(setting.isValid()){
        p->name = setting.name();
        this->fromSetting(setting);
    }
}

ConnectionSetting::ConnectionSetting(const QByteArray &name,
                                     const QVariantHash &detail,
                                     QObject *parent)
    : QObject{parent}, p{new ConnectionSettingPvt{this}}
{
    auto _name = name.trimmed();

    if (_name.isEmpty()) {
        srand(time(NULL));
        auto r = rand() % 100000000 + 1000;
        _name = QStringLiteral("connection%1").arg(r).toUtf8();
    }

    p->name = _name;
    auto metaObject = ConnectionSetting::staticMetaObject;
    for (int row = 0; row < metaObject.propertyCount(); ++row) {
        auto property = metaObject.property(row);
        auto vGet = detail.value(property.name());
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

ConnectionSetting &ConnectionSetting::fromHash(const QVariantHash &values)
{
    QVariantHash vHash;
    QHashIterator<QString, QVariant> i(values);
    while (i.hasNext()) {
        i.next();
        vHash.insert(i.key().toLower(), i.value());
    }

    auto &metaObject = *this->metaObject();
    for (int row = 0; row < metaObject.propertyCount(); ++row) {
        auto property = metaObject.property(row);
        QString key = property.name();
        auto value = vHash.value(key.toLower());

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
    auto vHash=QVariantHash{
                            {__driver, connection.driverName()},
                            {__dataBaseName, connection.databaseName()},
                            {__userName, connection.userName()},
                            {__password, connection.password()},
                            {__hostName, connection.hostName()},
                            {__port, connection.port()},
                            {__connectOptions, connection.connectOptions()},
                            };
    return this->fromHash(vHash);
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
    return p->replaceVar(p->driver);
}

void ConnectionSetting::setDriver(const QByteArray &value)
{
    p->driver = value;
}

QByteArray ConnectionSetting::name() const
{
    return p->replaceVar(p->name);
}

void ConnectionSetting::setName(const QByteArray &value)
{
    p->name = value.trimmed();
}

QByteArray ConnectionSetting::hostName() const
{
    return p->replaceVar(p->hostName.trimmed());
}

void ConnectionSetting::setHostName(const QByteArray &value)
{
    p->hostName = value.trimmed();
}

QByteArray ConnectionSetting::userName() const
{
    return p->replaceVar(p->userName.trimmed());
}

void ConnectionSetting::setUserName(const QByteArray &value)
{
    p->userName = value.trimmed();
}

QByteArray ConnectionSetting::password() const
{
    return p->replaceVar(p->password);
}

void ConnectionSetting::setPassword(const QByteArray &value)
{
    p->password = value.trimmed();
}

int ConnectionSetting::port() const
{
    QVariant v=p->replaceVar(p->port.toString());
    if(!v.isValid())
        return -1;
    return v.toInt();
}

void ConnectionSetting::setPort(const QVariant &value)
{
    p->port = value;
}

QByteArray ConnectionSetting::dataBaseName() const
{
    return p->replaceVar(p->dataBaseName);
}

void ConnectionSetting::setDataBaseName(const QByteArray &value)
{
    p->dataBaseName = value;
}

QStringList ConnectionSetting::schemaNames() const
{
    QString v=p->replaceVar(p->schemaNames.join(":"));
    auto lst=v.split(":");
    QStringList __return;
    for(auto&v: lst){
        if(!v.trimmed().isEmpty())
            __return.append(v.trimmed());
    }
    return __return;
}

void ConnectionSetting::setSchemaNames(const QStringList &value)
{
    p->schemaNames = value;
}

QByteArray ConnectionSetting::connectOptions() const
{
    return p->replaceVar(p->connectOptions);
}

void ConnectionSetting::setConnectOptions(const QByteArray &value)
{
    p->connectOptions = value;
}

QStringList ConnectionSetting::commandBeforeOpen() const
{
    return p->commandBeforeOpen;
}

void ConnectionSetting::setCommandBeforeOpen(const QStringList &value)
{
    p->commandBeforeOpen = value;
}

QStringList ConnectionSetting::commandAfterClose() const
{
    return p->commandAfterClose;
}

void ConnectionSetting::setCommandAfterClose(const QStringList &value)
{
    p->commandAfterClose = value;
}

} // namespace QOrm
