#pragma once

#include "./p_qorm_sql_suitable_parser_keywork.h"
#include "./p_qorm_model_info.h"
#include "../qorm_sql_suitable_types.h"

namespace QOrm {

class Q_ORM_EXPORT SqlSuitableKeyWordPSql : public SqlSuitableKeyWord{
public:
    explicit SqlSuitableKeyWordPSql(QObject *parent = nullptr):SqlSuitableKeyWord(QSqlDriver::PostgreSQL, parent){

    }
    ~SqlSuitableKeyWordPSql(){
    }
    virtual void init(){
        this->commands()[kgcSelectTop                ]=qbl("select");
        this->commands()[kgcSelectForExists          ]=qbl("select");
        this->commands()[kgcSelectDistinct           ]=qbl("select distinct");
        this->commands()[kgcSelectDistinctTop        ]=qbl("select distinct");
        this->commands()[kgcIsNull                   ]=qbl("coalesce");
        this->commands()[kgcIsNullCheckValue         ]=qbl("coalesce(%1, %2)");
        this->commands()[kgcUpsertSetValues          ]=qbl("merge %1 u using(%2)as m on when matched update set %3 when not matched %4 %5");
        this->commands()[kgcUpsertSet                ]=qbl_null;
        this->commands()[kgcSetApplicationName       ]=qbl("set application_name to '%1'");
        this->commands()[kgcSetSearchPath            ]=qbl("set search_path = %1");
        this->commands()[kgcSetTransactionReadOnlyOn ]=qbl("set default_transaction_read_only = on");
        this->commands()[kgcSetTransactionReadOnlyOff]=qbl("set default_transaction_read_only = off");
        this->commands()[kgcForUpdateFinishScript    ]=qbl("for update");
        this->commands()[kgcForSkipLockedFinishScript]=qbl("for update skip locked");
        this->commands()[kgcNextVal                  ]=qbl("nextval");
        this->commands()[kgcNextValSelect            ]=qbl("select nextval('%1') as value");
        this->commands()[kgcOffset                   ]=qbl("offset %1");
        this->commands()[kgcILike                    ]=qbl("%1 ilike %2 ").trimmed().toLower();
        this->commands()[kgcLike                     ]=qbl("%1 ilike %2 ").trimmed().toLower();

    }

    virtual QString parserCommand(int command){
        return SqlSuitableKeyWord::parserCommand(command);
    }

    virtual QStringList parserCommand(int command, const ModelInfo *modelInfo, const QVariant&value){
        QStringList RETURN;
        if(command==kgcUpdate || command==kgcUpdateSet || command==kgcUpdateSetValues){
            auto listRecords=value;
            if(qTypeId(listRecords)==QMetaType_QVariantList){
                auto l=listRecords.toList();
                listRecords=l.size()==1?l.first():l;
            }
            else if(qTypeId(listRecords)==QMetaType_QStringList){
                auto l=listRecords.toStringList();
                listRecords=l.size()==1?QVariant(l.first()):l;
            }

            auto tableNameFull=modelInfo->tableNameFull();
            auto propertyTableList=modelInfo->propertyTableList();

            auto values=this->formatValues(propertyTableList, listRecords);

            if(!values.isEmpty()){
                QString tableFieldsSet;
                QString tableFromValues;
                QString tableWhere;

                {
                    QStringList fields;
                    for(auto&v1:propertyTableList){
                        auto property=modelInfo->propertyByFieldName().value(v1);
                        auto vType=qTypeId(property);
                        auto v2=v1;
                        if(vType==QMetaType_QUuid)
                            v2+=qsl("::uuid");
                        else if(vType==QMetaType_QDateTime)
                            v2+=qsl("::timestamp");
                        else if(vType==QMetaType_QDate)
                            v2+=qsl("::date");
                        fields << qsl("%1=s.%2").arg(v1, v2);
                    }
                    tableFieldsSet=fields.join(qsl(","));
                }

                QString RETURNING;
                {
                    QStringList l;
                    for(auto&v:modelInfo->tablePk())
                        l<<qsl("u.")+v;
                    RETURNING = qsl("RETURNING %1").arg(l.join(qsl(",")));
                }

                if(!modelInfo->tablePk().isEmpty()){
                    QStringList fieldsWhere;
                    for(auto&v1:modelInfo->tablePk()){

                        auto property=modelInfo->propertyByFieldName().value(v1);
                        auto vType=qTypeId(property);

                        auto v2=v1;
                        if(vType==QMetaType_QUuid)
                            v2+=qsl("::uuid");
                        else if(vType==QMetaType_QDateTime)
                            v2+=qsl("::timestamp");
                        else if(vType==QMetaType_QDate)
                            v2+=qsl("::date");

                        fieldsWhere<<qsl("u.%1 = s.%2").arg(v1,v2);
                    }
                    tableWhere=qsl("where %1").arg(fieldsWhere.join(qsl(" and ")));
                }

                tableFromValues=this->parserCommand(kgcFromValues).arg(values.join(qsl(",")),qsl("s"),propertyTableList.join(qsl(",")));
                auto v1=this->parserCommand(kgcUpdateSet).arg(tableNameFull+qsl(" as u"), tableFieldsSet,tableFromValues,tableWhere,RETURNING);
                RETURN<<v1;
            }
            return RETURN;
        }
        else if(command==kgcUpsertSet || command == kgcUpsertSetValues){

            auto propertyTableList=modelInfo->propertyTableList();
            QVariantList list;
            if(qTypeId(value)==QMetaType_QVariantList)
                list=value.toList();
            else
                list<<value;

            QVariantList listRecords;
            for(auto&v:list){
                if(qTypeId(v)==QMetaType_QVariantMap || qTypeId(v)==QMetaType_QVariantHash)
                    listRecords<<v;
                else
                    listRecords<<QVariantMap();//deve gerar erro
            }

            if(!listRecords.isEmpty()){
                auto tablePk=modelInfo->tablePk();
                auto RETURNING=qsl("RETURNING %1").arg(modelInfo->tablePk().join(qsl(",")));

                QString mergeUsingOn;
                QString mergeUpdate;
                QString mergeInsert;
                {
                   {
                        QStringList onA;
                        QStringList onB;
                        for(auto&v:modelInfo->tablePk()){
                            onA << qsl("m.%1").arg(v);
                            onB << qsl("u.%1").arg(v);
                        }
                        mergeUsingOn=qsl("%1 = %2").arg(onA.join(qsl(",")), onB.join(qsl(",")));
                    }

                    {
                        QStringList fields;
                        for(auto&v1:propertyTableList){
                            auto property=modelInfo->propertyByFieldName().value(v1);
                            auto vType=qTypeId(property);
                            auto v2=v1;
                            if(vType==QMetaType_QUuid)
                                v2+=qsl("::uuid");
                            else if(vType==QMetaType_QDateTime)
                                v2+=qsl("::timestamp");
                            else if(vType==QMetaType_QDate)
                                v2+=qsl("::date");
                            fields << qsl("%1=excluded.%2").arg(v1, v2);
                        }

                        mergeUpdate=fields.join(qsl(","));
                    }

                    {
                        if(modelInfo->tablePkAutoGenerate() ){
                            for(auto&v:modelInfo->tablePk())
                                propertyTableList.removeAll(v);
                        }

                        QStringList fields;
                        QStringList onA;
                        QStringList onB;
                        for(auto&v:propertyTableList){
                            onA << qsl("%1").arg(v);
                            onB << qsl("m.%1").arg(v);
                        }
                        mergeInsert=qsl("insert into %1 (%2)values(%3)").arg(modelInfo->tableNameFull(),onA.join(qsl(",")), onB.join(qsl(",")));
                    }

                }

                auto values=this->formatValues(propertyTableList, listRecords);
                if(!values.isEmpty()){
                    //auto mergeTable = QString("merge %1 u").arg(modelInfo->tableNameFull());
                    //auto usingValues = QString("using(select*from (values %1) as x (%2)) u on %3").arg(values.join(qsl(","))).arg(propertyTableList.join(qsl(","))).arg(mergeUsingOn);
                    //auto matchedUpdate = QString("matched update %1").arg(mergeUpdate);
                    //auto matchedInsert = QString("not matched update %1").arg(mergeInsert);
                    //auto v1=QString("%1 %2 %3 %4").arg(mergeTable).arg(usingValues).arg(matchedUpdate).arg(matchedInsert);

                    //merge %1 u using(%2)as m on when matched update set %3 when not matched %4 %5
                    //auto v1=this->parserCommand(kgcUpsertSetValues).arg().arg(usingValues).arg(mergeUpdate).arg(mergeInsert).arg(RETURNING);

                    mergeInsert=qsl("insert into %1 (%2)values %3").arg(modelInfo->tableNameFull(), modelInfo->propertyTableList().join(qsl(",")), values.join(qsl(",")));

                    auto v1=qsl("%1 on conflict(%2) DO UPDATE SET %3 %4 ").arg(mergeInsert, modelInfo->tablePk().join(qsl(",")), mergeUpdate, RETURNING);

                    RETURN<<v1;
                }
            }
            return RETURN;
        }
        return SqlSuitableKeyWord::parserCommand(command, modelInfo, value);
    }
};

}
