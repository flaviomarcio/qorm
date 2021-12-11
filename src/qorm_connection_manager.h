#pragma once

#include "./qorm_connection_pool.h"
#include "./qorm_connection_notify.h"
#include "./qorm_connection_setting.h"
#include <QSettings>

namespace QOrm {

//!
//! \brief The ConnectionManager class
//!
class Q_ORM_EXPORT ConnectionManager : public QStm::Object
{
    Q_OBJECT
public:
    //!
    //! \brief ConnectionManager
    //! \param parent
    //!
    Q_INVOKABLE explicit ConnectionManager(QObject *parent = nullptr);

    //!
    //! \brief ConnectionManager
    //! \param manager
    //! \param parent
    //!
    explicit ConnectionManager(ConnectionManager &manager, QObject *parent=nullptr);

    //!
    //! \brief ConnectionManager
    //! \param setting
    //! \param parent
    //!
    Q_INVOKABLE explicit ConnectionManager(const QVariant&setting, QObject *parent = nullptr);

    //!
    //! \brief ~ConnectionManager
    //!
    Q_INVOKABLE ~ConnectionManager();

    Q_PROPERTY(QByteArray enviroment READ enviroment WRITE setEnviroment )
    Q_PROPERTY(QByteArray secretKey READ secretKey WRITE setSecretKey )
    Q_PROPERTY(QVariantHash paramaters READ paramaters WRITE setParamaters )

    //!
    //! \brief clear
    //!
    virtual void clear();

    //!
    //! \brief enviroment
    //! \return
    //!
    virtual QByteArray enviroment() const;

    //!
    //! \brief setEnviroment
    //! \param value
    //!
    virtual void setEnviroment(const QByteArray &value);

    //!
    //! \brief secretKey
    //! \return
    //!
    virtual QByteArray secretKey() const;

    //!
    //! \brief setSecretKey
    //! \param value
    //!
    virtual void setSecretKey(const QByteArray &value);

    //!
    //! \brief paramaters
    //! \return
    //!
    virtual QVariantHash paramaters() const;

    //!
    //! \brief setParamaters
    //! \param value
    //!
    virtual void setParamaters(const QVariantHash &value);

    //!
    //! \brief insert
    //! \param value
    //! \return
    //!
    virtual ConnectionManager&insert(ConnectionSetting &value);

    //!
    //! \brief insert
    //! \param value
    //! \return
    //!
    virtual ConnectionManager&insert(const QVariantHash &value);

    //!
    //! \brief detail
    //! \return
    //!
    virtual ConnectionSetting &detail();

    //!
    //! \brief detail
    //! \param value
    //! \return
    //!
    virtual ConnectionSetting &detail(const QByteArray &value);

    //!
    //! \brief pool
    //! \return
    //!
    virtual ConnectionPool&pool();

    //!
    //! \brief pool
    //! \param value
    //! \return
    //!
    virtual ConnectionPool&pool(const QByteArray &value);

    //!
    //! \brief toMap
    //! \return
    //!
    Q_INVOKABLE virtual QVariantMap toMap() const;

    //!
    //! \brief toHash
    //! \return
    //!
    Q_INVOKABLE virtual QVariantHash toHash() const;

    //!
    //! \brief isEmpty
    //! \return
    //!
    virtual bool isEmpty() const;

    //!
    //! \brief isLoaded
    //! \return
    //!
    virtual bool isLoaded() const;

    //!
    //! \brief load
    //! \param settings
    //! \return
    //!
    virtual bool load(const QVariant &settings);

    //!
    //! \brief load
    //! \param settingsObject
    //! \return
    //!
    virtual bool load(QObject *settingsObject);

    //!
    //! \brief load
    //! \param manager
    //! \return
    //!
    virtual bool load(const ConnectionManager&manager);

    //!
    //! \brief settingsFileName
    //! \return
    //!
    Q_INVOKABLE virtual QVariant settingsFileName();

    //!
    //! \brief notify
    //! \return
    //!
    ConnectionNotify&notify();

    //!
    //! \brief operator <<
    //! \param value
    //! \return
    //!
    ConnectionManager&operator<<(ConnectionSetting&value);

    //!
    //! \brief operator <<
    //! \param value
    //! \return
    //!
    ConnectionManager&operator<<(const QVariantHash&value);

private:
    void*p=nullptr;
};

}
