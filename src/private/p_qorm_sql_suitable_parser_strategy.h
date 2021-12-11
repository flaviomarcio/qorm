#pragma once

#include "./p_qorm_sql_suitable_parser_strategy_options.h"

namespace QOrm {
    class Q_ORM_EXPORT SqlParserSelect:public SqlParserFrom<SqlParserSelect>{
    public:
        explicit SqlParserSelect(const QVariant&v=QVariant()):SqlParserFrom<SqlParserSelect>(v){
        }

        /**
         * @brief limit
         * @param limit
         * @return
         */
        auto&limit(int limit){
            if(limit>0){
                auto map=this->toMap();
                map.insert(__func__, limit);
                this->setValue(map);
            }
            return*this;
        }

        /**
         * @brief offset
         * @param off_set
         * @return
         */
        auto&offset(int off_set){
            if(off_set>0){
                auto map=this->toMap();
                map.insert(__func__, off_set);
                this->setValue(map);
            }
            return*this;
        }

        /**
         * @brief offset
         * @param limit
         * @param off_set
         * @return
         */
        auto&offset(int limit, int off_set){
            if(limit>0)
                this->limit(limit);
            if(off_set>0)
                this->offset(off_set);
            return*this;
        }

        /**
         * @brief lock
         * @return
         */
        auto&lock(){
            auto map=this->toMap();
            map.insert(__func__,true);
            this->setValue(map);
            return*this;
        }

        /**
         * @brief lockSkip
         * @return
         */
        auto&lockSkip(){
            auto map=this->toMap();
            map.insert(__func__,true);
            this->setValue(map);
            return*this;
        }

        /**
         * @brief distinct
         * @return
         */
        auto&distinct(){
            auto map=this->toMap();
            map.insert(__func__, true);
            this->setValue(map);
            return*this;
        }

        /**
         * @brief distinct
         * @return
         */
        auto&distinct(bool value){
            auto map=this->toMap();
            map.insert(__func__, value);
            this->setValue(map);
            return*this;
        }

        /**
         * @brief fields
         * @return
         */
        auto&fields(){
            auto object=this->oPointer<SqlParserFields<SqlParserSelect>*>(__func__);
            if(object==nullptr){
                object=new SqlParserFields<SqlParserSelect>(this->vMap(__func__));
                this->setPointer(__func__,object);
            }
            return*object;
        }

        /**
         * @brief fields
         * @param parserObject
         * @return
         */
        auto&fields(const QVariant&parserObject){
            auto&fields=this->fields();
            fields.f(parserObject);
            return*this;
        }

        /**
         * @brief fieldsFrom
         * @param parserbject
         * @return
         */
        auto&fieldsFrom(const QVariant&parserbject){
            this->fields(parserbject);
            this->from(parserbject);
            return*this;
        }

        /**
         * @brief fromExists
         * @param parserbject
         * @return
         */
        auto&fromExists(const QVariant&parserbject){
            this->fields().f(qsl("1 as c"));
            this->limit(1);
            this->from(parserbject);
            return*this;
        }

        /**
         * @brief orderby
         * @return
         */
        auto&orderby(){
            auto object=this->oPointer<SqlParserFields<SqlParserSelect>*>(__func__);
            if(object==nullptr){
                object=new SqlParserFields<SqlParserSelect>(this->vMap(__func__));
                this->setPointer(__func__,object);
            }
            return*object;
        }

        /**
         * @brief toScript
         * @param parser
         * @return
         */
        virtual QStringList toScript(SqlSuitableKeyWord&parser){

            const auto groupingTypes=QOrm::TypeUtil::keywordGroupingTypes();

            auto mapObject=this->toMap();
            auto parser_distinct=getVariantStartsWith(qsl("distinct"),mapObject);
            auto parser_limit=getVariantStartsWith(qsl("limit"),mapObject);
            auto parser_offset=getVariantStartsWith(qsl("offset"),mapObject);
            auto parser_lockSkip=getVariantStartsWith(qsl("lockSkip"),mapObject).toBool();
            auto parser_lock=getVariantStartsWith(qsl("lock"),mapObject).toBool();

            auto parser_fields=SqlParserFields<SqlParserSelect>(unionMapStartsWith(qsl("fields"), mapObject));
            auto parser_orderby=SqlParserFields<SqlParserSelect>(unionMapStartsWith(qsl("orderby"), mapObject));

            QStringList parser_groupby;

            static auto local_list=qvsl{qsl("from"), qsl("join"), qsl("where")};
            auto parser_combination=appendMapStartsWith(local_list , mapObject);


            QStringList output;

            if(!parser_fields.isValid()){
                output<<parser.parserCommand(kgcSelectForExists);
            }
            else if(parser_limit.isValid()){
                auto cmd=parser.parserCommand(parser_distinct.isValid()?kgcSelectDistinctTop:kgcSelectTop);
                output<<(cmd.contains(qsl("%1"))?cmd.arg(parser_limit.toInt()):cmd);
            }
            else{
                output<<parser.parserCommand(parser_distinct.isValid()?kgcSelectDistinct:kgcSelect);
            }

            if(parser_fields.isValid()){
                auto fieldList=parser_fields.toMap().values();
                QStringList fields;
                QStringList group;
                bool isGrouping=false;
                for(auto&parser_fields:fieldList){
                    const auto&modelInfo=QOrm::ModelInfo::modelInfo(parser_fields);
                    if(modelInfo.isValid()){
                        QHashIterator<QString, QString> i(modelInfo.propertyTableVsShort());
                        while (i.hasNext()) {
                            i.next();
                            fields<<i.key();
                            group<<i.key();
                        }
                    }
                    else if(qTypeId(parser_fields)==QMetaType_QVariantMap || qTypeId(parser_fields)==QMetaType_QVariantHash || qTypeId(parser_fields)==QMetaType_QVariantList || qTypeId(parser_fields)==QMetaType_QStringList){
                        auto map=parser_fields.toMap();
                        auto grouping=map.value(qsl("grouping")).toInt();
                        auto value=map.value(qsl("value")).toString().trimmed();
                        auto isGroup=(groupingTypes.contains(grouping));
                        isGrouping = isGrouping || isGroup;
                        if(isGroup){
                            fields<<parser.parserCommand(KeywordGenericCommand(grouping),nullptr, map);
                        }
                        else{
                            group<<value;
                            fields<<value;
                        }
                    }
                    else{
                        fields<<parser_fields.toString();
                    }
                    if(isGrouping)
                        parser_groupby<<group;
                }
                //group.sort();
                //fields.sort();
                output<<(fields.isEmpty()?qsl_null:fields.join(','));
            }


            if(!parser_combination.isEmpty()){
                SqlParserCombinations<SqlParserSelect> obj(parser_combination);
                output<<obj.toScript(parser);
            }

            if(!parser_groupby.isEmpty()){
                output<<parser.parserCommand(kgcGroupBy);
                output<<parser_groupby.join(',');
            }

            if(parser_orderby.isValid()){
                auto fieldList=parser_orderby.toMap().values();
                if(!fieldList.isEmpty()){
                    QStringList fields;
                    for(auto&parser_order:fieldList){
                        const auto&modelInfo=QOrm::ModelInfo::modelInfo(parser_order);
                        if(modelInfo.isValid()){
                            QHashIterator<QString, QString> i(modelInfo.propertyTableVsShort());
                            while (i.hasNext()) {
                                i.next();
                                fields<<i.key();
                            }
                        }
                        else{
                            if(qTypeId(parser_order)==QMetaType_QVariantMap || qTypeId(parser_order)==QMetaType_QVariantHash){
                                auto item = SqlParserItem::from(parser_order);
                                QString name;
                                if(item.info()==koiObject)
                                    name=item.value().toString().trimmed();
                                else
                                    name=item.toString().trimmed();
                                if(!fields.contains(name))
                                    fields<<name;
                            }
                            else if(qTypeId(parser_order)==QMetaType_QVariantList || qTypeId(parser_order)==QMetaType_QVariantList){
                                for(auto&v:parser_order.toList()){
                                    auto name=v.toByteArray().trimmed();
                                    if(!fields.contains(name))
                                        fields<<name;
                                }
                            }
                            else{
                                auto name=parser_order.toByteArray().trimmed();
                                if(!fields.contains(name))
                                    fields<<name;
                            }
                        }
                    }
                    if(!fields.isEmpty()){
                        output<<parser.parserCommand(kgcOrderBy);
                        output<<(fields.isEmpty()?qsl_null:fields.join(','));
                    }
                }
            }

            if(parser_limit.isValid()){
                auto cmd=parser.parserCommand(kgcLimit);
                output<<cmd.arg(parser_limit.toInt());
            }

            if(parser_offset.isValid()){
                auto cmd=parser.parserCommand(kgcOffset);
                output<<cmd.arg(parser_offset.toInt());
            }

            if(parser_lockSkip)
                output<<parser.parserCommand(kgcForSkipLockedFinishScript);
            else if(parser_lock)
                output<<parser.parserCommand(kgcForUpdateFinishScript);

            //FOR UPDATE SKIP LOCKED


            return output;
        }
    };

    class Q_ORM_EXPORT SqlParserInsert:public SqlParserCommand{
    public:
        explicit SqlParserInsert():SqlParserCommand(){
        }

        explicit SqlParserInsert(const QVariant&v):SqlParserCommand(v){
        }

        auto&destine(const QVariant&v){
            auto object=this->oPointer<SqlParserCommand*>(__func__);
            if(object==nullptr){
                object=new SqlParserCommand(v);
                this->setPointer(__func__,object);
            }
            return*this;
        }
        auto&values(const QVariant&values){
            auto object=this->oPointer<SqlParserCommand*>(__func__);
            if(object==nullptr){
                object=new SqlParserCommand(values);
                this->setPointer(__func__,object);
            }
            return*this;
        }

        virtual QStringList toScript(SqlSuitableKeyWord&parser){
            auto map=this->toMap();
            auto destine=getVariantStartsWith(qsl("destine"),map);
            auto values=getVariantStartsWith(qsl("values"),map);
            const auto&modelInfo=QOrm::ModelInfo::modelInfo(destine);
            auto command=parser.parserCommand(kgcInsertInto, &modelInfo, values);
            return command;
        }
    };

    class Q_ORM_EXPORT SqlParserUpdate:public SqlParserCommand{
    public:

        auto&destine(const QVariant&v){
            auto object=this->oPointer<SqlParserCommand*>(__func__);
            if(object==nullptr){
                object=new SqlParserCommand(v);
                this->setPointer(__func__,object);
            }
            return*this;
        }
        auto&values(const QVariant&v){
            auto object=this->oPointer<SqlParserCommand*>(__func__);
            if(object==nullptr){
                object=new SqlParserCommand(v);
                this->setPointer(__func__,object);
            }
            return*this;
        }

        explicit SqlParserUpdate():SqlParserCommand(){
        }

        explicit SqlParserUpdate(const QVariant&v):SqlParserCommand(v){
        }

        virtual QStringList toScript(SqlSuitableKeyWord&parser){
            auto map=this->toMap();
            auto destine=getVariantStartsWith(qsl("destine"),map);
            auto values=getVariantStartsWith(qsl("values"), map);
            const auto&modelInfo=QOrm::ModelInfo::modelInfo(destine);
            auto command=parser.parserCommand(kgcUpdateSet, &modelInfo, values);
            return command;
        }
    };

    class Q_ORM_EXPORT SqlParserUpsert:public SqlParserCommand{
    public:

        auto&destine(const QVariant&v){
            auto object=this->oPointer<SqlParserCommand*>(__func__);
            if(object==nullptr){
                object=new SqlParserCommand(v);
                this->setPointer(__func__,object);
            }
            return*this;
        }
        auto&values(const QVariant&v){
            auto object=this->oPointer<SqlParserCommand*>(__func__);
            if(object==nullptr){
                object=new SqlParserCommand(v);
                this->setPointer(__func__,object);
            }
            return*this;
        }

        explicit SqlParserUpsert():SqlParserCommand(){
        }

        explicit SqlParserUpsert(const QVariant&v):SqlParserCommand(v){
        }

        virtual QStringList toScript(SqlSuitableKeyWord&parser){
            auto map=this->toMap();
            auto destine=getVariantStartsWith(qsl("destine"),map);
            auto values=getVariantStartsWith(qsl("values"),map);
            const auto&modelInfo=QOrm::ModelInfo::modelInfo(destine);
            auto command=parser.parserCommand(kgcUpsertSet, &modelInfo, values);
            return command;
        }
    };

    class Q_ORM_EXPORT SqlParserRemove:public SqlParserFrom<SqlParserSelect>{
    public:

        auto&destine(const QVariant&v){
            auto map=this->toMap();
            map.insert(__func__,v);
            this->setValue(map);
            return*this;
        }
        auto&values(const QVariant&values){
            auto map=this->toMap();
            map.insert(__func__,values);
            this->setValue(map);
            return *this;
        }

        explicit SqlParserRemove():SqlParserFrom<SqlParserSelect>(){
        }

        explicit SqlParserRemove(const QVariant&v):SqlParserFrom<SqlParserSelect>(v){
        }

        virtual QStringList toScript(SqlSuitableKeyWord&parser){
            auto mapObject=this->toMap();
            static auto parser_combination=appendMapStartsWith(qvsl{qsl("from"), qsl("join"), qsl("where"), qsl("using")}, mapObject);
            QStringList output;
            if(!parser_combination.isEmpty()){
                output<<parser.parserCommand(kgcDelete);

                if(!parser_combination.isEmpty()){
                    SqlParserCombinations<SqlParserRemove> obj(parser_combination);
                    output<<obj.toScript(parser);
                }
            }
            return output;
        }

    };

    class Q_ORM_EXPORT SqlParserProcedure:public SqlParserCallObject{
    public:
        explicit SqlParserProcedure(const QVariant&v=QVariant()):SqlParserCallObject(v){
        }
        virtual QStringList toScript(SqlSuitableKeyWord&parser){
            auto mapObject=this->toMap();
            QStringList output=parser.parserCallProcedure(mapObject);
            return output;
        }
    };

    class Q_ORM_EXPORT SqlParserFunction:public SqlParserCallObject{
    public:
        explicit SqlParserFunction(const QVariant&v=QVariant()):SqlParserCallObject(v){
        }

        auto&nextVal(const QVariant&v){
            auto object=new SqlParserCommand();
            object->makeUuid();
            QVariantMap map;
            map[qsl("type")]=kgcNextValSelect;
            map[qsl("object")]=v;
            object->setValue(map);
            this->setPointer(this->suuid(), object);
            return*this;
        }

        virtual QStringList toScript(SqlSuitableKeyWord&parser){
            auto mapObject=this->toMap();
            QMapIterator<QString, QVariant> i(mapObject);
            QStringList output;
            while (i.hasNext()) {
                i.next();
                auto vValue=i.value().toMap();
                auto type=vValue.value(qsl("type")).toInt();
                auto object=vValue.value(qsl("object"));
                auto value=vValue.value(qsl("value"));
                QStringList command;
                const auto&modelInfo=QOrm::ModelInfo::modelInfo(object);
                if(modelInfo.isValid())
                    command=parser.parserCommand(type, &modelInfo, value);
                else
                    command=parser.parserCommand(type, &modelInfo, object);
                for(auto&v:command)
                    output<<v;
            }

            return output;
        }
    };

    class Q_ORM_EXPORT SqlParserStructure:public SqlParserCallObject{
    public:
        explicit SqlParserStructure(const QVariant&v=QVariant()):SqlParserCallObject(v){
        }

        virtual bool ignorePrepare(){
            return true;
        }

        auto&truncateTable(const QVariant&v){
            auto object=new SqlParserCommand();
            object->makeUuid();
            QVariantMap map;
            map[qsl("type")]=kgcTruncateTable;
            map[qsl("object")]=v;
            object->setValue(map);
            this->setPointer(this->suuid(), object);
            return*this;
        }

        auto&truncateTableCascade(const QVariant&v){
            auto object=new SqlParserCommand();
            object->makeUuid();
            QVariantMap map;
            map[qsl("type")]=kgcTruncateTableCacade;
            map[qsl("object")]=v;
            object->setValue(map);
            this->setPointer(this->suuid(), object);
            return*this;
        }

        virtual QStringList toScript(SqlSuitableKeyWord&parser){
            auto mapObject=this->toMap();
            QMapIterator<QString, QVariant> i(mapObject);
            QStringList output;
            while (i.hasNext()) {
                i.next();
                auto vValue=i.value().toMap();
                auto type=vValue.value(qsl("type")).toInt();
                auto object=vValue.value(qsl("object"));
                auto value=vValue.value(qsl("value"));
                QStringList command;
                const auto&modelInfo=QOrm::ModelInfo::modelInfo(object);
                if(modelInfo.isValid())
                    command=parser.parserCommand(type, &modelInfo, value);
                for(auto&v:command)
                    output<<v;
            }

            return output;
        }
    };

    class Q_ORM_EXPORT SqlParserCombineSelect:public SqlParserCommand{
        friend class SqlSuitableBuilderPvt;
    public:

        explicit SqlParserCombineSelect(const QVariant&v);

        explicit SqlParserCombineSelect();

        ~SqlParserCombineSelect();

        SqlParserCombineSelect&unionMerge();

        SqlParserCombineSelect&unionAll();

        SqlParserCombineSelect&intersect();

        SqlParserCombineSelect&intersectAll();

        SqlParserCombineSelect&except();

        SqlParserCombineSelect&exceptAll();
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
