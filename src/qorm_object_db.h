#pragma once

#include "./qorm_global.h"
#include "./qorm_object.h"
#include "./qorm_object_db_extension.h"

namespace QOrm {

//!
//! \brief The ObjectDb class
//!
class Q_ORM_EXPORT ObjectDb : public QOrm::Object, public ObjectDbExtension
{
    Q_OBJECT
public:
    //!
    //! \brief ObjectDb
    //! \param parent
    //!
    Q_INVOKABLE explicit ObjectDb(QObject *parent = nullptr);
    explicit ObjectDb(const QSqlDatabase &connection, QObject *parent = nullptr);

};

} // namespace QOrm
