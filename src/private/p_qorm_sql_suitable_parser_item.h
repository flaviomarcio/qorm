#pragma once

#include "./p_qorm_sql_suitable_parser_command.h"

namespace QOrm{

//!
//! \brief The SqlParserItem class
//!
class Q_ORM_EXPORT SqlParserItem: public SqlParserCommand{
public:
    friend class SqlParserValue;
    friend class SqlParserObject;
    friend class SqlParserField;
public:
    explicit SqlParserItem();
    explicit SqlParserItem(const QVariant &value);

    explicit SqlParserItem(const QVariant &value, const QVariant&title, const KeywordObjectInfo&info);

    void init(QVariantHash&map);

    SqlParserItem setDefaultValue(const QVariant&defaultValue)const;

    SqlParserItem defVal(const QVariant&defaultValue);

    QVariantMap toMap()const;

    virtual QString toFormatParameter(SqlSuitableKeyWord &parser)const;

    virtual QString toFormat(SqlSuitableKeyWord &parser)const;

    static const SqlParserItem from(const QVariant &v);

    static SqlParserItem createObject(const QVariant &v);

    static SqlParserItem createValue(const QVariant &v);

    //!
    //! \brief operator =
    //! \param value
    //!
    void operator=(const QVariant &value);

    //!
    //! \brief info
    //! \return
    //!
    KeywordObjectInfo info()const;

    //!
    //! \brief value
    //! \return
    //!
    QVariant value()const;

    QVariant valueTypeId()const;

    bool isList()const;

    bool isMap()const;

    QVariant name()const;

    bool isObject()const;

    bool isValue()const;

    QVariant title()const;

};

}

Q_DECLARE_METATYPE(QOrm::SqlParserItem)
Q_DECLARE_METATYPE(QOrm::SqlParserItem*)
