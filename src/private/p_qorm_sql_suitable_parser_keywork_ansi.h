#pragma once

#include "./p_qorm_sql_suitable_parser_keywork.h"

namespace QOrm {

//!
//! \brief The SqlSuitableKeyWordAnsi class
//!
class Q_ORM_EXPORT SqlSuitableKeyWordAnsi : public SqlSuitableKeyWord{
public:
    explicit SqlSuitableKeyWordAnsi(QObject *parent = nullptr);

    ~SqlSuitableKeyWordAnsi();

    virtual void init();
};

}
