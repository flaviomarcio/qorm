#include "./p_qorm_sql_suitable_parser_keywork_oracle.h"
#include "../qorm_sql_suitable_types.h"

namespace QOrm {

SqlSuitableKeyWordOracle::SqlSuitableKeyWordOracle(QObject *parent):SqlSuitableKeyWord(QSqlDriver::Oracle, parent)
{
    this->commands()[kgcSelectTop]={};
    this->commands()[kgcUpsertSet]={};
}

SqlSuitableKeyWordOracle::~SqlSuitableKeyWordOracle()
{
}

void SqlSuitableKeyWordOracle::init()
{
    this->commands()[kgcSelectTop]={};
    this->commands()[kgcUpsertSet]={};
}

}
