#pragma once

#include "./p_qorm_sql_suitable_parser_keywork.h"

namespace QOrm {

class Q_ORM_EXPORT SqlSuitableKeyWordMySql : public SqlSuitableKeyWord{
public:
    explicit SqlSuitableKeyWordMySql(QObject *parent = nullptr):SqlSuitableKeyWord(QSqlDriver::MySqlServer, parent){
        this->commands().insert(kgcSelectTop, qbl_null);
        this->commands().insert(kgcUpsertSet, qbl_null.trimmed().toLower());
    }
    ~SqlSuitableKeyWordMySql(){
    }
    virtual void init(){
        this->commands().insert(kgcSelectTop, qbl_null);
    }
};

}

