#pragma once

#include "./qorm_object_db.h"
#include "./qorm_sql_suitable_value.h"

namespace QOrm {
class DaoPvt;
//!
//! \brief The Dao class
//!
class Q_ORM_EXPORT Dao : public ObjectDb
{
    Q_OBJECT
public:
    //!
    //! \brief Dao
    //! \param parent
    //!
    Q_INVOKABLE explicit Dao(QObject *parent = nullptr);

    //!
    //! \brief format
    //! \return
    //!
    QOrm::SqlSuitableValue &format();

    //!
    //! \brief f
    //!
    QOrm::SqlSuitableValue &f = format();

private:
    DaoPvt *p = nullptr;
};

} // namespace QOrm
