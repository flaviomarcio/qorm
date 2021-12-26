#include "./qorm_connection_setting.h"
#include "./qorm_macro.h"
#include <QFile>
#include <QSettings>
#include <QStringList>
#include <QMetaMethod>
#include <QMetaProperty>
#include <QDebug>
#include <QJsonDocument>

namespace QOrm {

#define dPvt()\
    auto&p = *reinterpret_cast<ConnectionSettingPrv*>(this->p)

class ConnectionSettingPrv{
public:
    QObject*parent=nullptr;
    QVariantHash variables;
    QByteArray driver;
    QByteArray name;
    QByteArray hostName;
    QByteArray userName;
    QByteArray password;
    QByteArray dataBaseName;
    int port=-1;
    QStringList schemaNames;
    QByteArray connectOptions;
    QStringList commandBeforeOpen;
    QStringList commandAfterClose;
    explicit ConnectionSettingPrv(QObject*parent)
    {
        this->parent=parent;
    }
    virtual ~ConnectionSettingPrv()
    {
    }

    QByteArray replaceVar(const QString&value)const
    {
        auto v=value;
        QHashIterator<QString, QVariant> i(this->variables);
        while (i.hasNext()) {
            i.next();
            auto iK=qsl("{$%1}").arg(i.key()).toUtf8();
            auto vV=i.value().toByteArray();
            v=v.replace(iK, vV);
        }
        return v.toUtf8();
    }

};


ConnectionSetting::ConnectionSetting(QObject *parent):QObject(parent)
{
    this->p = new ConnectionSettingPrv(this);
}

ConnectionSetting::ConnectionSetting(const QSqlDatabase &detail, QObject *parent):QObject(parent)
{
    this->p = new ConnectionSettingPrv(this);
    dPvt();
    p.name=detail.connectionName().toUtf8();
    this->fromConnection(detail);
}

ConnectionSetting::ConnectionSetting(const ConnectionSetting &detail, QObject *parent):QObject(parent)
{
    this->p = new ConnectionSettingPrv(this);
    auto name=detail.name();
    dPvt();
    p.name=name;
    this->fromSetting(detail);
}

ConnectionSetting::ConnectionSetting(const QByteArray&name, const QVariantHash &detailMap, QObject *parent):QObject(parent)
{
    this->p = new ConnectionSettingPrv(this);

    auto _name=name.trimmed();

    if(_name.isEmpty()){
        srand(time(NULL));
        auto r=rand() % 100000000 + 1000;
        _name=qsl("connection%1").arg(r).toUtf8();
    }
    dPvt();
    p.name=_name;
    auto metaObject=ConnectionSetting::staticMetaObject;
    for (int row = 0; row < metaObject.propertyCount(); ++row) {
        auto property = metaObject.property(row);
        auto vGet=detailMap.value(property.name());
        if(!vGet.isValid())
            continue;
        property.write(this, vGet);
    }
}

ConnectionSetting::~ConnectionSetting()
{
    dPvt();delete&p;
}

void ConnectionSetting::printLog()
{
    QStringList lst;
    int len=0;
    for (int row = 0; row < this->metaObject()->propertyCount(); ++row) {
        auto p=this->metaObject()->property(row);
        auto v=p.read(this);
        auto s=qsl("%1:%2").arg(QString::fromUtf8(p.name()), v.toString()).trimmed();
        lst<<s;
        len=s.length()>len?s.length():len;
    }
    len+=10;
    sWarning()<<qsl("=").leftJustified(len,'=');
    for(auto&s:lst){
        sWarning()<<qsl("+%1+").arg(s.leftJustified(len-2,' '));
    }
    sWarning()<<qsl("=").leftJustified(len,'=');
}

bool ConnectionSetting::isValid()const
{
    for (int row = 0; row < this->metaObject()->propertyCount(); ++row) {
        auto property=this->metaObject()->property(row);
        if(QByteArray(property.name())==QT_STRINGIFY2(objectName))
            continue;

        auto vGet = property.read(this);
        if(!vGet.isValid())
            continue;

        switch (qTypeId(vGet)) {
        case QMetaType_QString:
        case QMetaType_QByteArray:
        {
            if(vGet.toString().trimmed().isEmpty())
                continue;
            break;
        }
        case QMetaType_Double:
        case QMetaType_Int:
        case QMetaType_UInt:
        case QMetaType_LongLong:
        case QMetaType_ULongLong:
        {
            if(vGet.toLongLong()>0)
                continue;
            break;
        }
        case QMetaType_QVariantHash:
        case QMetaType_QVariantMap:
        {
            if(vGet.toHash().isEmpty())
                continue;
            break;
        }
        break;
        case QMetaType_QVariantList:
        case QMetaType_QStringList:
        {
            if(vGet.toList().isEmpty())
                continue;
            break;;
        }
        default:
            break;
        }
        return true;
    }
    return false;
}

ConnectionSetting&ConnectionSetting::fromSetting(const ConnectionSetting &setting)
{
    return this->fromMap(setting.toHash());
}

QVariantMap ConnectionSetting::toMap()const
{
    return QVariant(this->toHash()).toMap();
}

QVariantHash ConnectionSetting::toHash() const
{
    if(!this->isValid())
        return {};

    QVariantHash vHash;
    for (int row = 0; row < this->metaObject()->propertyCount(); ++row) {
        auto property=this->metaObject()->property(row);
        if(QByteArray(property.name())==QT_STRINGIFY2(objectName))
            continue;
        const auto value = property.read(this);
        if(value.isNull())
            continue;

        const auto key=property.name();
        vHash.insert(key, value);
    }
    return vHash;
}

ConnectionSetting &ConnectionSetting::fromMap(const QVariantHash &map)
{
    QVariantHash vMap;
    QHashIterator<QString, QVariant> i(map);
    while (i.hasNext()){
        i.next();
        vMap.insert(i.key().toLower(), i.value());
    }

    auto&metaObject = *this->metaObject();
    for (int row = 0; row < metaObject.propertyCount(); ++row) {
        auto property=metaObject.property(row);
        QString key = property.name();
        auto value = vMap.value(key.toLower());

        if(value.isNull() || !value.isValid())
            continue;

        if(property.write(this,value))
            continue;

        auto typeId=qTypeId(property);
        switch (typeId) {
        case QMetaType_Int:
        case QMetaType_UInt:
            property.write(this,value.toInt());
            break;
        case QMetaType_LongLong:
        case QMetaType_ULongLong:
            property.write(this,value.toLongLong());
            break;
        case QMetaType_Double:
            property.write(this,value.toDouble());
            break;
        case QMetaType_QVariantHash:
        case QMetaType_QVariantMap:
        case QMetaType_QVariantList:
        case QMetaType_QStringList:
        {
            auto v=QJsonDocument::fromJson(value.toByteArray()).toVariant();
            switch (typeId) {
            case QMetaType_QVariantHash:
                property.write(this, v.toHash());
                break;
            case QMetaType_QVariantMap:
                property.write(this, v.toMap());
                break;
            case QMetaType_QVariantList:
                property.write(this, v.toList());
                break;
            case QMetaType_QStringList:
                property.write(this, v.toStringList());
                break;
            default:
                property.write(this, v);
                break;
            }
            break;
        }
        default:
            break;
        }
    }
    return*this;
}

ConnectionSetting&ConnectionSetting::fromConnection(const QSqlDatabase &connection)
{
    QVariantHash vHash;
    vHash[qsl("driver")]=connection.driverName();
    vHash[qsl("dataBaseName")]=connection.databaseName();
    vHash[qsl("userName")]=connection.userName();
    vHash[qsl("password")]=connection.password();
    vHash[qsl("hostName")]=connection.hostName();
    vHash[qsl("port")]=connection.port();
    vHash[qsl("connectOptions")]=connection.connectOptions();
    return this->fromMap(vHash);
}

ConnectionSetting &ConnectionSetting::operator=(const QVariant &value)
{
    return this->fromMap(value.toHash());
}

QVariantHash ConnectionSetting::variables() const
{
    dPvt();
    return p.variables;
}

void ConnectionSetting::setVariables(const QVariantHash &value)
{
    dPvt();
    p.variables=value;
}

QByteArray ConnectionSetting::driver() const
{
    dPvt();
    return p.driver;
}

void ConnectionSetting::setDriver(const QByteArray &value)
{
    dPvt();
    p.driver=value;
}

QByteArray ConnectionSetting::name() const
{
    dPvt();
    return p.name;
}

void ConnectionSetting::setName(const QByteArray &value)
{
    dPvt();
    p.name = value.trimmed();
}

QByteArray ConnectionSetting::hostName() const
{
    dPvt();
    auto v=p.replaceVar(p.hostName.trimmed());
    return v;
}

void ConnectionSetting::setHostName(const QByteArray &value)
{
    dPvt();
    p.hostName = value.trimmed();
}

QByteArray ConnectionSetting::userName() const
{
    dPvt();
    auto v=p.replaceVar(p.userName.trimmed());
    return v;
}

void ConnectionSetting::setUserName(const QByteArray &value)
{
    dPvt();
    p.userName = value.trimmed();
}

QByteArray ConnectionSetting::password() const
{
    dPvt();
    auto v=p.replaceVar(p.password);
    return v;
}

void ConnectionSetting::setPassword(const QByteArray &value)
{
    dPvt();
    p.password = value.trimmed();
}

int ConnectionSetting::port() const
{
    dPvt();
    return p.port;
}

void ConnectionSetting::setPort(int value)
{
    dPvt();
    p.port = value;
}

QByteArray ConnectionSetting::dataBaseName() const
{
    dPvt();
    return p.dataBaseName;
}

void ConnectionSetting::setDataBaseName(const QByteArray &value)
{
    dPvt();
    p.dataBaseName = value;
}

QStringList ConnectionSetting::schemaNames() const
{
    dPvt();
    return p.schemaNames;
}

void ConnectionSetting::setSchemaNames(const QStringList &value)
{
    dPvt();
    p.schemaNames = value;
}

QByteArray ConnectionSetting::connectOptions() const
{
    dPvt();
    return p.connectOptions;
}

void ConnectionSetting::setConnectOptions(const QByteArray &value)
{
    dPvt();
    p.connectOptions = value;
}

QStringList ConnectionSetting::commandBeforeOpen() const
{
    dPvt();
    return p.commandBeforeOpen;
}

void ConnectionSetting::setCommandBeforeOpen(const QStringList &value)
{
    dPvt();
    p.commandBeforeOpen = value;
}

QStringList ConnectionSetting::commandAfterClose() const
{
    dPvt();
    return p.commandAfterClose;
}

void ConnectionSetting::setCommandAfterClose(const QStringList &value)
{
    dPvt();
    p.commandAfterClose = value;
}

}
