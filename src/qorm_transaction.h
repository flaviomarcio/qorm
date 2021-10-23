#pragma once

#include "./qorm_transaction_scope.h"

namespace QOrm {

class Q_ORM_EXPORT Transaction : public ObjectDb
{
    Q_OBJECT
public:
    Q_INVOKABLE explicit Transaction(QObject *parent);
    Q_INVOKABLE ~Transaction();

    /**
     * @brief transaction
     * @return
     * return value true when transction is inactive and transaction is success
     */
    virtual ResultValue &transaction();

    /**
     * @brief commit
     * @return
     *
     * return value true when transction is active and commit is success
     */
    virtual ResultValue &commit();

    /**
     * @brief rollback
     * @return
     *
     * return value true when transction is active and rollback is success
     */
    virtual ResultValue &rollback();

    /**
     * @brief inTransaction
     * @return
     *
     * return value true when there is a transaction
     */
    virtual ResultValue &inTransaction();

    /**
     * @brief isValid
     * @return
     *
     * return value true when connection is valid
     */
    virtual ResultValue &isValid();

    /**
     * @brief canTransaction
     * @return
     *
     * return value true when exists possibility start transaction with success
     */
    virtual ResultValue &canTransaction();

    /**
     * @brief rollbackOnError
     * @return
     *
     * default true
     *
     * please use only to ignore commit by mistake, the procedure is very, very dangerous
     */
    Q_INVOKABLE virtual bool rollbackOnError() const;
    Q_INVOKABLE virtual void setRollbackOnError(bool value);

    /**
     * @brief exceptionOnFail
     * @return
     *
     * create critical exception when a double-try transaction occurs on the current connection
     */
    Q_INVOKABLE virtual bool exceptionOnFail() const;
    Q_INVOKABLE virtual void setExceptionOnFail(bool value);

private:
    void*p=nullptr;
};

}
