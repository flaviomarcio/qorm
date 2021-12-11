#pragma once

#include "./p_qorm_sql_suitable_parser_keywork.h"

namespace QOrm {

class Q_ORM_EXPORT SqlSuitableKeyWordOracle : public SqlSuitableKeyWord{
public:
    explicit SqlSuitableKeyWordOracle(QObject *parent = nullptr):SqlSuitableKeyWord(QSqlDriver::Oracle, parent){
        this->commands()[kgcSelectTop]=qbl_null;
        this->commands()[kgcUpsertSet]=qbl_null;
    }
    ~SqlSuitableKeyWordOracle(){
    }
    virtual void init(){
        this->commands()[kgcSelectTop]=qbl_null;
    }
};

}
