#include "./qorm_connection_setting.h"
#include "./qorm_macro.h"
#include "../../qstm/src/qstm_envs.h"
#include <QJsonDocument>
#include <QMetaProperty>
#include <QStringList>

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
    ConnectionSetting *parent = nullptr;

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
    explicit ConnectionSettingPvt(ConnectionSetting *parent):QObject{parent}, parent{parent}, envs{parent}
    {
    }

    QVariantHash toHash()
    {
        if(!this->parent->isValid())
            return {};

        static const auto __objectName=QByteArrayLiteral("objectName");

        QVariantHash __return;

        for (int row = 0; row < this->parent->metaObject()->propertyCount(); ++row) {
            auto property = this->parent->metaObject()->property(row);
            if(!property.isReadable())
                continue;

            if (property.name() == __objectName)
                continue;

            auto vGet = property.read(this->parent);

            switch (vGet.typeId()) {
            case QMetaType::QString:
            case QMetaType::QByteArray:{
                if(vGet.toString().trimmed().isEmpty())
                    continue;
                break;
            }
            case QMetaType::Int:
            case QMetaType::UInt:
            case QMetaType::LongLong:
            case QMetaType::ULongLong:
            {
                bool ok=false;
                if(vGet.toLongLong(&ok)<=0 || !ok)
                    continue;
                break;
            }
            case QMetaType::Double:
            {
                bool ok=false;
                if(vGet.toDouble(&ok)<=0 || !ok)
                    continue;
                break;
            }
            case QMetaType::QVariantHash:
            case QMetaType::QVariantMap:
            case QMetaType::QVariantPair:
            {
                if(vGet.toHash().isEmpty())
                    continue;
                break;
            }
            case QMetaType::QVariantList:
            case QMetaType::QStringList:
            {
                if(vGet.toList().isEmpty())
                    continue;
                break;
            }
            default:
                break;
            }

            __return.insert(property.name(), vGet);
        }
        return __return;
    }

};

ConnectionSetting::ConnectionSetting(QObject *parent)
    :
    QObject{parent},
    p{new ConnectionSettingPvt{this}}
{
}

ConnectionSetting::ConnectionSetting(const QSqlDatabase &connection, QObject *parent)
    :
    QObject{parent},
    p{new ConnectionSettingPvt{this}}
{
    p->name = connection.connectionName().toUtf8();
    this->from(connection);
}

ConnectionSetting::ConnectionSetting(const QByteArray &name,
                                     const QVariantHash &detail,
                                     QObject *parent)
    :
    QObject{parent},
    p{new ConnectionSettingPvt{this}}
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

ConnectionSetting &ConnectionSetting::operator=(const QVariant &value)
{
    return this->from(value.toHash());
}

ConnectionSetting &ConnectionSetting::operator=(const QSqlDatabase &value)
{
    return this->from(value);
}

ConnectionSetting &ConnectionSetting::operator=(const ConnectionSetting &value)
{
    return this->from(value.toHash());
}

ConnectionSetting &ConnectionSetting::from(const ConnectionSetting &value)
{
    return this->from(value.toHash());
}

ConnectionSetting &ConnectionSetting::from(const QVariant &value)
{
    QVariantHash values;

    switch (value.typeId()) {
    case QMetaType::QVariantHash:
    case QMetaType::QVariantMap:
        values=value.toHash();
        break;
    case QMetaType::QString:
    case QMetaType::QByteArray:
        values=QJsonDocument::fromJson(value.toByteArray()).toVariant().toHash();
        break;
    default:
        break;
    }

    static const auto __objectName=QByteArrayLiteral("objectName");
    QVariantHash vHash;
    QHashIterator<QString, QVariant> i(values);
    while (i.hasNext()) {
        i.next();
        vHash.insert(i.key().toLower(), i.value());
    }

    auto &metaObject = *this->metaObject();
    for (int row = 0; row < metaObject.propertyCount(); ++row) {
        auto property = metaObject.property(row);
        if(!property.isWritable())
            continue;

        if(property.name()==__objectName)
            continue;

        QString key = property.name();
        auto value = vHash.value(key.toLower());

        if (value.isNull() || !value.isValid())
            continue;

        QVariant v;
        if(property.isResettable() && (!value.isValid() || v.isNull()) && property.reset(this))
            continue;
        else if(property.typeId()==value.typeId() && property.write(this, value))
            continue;
        else{
            switch (property.typeId()) {
            case QMetaType::QUuid:
                v=value.toUuid();
                break;
            case QMetaType::QUrl:
                v=value.toUrl();
                break;
            case QMetaType::Int:
                v=value.toInt();
                break;
            case QMetaType::UInt:
                v=value.toUInt();
                break;
            case QMetaType::LongLong:
                v=value.toLongLong();
                break;
            case QMetaType::ULongLong:
                v=value.toULongLong();
                break;
            case QMetaType::Double:
                v=value.toDouble();
                break;
            case QMetaType::QVariantHash:{
                v = QJsonDocument::fromJson(value.toByteArray()).toVariant();
                break;
            }
            case QMetaType::QVariantMap: {
                v = QJsonDocument::fromJson(value.toByteArray()).toVariant();
                break;
            }
            case QMetaType::QVariantList:{
                v = QJsonDocument::fromJson(value.toByteArray()).toVariant().toList();
                break;
            }
            case QMetaType::QStringList: {
                v = QJsonDocument::fromJson(value.toByteArray()).toVariant().toStringList();
                break;
            }
            default:
                v=value;
                break;
            }
            if(!property.write(this, v))
                qWarning()<<QStringLiteral("invalid write: property[%1]").arg(property.name());
        }
    }
    return *this;
}

ConnectionSetting &ConnectionSetting::from(const QSqlDatabase &value)
{
    auto vHash=QVariantHash{
                            {__driver, value.driverName()},
                            {__dataBaseName, value.databaseName()},
                            {__userName, value.userName()},
                            {__password, value.password()},
                            {__hostName, value.hostName()},
                            {__port, value.port()},
                            {__connectOptions, value.connectOptions()},
                            };
    return this->from(vHash);
}

const ConnectionSetting &ConnectionSetting::print() const
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
    if(this->name().trimmed().isEmpty() && this->hostName().trimmed().isEmpty() && this->dataBaseName().trimmed().isEmpty())
        return {};

    static const auto __objectName=QByteArrayLiteral("objectName");

    for (int row = 0; row < this->metaObject()->propertyCount(); ++row) {
        auto property = this->metaObject()->property(row);
        if(!property.isReadable())
            continue;

        if (property.name() == __objectName)
            continue;

        auto vGet = property.read(this);

        switch (vGet.typeId()) {
        case QMetaType::QString:
        case QMetaType::QByteArray:{
            if(!vGet.toString().trimmed().isEmpty())
                return true;
            break;
        }
        case QMetaType::Int:
        case QMetaType::UInt:
        case QMetaType::LongLong:
        case QMetaType::ULongLong:
        {
            bool ok=false;
            if(vGet.toLongLong(&ok)>=0 && ok)
                return true;
            break;
        }
        case QMetaType::Double:
        {
            bool ok=false;
            if(vGet.toDouble(&ok)>=0 && ok)
                return true;
            break;
        }
        case QMetaType::QVariantHash:
        case QMetaType::QVariantMap:
        case QMetaType::QVariantPair:
        {
            if(!vGet.toHash().isEmpty())
                return true;
            break;
        }
        case QMetaType::QVariantList:
        case QMetaType::QStringList:
        {
            if(!vGet.toList().isEmpty())
                return true;
            break;
        }
        default:
            break;
        }
    }
    return false;
}


const QVariantHash ConnectionSetting::toHash() const
{
    return p->toHash();
}

const QByteArray ConnectionSetting::toJson()
{
    return QJsonDocument::fromVariant(p->toHash())
        .toJson(QJsonDocument::Compact);
}

const QVariantHash &ConnectionSetting::variables() const
{
    return p->envs.customEnvs();
}

ConnectionSetting &ConnectionSetting::setVariables(const QVariantHash &value)
{
    p->envs.customEnvs(value);
    return *this;
}

const QByteArray ConnectionSetting::driver() const
{
    return p->envs.parser(p->driver).toByteArray();
}

ConnectionSetting &ConnectionSetting::setDriver(const QByteArray &value)
{
    p->driver = value;
    return *this;
}

const QByteArray ConnectionSetting::name() const
{
    return p->envs.parser(p->name).toByteArray();
}

ConnectionSetting &ConnectionSetting::setName(const QByteArray &value)
{
    p->name = value.trimmed();
    return *this;
}

const QByteArray ConnectionSetting::hostName() const
{
    return p->envs.parser(p->hostName.trimmed()).toByteArray();
}

ConnectionSetting &ConnectionSetting::setHostName(const QByteArray &value)
{
    p->hostName = value.trimmed();
    return *this;
}

const QByteArray ConnectionSetting::userName() const
{
    return p->envs.parser(p->userName.trimmed()).toByteArray();
}

ConnectionSetting &ConnectionSetting::setUserName(const QByteArray &value)
{
    p->userName = value.trimmed();
    return *this;
}

const QByteArray ConnectionSetting::password() const
{
    return p->envs.parser(p->password).toByteArray();
}

ConnectionSetting &ConnectionSetting::setPassword(const QByteArray &value)
{
    p->password = value.trimmed();
    return *this;
}

int ConnectionSetting::port() const
{
    QVariant v=p->envs.parser(p->port.toString());
    if(!v.isValid())
        return -1;
    return v.toInt();
}

ConnectionSetting &ConnectionSetting::setPort(const QVariant &value)
{
    p->port = value;
    return *this;
}

const QByteArray ConnectionSetting::dataBaseName() const
{
    return p->envs.parser(p->dataBaseName).toByteArray();
}

ConnectionSetting &ConnectionSetting::setDataBaseName(const QByteArray &value)
{
    p->dataBaseName = value;
    return *this;
}

const QStringList ConnectionSetting::schemaNames() const
{
    auto lst=p->envs.parser(p->schemaNames).toStringList();
    QStringList __return;
    for(auto &v: lst){
        if(!v.trimmed().isEmpty())
            __return.append(v.trimmed());
    }
    return __return;
}

ConnectionSetting &ConnectionSetting::setSchemaNames(const QStringList &value)
{
    p->schemaNames = value;
    return *this;
}

const QByteArray ConnectionSetting::connectOptions() const
{
    return p->envs.parser(p->commandBeforeOpen).toByteArray();
}

ConnectionSetting &ConnectionSetting::setConnectOptions(const QByteArray &value)
{
    p->connectOptions = value;
    return *this;
}

const QStringList ConnectionSetting::commandBeforeOpen() const
{
    return p->envs.parser(p->commandBeforeOpen).toStringList();
}

ConnectionSetting &ConnectionSetting::setCommandBeforeOpen(const QStringList &value)
{
    p->commandBeforeOpen = value;
    return *this;
}

const QStringList ConnectionSetting::commandAfterClose() const
{
    return p->envs.parser(p->commandAfterClose).toStringList();
}

ConnectionSetting &ConnectionSetting::setCommandAfterClose(const QStringList &value)
{
    p->commandAfterClose = value;
    return *this;
}

} // namespace QOrm
