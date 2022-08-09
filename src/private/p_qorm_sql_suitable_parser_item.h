#pragma once

#include "./p_qorm_sql_suitable_parser_command.h"
#include "../qorm_sql_suitable_types.h"

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

    //!
    //! \brief SqlParserItem
    //! \param value
    //! \param title
    //! \param info
    //!
    explicit SqlParserItem(const QVariant &value, const QVariant &title, const KeywordObjectInfo &info);

    //!
    //! \brief init
    //! \param map
    //!
    void init(QVariantHash &map);

    //!
    //! \brief setDefaultValue
    //! \param defaultValue
    //! \return
    //!
    SqlParserItem setDefaultValue(const QVariant &defaultValue)const;

    //!
    //! \brief defVal
    //! \param defaultValue
    //! \return
    //!
    SqlParserItem defVal(const QVariant &defaultValue);

    //!
    //! \brief toMap
    //! \return
    //!
    QVariantMap toMap()const;

    //!
    //! \brief toFormatParameter
    //! \param parser
    //! \return
    //!
    virtual QString toFormatParameter(SqlSuitableKeyWord &parser)const;

    //!
    //! \brief toFormat
    //! \param parser
    //! \return
    //!
    virtual QString toFormat(SqlSuitableKeyWord &parser)const;

    //!
    //! \brief from
    //! \param v
    //! \return
    //!
    static const SqlParserItem from(const QVariant &v);

    //!
    //! \brief createObject
    //! \param v
    //! \return
    //!
    static SqlParserItem createObject(const QVariant &v);

    //!
    //! \brief createValue
    //! \param v
    //! \return
    //!
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

    //!
    //! \brief valueTypeId
    //! \return
    //!
    QVariant valueTypeId()const;

    //!
    //! \brief isList
    //! \return
    //!
    bool isList()const;

    //!
    //! \brief isMap
    //! \return
    //!
    bool isMap()const;

    //!
    //! \brief name
    //! \return
    //!
    QVariant name()const;

    //!
    //! \brief isObject
    //! \return
    //!
    bool isObject()const;

    //!
    //! \brief isValue
    //! \return
    //!
    bool isValue()const;

    //!
    //! \brief title
    //! \return
    //!
    QVariant title()const;

};

}

Q_DECLARE_METATYPE(QOrm::SqlParserItem)
Q_DECLARE_METATYPE(QOrm::SqlParserItem*)
