#pragma once

#include "../../qstm/src/qstm_startup.h"
#include "./qorm_global.h"

namespace QOrm {

#define Q_ORM_STARTUP_FUNCTION(FUNC) Q_STM_STARTUP_APPEND(QOrm, FUNC)

//!
//! \brief The StartUp class
//!
class Q_ORM_EXPORT StartUp : public QStm::StartUp
{
    Q_OBJECT
    Q_STM_STARTUP_CONSTUCTOR(2)
};

} // namespace QOrm
