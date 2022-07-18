#pragma once

#include "../qstm/src/qstm_result.h"

namespace QOrm {

class ObjectDb;

//!
//! \brief ModelActionMethod
//!
typedef std::function<ResultValue &(QOrm::ObjectDb *controller, const QVariant &vBody)> ModelActionMethod;
typedef std::function<ResultValue *(QOrm::ObjectDb *controller, const QVariant &vBody)> ModelActionMethodPointer;


#define ____QORM_DECLARE_FORM_ENUMS \
enum FormType { CustomForm = 0, RegisterForm=2, AddressForm = 3, ReportForm = 4 };\
enum FormLayout { Vertical = 0, Horizontal = 1 };

#define QORM_DECLARE_FORM_ENUMS \
____QORM_DECLARE_FORM_ENUMS \
Q_ENUM(FormType)\
Q_ENUM(FormLayout)\

//____QORM_DECLARE_FORM_ENUMS

} // namespace QOrm


