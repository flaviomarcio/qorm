#pragma once

#include "./qorm_connection_pool.h"
#include "./qorm_connection_setting.h"

namespace QOrm {
class ConnectionManagerPvt;
//!
//! \brief The ConnectionManager class
//!
class Q_ORM_EXPORT ConnectionManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QByteArray enviroment READ enviroment WRITE setEnviroment NOTIFY enviromentChanged)
    Q_PROPERTY(QByteArray secretKey READ secretKey WRITE setSecretKey NOTIFY secreChanged)
    Q_PROPERTY(QVariantHash settings READ settings WRITE setSettings NOTIFY settingsChanged)
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
    explicit ConnectionManager(const ConnectionManager &managerParent, QObject *parent = nullptr);

    //!
    //! \brief ConnectionManager
    //! \param setting
    //! \param parent
    //!
    explicit ConnectionManager(const QVariantHash &setting, QObject *parent = nullptr);

    //!
    //! \brief operator =
    //! \param value
    //! \return
    //!
    ConnectionManager &operator=(const ConnectionSetting &value);
    ConnectionManager &operator=(const QVariant &value);
    ConnectionManager &operator=(const QSqlDatabase &value);

    //!
    //! \brief operator <<
    //! \param value
    //! \return
    //!
    ConnectionManager &operator<<(const ConnectionSetting &value);
    ConnectionManager &operator<<(const QVariantHash &value);
    ConnectionManager &operator<<(const QSqlDatabase &value);

    //!
    //! \brief toHash
    //! \return
    //!
    const QVariantHash toHash()const;

    //!
    //! \brief clear
    //!
    const ConnectionManager &clear();

    //!
    //! \brief enviroment
    //! \return
    //!
    const QByteArray enviroment() const;
    ConnectionManager &setEnviroment(const QByteArray &value);
    auto &enviroment(const QByteArray &value){ return this->setEnviroment(value);};

    //!
    //! \brief secretKey
    //! \return
    //!
    const QByteArray secretKey() const;
    ConnectionManager &setSecretKey(const QByteArray &value);
    auto &secretKey(const QByteArray &value){ return this->setSecretKey(value);};

    //!
    //! \brief settings
    //! \return
    //!
    const QVariantHash settings() const;
    ConnectionManager &setSettings(const QVariant &value);
    auto &settings(const QVariantHash &value){ return this->setSettings(value);};

    //!
    //! \brief insert
    //! \param value
    //! \return
    //!
    ConnectionManager &insert(const QSqlDatabase &value);

    //!
    //! \brief insert
    //! \param value
    //! \return
    //!
    ConnectionManager &insert(const ConnectionSetting &value);

    //!
    //! \brief insert
    //! \param value
    //! \return
    //!
    ConnectionManager &insert(const QVariantHash &value);

    //!
    //! \brief pool
    //! \return
    //!
    ConnectionPool &pool();

    //!
    //! \brief pool
    //! \param value
    //! \return
    //!
    ConnectionPool &pool(const QByteArray &value);

    //!
    //! \brief isEmpty
    //! \return
    //!
    bool isEmpty() const;

    //!
    //! \brief isLoaded
    //! \return
    //!
    bool isLoaded() const;

    //!
    //! \brief load
    //! \param settings
    //! \return
    //!
    bool load(const QVariantHash &settings);

    //!
    //! \brief load
    //! \param manager
    //! \return
    //!
    bool load(const ConnectionManager &manager);

signals:
    void enviromentChanged();
    void secreChanged();
    void settingsChanged();
private:
    ConnectionManagerPvt *p = nullptr;
};

} // namespace QOrm
