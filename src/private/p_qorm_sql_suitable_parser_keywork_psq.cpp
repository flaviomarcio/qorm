#include "./p_qorm_sql_suitable_parser_keywork_psq.h"
#include "../qorm_sql_suitable_types.h"


namespace QOrm {

SqlSuitableKeyWordPSql::SqlSuitableKeyWordPSql(QObject *parent):SqlSuitableKeyWord(QSqlDriver::PostgreSQL, parent)
{

}

SqlSuitableKeyWordPSql::~SqlSuitableKeyWordPSql()
{
}

void SqlSuitableKeyWordPSql::init()
{
    this->commands()[kgcSelectTop                ]=QByteArrayLiteral("select");
    this->commands()[kgcSelectForExists          ]=QByteArrayLiteral("select");
    this->commands()[kgcSelectDistinct           ]=QByteArrayLiteral("select distinct");
    this->commands()[kgcSelectDistinctTop        ]=QByteArrayLiteral("select distinct");
    this->commands()[kgcIsNull                   ]=QByteArrayLiteral("coalesce");
    this->commands()[kgcIsNullCheckValue         ]=QByteArrayLiteral("coalesce(%1, %2)");
    this->commands()[kgcUpsertSetValues          ]=QByteArrayLiteral("merge %1 u using(%2)as m on when matched update set %3 when not matched %4 %5");
    this->commands()[kgcUpsertSet                ]={};
    this->commands()[kgcSetApplicationName       ]=QByteArrayLiteral("set application_name to '%1'");
    this->commands()[kgcSetSearchPath            ]=QByteArrayLiteral("set search_path = %1");
    this->commands()[kgcSetTransactionReadOnlyOn ]=QByteArrayLiteral("set default_transaction_read_only = on");
    this->commands()[kgcSetTransactionReadOnlyOff]=QByteArrayLiteral("set default_transaction_read_only = off");
    this->commands()[kgcForUpdateFinishScript    ]=QByteArrayLiteral("for update");
    this->commands()[kgcForSkipLockedFinishScript]=QByteArrayLiteral("for update skip locked");
    this->commands()[kgcNextVal                  ]=QByteArrayLiteral("nextval");
    this->commands()[kgcNextValSelect            ]=QByteArrayLiteral("select nextval('%1') as value");
    this->commands()[kgcOffset                   ]=QByteArrayLiteral("offset %1");
    this->commands()[kgcILike                    ]=QByteArrayLiteral("%1 ilike %2 ").trimmed().toLower();
    this->commands()[kgcLike                     ]=QByteArrayLiteral("%1 ilike %2 ").trimmed().toLower();

}

QString SqlSuitableKeyWordPSql::parserCommand(int command){
    return SqlSuitableKeyWord::parserCommand(command);
}

QStringList SqlSuitableKeyWordPSql::parserCommand(int command, const ModelInfo *modelInfo, const QVariant &value){
    QStringList RETURN;
    if(command==kgcUpdate || command==kgcUpdateSet || command==kgcUpdateSetValues){
        auto listRecords=value;
        switch (listRecords.typeId()) {
        case QMetaType::QVariantList:
        case QMetaType::QStringList:
        {
            auto l=listRecords.toList();
            listRecords=l.size()==1?l.first():l;
            break;
        }
        default:
            break;
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
                for(auto &v1:propertyTableList){
                    auto property=modelInfo->propertyByFieldName().value(v1);
                    auto vType=property.typeId();
                    auto v2=v1;
                    if(vType==QMetaType::QUuid)
                        v2+=QStringLiteral("::uuid");
                    else if(vType==QMetaType::QDateTime)
                        v2+=QStringLiteral("::timestamp");
                    else if(vType==QMetaType::QDate)
                        v2+=QStringLiteral("::date");
                    fields.append(QStringLiteral("%1=s.%2").arg(v1, v2));
                }
                tableFieldsSet=fields.join(QStringLiteral(","));
            }

            QString RETURNING;
            {
                QStringList l;
                for(auto &v:modelInfo->tablePk())
                    l.append(QStringLiteral("u.")+v);
                RETURNING = QStringLiteral("returning %1").arg(l.join(QStringLiteral(",")));
            }

            if(!modelInfo->tablePk().isEmpty()){
                QStringList fieldsWhere;
                for(auto &v1:modelInfo->tablePk()){

                    auto property=modelInfo->propertyByFieldName().value(v1);
                    auto vType=property.typeId();

                    auto v2=v1;
                    if(vType==QMetaType::QUuid)
                        v2+=QStringLiteral("::uuid");
                    else if(vType==QMetaType::QDateTime)
                        v2+=QStringLiteral("::timestamp");
                    else if(vType==QMetaType::QDate)
                        v2+=QStringLiteral("::date");

                    fieldsWhere.append(QStringLiteral("u.%1 = s.%2").arg(v1,v2));
                }
                static const auto __whereFormat=QStringLiteral("where %1");
                tableWhere=__whereFormat.arg(fieldsWhere.join(QStringLiteral(" and ")));
            }

            tableFromValues=this->parserCommand(kgcFromValues).arg(values.join(QStringLiteral(",")),QStringLiteral("s"),propertyTableList.join(QStringLiteral(",")));
            auto v1=this->parserCommand(kgcUpdateSet).arg(tableNameFull+QStringLiteral(" as u"), tableFieldsSet,tableFromValues,tableWhere,RETURNING);
            RETURN.append(v1);
        }
        return RETURN;
    }
    else if(command==kgcUpsertSet || command == kgcUpsertSetValues){

        auto propertyTableList=modelInfo->propertyTableList();
        QVariantList list;
        if(value.typeId()==QMetaType::QVariantList)
            list=value.toList();
        else
            list.append(value);

        QVariantList listRecords;
        for(auto &v:list){
            if(v.typeId()==QMetaType::QVariantMap || v.typeId()==QMetaType::QVariantHash)
                listRecords.append(v);
            else
                listRecords.append(QVariantMap{});//deve gerar erro
        }

        if(!listRecords.isEmpty()){
            //auto tablePk=modelInfo->tablePk();
            auto RETURNING=QStringLiteral("returning %1").arg(modelInfo->tablePk().join(QStringLiteral(",")));

            QString mergeUsingOn;
            QString mergeUpdate;
            QString mergeInsert;
            {
                {
                    QStringList onA;
                    QStringList onB;
                    for(auto &v:modelInfo->tablePk()){
                        onA.append(QStringLiteral("m.%1").arg(v));
                        onB.append(QStringLiteral("u.%1").arg(v));
                    }
                    mergeUsingOn=QStringLiteral("%1 = %2").arg(onA.join(QStringLiteral(",")), onB.join(QStringLiteral(",")));
                }

                {
                    QStringList fields;
                    for(auto &v1:propertyTableList){
                        auto property=modelInfo->propertyByFieldName().value(v1);
                        auto vType=property.typeId();
                        auto v2=v1;
                        if(vType==QMetaType::QUuid)
                            v2+=QStringLiteral("::uuid");
                        else if(vType==QMetaType::QDateTime)
                            v2+=QStringLiteral("::timestamp");
                        else if(vType==QMetaType::QDate)
                            v2+=QStringLiteral("::date");
                        fields.append(QStringLiteral("%1=excluded.%2").arg(v1, v2));
                    }

                    mergeUpdate=fields.join(QStringLiteral(","));
                }

                {
                    if(modelInfo->tablePkAutoGenerate() ){
                        for(auto &v:modelInfo->tablePk())
                            propertyTableList.removeAll(v);
                    }

                    //QStringList fields;
                    QStringList onA;
                    QStringList onB;
                    for(auto &v:propertyTableList){
                        onA.append(QStringLiteral("%1").arg(v));
                        onB.append(QStringLiteral("m.%1").arg(v));
                    }
                    mergeInsert=QStringLiteral("insert into %1 (%2)values(%3)").arg(modelInfo->tableNameFull(),onA.join(QStringLiteral(",")), onB.join(QStringLiteral(",")));
                }

            }

            auto values=this->formatValues(propertyTableList, listRecords);
            if(!values.isEmpty()){
                //auto mergeTable = QString("merge %1 u").arg(modelInfo->tableNameFull());
                //auto usingValues = QString("using(select*from (values %1) as x (%2)) u on %3").arg(values.join(QStringLiteral(","))).arg(propertyTableList.join(QStringLiteral(","))).arg(mergeUsingOn);
                //auto matchedUpdate = QString("matched update %1").arg(mergeUpdate);
                //auto matchedInsert = QString("not matched update %1").arg(mergeInsert);
                //auto v1=QString("%1 %2 %3 %4").arg(mergeTable).arg(usingValues).arg(matchedUpdate).arg(matchedInsert);

                //merge %1 u using(%2)as m on when matched update set %3 when not matched %4 %5
                //auto v1=this->parserCommand(kgcUpsertSetValues).arg().arg(usingValues).arg(mergeUpdate).arg(mergeInsert).arg(RETURNING);

                mergeInsert=QStringLiteral("insert into %1 (%2)values %3").arg(modelInfo->tableNameFull(), modelInfo->propertyTableList().join(QStringLiteral(",")), values.join(QStringLiteral(",")));

                auto v1=QStringLiteral("%1 on conflict(%2) do update set %3 %4 ").arg(mergeInsert, modelInfo->tablePk().join(QStringLiteral(",")), mergeUpdate, RETURNING);

                RETURN.append(v1);
            }
        }
        return RETURN;
    }
    return SqlSuitableKeyWord::parserCommand(command, modelInfo, value);
}

}
