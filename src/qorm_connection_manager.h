#pragma once

#include "./qorm_connection_pool.h"
#include "./qorm_connection_notify.h"
#include "./qorm_connection_setting.h"
#include <QSettings>

namespace QOrm {

class Q_ORM_EXPORT ConnectionManager : public QStm::Object
{
    Q_OBJECT
public:
    Q_INVOKABLE explicit ConnectionManager(QObject *parent = nullptr);
    explicit ConnectionManager(ConnectionManager &manager, QObject *parent=nullptr);
    Q_INVOKABLE explicit ConnectionManager(const QVariant&setting, QObject *parent = nullptr);
    Q_INVOKABLE ~ConnectionManager();

    Q_PROPERTY(QByteArray enviroment READ enviroment WRITE setEnviroment )
    Q_PROPERTY(QByteArray secretKey READ secretKey WRITE setSecretKey )
    Q_PROPERTY(QVariantHash paramaters READ paramaters WRITE setParamaters )


    /**
     * @brief clear
     */
    virtual void clear();

    /**
     * @brief enviroment
     * @return
     */
    virtual QByteArray enviroment() const;
    virtual void setEnviroment(const QByteArray &value);

    /**
     * @brief secretKey
     * @return
     */
    virtual QByteArray secretKey() const;
    virtual void setSecretKey(const QByteArray &value);

    /**
     * @brief paramaters
     * @return
     */
    virtual QVariantHash paramaters() const;
    virtual void setParamaters(const QVariantHash &value);

    /**
     * @brief insert
     * @param settingName
     */
    virtual ConnectionManager&insert(ConnectionSetting &value);
    virtual ConnectionManager&insert(const QVariantHash &value);

    /**
     * @brief detail
     * @return
     */
    virtual ConnectionSetting &detail();
    virtual ConnectionSetting &detail(const QByteArray &value);

    /**
     * @brief pool
     * @return
     */
    virtual ConnectionPool&pool();
    virtual ConnectionPool&pool(const QByteArray &value);

    /**
     * @brief toMap
     * @return
     */
    Q_INVOKABLE virtual QVariantMap toMap() const;

    /**
     * @brief toHash
     * @return
     */
    Q_INVOKABLE virtual QVariantHash toHash() const;

    /**
     * @brief isEmpty
     * @return
     */
    virtual bool isEmpty() const;

    /**
     * @brief isLoaded
     * @return
     */
    virtual bool isLoaded() const;

    /**
     * @brief load
     * @param settings
     * @return
     */
    virtual bool load(const QVariant &settings);
    virtual bool load(QObject *settingsObject);
    virtual bool load(const ConnectionManager&manager);

    Q_INVOKABLE virtual QVariant settingsFileName();

    ConnectionNotify&notify();

    ConnectionManager&operator<<(ConnectionSetting&value);
    ConnectionManager&operator<<(const QVariantHash&value);

private:
    void*p=nullptr;
};

}
