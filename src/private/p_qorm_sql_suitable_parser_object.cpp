#include "p_qorm_sql_suitable_parser_object.h"

namespace QOrm {

SqlParserObject::SqlParserObject():SqlParserItem(QVariant(), QVariant(), KeywordObjectInfo::koiValue)
{
}

SqlParserObject::SqlParserObject(const QVariant &v):SqlParserItem(v, QVariant(), KeywordObjectInfo::koiValue)
{
}

}
