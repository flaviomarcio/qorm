#pragma once

#include "./qorm_object_db.h"

namespace QOrm {
class TransactionScopePvt;
//!
//! \brief The TransactionScope class
//!
class Q_ORM_EXPORT TransactionScope : public ObjectDb
{
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
    TransactionScopePvt *p = nullptr;
};

} // namespace QOrm
