#include "./p_qorm_sql_suitable_parser_keywork_mysql.h"

namespace QOrm {

SqlSuitableKeyWordMySql::SqlSuitableKeyWordMySql(QObject *parent):SqlSuitableKeyWord(QSqlDriver::MySqlServer, parent)
{
    this->commands()[kgcSelectTop]=qbl_null;
    this->commands()[kgcUpsertSet]=qbl_null;
}

SqlSuitableKeyWordMySql::~SqlSuitableKeyWordMySql()
{
}

void SqlSuitableKeyWordMySql::init()
{
    this->commands()[kgcSelectTop]=qbl_null;
}

}
