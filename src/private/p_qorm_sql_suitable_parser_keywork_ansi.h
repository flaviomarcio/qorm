#pragma once

#include "./p_qorm_sql_suitable_parser_keywork.h"

namespace QOrm {

class Q_ORM_EXPORT SqlSuitableKeyWordAnsi : public SqlSuitableKeyWord{
public:
    explicit SqlSuitableKeyWordAnsi(QObject *parent = nullptr):SqlSuitableKeyWord(QList<QSqlDriver::DbmsType>()<<QSqlDriver::Sybase<<QSqlDriver::Interbase<<QSqlDriver::DB2, parent){

    }
    ~SqlSuitableKeyWordAnsi(){
    }

    virtual void init(){
        this->commands()[kgcSelectTop]=qbl_null;
    }
};

}
