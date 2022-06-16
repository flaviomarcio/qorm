#pragma once

#include "./p_qorm_sql_suitable_parser_keywork.h"
#include "./p_qorm_model_info.h"
#include "../qorm_sql_suitable_types.h"

namespace QOrm {

//!
//! \brief The SqlSuitableKeyWordPSql class
//!
class Q_ORM_EXPORT SqlSuitableKeyWordPSql : public SqlSuitableKeyWord{
public:
    explicit SqlSuitableKeyWordPSql(QObject *parent = nullptr);
    ~SqlSuitableKeyWordPSql();
    virtual void init();

    virtual QString parserCommand(int command);

    virtual QStringList parserCommand(int command, const ModelInfo *modelInfo, const QVariant &value);
};

}
