#pragma once

#include "./p_qorm_sql_suitable_parser_keywork.h"

namespace QOrm {

//!
//! \brief The SqlSuitableKeyWordOracle class
//!
class Q_ORM_EXPORT SqlSuitableKeyWordOracle : public SqlSuitableKeyWord{
public:
    explicit SqlSuitableKeyWordOracle(QObject *parent = nullptr);

    ~SqlSuitableKeyWordOracle();
    virtual void init();
};

}
