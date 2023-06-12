#pragma once

#include "./qorm_object_db.h"

namespace QOrm {
class ControllerPvt;

//!
//! \brief The Controller class
//!
class Q_ORM_EXPORT Controller : public ObjectDb
{
    Q_OBJECT
public:
    //!
    //! \brief Controller
    //! \param parent
    //!
    Q_INVOKABLE explicit Controller(QObject *parent = nullptr);

private:
    ControllerPvt *p = nullptr;
};

} // namespace QOrm
