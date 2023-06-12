#pragma once

#include "./qorm_connection_pool.h"
#include "./qorm_connection_setting.h"
#include "./qorm_object.h"

namespace QOrm {
class ConnectionManagerPvt;
//!
//! \brief The ConnectionManager class
//!
class Q_ORM_EXPORT ConnectionManager : public QOrm::Object
{
    Q_OBJECT
    Q_PROPERTY(QByteArray enviroment READ enviroment WRITE setEnviroment NOTIFY enviromentChanged)
    Q_PROPERTY(QByteArray secretKey READ secretKey WRITE setSecretKey NOTIFY secreChanged)
    Q_PROPERTY(QVariantHash paramaters READ paramaters WRITE setParamaters NOTIFY paramatersChanged)
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
    explicit ConnectionManager(const ConnectionManager &manager, QObject *parent = nullptr);

    //!
    //! \brief ConnectionManager
    //! \param setting
    //! \param parent
    //!
    explicit ConnectionManager(const QVariantHash &setting, QObject *parent = nullptr);


    //!
    //! \brief clear
    //!
    virtual ConnectionManager &clear();

    //!
    //! \brief enviroment
    //! \return
    //!
    virtual const QByteArray &enviroment() const;

    //!
    //! \brief setEnviroment
    //! \param value
    //!
    virtual ConnectionManager &setEnviroment(const QByteArray &value);

    //!
    //! \brief secretKey
    //! \return
    //!
    virtual const QByteArray &secretKey() const;

    //!
    //! \brief setSecretKey
    //! \param value
    //!
    virtual ConnectionManager &setSecretKey(const QByteArray &value);

    //!
    //! \brief paramaters
    //! \return
    //!
    virtual const QVariantHash &paramaters() const;

    //!
    //! \brief setParamaters
    //! \param value
    //!
    virtual ConnectionManager &setParamaters(const QVariantHash &value);

    //!
    //! \brief insert
    //! \param value
    //! \return
    //!
    virtual ConnectionManager &insert(ConnectionSetting &value);

    //!
    //! \brief insert
    //! \param value
    //! \return
    //!
    virtual ConnectionManager &insert(const QVariantHash &value);

    //!
    //! \brief pool
    //! \return
    //!
    virtual ConnectionPool &pool();

    //!
    //! \brief pool
    //! \param value
    //! \return
    //!
    virtual ConnectionPool &pool(const QByteArray &value);

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
    virtual bool load(const QVariantHash &settings);

    //!
    //! \brief load
    //! \param manager
    //! \return
    //!
    virtual bool load(const ConnectionManager &manager);

    //!
    //! \brief operator <<
    //! \param value
    //! \return
    //!
    ConnectionManager &operator<<(ConnectionSetting &value);

    //!
    //! \brief operator <<
    //! \param value
    //! \return
    //!
    ConnectionManager &operator<<(const QVariantHash &value);
signals:
    void enviromentChanged();
    void secreChanged();
    void paramatersChanged();
private:
    ConnectionManagerPvt *p = nullptr;
};

} // namespace QOrm
