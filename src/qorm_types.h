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
enum FormType { ReportForm , RegisterForm, EditFields, EditAddress, EditDualList };\
enum FormLayout { Vertical, Horizontal };\
Q_ENUM(FormType)\
Q_ENUM(FormLayout)


} // namespace QOrm


