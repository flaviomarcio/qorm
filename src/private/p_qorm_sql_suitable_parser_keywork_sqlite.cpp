#include "./p_qorm_sql_suitable_parser_keywork_sqlite.h"
#include "../qorm_sql_suitable_types.h"

namespace QOrm {

SqlSuitableKeyWordSQLite::SqlSuitableKeyWordSQLite(QObject *parent):SqlSuitableKeyWord(QSqlDriver::SQLite, parent)
{
    this->commands()[kgcSelectTop]={};
    this->commands()[kgcUpsertSet]={};
}

SqlSuitableKeyWordSQLite::~SqlSuitableKeyWordSQLite()
{
}

}
