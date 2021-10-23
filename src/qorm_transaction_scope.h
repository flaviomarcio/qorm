#pragma once

#include "./qorm_object_db.h"

namespace QOrm {

class Q_ORM_EXPORT TransactionScope : public ObjectDb{
    Q_OBJECT
public:
    Q_INVOKABLE explicit TransactionScope(QObject *parent);
    Q_INVOKABLE ~TransactionScope();

    /**
     * @brief rollback
     * @return
     *
     * execute rollback transaction
     */
    Q_INVOKABLE virtual bool rollback();
private:
    void*p=nullptr;
};

}
