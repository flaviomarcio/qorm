#include "./qorm_transaction.h"
#include <QMutex>

namespace QOrm {

typedef QMap<Transaction*,QString> TransactionStringMap ;
Q_GLOBAL_STATIC(QMutex, ___mutex_cache__);
Q_GLOBAL_STATIC(TransactionStringMap, ___connections__);

static auto&___mutex_cache=*___mutex_cache__;
static auto&___connections=*___connections__;

#define dPvt()\
    auto&p =*reinterpret_cast<TransactionPvt*>(this->p)

class TransactionPvt:public QObject{
public:
    Transaction*objTran=nullptr;

    bool rollbackOnError=true;
    bool exceptionOnFail=true;
    Transaction*parent=nullptr;
    explicit TransactionPvt(Transaction*parent):QObject(parent)
    {
        this->parent=parent;
    }

    ~TransactionPvt(){
    }

    Transaction*objReg()
    {
        auto cnn=this->parent->connection().connectionName();
        auto obj=___connections.key(cnn);

        if(cnn.isEmpty())
            this->failTryException(tr("Fail:Invalid connection on transaction"));
        else if(obj==this->parent)
            this->failTryException(tr("Double transaction on the same object"));
        else if(obj!=nullptr)
            this->failTryException(tr("Double transaction in connection on the other object"));
        else {
            QMutexLocker locker(&___mutex_cache);
            ___connections.insert(this->parent, cnn);
            this->objTran=this->parent;
        }
        return this->objTran;
    }

    void objUnreg()
    {
        QMutexLocker locker(&___mutex_cache);
        auto cnn=this->parent->connection().connectionName();
        auto obj=___connections.key(cnn);
        if(obj==this->parent){
            ___connections.take(this->parent);
        }
        this->objTran=nullptr;
    }

    bool inTransaction()
    {
        auto objectTransaction=this->objectTransaction();
        return (objectTransaction==nullptr);
    }

    void failTryException(const QString&v)
    {
        this->parent->lr().setCritical(v);
        if(!this->exceptionOnFail)
            sWarning()<<tr("dangerous failure detected and ignored during try-rollback, try-transaction or try-commit");
        else
            qFatal("dangerous failure detected and ignored during try-rollback, try-transaction or try-commit");
    }

    Transaction*objectTransaction()
    {
        if(this->objTran==nullptr){
            QMutexLocker locker(&___mutex_cache);
            auto cnn=this->parent->connectionId();
            if(!cnn.isEmpty()){
                this->objTran=___connections.key(cnn);
            }
        }
        return this->objTran;
    }
};

Transaction::Transaction(QObject *parent) : ObjectDb(parent)
{
    this->p = new TransactionPvt(this);
}

Transaction::~Transaction()
{
    dPvt();
    delete&p;
}

ResultValue &Transaction::transaction()
{
    dPvt();
    auto objectTransaction=p.objectTransaction();
    auto db=this->connection();
    if(objectTransaction==this)
        p.failTryException(tr("Double transaction on the same object"));
    else if(objectTransaction==nullptr){
        if(!db.isValid())
            this->lr().setCritical(tr("Invalid connection on transaction"));
        else if(!db.isOpen())
            this->lr().setCritical(tr("Connection is not opened"));
        else if(!this->lr())
            p.failTryException(this->lr().toString());
        else if(!db.transaction())
            p.failTryException(db.lastError().text());
        else
            p.objReg();
    }
    return this->lr();
}

ResultValue &Transaction::commit()
{
    dPvt();
    auto objectTransaction=p.objectTransaction();
    if(objectTransaction==this && this->lr()){
        auto db = this->connection();
        if(!db.isValid())
            this->lr().setCritical(tr("Invalid connection on commit"));
        else if(!db.isOpen())
            this->lr().setCritical(tr("Connection is not opened"));
        else if(!p.rollbackOnError)//
            this->lr()=db.commit();
        else if(this->lr())
            this->lr()=db.commit();
        else{
            this->lr()=db.rollback();
            sWarning()<<qsl("Detected error on commit, automatic rollback");
            sWarning()<<this->lr().toString();
        }

        p.objUnreg();
    }
    return this->lr();
}

ResultValue &Transaction::rollback()
{
    dPvt();
    auto objectTransaction=p.objectTransaction();
    if(objectTransaction==this){
        auto db=this->connection();
        db.rollback();
        this->lr();
        p.objUnreg();
    }
    return this->lr();
}

ResultValue &Transaction::inTransaction()
{
    dPvt();
    auto db = this->connection();
    if(!this->lr())
        return this->lr();

    if(!db.isValid())
        return this->lr().setCritical(tr("Invalid connection on inTransaction"));

    if(!db.isOpen())
        return this->lr().setCritical(tr("Connection is not opened"));

    return this->lr().clear()=p.inTransaction();
}

ResultValue &Transaction::isValid()
{
    auto db = this->connection();
    if(!db.isValid())
        return this->lr().setCritical(tr("Invalid connection on isValid"));

    if(!db.isOpen())
        return this->lr().setCritical(tr("Connection is not opened"));

    return this->lr();
}

ResultValue &Transaction::canTransaction()
{
    dPvt();
    if(!this->isValid())
        return this->lr();

    return this->lr()=!p.inTransaction();
}

bool Transaction::rollbackOnError() const
{
    dPvt();
    return p.rollbackOnError;
}

void Transaction::setRollbackOnError(bool value)
{
    dPvt();
    if(!value)
        sWarning()<<qsl("in the business structure disable autorollback this can be very, very dangerous");
    p.rollbackOnError = value;
}

bool Transaction::exceptionOnFail() const
{
    dPvt();
    return p.exceptionOnFail;
}

void Transaction::setExceptionOnFail(bool value)
{
    dPvt();
    if(!value)
        sWarning()<<qsl("in the business structure disable autorollback this can be very, very dangerous");
    p.exceptionOnFail = value;
}

}
