#pragma once

#include "./p_qorm_sql_suitable_parser_keywork.h"

namespace QOrm {

class Q_ORM_EXPORT SqlSuitableKeyWordTSQL : public SqlSuitableKeyWord{
public:
    explicit SqlSuitableKeyWordTSQL(QObject *parent = nullptr):SqlSuitableKeyWord(QSqlDriver::MSSqlServer, parent){
        this->commands()[kgcSelectForExists]=qbl("select top 1 null");
        this->commands()[kgcUpsertSet]=qbl_null.trimmed().toLower();
        this->commands()[kgcIsNull]=qbl("isnull");
        this->commands()[kgcIsNullCheckValue]=qbl("isnull(%1, %2)");
        this->commands()[kgcLimit]=qbl_null;
        this->commands()[kgcOffset]=qbl_null;
        this->commands()[kgcSetTransactionReadOnlyOn]=qbl("ApplicationIntent=ReadOnly");
        this->commands()[kgcSetTransactionReadOnlyOff]=qbl_null;
    }
    ~SqlSuitableKeyWordTSQL(){
    }
};

}
