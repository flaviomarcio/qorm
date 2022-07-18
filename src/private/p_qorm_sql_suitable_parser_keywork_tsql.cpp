#include "./p_qorm_sql_suitable_parser_keywork_tsql.h"
#include "../qorm_sql_suitable_types.h"

namespace QOrm {

SqlSuitableKeyWordTSQL::SqlSuitableKeyWordTSQL(QObject *parent):SqlSuitableKeyWord(QSqlDriver::MSSqlServer, parent)
{
    this->commands()[kgcSelectForExists]=QByteArrayLiteral("select top 1 null");
    this->commands()[kgcUpsertSet]={};
    this->commands()[kgcIsNull]=QByteArrayLiteral("isnull");
    this->commands()[kgcIsNullCheckValue]=QByteArrayLiteral("isnull(%1, %2)");
    this->commands()[kgcLimit]={};
    this->commands()[kgcOffset]={};
    this->commands()[kgcSetTransactionReadOnlyOn]=QByteArrayLiteral("ApplicationIntent=ReadOnly");
    this->commands()[kgcSetTransactionReadOnlyOff]={};
}

SqlSuitableKeyWordTSQL::~SqlSuitableKeyWordTSQL()
{
}

}
