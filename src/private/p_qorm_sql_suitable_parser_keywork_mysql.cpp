#include "./p_qorm_sql_suitable_parser_keywork_mysql.h"
#include "../qorm_sql_suitable_types.h"

namespace QOrm {

SqlSuitableKeyWordMySql::SqlSuitableKeyWordMySql(QObject *parent):SqlSuitableKeyWord(QSqlDriver::MySqlServer, parent)
{
    this->commands()[kgcSelectTop]={};
    this->commands()[kgcUpsertSet]={};
}

SqlSuitableKeyWordMySql::~SqlSuitableKeyWordMySql()
{
}

void SqlSuitableKeyWordMySql::init()
{
    this->commands()[kgcSelectTop]={};
}

}
