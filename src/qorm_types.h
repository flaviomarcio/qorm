#pragma once

#include "../qstm/src/qstm_result.h"

namespace QOrm {

class ObjectDb;

//!
//! \brief ModelActionMethod
//!
typedef std::function<ResultValue &(QOrm::ObjectDb *controller, const QVariant &vBody)> ModelActionMethod;
typedef std::function<ResultValue *(QOrm::ObjectDb *controller, const QVariant &vBody)> ModelActionMethodPointer;

#define QORM_MODEL_DECLARE_FORM_LAYOUT enum Layout{ Vertical, Horizontal}; Q_ENUM(Layout)

#define QORM_MODEL_DECLARE_FORM_ACTION_START enum ActionStart{ asCREATE=0, asSEARCH=1 }; Q_ENUM(ActionStart);

#define QORM_MODEL_DECLARE_FORM_ENUMS QORM_MODEL_DECLARE_FORM_LAYOUT QORM_MODEL_DECLARE_FORM_ACTION_START

} // namespace QOrm


