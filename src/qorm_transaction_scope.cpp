#include "./qorm_transaction_scope.h"
#include "./qorm_transaction.h"
#include <QMutex>

namespace QOrm {

class TransactionScopePvt : public QObject
{
public:
    Transaction transaction;
    explicit TransactionScopePvt(QObject *parent) : QObject{parent}, transaction(parent)
    {
        transaction.setRollbackOnError(true);
        transaction.setExceptionOnFail(true);
        if (!transaction.transaction())
            this->failTryException(qsl("failt create transation"));
    }

    ~TransactionScopePvt() { this->transaction.commit(); }

    void failTryException(const QString &v)
    {
        this->transaction.lr().setCritical(v);
        if (!this->transaction.exceptionOnFail())
            sWarning() << qsl("dangerous failure detected and ignored during try-rollback, "
                              "try-transaction or try-commit");
        else
            qFatal("dangerous failure detected and ignored during try-rollback, try-transaction or "
                   "try-commit");
    }
};

TransactionScope::TransactionScope(QObject *parent) : ObjectDb(parent)
{
    this->p = new TransactionScopePvt{this};
}

TransactionScope::~TransactionScope()
{
}

bool TransactionScope::rollback()
{

    return p->transaction.rollback();
}

} // namespace QOrm
