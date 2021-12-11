#pragma once

#include "./qorm_object_db.h"
#include "./qorm_sql_suitable_value.h"
#include "./qorm_wrapper.h"

namespace QOrm {

//!
//! \brief The Dao class
//!
class Q_ORM_EXPORT Dao : public ObjectDb
{
public:
    //!
    //! \brief Dao
    //! \param parent
    //!
    Q_INVOKABLE explicit Dao(QObject *parent = nullptr);

    //!
    //! \brief Dao
    //! \param connection
    //! \param parent
    //!
    explicit Dao(const QSqlDatabase&connection, QObject *parent = nullptr);

    //!
    //!
    ~Dao();

    //!
    //! \brief format
    //! \return
    //!
    QOrm::SqlSuitableValue&format();

    //!
    //! \brief f
    //!
    QOrm::SqlSuitableValue&f=format();
private:
    void*p=nullptr;
};

}
