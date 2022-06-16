#pragma once

#include "./p_qorm_sql_suitable_parser_item.h"

namespace QOrm {

class Q_ORM_EXPORT SqlParserField: public SqlParserItem{

public:
    explicit SqlParserField();

    explicit SqlParserField(const QVariant &v);

    explicit SqlParserField(const KeywordGrouping&grouping, const QVariant&name, const QVariant&nameAs, const QVariant &value, const QVariant&defaultValue);

    ~SqlParserField();

    virtual QString toFormat(SqlSuitableKeyWord &parser)const;
};

template<class TemplateParent>
class SqlParserFields: public SqlParserCommand{
public:
    TemplateParent*parent=nullptr;
    explicit SqlParserFields(const QVariant &v=QVariant()):SqlParserCommand(v)
    {
    }
    explicit SqlParserFields(TemplateParent*parent, const QVariant &v=QVariant()):SqlParserCommand(v)
    {
        this->parent=parent;
    }
    auto &f(const QVariant &v)
    {
        this->insert(qsl("f.")+QUuid::createUuid().toString(),v);
        return*this;
    }
    auto &as(const QVariant&name, const QVariant&nameAs)
    {
        return this->f(SqlParserField(KeywordGrouping::kgNone, name, nameAs, QVariant(), QVariant()));
    }
    auto &as(const QVariant&name, const QVariant&nameAs, const QVariant&defaultValue)
    {
        return this->f(SqlParserField(KeywordGrouping::kgNone, name, nameAs, QVariant(), defaultValue));
    }
    auto &sum(const QVariant&name, const QVariant&nameAs, const QVariant&defaultValue)
    {
        return this->f(SqlParserField(KeywordGrouping::kgSumAs, name, nameAs, QVariant(), defaultValue));
    }
    auto &max(const QVariant&name, const QVariant&nameAs, const QVariant&defaultValue)
    {
        return this->f(SqlParserField(KeywordGrouping::kgMaxAs, name, nameAs, QVariant(), defaultValue));
    }
    auto &min(const QVariant&name, const QVariant&nameAs, const QVariant&defaultValue)
    {
        return this->f(SqlParserField(KeywordGrouping::kgMinAs, name, nameAs, QVariant(), defaultValue));
    }
    auto &avg(const QVariant&name, const QVariant&nameAs, const QVariant&defaultValue)
    {
        return this->f(SqlParserField(KeywordGrouping::kgAvgAs, name, nameAs, QVariant(), defaultValue));
    }
    auto &count()
    {
        return this->f(SqlParserField(KeywordGrouping::kgCountAs, 1, qsl("rows"), 1, 0));
    }
    auto &count(const QVariant&name, const QVariant&nameAs, const QVariant&defaultValue)
    {
        auto _name=name;
        if(name.isNull())
            _name=1;

        auto _nameAs=nameAs.toString();
        if(_nameAs.isNull())
            _nameAs=qsl("rows");
        return this->f(SqlParserField(KeywordGrouping::kgCountAs, _name, _nameAs, QVariant(), defaultValue));
    }

};


}

Q_DECLARE_METATYPE(QOrm::SqlParserField)
Q_DECLARE_METATYPE(QOrm::SqlParserField*)
