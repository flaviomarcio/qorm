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

//!
//! \brief The ConnectionManagerPvt class
//!
class ConnectionManagerPvt:public QObject
{
    Q_OBJECT
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
    //!
    //! \brief ConnectionManagerPvt
    //! \param parent
    //!
    explicit ConnectionManagerPvt(ConnectionManager*parent);
    virtual ~ConnectionManagerPvt();

    //!
    //! \brief init
    //!
    void init();

    //!
    //! \brief pool
    //! \param value
    //! \return
    //!
    ConnectionPool &pool(const QByteArray &value);

    //!
    //! \brief isLoaded
    //! \return
    //!
    bool isLoaded()const;

    //!
    //! \brief isEmpty
    //! \return
    //!
    bool isEmpty()const;

    //!
    //! \brief clear
    //!
    void clear();

    //!
    //! \brief settingNameAdjust
    //! \param settingName
    //! \return
    //!
    QByteArray settingNameAdjust(QByteArray settingName);

    //!
    //! \brief detailGetCheck
    //! \param settingName
    //! \return
    //!
    ConnectionSetting&detailGetCheck(QByteArray&settingName);

    //!
    //! \brief insert
    //! \param value
    //! \return
    //!
    ConnectionManager &insert(const QVariantHash &value);

    //!
    //! \brief v_load
    //! \param v
    //! \return
    //!
    bool v_load(const QVariant &v);

    //!
    //! \brief load
    //! \param vSettings
    //! \return
    //!
    bool load(const QVariantHash &vSettings);

    //!
    //! \brief load
    //! \param settingsFileName
    //! \return
    //!
    bool load(const QString &settingsFileName);

    //!
    //! \brief load
    //! \param settingsFileName
    //! \return
    //!
    bool load(const QStringList &settingsFileName);

    //!
    //! \brief load
    //! \param settingsObject
    //! \return
    //!
    bool load(QObject *settingsObject);

};

}
