#pragma once

#include "./qorm_connection_manager.h"
#include "./qorm_object_db.h"
//#include "./qorm_transaction.h"
//#include "./qorm_transaction_scope.h"
//#include "./qorm_wrapper.h"

#define QORM_VCHECK_IS_EMPTY(v,m)\
{\
Q_DECLARE_VU;\
if(vu.vIsEmpty(v))\
    return this->lr().setValidation(m);\
}

#define QORM_CONTROLLER_CONSTRUCTOR(ObjectName) \
Q_INVOKABLE explicit ObjectName(QObject *parent = nullptr):QOrm::Controller(parent){};\
Q_INVOKABLE explicit ObjectName(const QSqlDatabase&connection, QObject *parent = nullptr):QOrm::Controller(connection, parent){}\
virtual ~ObjectName(){}

namespace QOrm {
class ControllerPvt;
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
    Q_INVOKABLE explicit Controller(const QSqlDatabase &connection, QObject *parent = nullptr);



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
    bool dbConnect(const QSqlDatabase &connection);

    //!
    //! \brief dbConnect
    //! \param connectionId
    //! \return
    //!
    bool dbConnect(const QString &connectionId);

private:
    ControllerPvt *p = nullptr;
};

} // namespace QOrm
