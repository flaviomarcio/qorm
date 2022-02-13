#pragma once

#include "../qorm_connection_manager.h"
#include "../../../qstm/src/qstm_types.h"
#include <QFile>
#include <QDir>
#include <QSettings>
#include <QStringList>
#include <QVariantHash>
#include <QVariantList>
#include <QVariantHash>
#include <QMetaMethod>
#include <QMetaProperty>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QGenericArgument>

namespace QOrm {

#define dPvt()\
    auto&p = *reinterpret_cast<ConnectionManagerPrv*>(this->p)

class ConnectionManagerPrv{
public:
    ConnectionNotify notify;
    QVariant settingsFileName;
    QByteArray enviroment;
    QByteArray secret;
    QHash<QString, ConnectionSetting*> settings;
    QMap<QString, ConnectionPool*> pools;
    ConnectionManager*parent=nullptr;
    QObject*parentParent=nullptr;
    ConnectionPool defaultPool;
    explicit ConnectionManagerPrv(ConnectionManager*parent);
    virtual ~ConnectionManagerPrv();

    void init();

    ConnectionPool&pool(const QByteArray &value);

    bool isLoaded()const;

    bool isEmpty()const;

    void clear();

    QByteArray settingNameAdjust(QByteArray settingName);

    ConnectionSetting&detailGetCheck(QByteArray&settingName);

    ConnectionManager &insert(const QVariantHash &value);

    bool v_load(const QVariant &v);

    bool load(const QVariantHash &vSettings);

    bool load(const QString &settingsFileName);

    bool load(const QStringList &settingsFileName);

    bool load(QObject *settingsObject);

};

}
