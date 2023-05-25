#pragma once

#include "./p_qorm_sql_suitable_parser_strategy_options.h"
#include "./p_qorm_sql_suitable_parser_field.h"

namespace QOrm {

static const auto __from="from";
static const auto __join="join";
static const auto __where="where";
static const auto __distinct="distinct";
static const auto __limit="limit";
static const auto __offset="offset";
static const auto __lockSkip="lockSkip";
static const auto __lock="lock";
static const auto __fields="fields";
static const auto __orderby="orderby";
static const auto __grouping="grouping";
static const auto __value="value";
static const auto __values="values";
static const auto __destine="destine";
static const auto __using="using";
static const auto __type="type";
static const auto __object="object";
static const auto __arg1="%1";

//!
//! \brief The SqlParserSelect class
//!
class Q_ORM_EXPORT SqlParserSelect:public SqlParserFrom<SqlParserSelect>{
public:

    //!
    //! \brief SqlParserSelect
    //! \param v
    //!
    explicit SqlParserSelect(const QVariant &v={}):SqlParserFrom<SqlParserSelect>(v)
    {
    }

    //!
    //! \brief limit
    //! \param limit
    //! \return
    //!
    auto &limit(int limit)
    {
        if(limit>0){
            auto map=this->toMap();
            map.insert(__func__, limit);
            this->setValue(map);
        }
        return *this;
    }

    //!
    //! \brief offset
    //! \param off_set
    //! \return
    //!
    auto &offset(int off_set)
    {
        if(off_set>0){
            auto map=this->toMap();
            map.insert(__func__, off_set);
            this->setValue(map);
        }
        return *this;
    }

    //!
    //! \brief offset
    //! \param limit
    //! \param off_set
    //! \return
    //!
    auto &offset(int limit, int off_set)
    {
        if(limit>0)
            this->limit(limit);
        if(off_set>0)
            this->offset(off_set);
        return *this;
    }

    //!
    //! \brief lock
    //! \return
    //!
    auto &lock(){
        auto map=this->toMap();
        map.insert(__func__,true);
        this->setValue(map);
        return *this;
    }

    //!
    //! \brief lockSkip
    //! \return
    //!
    auto &lockSkip()
    {
        auto map=this->toMap();
        map.insert(__func__,true);
        this->setValue(map);
        return *this;
    }

    //!
    //! \brief distinct
    //! \return
    //!
    auto &distinct()
    {
        auto map=this->toMap();
        map.insert(__func__, true);
        this->setValue(map);
        return *this;
    }

    //!
    //! \brief distinct
    //! \param value
    //! \return
    //!
    auto &distinct(bool value)
    {
        auto map=this->toMap();
        map.insert(__func__, value);
        this->setValue(map);
        return *this;
    }

    //!
    //! \brief fields
    //! \return
    //!
    auto &fields()
    {
        auto object=this->oPointer<SqlParserFields<SqlParserSelect>*>(__func__);
        if(object==nullptr){
            object=new SqlParserFields<SqlParserSelect>(this->vMap(__func__));
            this->setPointer(__func__,object);
        }
        return*object;
    }

    //!
    //! \brief fields
    //! \param parserObject
    //! \return
    //!
    auto &fields(const QVariant &parserObject)
    {
        auto &fields=this->fields();
        fields.f(parserObject);
        return *this;
    }

    //!
    //! \brief fieldsFrom
    //! \param parserbject
    //! \return
    //!
    auto &fieldsFrom(const QVariant &parserbject)
    {
        this->fields(parserbject);
        this->from(parserbject);
        return *this;
    }

    //!
    //! \brief fromExists
    //! \param parserbject
    //! \return
    //!
    auto &fromExists(const QVariant &parserbject)
    {
        static const auto sAs="1 as c";
        this->fields().f(sAs);
        this->limit(1);
        this->from(parserbject);
        return *this;
    }

    //!
    //! \brief orderby
    //! \return
    //!
    auto &orderby()
    {
        auto object=this->oPointer<SqlParserFields<SqlParserSelect>*>(__func__);
        if(object==nullptr){
            object=new SqlParserFields<SqlParserSelect>(this->vMap(__func__));
            this->setPointer(__func__,object);
        }
        return*object;
    }

    //!
    //! \brief toScript
    //! \param parser
    //! \return
    //!
    virtual QStringList toScript(SqlSuitableKeyWord &parser)
    {

        const auto groupingTypes=QOrm::TypeUtil::keywordGroupingTypes();

        auto mapObject=this->toMap();
        auto parser_distinct=getVariantStartsWith(__distinct, mapObject);
        auto parser_limit=getVariantStartsWith(__limit, mapObject);
        auto parser_offset=getVariantStartsWith(__offset, mapObject);
        auto parser_lockSkip=getVariantStartsWith(__lockSkip,mapObject).toBool();
        auto parser_lock=getVariantStartsWith(__lock,mapObject).toBool();

        auto parser_fields=SqlParserFields<SqlParserSelect>(unionMapStartsWith(__fields, mapObject));
        auto parser_orderby=SqlParserFields<SqlParserSelect>(unionMapStartsWith(__orderby, mapObject));

        QStringList parser_groupby;


        static auto local_keys=QStringList{__from, __join, __where};
        auto parser_combination=appendMapStartsWith(local_keys , mapObject);


        QStringList output;

        if(!parser_fields.isValid()){
            output.append(parser.parserCommand(kgcSelectForExists));
        }
        else if(parser_limit.isValid()){
            auto cmd=parser.parserCommand(parser_distinct.isValid()?kgcSelectDistinctTop:kgcSelectTop);
            output.append(cmd.contains(__arg1)?cmd.arg(parser_limit.toInt()):cmd);
        }
        else{
            output.append(parser.parserCommand(parser_distinct.isValid()?kgcSelectDistinct:kgcSelect));
        }

        if(parser_fields.isValid()){
            auto fieldList=parser_fields.toMap().values();
            QStringList fields;
            QStringList group;
            bool isGrouping=false;
            for(auto &parser_fields:fieldList){
                const auto &modelInfo=QOrm::ModelInfo::from(parser_fields);
                if(modelInfo.isValid()){
                    QHashIterator<QString, QString> i(modelInfo.propertyTableVsShort());
                    while (i.hasNext()) {
                        i.next();
                        auto fieldName=i.key().trimmed();
                        if(fieldName.isEmpty())
                            continue;
                        fields.append(fieldName);
                        group.append(fieldName);
                    }
                    if(isGrouping)
                        parser_groupby<<group;
                    continue;
                }

                switch (parser_fields.typeId()){
                case QMetaType::QVariantMap:
                case QMetaType::QVariantHash:
                case QMetaType::QVariantList:
                case QMetaType::QStringList:
                {
                    auto map=parser_fields.toMap();
                    auto grouping=map.value(__grouping).toInt();
                    auto value=map.value(__value).toString().trimmed();
                    auto isGroup=(groupingTypes.contains(grouping));
                    isGrouping = isGrouping || isGroup;
                    if(isGroup){
                        fields<<parser.parserCommand(KeywordGenericCommand(grouping),nullptr, map);
                    }
                    else if(!value.isEmpty()){
                        group.append(value);
                        fields.append(value);
                    }
                    break;
                }
                default:
                    fields<<parser_fields.toString();
                }
                if(isGrouping)
                    parser_groupby.append(group);
            }
            output.append((fields.isEmpty()?"":fields.join(',')));
        }


        if(!parser_combination.isEmpty()){
            SqlParserCombinations<SqlParserSelect> obj(parser_combination);
            output.append(obj.toScript(parser));
        }

        if(!parser_groupby.isEmpty()){
            output.append(parser.parserCommand(kgcGroupBy));
            output.append(parser_groupby.join(','));
        }

        if(parser_orderby.isValid()){
            auto fieldList=parser_orderby.toMap().values();
            if(!fieldList.isEmpty()){
                QStringList fields;
                for(auto &parser_order:fieldList){
                    const auto &modelInfo=QOrm::ModelInfo::from(parser_order);
                    if(modelInfo.isValid()){
                        QHashIterator<QString, QString> i(modelInfo.propertyTableVsShort());
                        while (i.hasNext()) {
                            i.next();
                            fields.append(i.key());
                        }
                        continue;
                    }

                    switch (parser_order.typeId()) {
                    case QMetaType::QVariantMap:
                    case QMetaType::QVariantHash:
                    {
                        auto item = SqlParserItem::from(parser_order);
                        QString name;
                        if(item.info()==koiObject)
                            name=item.value().toString().trimmed();
                        else
                            name=item.toString().trimmed();
                        if(!fields.contains(name))
                            fields.append(name);
                        break;
                    }
                    case QMetaType::QVariantList:
                    case QMetaType::QStringList:
                    {
                        for(auto &v:parser_order.toList()){
                            auto name=v.toByteArray().trimmed();
                            if(!fields.contains(name))
                                fields.append(name);
                        }
                        break;
                    }
                    default:
                        auto name=parser_order.toByteArray().trimmed();
                        if(!fields.contains(name))
                            fields.append(name);
                    }
                }
                if(!fields.isEmpty()){
                    output.append(parser.parserCommand(kgcOrderBy));
                    output.append(fields.isEmpty()?"":fields.join(','));
                }
            }
        }

        if(parser_limit.isValid()){
            auto cmd=parser.parserCommand(kgcLimit);
            output.append(cmd.arg(parser_limit.toInt()));
        }

        if(parser_offset.isValid()){
            auto cmd=parser.parserCommand(kgcOffset);
            output.append(cmd.arg(parser_offset.toInt()));
        }

        if(parser_lockSkip)
            output.append(parser.parserCommand(kgcForSkipLockedFinishScript));
        else if(parser_lock)
            output.append(parser.parserCommand(kgcForUpdateFinishScript));

        //FOR UPDATE SKIP LOCKED


        return output;
    }
};

class Q_ORM_EXPORT SqlParserInsert:public SqlParserCommand{
public:
    explicit SqlParserInsert():SqlParserCommand{}
    {
    }

    explicit SqlParserInsert(const QVariant &v):SqlParserCommand{v}
    {
    }

    auto &destine(const QVariant &v)
    {
        auto object=this->oPointer<SqlParserCommand*>(__func__);
        if(object==nullptr){
            object=new SqlParserCommand{v};
            this->setPointer(__func__,object);
        }
        return *this;
    }
    auto &values(const QVariant &values)
    {
        auto object=this->oPointer<SqlParserCommand*>(__func__);
        if(object==nullptr){
            object=new SqlParserCommand(values);
            this->setPointer(__func__,object);
        }
        return *this;
    }

    virtual QStringList toScript(SqlSuitableKeyWord &parser)
    {
        auto map=this->toMap();
        auto destine=getVariantStartsWith(__destine,map);
        auto values=getVariantStartsWith(__values,map);
        const auto &modelInfo=QOrm::ModelInfo::from(destine);
        auto command=parser.parserCommand(kgcInsertInto, &modelInfo, values);
        return command;
    }
};

class Q_ORM_EXPORT SqlParserUpdate:public SqlParserCommand{
public:

    auto &destine(const QVariant &v)
    {
        auto object=this->oPointer<SqlParserCommand*>(__func__);
        if(object==nullptr){
            object=new SqlParserCommand{v};
            this->setPointer(__func__,object);
        }
        return *this;
    }

    auto &values(const QVariant &v)
    {
        auto object=this->oPointer<SqlParserCommand*>(__func__);
        if(object==nullptr){
            object=new SqlParserCommand{v};
            this->setPointer(__func__,object);
        }
        return *this;
    }

    explicit SqlParserUpdate():SqlParserCommand{}
    {
    }

    explicit SqlParserUpdate(const QVariant &v):SqlParserCommand{v}
    {
    }

    virtual QStringList toScript(SqlSuitableKeyWord &parser)
    {
        auto map=this->toMap();
        auto destine=getVariantStartsWith(__destine,map);
        auto values=getVariantStartsWith(__values,map);
        const auto &modelInfo=QOrm::ModelInfo::from(destine);
        auto command=parser.parserCommand(kgcUpdateSet, &modelInfo, values);
        return command;
    }
};

class Q_ORM_EXPORT SqlParserUpsert:public SqlParserCommand{
public:

    auto &destine(const QVariant &v)
    {
        auto object=this->oPointer<SqlParserCommand*>(__func__);
        if(object==nullptr){
            object=new SqlParserCommand{v};
            this->setPointer(__func__,object);
        }
        return *this;
    }

    auto &values(const QVariant &v)
    {
        auto object=this->oPointer<SqlParserCommand*>(__func__);
        if(object==nullptr){
            object=new SqlParserCommand{v};
            this->setPointer(__func__,object);
        }
        return *this;
    }

    explicit SqlParserUpsert():SqlParserCommand{}
    {
    }

    explicit SqlParserUpsert(const QVariant &v):SqlParserCommand{v}
    {
    }

    virtual QStringList toScript(SqlSuitableKeyWord &parser)
    {
        auto map=this->toMap();
        auto destine=getVariantStartsWith(__destine, map);
        auto values=getVariantStartsWith(__values, map);
        const auto &modelInfo=QOrm::ModelInfo::from(destine);
        auto command=parser.parserCommand(kgcUpsertSet, &modelInfo, values);
        return command;
    }
};

class Q_ORM_EXPORT SqlParserRemove:public SqlParserFrom<SqlParserSelect>{
public:

    auto &destine(const QVariant &v)
    {
        auto map=this->toMap();
        map.insert(__func__,v);
        this->setValue(map);
        return *this;
    }

    auto &values(const QVariant &values)
    {
        auto map=this->toMap();
        map.insert(__func__,values);
        this->setValue(map);
        return *this;
    }

    explicit SqlParserRemove():SqlParserFrom<SqlParserSelect>()
    {
    }

    explicit SqlParserRemove(const QVariant &v):SqlParserFrom<SqlParserSelect>(v)
    {
    }

    virtual QStringList toScript(SqlSuitableKeyWord &parser)
    {
        auto mapObject=this->toMap();
        static auto local_keys=QStringList{__from, __join, __where, __using};
        auto parser_combination=appendMapStartsWith(local_keys, mapObject);
        QStringList output;
        if(!parser_combination.isEmpty()){
            output.append(parser.parserCommand(kgcDelete));

            if(!parser_combination.isEmpty()){
                SqlParserCombinations<SqlParserRemove> obj(parser_combination);
                output.append(obj.toScript(parser));
            }
        }
        return output;
    }

};

class Q_ORM_EXPORT SqlParserProcedure:public SqlParserCallObject{
public:
    explicit SqlParserProcedure(const QVariant &v={}):SqlParserCallObject{v}
    {
    }

    virtual QStringList toScript(SqlSuitableKeyWord &parser)
    {
        auto mapObject=this->toMap();
        QStringList output=parser.parserCallProcedure(mapObject);
        return output;
    }
};

class Q_ORM_EXPORT SqlParserFunction:public SqlParserCallObject{
public:

    explicit SqlParserFunction(const QVariant &v={}):SqlParserCallObject{v}
    {
    }

    auto &nextVal(const QVariant &v)
    {
        auto object=new SqlParserCommand{};
        object->makeUuid();
        object->setValue(QVariantMap{{__type, kgcNextValSelect},{__object, v}});
        this->setPointer(this->suuid(), object);
        return *this;
    }

    virtual QStringList toScript(SqlSuitableKeyWord &parser)
    {
        auto mapObject=this->toMap();
        QMapIterator<QString, QVariant> i(mapObject);
        QStringList output;
        while (i.hasNext()) {
            i.next();
            auto vValue=i.value().toMap();
            auto type=vValue.value(__type).toInt();
            auto object=vValue.value(__object);
            auto value=vValue.value(__value);
            QStringList command;
            const auto &modelInfo=QOrm::ModelInfo::from(object);
            if(modelInfo.isValid())
                command=parser.parserCommand(type, &modelInfo, value);
            else
                command=parser.parserCommand(type, &modelInfo, object);
            for(auto &v:command)
                output.append(v);
        }

        return output;
    }
};

class Q_ORM_EXPORT SqlParserStructure:public SqlParserCallObject{
public:
    explicit SqlParserStructure(const QVariant &v={}):SqlParserCallObject{v}
    {
    }

    virtual bool ignorePrepare()
    {
        return true;
    }

    auto &truncateTable(const QVariant &v)
    {
        auto object=new SqlParserCommand{};
        object->makeUuid();
        object->setValue(QVariantMap{{__type, kgcTruncateTable},{__object, v}});
        this->setPointer(this->suuid(), object);
        return *this;
    }

    auto &truncateTableCascade(const QVariant &v)
    {
        auto object=new SqlParserCommand{};
        object->makeUuid();
        object->setValue(QVariantMap{{__type, kgcTruncateTableCacade},{__object, v}});
        this->setPointer(this->suuid(), object);
        return *this;
    }

    virtual QStringList toScript(SqlSuitableKeyWord &parser)
    {
        auto mapObject=this->toMap();
        QMapIterator<QString, QVariant> i(mapObject);
        QStringList output;
        while (i.hasNext()) {
            i.next();
            auto vValue=i.value().toMap();
            auto type=vValue.value(__type).toInt();
            auto object=vValue.value(__object);
            auto value=vValue.value(__value);
            QStringList command;
            const auto &modelInfo=QOrm::ModelInfo::from(object);
            if(modelInfo.isValid())
                command=parser.parserCommand(type, &modelInfo, value);
            for(auto &v:command)
                output.append(v);
        }

        return output;
    }
};

class Q_ORM_EXPORT SqlParserCombineSelect:public SqlParserCommand
{
    friend class SqlSuitableBuilderPvt;
public:

    explicit SqlParserCombineSelect(const QVariant &v);

    explicit SqlParserCombineSelect();

    SqlParserCombineSelect &unionMerge();

    SqlParserCombineSelect &unionAll();

    SqlParserCombineSelect &intersect();

    SqlParserCombineSelect &intersectAll();

    SqlParserCombineSelect &except();

    SqlParserCombineSelect &exceptAll();
    QObject*q=nullptr;
private:
    SqlParserCombineSelect &setV(int t);
};

}

Q_DECLARE_METATYPE(QOrm::SqlParserSelect)
Q_DECLARE_METATYPE(QOrm::SqlParserInsert)
Q_DECLARE_METATYPE(QOrm::SqlParserUpsert)
Q_DECLARE_METATYPE(QOrm::SqlParserRemove)
Q_DECLARE_METATYPE(QOrm::SqlParserProcedure)
Q_DECLARE_METATYPE(QOrm::SqlParserFunction)
Q_DECLARE_METATYPE(QOrm::SqlParserStructure)
Q_DECLARE_METATYPE(QOrm::SqlParserCombineSelect)

Q_DECLARE_METATYPE(QOrm::SqlParserSelect*)
Q_DECLARE_METATYPE(QOrm::SqlParserInsert*)
Q_DECLARE_METATYPE(QOrm::SqlParserUpsert*)
Q_DECLARE_METATYPE(QOrm::SqlParserRemove*)
Q_DECLARE_METATYPE(QOrm::SqlParserProcedure*)
Q_DECLARE_METATYPE(QOrm::SqlParserFunction*)
Q_DECLARE_METATYPE(QOrm::SqlParserConditions<QOrm::SqlParserSelect>*)
Q_DECLARE_METATYPE(QOrm::SqlParserConditions<QOrm::SqlParserCombinations<QOrm::SqlParserSelect>>*)
Q_DECLARE_METATYPE(QOrm::SqlParserStructure*)
Q_DECLARE_METATYPE(QOrm::SqlParserCombineSelect*)
