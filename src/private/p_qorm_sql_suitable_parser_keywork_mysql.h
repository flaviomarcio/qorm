#pragma once

#include "./p_qorm_sql_suitable_parser_keywork.h"

namespace QOrm {

//!
//! \brief The SqlSuitableKeyWordMySql class
//!
class Q_ORM_EXPORT SqlSuitableKeyWordMySql : public SqlSuitableKeyWord{
public:
    explicit SqlSuitableKeyWordMySql(QObject *parent = nullptr);
    ~SqlSuitableKeyWordMySql();
    virtual void init();
};

}

