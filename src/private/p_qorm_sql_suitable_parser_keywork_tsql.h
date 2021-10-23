#pragma once

#include "./p_qorm_sql_suitable_parser_keywork.h"

namespace QOrm {

class Q_ORM_EXPORT SqlSuitableKeyWordTSQL : public SqlSuitableKeyWord{
public:
    explicit SqlSuitableKeyWordTSQL(QObject *parent = nullptr):SqlSuitableKeyWord(QSqlDriver::MSSqlServer, parent){
        this->commands().insert(kgcSelectForExists, qbl("select top 1 null"));
        this->commands().insert(kgcUpsertSet, qbl_null.trimmed().toLower());
        this->commands().insert(kgcIsNull, qbl("isnull"));
        this->commands().insert(kgcIsNullCheckValue, qbl("isnull(%1, %2)"));
        this->commands().insert(kgcLimit, qbl_null);
        this->commands().insert(kgcOffset, qbl_null);
        this->commands().insert(kgcSetTransactionReadOnlyOn, qbl("ApplicationIntent=ReadOnly"));
        this->commands().insert(kgcSetTransactionReadOnlyOff, qbl_null);
        //ApplicationIntent=ReadOnly
    }
    ~SqlSuitableKeyWordTSQL(){
    }
};

}
