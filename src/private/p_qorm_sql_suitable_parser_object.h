#pragma once

#include "./p_qorm_sql_suitable_parser_item.h"

namespace QOrm {

class Q_ORM_EXPORT SqlParserObject: public SqlParserItem{
public:
    explicit SqlParserObject();
    explicit SqlParserObject(const QVariant &v);
};

}

Q_DECLARE_METATYPE(QOrm::SqlParserObject)
Q_DECLARE_METATYPE(QOrm::SqlParserObject*)
