#include "./qorm_transaction.h"
#include <QMutex>

namespace QOrm {

typedef QMap<Transaction *, QString> TransactionStringMap;
Q_GLOBAL_STATIC(QMutex, ___mutex_cache)
Q_GLOBAL_STATIC(TransactionStringMap, ___connections)

class TransactionPvt:public QObject
{
public:
    Transaction *objTran = nullptr;

    bool rollbackOnError = true;
    bool exceptionOnFail = true;
    Transaction *parent = nullptr;
    explicit TransactionPvt(Transaction *parent):QObject{parent} { this->parent = parent; }

    virtual ~TransactionPvt() {}

    void objReg()
    {
        auto cnn = this->parent->connection().connectionName();
        auto obj = ___connections->key(cnn);

        if (cnn.isEmpty()) {
            this->failTryException(qsl("Fail:Invalid connection on transaction"));
            return;
        }

        if (obj == this->parent) {
            this->failTryException(qsl("Double transaction on the same object"));
            return;
        }

        if (obj != nullptr) {
            this->failTryException(qsl("Double transaction in connection on the other object"));
            return;
        }

        QMutexLOCKER locker(___mutex_cache);
        ___connections->insert(this->parent, cnn);
        this->objTran = this->parent;
    }

    void objUnreg()
    {
        QMutexLOCKER locker(___mutex_cache);
        auto cnn = this->parent->connection().connectionName();
        auto obj = ___connections->key(cnn);
        if (obj == this->parent) {
            ___connections->take(this->parent);
        }
        this->objTran = nullptr;
    }

    bool inTransaction()
    {
        auto objectTransaction = this->objectTransaction();
        return (objectTransaction == nullptr);
    }

    ResultValue &failTryException(const QString &v)
    {
        this->parent->lr().setCritical(v);
        if (!this->exceptionOnFail)
            sWarning() << qsl("dangerous failure detected and ignored during try-rollback, "
                              "try-transaction or try-commit");
        else
            qFatal("dangerous failure detected and ignored during try-rollback, try-transaction or "
                   "try-commit");
        return this->parent->lr();
    }

    Transaction *objectTransaction()
    {
        if (this->objTran != nullptr)
            return this->objTran;

        QMutexLOCKER locker(___mutex_cache);
        auto cnn = this->parent->connectionId();
        if (!cnn.isEmpty()) {
            this->objTran = ___connections->key(cnn);
        }
        return this->objTran;
    }
};

Transaction::Transaction(QObject *parent) : ObjectDb{parent}
{
    this->p = new TransactionPvt{this};
}

Transaction::~Transaction()
{

}

ResultValue &Transaction::transaction()
{

    auto objectTransaction = p->objectTransaction();
    auto db = this->connection();
    if (objectTransaction == this)
        return p->failTryException(tr("Double transaction on the same object"));

    if (objectTransaction != nullptr)
        return this->lr();

    if (!db.isValid())
        return this->lr().setCritical(tr("Invalid connection on transaction"));

    if (!db.isOpen())
        return this->lr().setCritical(tr("Connection is not opened"));

    if (!this->lr())
        return p->failTryException(this->lr().toString());

    if (!db.transaction())
        return p->failTryException(db.lastError().text());

    p->objReg();
    return this->lr();
}

ResultValue &Transaction::commit()
{

    auto objectTransaction = p->objectTransaction();
    if (objectTransaction == this && this->lr()) {
        auto db = this->connection();
        if (!db.isValid())
            this->lr().setCritical(tr("Invalid connection on commit"));
        else if (!db.isOpen())
            this->lr().setCritical(tr("Connection is not opened"));
        else if (!p->rollbackOnError) //
            this->lr() = db.commit();
        else if (this->lr())
            this->lr() = db.commit();
        else {
            this->lr() = db.rollback();
            sWarning() << qsl("Detected error on commit, automatic rollback");
            sWarning() << this->lr().toString();
        }

        p->objUnreg();
    }
    return this->lr();
}

ResultValue &Transaction::rollback()
{

    auto objectTransaction = p->objectTransaction();
    if (objectTransaction == this) {
        auto db = this->connection();
        db.rollback();
        this->lr();
        p->objUnreg();
    }
    return this->lr();
}

ResultValue &Transaction::inTransaction()
{

    auto db = this->connection();
    if (!this->lr())
        return this->lr();

    if (!db.isValid())
        return this->lr().setCritical(tr("Invalid connection on inTransaction"));

    if (!db.isOpen())
        return this->lr().setCritical(tr("Connection is not opened"));

    return this->lr().clear() = p->inTransaction();
}

ResultValue &Transaction::isValid()
{
    auto db = this->connection();
    if (!db.isValid())
        return this->lr().setCritical(tr("Invalid connection on isValid"));

    if (!db.isOpen())
        return this->lr().setCritical(tr("Connection is not opened"));

    return this->lr();
}

ResultValue &Transaction::canTransaction()
{

    if (!this->isValid())
        return this->lr();

    return this->lr() = !p->inTransaction();
}

bool Transaction::rollbackOnError() const
{

    return p->rollbackOnError;
}

void Transaction::setRollbackOnError(bool value)
{

    if (!value)
        sWarning() << qsl(
            "in the business structure disable autorollback this can be very, very dangerous");
    p->rollbackOnError = value;
}

bool Transaction::exceptionOnFail() const
{

    return p->exceptionOnFail;
}

void Transaction::setExceptionOnFail(bool value)
{

    if (!value)
        sWarning() << qsl(
            "in the business structure disable autorollback this can be very, very dangerous");
    p->exceptionOnFail = value;
}

} // namespace QOrm
