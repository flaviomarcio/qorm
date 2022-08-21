#pragma once

#include "../qstm/src/qstm_result.h"

namespace QOrm {

class ObjectDb;

//!
//! \brief ModelActionMethod
//!
typedef std::function<ResultValue &(QOrm::ObjectDb *controller, const QVariant &vBody)> ModelActionMethod;
typedef std::function<ResultValue *(QOrm::ObjectDb *controller, const QVariant &vBody)> ModelActionMethodPointer;

#define QORM_MODEL_DECLARE_FORM_TYPE enum FormType { UndefinedForm, ReportForm , RegisterForm, EditFields, EditAddress, EditDualList }; Q_ENUM(FormType)
#define QORM_MODEL_DECLARE_FORM_LAYOUT enum Layout{ Vertical, Horizontal}; Q_ENUM(Layout)

#define QORM_MODEL_DECLARE_FORM_ENUMS QORM_MODEL_DECLARE_FORM_TYPE QORM_MODEL_DECLARE_FORM_LAYOUT

} // namespace QOrm


