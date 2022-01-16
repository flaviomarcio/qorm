#pragma once

#include <QObject>
#include <QString>
#include <QVariant>
#include <QDateTime>
#include <QUrl>
#include <QUuid>
#include <QVariant>
#include <QVariantHash>
#include <QVariant>
#include <QList>
#include <QVector>
#include <QtSql/QSqlError>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlDriver>
#include <QtSql/QSqlRecord>
#include <QtSql/QSqlError>
#include "../../qstm/src/qstm_types.h"
#include "../../qstm/src/qstm_currency.h"
#include "../../qstm/src/qstm_vvm.h"
#include "../../qstm/src/qstm_svm.h"
#include "../../qstm/src/qstm_url.h"

namespace QOrm {

class ObjectDb;

//!
//! \brief ModelActionMethod
//!
typedef std::function<ResultValue&(QOrm::ObjectDb*controller, const QVariant&vBody)> ModelActionMethod;
typedef std::function<ResultValue*(QOrm::ObjectDb*controller, const QVariant&vBody)> ModelActionMethodPointer;
}
