#pragma once

#include "./p_qorm_sql_suitable_parser_item.h"
//#include "../../qstm/src/qstm_macro.h"

namespace QOrm {

class Q_ORM_EXPORT SqlParserField: public SqlParserItem{

public:
    explicit SqlParserField();

    explicit SqlParserField(const QVariant &v);

    explicit SqlParserField(const KeywordGrouping&grouping, const QVariant &name, const QVariant &nameAs, const QVariant &value, const QVariant &defaultValue);

    ~SqlParserField();

    virtual QString toFormat(SqlSuitableKeyWord &parser)const;
};

template<class TemplateParent>
class SqlParserFields: public SqlParserCommand{
public:
    TemplateParent*parent=nullptr;
    explicit SqlParserFields(const QVariant &v={}):SqlParserCommand{v}
    {
    }
    explicit SqlParserFields(TemplateParent*parent, const QVariant &v={}):SqlParserCommand{v}, parent{parent}
    {
    }
    auto &f(const QVariant &v)
    {
        static const auto __format=QStringLiteral("f.");
        this->insert(__format+QUuid::createUuid().toString(),v);
        return *this;
    }
    auto &as(const QVariant &name, const QVariant &nameAs)
    {
        return this->f(SqlParserField(KeywordGrouping::kgNone, name, nameAs, {}, {}));
    }
    auto &as(const QVariant &name, const QVariant &nameAs, const QVariant &defaultValue)
    {
        return this->f(SqlParserField(KeywordGrouping::kgNone, name, nameAs, {}, defaultValue));
    }
    auto &sum(const QVariant &name, const QVariant &nameAs, const QVariant &defaultValue)
    {
        return this->f(SqlParserField(KeywordGrouping::kgSumAs, name, nameAs, {}, defaultValue));
    }
    auto &max(const QVariant &name, const QVariant &nameAs, const QVariant &defaultValue)
    {
        return this->f(SqlParserField(KeywordGrouping::kgMaxAs, name, nameAs, {}, defaultValue));
    }
    auto &min(const QVariant &name, const QVariant &nameAs, const QVariant &defaultValue)
    {
        return this->f(SqlParserField(KeywordGrouping::kgMinAs, name, nameAs, {}, defaultValue));
    }
    auto &avg(const QVariant &name, const QVariant &nameAs, const QVariant &defaultValue)
    {
        return this->f(SqlParserField(KeywordGrouping::kgAvgAs, name, nameAs, {}, defaultValue));
    }
    auto &count()
    {
        return this->f(SqlParserField(KeywordGrouping::kgCountAs, 1, QStringLiteral("rows"), 1, 0));
    }
    auto &count(const QVariant &name, const QVariant &nameAs, const QVariant &defaultValue)
    {
        static const auto __rows=QStringLiteral("rows");
        auto _name=name;
        if(name.isNull())
            _name=1;

        auto _nameAs=nameAs.toString();
        if(_nameAs.isNull())
            _nameAs=__rows;
        return this->f(SqlParserField(KeywordGrouping::kgCountAs, _name, _nameAs, {}, defaultValue));
    }

};


}

Q_DECLARE_METATYPE(QOrm::SqlParserField)
Q_DECLARE_METATYPE(QOrm::SqlParserField*)
