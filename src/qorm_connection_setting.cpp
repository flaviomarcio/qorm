#include "./qorm_connection_setting.h"
#include "./qorm_macro.h"
#include <QDebug>
#include <QFile>
#include <QJsonDocument>
#include <QMetaMethod>
#include <QMetaProperty>
#include <QSettings>
#include <QStringList>

namespace QOrm {

class ConnectionSettingPvt:public QObject
{
public:
    QObject *parent = nullptr;
    QVariantHash _variables;
    QByteArray _driver;
    QByteArray _name;
    QByteArray _hostName;
    QByteArray _userName;
    QByteArray _password;
    QByteArray _dataBaseName;
    int _port = -1;
    QStringList _schemaNames;
    QByteArray _connectOptions;
    QStringList _commandBeforeOpen;
    QStringList _commandAfterClose;

    explicit ConnectionSettingPvt(QObject *parent):QObject{parent} { this->parent = parent; }

    virtual ~ConnectionSettingPvt() {}

    QByteArray replaceVar(const QString &value) const
    {
        auto v = value;
        QHashIterator<QString, QVariant> i(this->_variables);
        while (i.hasNext()) {
            i.next();
            auto iK = QStringLiteral("{$%1}").arg(i.key()).toUtf8();
            auto vV = i.value().toByteArray();
            v = v.replace(iK, vV);
        }
        return v.toUtf8();
    }
};

ConnectionSetting::ConnectionSetting(QObject *parent) : QObject{parent}
{
    this->p = new ConnectionSettingPvt{this};
}

ConnectionSetting::ConnectionSetting(const QSqlDatabase &detail, QObject *parent) : QObject{parent}
{
    this->p = new ConnectionSettingPvt{this};

    p->_name = detail.connectionName().toUtf8();
    this->fromConnection(detail);
}

ConnectionSetting::ConnectionSetting(const ConnectionSetting &detail, QObject *parent)
    : QObject{parent}
{
    this->p = new ConnectionSettingPvt{this};
    auto name = detail.name();

    p->_name = name;
    this->fromSetting(detail);
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

ConnectionSetting::~ConnectionSetting()
{

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
    return this->fromMap(setting.toHash());
}

QVariantMap ConnectionSetting::toMap() const
{
    return QVariant(this->toHash()).toMap();
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

ConnectionSetting &ConnectionSetting::fromMap(const QVariantHash &map)
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
    return this->fromMap(map);
}

ConnectionSetting &ConnectionSetting::operator=(const QVariant &value)
{
    return this->fromMap(value.toHash());
}

QVariantHash ConnectionSetting::variables() const
{

    return p->_variables;
}

void ConnectionSetting::setVariables(const QVariantHash &value)
{

    p->_variables = value;
}

QByteArray ConnectionSetting::driver() const
{

    return p->_driver;
}

void ConnectionSetting::setDriver(const QByteArray &value)
{

    p->_driver = value;
}

QByteArray ConnectionSetting::name() const
{

    return p->_name;
}

void ConnectionSetting::setName(const QByteArray &value)
{

    p->_name = value.trimmed();
}

QByteArray ConnectionSetting::hostName() const
{

    auto v = p->replaceVar(p->_hostName.trimmed());
    return v;
}

void ConnectionSetting::setHostName(const QByteArray &value)
{

    p->_hostName = value.trimmed();
}

QByteArray ConnectionSetting::userName() const
{

    auto v = p->replaceVar(p->_userName.trimmed());
    return v;
}

void ConnectionSetting::setUserName(const QByteArray &value)
{

    p->_userName = value.trimmed();
}

QByteArray ConnectionSetting::password() const
{

    auto v = p->replaceVar(p->_password);
    return v;
}

void ConnectionSetting::setPassword(const QByteArray &value)
{

    p->_password = value.trimmed();
}

int ConnectionSetting::port() const
{

    return p->_port;
}

void ConnectionSetting::setPort(int value)
{

    p->_port = value;
}

QByteArray ConnectionSetting::dataBaseName() const
{

    return p->_dataBaseName;
}

void ConnectionSetting::setDataBaseName(const QByteArray &value)
{

    p->_dataBaseName = value;
}

QStringList ConnectionSetting::schemaNames() const
{

    return p->_schemaNames;
}

void ConnectionSetting::setSchemaNames(const QStringList &value)
{

    p->_schemaNames = value;
}

QByteArray ConnectionSetting::connectOptions() const
{

    return p->_connectOptions;
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
