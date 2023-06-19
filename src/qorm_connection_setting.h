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
    //! \param connection
    //! \param parent
    //!
    explicit ConnectionSetting(const QSqlDatabase &connection, QObject *parent);

    //!
    //! \brief ConnectionSetting
    //! \param setting
    //! \param parent
    //!
    explicit ConnectionSetting(const ConnectionSetting &setting, QObject *parent);

    //!
    //! \brief ConnectionSetting
    //! \param name
    //! \param detail
    //! \param parent
    //!
    explicit ConnectionSetting(const QByteArray &name, const QVariantHash &detail, QObject *parent);

public:
    //!
    //! \brief operator =
    //! \param value
    //! \return
    //!
    ConnectionSetting &operator=(const QVariant &value);
    ConnectionSetting &operator=(const QSqlDatabase &value);
    ConnectionSetting &operator=(const ConnectionSetting &value);

    //!
    //! \brief from
    //! \param setting
    //! \return
    //!
    ConnectionSetting &from(const ConnectionSetting &value);
    ConnectionSetting &from(const QVariant &value);
    ConnectionSetting &from(const QSqlDatabase &value);

    //!
    //! \brief print
    //! \return
    //!
    const ConnectionSetting &print()const ;

    //!
    //! \brief isValid
    //! \return
    //!
    bool isValid() const;

    //!
    //! \brief toHash
    //! \return
    //!
    const QVariantHash &toHash() const;

    //!
    //! \brief variables
    //! \return
    //!
    const QVariantHash &variables() const;
    ConnectionSetting &setVariables(const QVariantHash &value);
    ConnectionSetting &variables(const QVariantHash &value){ return this->setVariables(value);};

    //!
    //! \brief driver
    //! \return
    //!
    const QByteArray &driver() const;
    ConnectionSetting &setDriver(const QByteArray &value);
    ConnectionSetting &driver(const QByteArray &value){ return this->setDriver(value);};

    //!
    //! \brief name
    //! \return
    //!
    const QByteArray &name() const;
    ConnectionSetting &setName(const QByteArray &value);
    ConnectionSetting &name(const QByteArray &value){ return this->setName(value);};

    //!
    //! \brief hostName
    //! \return
    //!
    const QByteArray &hostName() const;
    ConnectionSetting &setHostName(const QByteArray &value);
    ConnectionSetting &hostName(const QByteArray &value){ return this->setHostName(value);};

    //!
    //! \brief userName
    //! \return
    //!
    const QByteArray &userName() const;
    ConnectionSetting &setUserName(const QByteArray &value);
    ConnectionSetting &userName(const QByteArray &value){ return this->setUserName(value);};

    //!
    //! \brief password
    //! \return
    //!
    const QByteArray &password() const;
    ConnectionSetting &setPassword(const QByteArray &value);
    ConnectionSetting &password(const QByteArray &value){ return this->setPassword(value);};

    //!
    //! \brief port
    //! \return
    //!
    int port() const;
    ConnectionSetting &setPort(const QVariant &value);
    ConnectionSetting &port(const QVariant &value){ return this->setPort(value);};

    //!
    //! \brief dataBaseName
    //! \return
    //!
    const QByteArray &dataBaseName() const;
    ConnectionSetting &setDataBaseName(const QByteArray &value);
    ConnectionSetting &dataBaseName(const QByteArray &value){ return this->setDataBaseName(value);};

    //!
    //! \brief schemaNames
    //! \return
    //!
    QStringList &schemaNames() const;
    ConnectionSetting &setSchemaNames(const QStringList &value);
    ConnectionSetting &schemaNames(const QStringList &value){ return this->setSchemaNames(value);};

    //!
    //! \brief connectOptions
    //! \return
    //!
    const QByteArray &connectOptions() const;
    ConnectionSetting &setConnectOptions(const QByteArray &value);
    ConnectionSetting &connectOptions(const QByteArray &value){ return this->setConnectOptions(value);};

    //!
    //! \brief commandBeforeOpen
    //! \return
    //!
    const QStringList &commandBeforeOpen() const;
    ConnectionSetting &setCommandBeforeOpen(const QStringList &value);
    ConnectionSetting &commandBeforeOpen(const QStringList &value){ return this->setCommandBeforeOpen(value);};

    //!
    //! \brief commandAfterClose
    //! \return
    //!
    const QStringList &commandAfterClose() const;
    ConnectionSetting &setCommandAfterClose(const QStringList &value);
    ConnectionSetting &commandAfterClose(const QStringList &value){ return this->setCommandAfterClose(value);};

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
