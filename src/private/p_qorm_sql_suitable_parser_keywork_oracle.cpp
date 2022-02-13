#include "./p_qorm_sql_suitable_parser_keywork_oracle.h"

namespace QOrm {

SqlSuitableKeyWordOracle::SqlSuitableKeyWordOracle(QObject *parent):SqlSuitableKeyWord(QSqlDriver::Oracle, parent)
{
    this->commands()[kgcSelectTop]=qbl_null;
    this->commands()[kgcUpsertSet]=qbl_null;
}

SqlSuitableKeyWordOracle::~SqlSuitableKeyWordOracle()
{
}

void SqlSuitableKeyWordOracle::init()
{
    this->commands()[kgcSelectTop]=qbl_null;
    this->commands()[kgcUpsertSet]=qbl_null;
}

}
