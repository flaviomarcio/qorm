#pragma once

#include "./p_qorm_sql_suitable_parser_keywork.h"

namespace QOrm {

//!
//! \brief The SqlSuitableKeyWordTSQL class
//!
class Q_ORM_EXPORT SqlSuitableKeyWordTSQL : public SqlSuitableKeyWord{
public:
    explicit SqlSuitableKeyWordTSQL(QObject *parent = nullptr);
    ~SqlSuitableKeyWordTSQL();
};

}
