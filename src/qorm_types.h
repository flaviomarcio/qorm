#pragma once

#include "../../qstm/src/qstm_currency.h"
#include "../../qstm/src/qstm_meta_types.h"
#include "../../qstm/src/qstm_svm.h"
#include "../../qstm/src/qstm_types.h"
#include "../../qstm/src/qstm_url.h"
#include "../../qstm/src/qstm_vvm.h"
#include <QDateTime>
#include <QList>
#include <QObject>
#include <QString>
#include <QUrl>
#include <QUuid>
#include <QVariant>
#include <QVariantHash>
#include <QVector>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlDriver>
#include <QtSql/QSqlError>
#include <QtSql/QSqlRecord>

namespace QOrm {

class ObjectDb;

//!
//! \brief ModelActionMethod
//!
typedef std::function<ResultValue &(QOrm::ObjectDb *controller, const QVariant &vBody)> ModelActionMethod;
typedef std::function<ResultValue *(QOrm::ObjectDb *controller, const QVariant &vBody)>ModelActionMethodPointer;


#define ____QORM_DECLARE_FORM_ENUMS \
enum FormType { CustomForm = 0, RegisterForm=2, AddressForm = 3, ReportForm = 4 };\
enum FormLayout { Vertical = 0, Horizontal = 1 };

#define QORM_DECLARE_FORM_ENUMS \
____QORM_DECLARE_FORM_ENUMS \
Q_ENUM(FormType)\
Q_ENUM(FormLayout)\

//____QORM_DECLARE_FORM_ENUMS

} // namespace QOrm


