#pragma once

#include "./p_qorm_sql_suitable_parser_item.h"
#include <QVariant>

namespace QOrm {

class Q_ORM_EXPORT SqlParserValue: public SqlParserItem{
public:
    explicit SqlParserValue();
    explicit SqlParserValue(const QVariant &v);
};

class SqlParserValues:public SqlParserCommand{
public:
    explicit SqlParserValues(const QVariant &v={});

    SqlParserValues &value(const QVariant &v);

    SqlParserValues &v(const QVariant &v);

    QStringList toScript(SqlSuitableKeyWord &parser);
};


}

Q_DECLARE_METATYPE(QOrm::SqlParserValue*)
Q_DECLARE_METATYPE(QOrm::SqlParserValue)

Q_DECLARE_METATYPE(QOrm::SqlParserValues*)
Q_DECLARE_METATYPE(QOrm::SqlParserValues)
