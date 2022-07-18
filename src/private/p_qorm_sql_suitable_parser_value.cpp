#include "p_qorm_sql_suitable_parser_value.h"

namespace QOrm {

SqlParserValue::SqlParserValue():SqlParserItem{{}, {}, KeywordObjectInfo::koiValue}
{
}

SqlParserValue::SqlParserValue(const QVariant &v):SqlParserItem{v, {}, KeywordObjectInfo::koiValue}
{
}

SqlParserValues::SqlParserValues(const QVariant &v):SqlParserCommand(v)
{
}


SqlParserValues &SqlParserValues::value(const QVariant &v)
{
    SqlParserItem*c=nullptr;
    QMapIterator<QString, SqlParserCommand*> i(this->mPointer());
    while (i.hasNext()) {
        i.next();
        if(i.key().startsWith(__func__))
            c=static_cast<SqlParserItem*>(i.value());
    }
    if(c==nullptr){
        c = new SqlParserItem(v);
        this->setPointer(c->makeUuid().toString(), c);
    }

    switch (v.typeId()) {
    case QMetaType::QVariantList:
    case QMetaType::QStringList:
    {
        for(auto &i:v.toList())
            this->value(i);
        return*this;
    }
    case QMetaType::QVariantHash:
    case QMetaType::QVariantMap:
    {
        QHashIterator<QString, QVariant> i(v.toHash());
        while (i.hasNext()) {
            i.next();
            this->value(i.value());
        }
        return*this;
    }
    default:
        this->value(v);
        return*this;
    }
}

SqlParserValues &SqlParserValues::v(const QVariant &v)
{
    return this->value(v);
}

QStringList SqlParserValues::toScript(SqlSuitableKeyWord &parser)
{
    QStringList output;
    auto map=this->toMap();
    QMapIterator<QString, QVariant> i(map);
    while (i.hasNext()) {
        i.next();
        output<<parser.formatValue(i.value());
    }
    return output;
}

}
