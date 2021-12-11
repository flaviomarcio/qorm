#pragma once

#include "./p_qorm_sql_suitable_parser_keywork.h"

namespace QOrm {

class Q_ORM_EXPORT SqlSuitableKeyWordMySql : public SqlSuitableKeyWord{
public:
    explicit SqlSuitableKeyWordMySql(QObject *parent = nullptr):SqlSuitableKeyWord(QSqlDriver::MySqlServer, parent){
        this->commands()[kgcSelectTop]=qbl_null;
        this->commands()[kgcUpsertSet]=qbl_null;
    }
    ~SqlSuitableKeyWordMySql(){
    }
    virtual void init(){
        this->commands()[kgcSelectTop]=qbl_null;
    }
};

}

