#pragma once

#include "./qorm_object.h"

namespace QOrm {
class ConnectionPoolPvt;
class ConnectionSetting;
//!
//! \brief The ConnectionPool class
//!
class Q_ORM_EXPORT ConnectionPool
{
public:
    //!
    //! \brief ConnectionPool
    //! \param parent
    //!
    Q_INVOKABLE explicit ConnectionPool(QObject *parent = nullptr);

    //!
    //! \brief ConnectionPool
    //! \param connectionSetting
    //! \param parent
    //!
    explicit ConnectionPool(const ConnectionSetting &connectionSetting, QObject *parent = nullptr);

    //!
    //! \brief ConnectionPool
    //! \param connection
    //! \param parent
    //!
    explicit ConnectionPool(const QVariant &connection, QObject *parent = nullptr);

    //!
    //! \brief ConnectionPool
    //! \param pool
    //! \param parent
    //!
    explicit ConnectionPool(const ConnectionPool &pool, QObject *parent = nullptr);

    //!
    //! \brief ~ConnectionPool
    //!
    ~ConnectionPool();

    //!
    //! \brief setting
    //! \return
    //!
    virtual ConnectionSetting &setting() const;

    //!
    //! \brief isValid
    //! \return
    //!
    bool isValid();

    //!
    //! \brief from
    //! \param pool
    //! \return
    //!
    virtual bool from(ConnectionPool &pool);

    //!
    //! \brief from
    //! \param setting
    //! \return
    //!
    virtual bool from(const ConnectionSetting &setting);

    //!
    //! \brief from
    //! \param connection
    //! \return
    //!
    virtual bool from(const QVariant &connection);

    //!
    //! \brief from
    //! \param connection
    //! \return
    //!
    virtual bool from(const QSqlDatabase &connection);

    //!
    //! \brief get
    //! \return
    //!
    QSqlDatabase get();

    //!
    //! \brief get
    //! \param connection
    //! \return
    //!
    bool get(QSqlDatabase &connection);

    //!
    //! \brief getReadOnly
    //! \return
    //!
    QSqlDatabase getReadOnly();

    //!
    //! \brief getReadOnly
    //! \param connection
    //! \return
    //!
    bool getReadOnly(QSqlDatabase &connection);

    //!
    //! \brief finish
    //!
    ConnectionPool &finish();

    //!
    //! \brief finish
    //! \param connection
    //! \return
    //!
    bool finish(QSqlDatabase &connection);

    //!
    //! \brief finish
    //! \param connection
    //! \return
    //!
    bool finish(QString connection);

    //!
    //! \brief lastError
    //! \return
    //!
    QSqlError &lastError();

private:
    ConnectionPoolPvt *p = nullptr;
};

} // namespace QOrm
