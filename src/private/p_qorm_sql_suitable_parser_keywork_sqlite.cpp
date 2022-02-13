#include "./p_qorm_sql_suitable_parser_keywork_sqlite.h"

namespace QOrm {

SqlSuitableKeyWordSQLite::SqlSuitableKeyWordSQLite(QObject *parent):SqlSuitableKeyWord(QSqlDriver::SQLite, parent)
{
    this->commands()[kgcSelectTop]=qbl_null;
    this->commands()[kgcUpsertSet]=qbl_null;
}

SqlSuitableKeyWordSQLite::~SqlSuitableKeyWordSQLite()
{
}

}
