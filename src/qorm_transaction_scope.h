#pragma once

#include "./qorm_object_db.h"

namespace QOrm {

//!
//! \brief The TransactionScope class
//!
class Q_ORM_EXPORT TransactionScope : public ObjectDb{
    Q_OBJECT
public:
    //!
    //! \brief TransactionScope
    //! \param parent
    //!
    explicit TransactionScope(QObject *parent);

    //!
    //! \brief ~TransactionScope
    //!
    ~TransactionScope();

    //!
    //! \brief rollback
    //! \return
    //!execute rollback transaction
    Q_INVOKABLE virtual bool rollback();
private:
    void*p=nullptr;
};

}
