#pragma once

#include "./qorm_global.h"
#include <QSqlDatabase>
#include <QVariantHash>
#include <QStringList>

namespace QOrm {
class ConnectionSettingPvt;
//!
//! \brief The ConnectionSetting class
//!
class Q_ORM_EXPORT ConnectionSetting : public QObject
{
    Q_OBJECT
public:
    Q_PROPERTY(QVariantHash variables READ variables WRITE setVariables NOTIFY variablesChanged)
    Q_PROPERTY(QByteArray driver READ driver WRITE setDriver NOTIFY driverChanged)
    Q_PROPERTY(QByteArray name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QByteArray hostName READ hostName WRITE setHostName NOTIFY hostNameChanged)
    Q_PROPERTY(QByteArray userName READ userName WRITE setUserName NOTIFY userNameChanged)
    Q_PROPERTY(QByteArray password READ password WRITE setPassword NOTIFY passwordChanged)
    Q_PROPERTY(QVariant port READ port WRITE setPort NOTIFY portChanged)
    Q_PROPERTY(QByteArray dataBaseName READ dataBaseName WRITE setDataBaseName NOTIFY dataBaseNameChanged)
    Q_PROPERTY(QStringList schemaNames READ schemaNames WRITE setSchemaNames NOTIFY schemaNamesChanged)
    Q_PROPERTY(QByteArray connectOptions READ connectOptions WRITE setConnectOptions NOTIFY connectOptionsChanged)
    Q_PROPERTY(QStringList commandBeforeOpen READ commandBeforeOpen WRITE setCommandBeforeOpen NOTIFY commandBeforeOpenChanged)
    Q_PROPERTY(QStringList commandAfterClose READ commandAfterClose WRITE setCommandAfterClose NOTIFY commandAfterCloseChanged)
public:
    //!
    //! \brief ConnectionSetting
    //! \param parent
    //!
    Q_INVOKABLE explicit ConnectionSetting(QObject *parent = nullptr);

    //!
    //! \brief ConnectionSetting
    //! \param detail
    //! \param parent
    //!
    explicit ConnectionSetting(const QSqlDatabase &detail, QObject *parent);

    //!
    //! \brief ConnectionSetting
    //! \param detail
    //! \param parent
    //!
    explicit ConnectionSetting(const ConnectionSetting &detail, QObject *parent);

    //!
    //! \brief ConnectionSetting
    //! \param name
    //! \param detailMap
    //! \param parent
    //!
    explicit ConnectionSetting(const QByteArray &name,
                               const QVariantHash &detailMap,
                               QObject *parent);

    //!
    //! \brief printLog
    //!
    ConnectionSetting &printLog();

    //!
    //! \brief isValid
    //! \return
    //!
    bool isValid() const;

    //!
    //! \brief fromSetting
    //! \param setting
    //! \return
    //!
    ConnectionSetting &fromSetting(const ConnectionSetting &setting);

    //!
    //! \brief toHash
    //! \return
    //!
    QVariantHash toHash() const;

    //!
    //! \brief fromHash
    //! \param map
    //! \return
    //!
    ConnectionSetting &fromHash(const QVariantHash &map);

    //!
    //! \brief fromConnection
    //! \param connection
    //! \return
    //!
    ConnectionSetting &fromConnection(const QSqlDatabase &connection);

public:
    //!
    //! \brief operator =
    //! \param value
    //! \return
    //!
    ConnectionSetting &operator=(const QVariant &value);

    //!
    //! \brief variables
    //! \return
    //!
    virtual QVariantHash variables() const;

    //!
    //! \brief setVariables
    //! \param value
    //!
    virtual void setVariables(const QVariantHash &value);

    //!
    //! \brief driver
    //! \return
    //!
    virtual QByteArray driver() const;

    //!
    //! \brief setDriver
    //! \param value
    //!
    virtual void setDriver(const QByteArray &value);

    //!
    //! \brief name
    //! \return
    //!
    virtual QByteArray name() const;

    //!
    //! \brief setName
    //! \param value
    //!
    virtual void setName(const QByteArray &value);

    //!
    //! \brief hostName
    //! \return
    //!
    virtual QByteArray hostName() const;

    //!
    //! \brief setHostName
    //! \param value
    //!
    virtual void setHostName(const QByteArray &value);

    //!
    //! \brief userName
    //! \return
    //!
    virtual QByteArray userName() const;

    //!
    //! \brief setUserName
    //! \param value
    //!
    virtual void setUserName(const QByteArray &value);

    //!
    //! \brief password
    //! \return
    //!
    virtual QByteArray password() const;

    //!
    //! \brief setPassword
    //! \param value
    //!
    virtual void setPassword(const QByteArray &value);

    //!
    //! \brief port
    //! \return
    //!
    virtual int port() const;

    //!
    //! \brief setPort
    //! \param value
    //!
    virtual void setPort(const QVariant &value);

    //!
    //! \brief dataBaseName
    //! \return
    //!
    virtual QByteArray dataBaseName() const;

    //!
    //! \brief setDataBaseName
    //! \param value
    //!
    virtual void setDataBaseName(const QByteArray &value);

    //!
    //! \brief schemaNames
    //! \return
    //!
    virtual QStringList schemaNames() const;

    //!
    //! \brief setSchemaNames
    //! \param value
    //!
    virtual void setSchemaNames(const QStringList &value);

    //!
    //! \brief connectOptions
    //! \return
    //!
    virtual QByteArray connectOptions() const;

    //!
    //! \brief setConnectOptions
    //! \param value
    //!
    virtual void setConnectOptions(const QByteArray &value);

    //!
    //! \brief commandBeforeOpen
    //! \return
    //!
    virtual QStringList commandBeforeOpen() const;

    //!
    //! \brief setCommandBeforeOpen
    //! \param value
    //!
    virtual void setCommandBeforeOpen(const QStringList &value);

    //!
    //! \brief commandAfterClose
    //! \return
    //!
    virtual QStringList commandAfterClose() const;

    //!
    //! \brief setCommandAfterClose
    //! \param value
    //!
    virtual void setCommandAfterClose(const QStringList &value);

private:
    ConnectionSettingPvt *p = nullptr;
signals:
    void variablesChanged();
    void driverChanged();
    void nameChanged();
    void hostNameChanged();
    void userNameChanged();
    void passwordChanged();
    void portChanged();
    void dataBaseNameChanged();
    void schemaNamesChanged();
    void connectOptionsChanged();
    void commandBeforeOpenChanged();
    void commandAfterCloseChanged();
};

} // namespace QOrm
