#pragma once

#include "./qorm_object_db.h"
#include "./qorm_connection_manager.h"
#include "./qorm_wrapper.h"
#include "./qorm_transaction.h"
#include "./qorm_transaction_scope.h"

namespace QOrm {

//!
//! \brief The Controller class
//!
class Q_ORM_EXPORT Controller : public ObjectDb
{
    Q_OBJECT
public:
    //!
    //! \brief Controller
    //! \param parent
    //!
    Q_INVOKABLE explicit Controller(QObject *parent = nullptr);

    //!
    //! \brief Controller
    //! \param connection
    //! \param parent
    //!
    explicit Controller(const QSqlDatabase&connection, QObject *parent = nullptr);

    ~Controller();

    //!
    //! \brief dbConnect
    //! \param objectConnection
    //! \return
    //!
    bool dbConnect(QObject *objectConnection);

    /**
     * @brief dbConnect
     * @param connectionManager
     * @return
     */
    bool dbConnect(ConnectionManager &connectionManager);

    //!
    //! \brief dbConnect
    //! \param connectionPool
    //! \return
    //!
    bool dbConnect(ConnectionPool &connectionPool);

    //!
    //! \brief dbConnect
    //! \param connection
    //! \return
    //!
    bool dbConnect(const QSqlDatabase&connection);

    //!
    //! \brief dbConnect
    //! \param connectionId
    //! \return
    //!
    bool dbConnect(const QString&connectionId);
private:
    void*p=nullptr;
};

}
