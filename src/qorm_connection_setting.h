#pragma once

#include "./qorm_global.h"
#include "./qorm_types.h"

namespace QOrm {

//!
//! \brief The ConnectionSetting class
//!
class Q_ORM_EXPORT ConnectionSetting:public QObject{
    Q_OBJECT
public:
    Q_PROPERTY(QVariantHash variables         READ variables            WRITE setVariables           )
    Q_PROPERTY(QByteArray   driver            READ driver               WRITE setDriver              )
    Q_PROPERTY(QByteArray   name              READ name                 WRITE setName                )
    Q_PROPERTY(QByteArray   hostName          READ hostName             WRITE setHostName            )
    Q_PROPERTY(QByteArray   userName          READ userName             WRITE setUserName            )
    Q_PROPERTY(QByteArray   password          READ password             WRITE setPassword            )
    Q_PROPERTY(int          port              READ port                 WRITE setPort                )
    Q_PROPERTY(QByteArray   dataBaseName      READ dataBaseName         WRITE setDataBaseName        )
    Q_PROPERTY(QStringList  schemaNames       READ schemaNames          WRITE setSchemaNames         )
    Q_PROPERTY(QByteArray   connectOptions    READ connectOptions       WRITE setConnectOptions      )
    Q_PROPERTY(QStringList  commandBeforeOpen READ commandBeforeOpen    WRITE setCommandBeforeOpen   )
    Q_PROPERTY(QStringList  commandAfterClose READ commandAfterClose    WRITE setCommandAfterClose   )
public:
    //!
    //! \brief ConnectionSetting
    //! \param parent
    //!
    Q_INVOKABLE explicit ConnectionSetting(QObject*parent=nullptr);

    //!
    //! \brief ConnectionSetting
    //! \param detail
    //! \param parent
    //!
    explicit ConnectionSetting(const QSqlDatabase &detail, QObject*parent);

    //!
    //! \brief ConnectionSetting
    //! \param detail
    //! \param parent
    //!
    explicit ConnectionSetting(const ConnectionSetting &detail, QObject*parent);

    //!
    //! \brief ConnectionSetting
    //! \param name
    //! \param detailMap
    //! \param parent
    //!
    explicit ConnectionSetting(const QByteArray &name, const QVariantHash &detailMap, QObject*parent);

    //!
    //! \brief ~ConnectionSetting
    //!
    Q_INVOKABLE ~ConnectionSetting();

    //!
    //! \brief printLog
    //!
    Q_INVOKABLE void printLog();

    //!
    //! \brief isValid
    //! \return
    //!
    Q_INVOKABLE bool isValid() const;

    //!
    //! \brief fromSetting
    //! \param setting
    //! \return
    //!
    ConnectionSetting &fromSetting(const ConnectionSetting &setting);

    //!
    //! \brief toMap
    //! \return
    //!
    Q_INVOKABLE QVariantMap toMap() const;

    //!
    //! \brief toHash
    //! \return
    //!
    Q_INVOKABLE QVariantHash toHash() const;

    //!
    //! \brief fromMap
    //! \param map
    //! \return
    //!
    ConnectionSetting &fromMap(const QVariantHash &map);

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
    ConnectionSetting&operator=(const QVariant&value);

    //!
    //! \brief variables
    //! \return
    //!
    Q_INVOKABLE virtual QVariantHash variables() const;

    //!
    //! \brief setVariables
    //! \param value
    //!
    Q_INVOKABLE virtual void setVariables(const QVariantHash &value);

    //!
    //! \brief driver
    //! \return
    //!
    Q_INVOKABLE virtual QByteArray driver() const;

    //!
    //! \brief setDriver
    //! \param value
    //!
    Q_INVOKABLE virtual void setDriver(const QByteArray &value);

    //!
    //! \brief name
    //! \return
    //!
    Q_INVOKABLE virtual QByteArray name() const;

    //!
    //! \brief setName
    //! \param value
    //!
    Q_INVOKABLE virtual void setName(const QByteArray &value);

    //!
    //! \brief hostName
    //! \return
    //!
    Q_INVOKABLE virtual QByteArray hostName() const;

    //!
    //! \brief setHostName
    //! \param value
    //!
    Q_INVOKABLE virtual void setHostName(const QByteArray &value);

    //!
    //! \brief userName
    //! \return
    //!
    Q_INVOKABLE virtual QByteArray userName() const;

    //!
    //! \brief setUserName
    //! \param value
    //!
    Q_INVOKABLE virtual void setUserName(const QByteArray &value);

    //!
    //! \brief password
    //! \return
    //!
    Q_INVOKABLE virtual QByteArray password() const;

    //!
    //! \brief setPassword
    //! \param value
    //!
    Q_INVOKABLE virtual void setPassword(const QByteArray &value);

    //!
    //! \brief port
    //! \return
    //!
    Q_INVOKABLE virtual int port() const;

    //!
    //! \brief setPort
    //! \param value
    //!
    Q_INVOKABLE virtual void setPort(int value);

    //!
    //! \brief dataBaseName
    //! \return
    //!
    Q_INVOKABLE virtual QByteArray dataBaseName() const;

    //!
    //! \brief setDataBaseName
    //! \param value
    //!
    Q_INVOKABLE virtual void setDataBaseName(const QByteArray &value);

    //!
    //! \brief schemaNames
    //! \return
    //!
    Q_INVOKABLE virtual QStringList schemaNames() const;

    //!
    //! \brief setSchemaNames
    //! \param value
    //!
    Q_INVOKABLE virtual void setSchemaNames(const QStringList &value);

    //!
    //! \brief connectOptions
    //! \return
    //!
    Q_INVOKABLE virtual QByteArray connectOptions() const;

    //!
    //! \brief setConnectOptions
    //! \param value
    //!
    Q_INVOKABLE virtual void setConnectOptions(const QByteArray &value);

    //!
    //! \brief commandBeforeOpen
    //! \return
    //!
    Q_INVOKABLE virtual QStringList commandBeforeOpen() const;

    //!
    //! \brief setCommandBeforeOpen
    //! \param value
    //!
    Q_INVOKABLE virtual void setCommandBeforeOpen(const QStringList &value);

    //!
    //! \brief commandAfterClose
    //! \return
    //!
    Q_INVOKABLE virtual QStringList commandAfterClose() const;

    //!
    //! \brief setCommandAfterClose
    //! \param value
    //!
    Q_INVOKABLE virtual void setCommandAfterClose(const QStringList &value);
private:
    void*p=nullptr;
};

}
