#include "p_qorm_sql_suitable_parser_field.h"

namespace QOrm {

SqlParserField::SqlParserField():SqlParserItem()
{
}

SqlParserField::SqlParserField(const QVariant &v):SqlParserItem(v)
{
}

SqlParserField::SqlParserField(const KeywordGrouping &grouping, const QVariant &name, const QVariant &nameAs, const QVariant &value, const QVariant &defaultValue):SqlParserItem(value)
{
    auto map=this->toHash();
    map.insert(qsl("grouping"),grouping);
    map.insert(qsl("name"),name);
    map.insert(qsl("nameAs"),nameAs);
    map.insert(qsl("value"),value);
    map.insert(qsl("defaultValue"),defaultValue);
    this->setValue(map);
}

SqlParserField::~SqlParserField()
{
}

QString SqlParserField::toFormat(SqlSuitableKeyWord &parser) const
{
    auto typeId=qTypeId(qTypeId(*this));
    if(QStmTypesVariantDictionary.contains(typeId))
        return qsl_null;
    return SqlParserItem::toFormat(parser);
}

}
