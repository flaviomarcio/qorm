#include "./p_qorm_sql_suitable_parser_keywork_ansi.h"
#include "../qorm_sql_suitable_types.h"

namespace QOrm {

SqlSuitableKeyWordAnsi::SqlSuitableKeyWordAnsi(QObject *parent):SqlSuitableKeyWord(QList<QSqlDriver::DbmsType>()<<QSqlDriver::Sybase<<QSqlDriver::Interbase<<QSqlDriver::DB2, parent)
{

}

SqlSuitableKeyWordAnsi::~SqlSuitableKeyWordAnsi()
{
}

void SqlSuitableKeyWordAnsi::init()
{
    this->commands()[kgcSelectTop]={};
}

}
