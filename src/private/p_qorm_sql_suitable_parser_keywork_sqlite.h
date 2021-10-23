#pragma once

#include "./p_qorm_sql_suitable_parser_keywork.h"

namespace QOrm {

class Q_ORM_EXPORT SqlSuitableKeyWordSQLite : public SqlSuitableKeyWord{
public:
    explicit SqlSuitableKeyWordSQLite(QObject *parent = nullptr):SqlSuitableKeyWord(QSqlDriver::SQLite, parent){
    }
    ~SqlSuitableKeyWordSQLite(){
    }

};

}
