#pragma once

#include "../qstm/src/qstm_result.h"

namespace QOrm {

class ObjectDb;

//!
//! \brief ModelActionMethod
//!
typedef std::function<ResultValue &(QOrm::ObjectDb *controller, const QVariant &vBody)> ModelActionMethod;
typedef std::function<ResultValue *(QOrm::ObjectDb *controller, const QVariant &vBody)> ModelActionMethodPointer;

#define QORM_MODEL_DECLARE_FORM_ENUMS \
enum FormType { UndefinedForm, ReportForm , RegisterForm, EditFields, EditAddress, EditDualList };\
Q_ENUM(FormType)


} // namespace QOrm


