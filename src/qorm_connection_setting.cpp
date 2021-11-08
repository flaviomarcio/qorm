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
    QVariantHash _variables;
    QByteArray _driver;
    QByteArray _name;
    QByteArray _hostName;
    QByteArray _userName;
    QByteArray _password;
    QByteArray _dataBaseName;
    int _port=-1;
    QStringList _schemaNames;
    QByteArray _connectOptions;
    QStringList _commandBeforeOpen;
    QStringList _commandAfterClose;
    explicit ConnectionSettingPrv(QObject*parent){
        this->parent=parent;
    }
    virtual ~ConnectionSettingPrv(){
    }

    QByteArray replaceVar(const QString&value)const{
        auto v=value;
        QHashIterator<QString, QVariant> i(this->_variables);
        while (i.hasNext()) {
            i.next();
            auto iK=qsl("{$%1}").arg(i.key()).toUtf8();
            auto vV=i.value().toByteArray();
            v=v.replace(iK, vV);
        }
        return v.toUtf8();
    }

};


ConnectionSetting::ConnectionSetting(QObject *parent):QObject(parent){
    this->p = new ConnectionSettingPrv(this);
}

ConnectionSetting::ConnectionSetting(const QSqlDatabase &detail, QObject *parent):QObject(parent)
{
    this->p = new ConnectionSettingPrv(this);
    dPvt();
    p._name=detail.connectionName().toUtf8();
    this->fromConnection(detail);
}

ConnectionSetting::ConnectionSetting(const ConnectionSetting &detail, QObject *parent):QObject(parent){
    this->p = new ConnectionSettingPrv(this);
    auto name=detail.name();
    dPvt();
    p._name=name;
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
    p._name=_name;
    auto metaObject=ConnectionSetting::staticMetaObject;
    for (int row = 0; row < metaObject.propertyCount(); ++row) {
        auto property = metaObject.property(row);
        auto vGet=detailMap.value(property.name());
        if(vGet.isValid()){
            property.write(this, vGet);
        }
    }
}

ConnectionSetting::~ConnectionSetting(){
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
        else{
            auto vGet = property.read(this);
            if(vGet.isValid()){
                auto t=vGet.typeId();
                if((t==QMetaType::QString || t==QMetaType::QByteArray) && vGet.toString().trimmed().isEmpty())
                    continue;
                if((t==QMetaType::Double || t==QMetaType::Int || t==QMetaType::UInt || t==QMetaType::LongLong || t==QMetaType::ULongLong) && vGet.toLongLong()>0)
                    continue;
                if((t==QMetaType::QVariantHash || t==QMetaType::QVariantHash) && vGet.toHash().isEmpty())
                    continue;
                if((t==QMetaType::QVariantList || t==QMetaType::QStringList) && vGet.toList().isEmpty())
                    continue;
                
		return true;
            }
        }
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
        auto type=property.typeId();
        if(value.isNull() || !value.isValid())
            continue;
        else if(!property.write(this,value)){
            if(type==QMetaType::Int || type==QMetaType::UInt)
                property.write(this,value.toInt());
            else if(type==QMetaType::LongLong || type==QMetaType::ULongLong)
                property.write(this,value.toLongLong());
            else if(type==QMetaType::Double)
                property.write(this,value.toDouble());
            else if(type==QMetaType::QVariantHash){
                auto v=QJsonDocument::fromJson(value.toByteArray()).toVariant();
                property.write(this,v.toHash());
            }
            else if(type==QMetaType::QVariantMap){
                auto v=QJsonDocument::fromJson(value.toByteArray()).toVariant();
                property.write(this,v.toHash());
            }
            else if(type==QMetaType::QVariantList){
                auto v=QJsonDocument::fromJson(value.toByteArray()).toVariant();
                property.write(this,v.toList());
            }
            else if(type==QMetaType::QStringList){
                auto v=QJsonDocument::fromJson(value.toByteArray()).toVariant();
                property.write(this,v.toString());
            }
        }
    }
    return*this;
}

ConnectionSetting&ConnectionSetting::fromConnection(const QSqlDatabase &connection)
{
    QVariantHash map;
    map.insert(qsl("driver"        ) , connection.driverName    ());
    map.insert(qsl("dataBaseName"  ) , connection.databaseName  ());
    map.insert(qsl("userName"      ) , connection.userName      ());
    map.insert(qsl("password"      ) , connection.password      ());
    map.insert(qsl("hostName"      ) , connection.hostName      ());
    map.insert(qsl("port"          ) , connection.port          ());
    map.insert(qsl("connectOptions") , connection.connectOptions());

    return this->fromMap(map);
}

ConnectionSetting &ConnectionSetting::operator=(const QVariant &value)
{
    return this->fromMap(value.toHash());
}

QVariantHash ConnectionSetting::variables() const
{
    dPvt();
    return p._variables;
}

void ConnectionSetting::setVariables(const QVariantHash &value)
{
    dPvt();
    p._variables=value;
}

QByteArray ConnectionSetting::driver() const
{
    dPvt();
    return p._driver;
}

void ConnectionSetting::setDriver(const QByteArray &value)
{
    dPvt();
    p._driver=value;
}

QByteArray ConnectionSetting::name() const
{
    dPvt();
    return p._name;
}

void ConnectionSetting::setName(const QByteArray &value)
{
    dPvt();
    p._name = value.trimmed();
}

QByteArray ConnectionSetting::hostName() const
{
    dPvt();
    auto v=p.replaceVar(p._hostName.trimmed());
    return v;
}

void ConnectionSetting::setHostName(const QByteArray &value)
{
    dPvt();
    p._hostName = value.trimmed();
}

QByteArray ConnectionSetting::userName() const
{
    dPvt();
    auto v=p.replaceVar(p._userName.trimmed());
    return v;
}

void ConnectionSetting::setUserName(const QByteArray &value)
{
    dPvt();
    p._userName = value.trimmed();
}

QByteArray ConnectionSetting::password() const
{
    dPvt();
    auto v=p.replaceVar(p._password);
    return v;
}

void ConnectionSetting::setPassword(const QByteArray &value)
{
    dPvt();
    p._password = value.trimmed();
}

int ConnectionSetting::port() const
{
    dPvt();
    return p._port;
}

void ConnectionSetting::setPort(int value)
{
    dPvt();
    p._port = value;
}

QByteArray ConnectionSetting::dataBaseName() const
{
    dPvt();
    return p._dataBaseName;
}

void ConnectionSetting::setDataBaseName(const QByteArray &value)
{
    dPvt();
    p._dataBaseName = value;
}

QStringList ConnectionSetting::schemaNames() const
{
    dPvt();
    return p._schemaNames;
}

void ConnectionSetting::setSchemaNames(const QStringList &value)
{
    dPvt();
    p._schemaNames = value;
}

QByteArray ConnectionSetting::connectOptions() const
{
    dPvt();
    return p._connectOptions;
}

void ConnectionSetting::setConnectOptions(const QByteArray &value)
{
    dPvt();
    p._connectOptions = value;
}

QStringList ConnectionSetting::commandBeforeOpen() const
{
    dPvt();
    return p._commandBeforeOpen;
}

void ConnectionSetting::setCommandBeforeOpen(const QStringList &value)
{
    dPvt();
    p._commandBeforeOpen = value;
}

QStringList ConnectionSetting::commandAfterClose() const
{
    dPvt();
    return p._commandAfterClose;
}

void ConnectionSetting::setCommandAfterClose(const QStringList &value)
{
    dPvt();
    p._commandAfterClose = value;
}

}
