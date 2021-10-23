#pragma once

#include "./p_qorm_sql_suitable_parser_keywork.h"

namespace QOrm {

class Q_ORM_EXPORT SqlSuitableKeyWordOracle : public SqlSuitableKeyWord{
public:
    explicit SqlSuitableKeyWordOracle(QObject *parent = nullptr):SqlSuitableKeyWord(QSqlDriver::Oracle, parent){
        this->commands().insert(kgcSelectTop, qbl_null);
        //this->commands().insert(kgcUpdateSet         , QByteArray("update %1 set %2 from %3 where %4 ").trimmed().toLower());
        //this->commands().insert(kgcUpdateSetValues   , QByteArray("update %1 set %2 from %3 where %4 ").trimmed().toLower());
        this->commands().insert(kgcUpsertSet         , qbl(""                                  ).trimmed().toLower());
    }
    ~SqlSuitableKeyWordOracle(){
    }
    virtual void init(){
        this->commands().insert(kgcSelectTop, qbl_null);
    }
};

}
