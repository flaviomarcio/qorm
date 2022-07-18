#include "p_qorm_sql_suitable_parser_field.h"

namespace QOrm {

SqlParserField::SqlParserField()
    :
      SqlParserItem{}
{
}

SqlParserField::SqlParserField(const QVariant &v)
    :
      SqlParserItem{v}
{
}

SqlParserField::SqlParserField(const KeywordGrouping &grouping, const QVariant &name, const QVariant &nameAs, const QVariant &value, const QVariant &defaultValue)
    :
      SqlParserItem{value}
{
    auto map=this->toHash();
    map.insert(QStringLiteral("grouping"),grouping);
    map.insert(QStringLiteral("name"),name);
    map.insert(QStringLiteral("nameAs"),nameAs);
    map.insert(QStringLiteral("value"),value);
    map.insert(QStringLiteral("defaultValue"),defaultValue);
    this->setValue(map);
}

SqlParserField::~SqlParserField()
{
}

QString SqlParserField::toFormat(SqlSuitableKeyWord &parser) const
{

    switch (this->typeId()) {
    case QMetaType::QVariantHash:
    case QMetaType::QVariantMap:
        return {};
    default:
        return SqlParserItem::toFormat(parser);
    }
}

}
