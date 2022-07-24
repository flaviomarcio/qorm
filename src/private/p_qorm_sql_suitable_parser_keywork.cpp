#include "./p_qorm_sql_suitable_parser_keywork.h"
#include "./p_qorm_sql_suitable_parser_keywork_ansi.h"
#include "./p_qorm_sql_suitable_parser_keywork_psq.h"
#include "./p_qorm_sql_suitable_parser_keywork_tsql.h"
#include "./p_qorm_sql_suitable_parser_keywork_sqlite.h"
#include "./p_qorm_sql_suitable_parser_keywork_oracle.h"
#include "./p_qorm_sql_suitable_parser_keywork_mysql.h"
#include "./p_qorm_model_info.h"
#include "../qorm_macro.h"
#include <QMutex>
#include <QStringList>
#include <QCoreApplication>
#include <QStm>

namespace QOrm {

typedef QMap<QSqlDriver::DbmsType,QOrm::SqlSuitableKeyWord*> DriverSuiteMap;
typedef QList<QOrm::SqlSuitableKeyWord*> DriverSuiteList;


Q_GLOBAL_STATIC_WITH_ARGS(QString, format_date_time, ("yyyy-MM-dd hh:mm:ss.zzz"))
Q_GLOBAL_STATIC_WITH_ARGS(QString, format_date, ("yyyy-MM-dd"))
Q_GLOBAL_STATIC_WITH_ARGS(QString, format_time, ("hh:mm:ss.zzz"))
Q_GLOBAL_STATIC_WITH_ARGS(QDate, __d1900, (QDate{1900,1,1}))
Q_GLOBAL_STATIC_WITH_ARGS(QDateTime, __dt1900, (QDateTime{QDate{1900,1,1}, {}}))

Q_GLOBAL_STATIC(QMutex, staticSqlSuitableKeyWordLocker)
Q_GLOBAL_STATIC(DriverSuiteMap, staticSqlSuitableKeyWord)
Q_GLOBAL_STATIC(DriverSuiteList, staticSqlSuitableKeyWordList)

static void deinitKeyWork()
{
    qDeleteAll(*staticSqlSuitableKeyWordList);
    staticSqlSuitableKeyWordList->clear();
    staticSqlSuitableKeyWord->clear();
}

static void init()
{
    QORM_DECLARE_KEY_WORD(SqlSuitableKeyWordAnsi);
    QORM_DECLARE_KEY_WORD(SqlSuitableKeyWordMySql);
    QORM_DECLARE_KEY_WORD(SqlSuitableKeyWordOracle);
    QORM_DECLARE_KEY_WORD(SqlSuitableKeyWordPSql);
    QORM_DECLARE_KEY_WORD(SqlSuitableKeyWordSQLite);
    QORM_DECLARE_KEY_WORD(SqlSuitableKeyWordTSQL);
    qAddPostRoutine(deinitKeyWork); //gera loop infinito e nao deixa a aplicacao terminar
}

Q_COREAPP_STARTUP_FUNCTION(init);


class SqlSuitableKeyWordPvt:public QObject{
public:
    QList<QSqlDriver::DbmsType> drivers;
    KeywordCache staticKeywordCache;
    SqlSuitableKeyWord*parent=nullptr;

    explicit SqlSuitableKeyWordPvt(SqlSuitableKeyWord*parent):QObject{parent}
    {
        this->parent=parent;
    }

    explicit SqlSuitableKeyWordPvt(SqlSuitableKeyWord*parent, QSqlDriver::DbmsType driver):QObject{parent}
    {
        this->parent=parent;
        if(!this->drivers.contains(driver))
            this->drivers.append(driver);
    }
    explicit SqlSuitableKeyWordPvt(SqlSuitableKeyWord*parent, QList<QSqlDriver::DbmsType> drivers):QObject{parent}
    {
        this->parent=parent;
        this->drivers=drivers;
    }
    virtual ~SqlSuitableKeyWordPvt()
    {

    }

    auto isValid()
    {
        return !this->drivers.isEmpty();
    }


    void init()
    {
        QMutexLocker<QMutex> locker(staticSqlSuitableKeyWordLocker);
        auto vList=this->parent->drivers();
        for(auto &driver : vList){
            if(staticSqlSuitableKeyWordList->contains(this->parent))
                staticSqlSuitableKeyWordList->append(this->parent);

            if(!staticSqlSuitableKeyWord->contains(driver))
                staticSqlSuitableKeyWord->insert(driver, this->parent);
        }

        {
            staticKeywordCache[kgcAll                   ]=QByteArrayLiteral("All "                                  ).trimmed().toLower();
            staticKeywordCache[kgcAlter                 ]=QByteArrayLiteral("Alter "                                ).trimmed().toLower();
            staticKeywordCache[kgcAnd                   ]=QByteArrayLiteral("And "                                  ).trimmed().toLower();
            staticKeywordCache[kgcAny                   ]=QByteArrayLiteral("%1 Any(%2) "                           ).trimmed().toLower();
            staticKeywordCache[kgcAs                    ]=QByteArrayLiteral("As "                                   ).trimmed().toLower();
            staticKeywordCache[kgcAsc                   ]=QByteArrayLiteral("Asc "                                  ).trimmed().toLower();
            staticKeywordCache[kgcAvg                   ]=QByteArrayLiteral("Avg(%1) "                              ).trimmed().toLower();
            staticKeywordCache[kgcAvgAs                 ]=QByteArrayLiteral("Avg(%1) as %2 "                        ).trimmed().toLower();
            staticKeywordCache[kgcBetween               ]=QByteArrayLiteral("%1 Between %2 and %3 "                 ).trimmed().toLower();
            staticKeywordCache[kgcNotBetween            ]=QByteArrayLiteral("not %1 Between %2 and %3 "             ).trimmed().toLower();
            staticKeywordCache[kgcBigger                ]=QByteArrayLiteral("%1 > %2 "                              ).trimmed().toLower();
            staticKeywordCache[kgcCascade               ]=QByteArrayLiteral("cascade "                              ).trimmed().toLower();
            staticKeywordCache[kgcCase                  ]=QByteArrayLiteral("Case "                                 ).trimmed().toLower();
            staticKeywordCache[kgcCheck                 ]=QByteArrayLiteral("Check "                                ).trimmed().toLower();
            staticKeywordCache[kgcClustered             ]=QByteArrayLiteral("Clustered "                            ).trimmed().toLower();
            staticKeywordCache[kgcConstraint            ]=QByteArrayLiteral("Constraint "                           ).trimmed().toLower();
            staticKeywordCache[kgcCount                 ]=QByteArrayLiteral("Count(%1) "                            ).trimmed().toLower();
            staticKeywordCache[kgcCountAs               ]=QByteArrayLiteral("Count(%1) as %2 "                      ).trimmed().toLower();
            staticKeywordCache[kgcCreate                ]=QByteArrayLiteral("Create "                               ).trimmed().toLower();
            staticKeywordCache[kgcCreateOrReplace       ]=QByteArrayLiteral("create or Replace "                    ).trimmed().toLower();
            staticKeywordCache[kgcDataBase              ]=QByteArrayLiteral("DataBase "                             ).trimmed().toLower();
            staticKeywordCache[kgcDeclare               ]=QByteArrayLiteral("Declare "                              ).trimmed().toLower();
            staticKeywordCache[kgcDefault               ]=QByteArrayLiteral("Default "                              ).trimmed().toLower();
            staticKeywordCache[kgcDelete                ]=QByteArrayLiteral("delete "                               ).trimmed().toLower();
            staticKeywordCache[kgcDeleteFrom            ]=QByteArrayLiteral("delete from %1 "                       ).trimmed().toLower();
            staticKeywordCache[kgcDeleteFromUsing       ]=QByteArrayLiteral("delete from %1 using %2 %3 "           ).trimmed().toLower();
            staticKeywordCache[kgcDesc                  ]=QByteArrayLiteral("Desc "                                 ).trimmed().toLower();
            staticKeywordCache[kgcDistinct              ]=QByteArrayLiteral("Distinct "                             ).trimmed().toLower();
            staticKeywordCache[kgcDrop                  ]=QByteArrayLiteral("Drop "                                 ).trimmed().toLower();
            staticKeywordCache[kgcEnd                   ]=QByteArrayLiteral("End "                                  ).trimmed().toLower();
            staticKeywordCache[kgcEqual                 ]=QByteArrayLiteral("%1 = %2 "                              ).trimmed().toLower();
            staticKeywordCache[kgcNotEqual              ]=QByteArrayLiteral("%1 <> %2 "                             ).trimmed().toLower();
            staticKeywordCache[kgcEqualBigger           ]=QByteArrayLiteral("%1 >= %2 "                             ).trimmed().toLower();
            staticKeywordCache[kgcEqualMinor            ]=QByteArrayLiteral("%1 <= %2 "                             ).trimmed().toLower();
            staticKeywordCache[kgcExists                ]=QByteArrayLiteral("exists "                               ).trimmed().toLower();
            staticKeywordCache[kgcForeignKey            ]=QByteArrayLiteral("Foreign Key "                          ).trimmed().toLower();
            staticKeywordCache[kgcFrom                  ]=QByteArrayLiteral("from "                                 ).trimmed().toLower();
            staticKeywordCache[kgcFromValues            ]=QByteArrayLiteral("from(values %1 )as %2 (%3) "           ).trimmed().toLower();
            staticKeywordCache[kgcFull                  ]=QByteArrayLiteral("Full "                                 ).trimmed().toLower();
            staticKeywordCache[kgcFunction              ]=QByteArrayLiteral("Function "                             ).trimmed().toLower();
            staticKeywordCache[kgcFunction              ]=QByteArrayLiteral("Function "                             ).trimmed().toLower();
            staticKeywordCache[kgcGroupBy               ]=QByteArrayLiteral("Group by "                             ).trimmed().toLower();
            staticKeywordCache[kgcHaving                ]=QByteArrayLiteral("Having "                               ).trimmed().toLower();
            staticKeywordCache[kgcIfExists              ]=QByteArrayLiteral("if exists "                            ).trimmed().toLower();
            staticKeywordCache[kgcIn                    ]=QByteArrayLiteral("%1 In (%2) "                           ).trimmed().toLower();
            staticKeywordCache[kgcIndex                 ]=QByteArrayLiteral("Index "                                ).trimmed().toLower();
            staticKeywordCache[kgcInOut                 ]=QByteArrayLiteral("not %1 in (%2) "                       ).trimmed().toLower();
            staticKeywordCache[kgcInsert                ]=QByteArrayLiteral("Insert "                               ).trimmed().toLower();
            staticKeywordCache[kgcInsertInto            ]=QByteArrayLiteral("Insert into %1(%2) "                   ).trimmed().toLower();
            staticKeywordCache[kgcIsNull                ]=QByteArrayLiteral(""                                      ).trimmed().toLower();
            staticKeywordCache[kgcIsNullCheckValue      ]=QByteArrayLiteral(""                                      ).trimmed().toLower();
            staticKeywordCache[kgcValueIsNull           ]=QByteArrayLiteral("%1 is null "                           ).trimmed().toLower();
            staticKeywordCache[kgcValueIsNotNull        ]=QByteArrayLiteral("not %1 is null "                       ).trimmed().toLower();
            staticKeywordCache[kgcJoin                  ]=QByteArrayLiteral("Join "                                 ).trimmed().toLower();
            staticKeywordCache[kgcJoinFull              ]=QByteArrayLiteral("Full Join "                            ).trimmed().toLower();
            staticKeywordCache[kgcJoinInner             ]=QByteArrayLiteral("Inner Join "                           ).trimmed().toLower();
            staticKeywordCache[kgcJoinLeft              ]=QByteArrayLiteral("Left Join "                            ).trimmed().toLower();
            staticKeywordCache[kgcJoinRigth             ]=QByteArrayLiteral("Rigth Join "                           ).trimmed().toLower();
            staticKeywordCache[kgcKey                   ]=QByteArrayLiteral("Key "                                  ).trimmed().toLower();
            staticKeywordCache[kgcLeft                  ]=QByteArrayLiteral("Left "                                 ).trimmed().toLower();
            staticKeywordCache[kgcILike                 ]=QByteArrayLiteral("%1 like %2 "                           ).trimmed().toLower();
            staticKeywordCache[kgcLike                  ]=QByteArrayLiteral("%1 like %2 "                           ).trimmed().toLower();
            staticKeywordCache[kgcSoundexEqual          ]=QByteArrayLiteral("soundex(%1) = soundex(%2)"             ).trimmed().toLower();
            staticKeywordCache[kgcLimit                 ]=QByteArrayLiteral("Limit %1 "                             ).trimmed().toLower();
            staticKeywordCache[kgcOffset                ]=QByteArrayLiteral("offset %1 "                            ).trimmed().toLower();
            staticKeywordCache[kgcMax                   ]=QByteArrayLiteral("Max(%1) "                              ).trimmed().toLower();
            staticKeywordCache[kgcMaxAs                 ]=QByteArrayLiteral("Max(%1) as %2 "                        ).trimmed().toLower();
            staticKeywordCache[kgcMin                   ]=QByteArrayLiteral("Min(%1) "                              ).trimmed().toLower();
            staticKeywordCache[kgcMinAs                 ]=QByteArrayLiteral("Min(%1) as %2 "                        ).trimmed().toLower();
            staticKeywordCache[kgcMinor                 ]=QByteArrayLiteral("%1 < %2 "                              ).trimmed().toLower();
            staticKeywordCache[kgcNot                   ]=QByteArrayLiteral("Not "                                  ).trimmed().toLower();
            staticKeywordCache[kgcNotExists             ]=QByteArrayLiteral("not Exists "                           ).trimmed().toLower();
            staticKeywordCache[kgcNotNull               ]=QByteArrayLiteral("Not Null "                             ).trimmed().toLower();
            staticKeywordCache[kgcNull                  ]=QByteArrayLiteral("Null "                                 ).trimmed().toLower();
            staticKeywordCache[kgcOn                    ]=QByteArrayLiteral("On "                                   ).trimmed().toLower();
            staticKeywordCache[kgcOr                    ]=QByteArrayLiteral("or "                                   ).trimmed().toLower();
            staticKeywordCache[kgcOrderBy               ]=QByteArrayLiteral("Order by "                             ).trimmed().toLower();
            staticKeywordCache[kgcJoinOuter             ]=QByteArrayLiteral("Outer Join "                           ).trimmed().toLower();
            staticKeywordCache[kgcPivot                 ]=QByteArrayLiteral("Pivot "                                ).trimmed().toLower();
            staticKeywordCache[kgcPrimary               ]=QByteArrayLiteral("Primary "                              ).trimmed().toLower();
            staticKeywordCache[kgcPrimaryKey            ]=QByteArrayLiteral("Primary Key "                          ).trimmed().toLower();
            staticKeywordCache[kgcProcedure             ]=QByteArrayLiteral("Procedure "                            ).trimmed().toLower();
            staticKeywordCache[kgcReplace               ]=QByteArrayLiteral("Replace "                              ).trimmed().toLower();
            staticKeywordCache[kgcRigth                 ]=QByteArrayLiteral("Rigth "                                ).trimmed().toLower();
            staticKeywordCache[kgcRowNumber             ]=QByteArrayLiteral("RowNumber "                            ).trimmed().toLower();
            staticKeywordCache[kgcSchema                ]=QByteArrayLiteral("Schema "                               ).trimmed().toLower();
            staticKeywordCache[kgcSelect                ]=QByteArrayLiteral("Select "                               ).trimmed().toLower();
            staticKeywordCache[kgcSelectTop             ]=QByteArrayLiteral("Select Top %1 "                        ).trimmed().toLower();
            staticKeywordCache[kgcSelectDistinct        ]=QByteArrayLiteral("Select distinct "                      ).trimmed().toLower();
            staticKeywordCache[kgcSelectDistinctTop     ]=QByteArrayLiteral("Select distinct top %1 "               ).trimmed().toLower();
            staticKeywordCache[kgcSelectForExists       ]=QByteArrayLiteral("Select null "                          ).trimmed().toLower();
            staticKeywordCache[kgcSelectValues          ]=QByteArrayLiteral("select %1 from(values(%2))as %3(%4) "  ).trimmed().toLower();
            staticKeywordCache[kgcSet                   ]=QByteArrayLiteral("Set "                                  ).trimmed().toLower();
            staticKeywordCache[kgcSum                   ]=QByteArrayLiteral("sum(%1) "                              ).trimmed().toLower();
            staticKeywordCache[kgcSumAs                 ]=QByteArrayLiteral("sum(%1) as %2 "                        ).trimmed().toLower();
            staticKeywordCache[kgcTable                 ]=QByteArrayLiteral("Table "                                ).trimmed().toLower();
            staticKeywordCache[kgcTableTemp             ]=QByteArrayLiteral("Temp Table "                           ).trimmed().toLower();
            staticKeywordCache[kgcTemp                  ]=QByteArrayLiteral("Temp "                                 ).trimmed().toLower();
            staticKeywordCache[kgcThen                  ]=QByteArrayLiteral("Then "                                 ).trimmed().toLower();
            staticKeywordCache[kgcTruncate              ]=QByteArrayLiteral("truncate "                             ).trimmed().toLower();
            staticKeywordCache[kgcTruncateTable         ]=QByteArrayLiteral("truncate table %1 "                    ).trimmed().toLower();
            staticKeywordCache[kgcTruncateTableCacade   ]=QByteArrayLiteral("truncate table %1 cascade "            ).trimmed().toLower();
            staticKeywordCache[kgcUnion                 ]=QByteArrayLiteral("union "                                ).trimmed().toLower();
            staticKeywordCache[kgcUnionAll              ]=QByteArrayLiteral("union all "                            ).trimmed().toLower();
            staticKeywordCache[kgcIntersect             ]=QByteArrayLiteral("intersect "                            ).trimmed().toLower();
            staticKeywordCache[kgcIntersectAll          ]=QByteArrayLiteral("intersect all "                        ).trimmed().toLower();
            staticKeywordCache[kgcExcept                ]=QByteArrayLiteral("Except "                               ).trimmed().toLower();
            staticKeywordCache[kgcExceptAll             ]=QByteArrayLiteral("Except all "                           ).trimmed().toLower();
            staticKeywordCache[kgcUnique                ]=QByteArrayLiteral("unique "                               ).trimmed().toLower();
            staticKeywordCache[kgcUpdate                ]=QByteArrayLiteral("update "                               ).trimmed().toLower();
            staticKeywordCache[kgcUpdateSet             ]=QByteArrayLiteral("update %1 set %2 %3 %4 %5 "            ).trimmed().toLower();
            staticKeywordCache[kgcUpdateSetValues       ]=QByteArrayLiteral(" "                                     ).trimmed().toLower();
            staticKeywordCache[kgcUpsertSet             ]=QByteArrayLiteral(" "                                     ).trimmed().toLower();
            staticKeywordCache[kgcUsing                 ]=QByteArrayLiteral("Using "                                ).trimmed().toLower();
            staticKeywordCache[kgcValues                ]=QByteArrayLiteral("values %1 "                            ).trimmed().toLower();
            staticKeywordCache[kgcView                  ]=QByteArrayLiteral("view "                                 ).trimmed().toLower();
            staticKeywordCache[kgcWhen                  ]=QByteArrayLiteral("when "                                 ).trimmed().toLower();
            staticKeywordCache[kgcWhere                 ]=QByteArrayLiteral("where "                                ).trimmed().toLower();
            staticKeywordCache[kgcSetApplicationName    ]=QByteArrayLiteral(" "                                     ).trimmed().toLower();
            staticKeywordCache[kgcSetSearchPath         ]=QByteArrayLiteral(" "                                     ).trimmed().toLower();
            staticKeywordCache[kgcNextVal               ]={};
            staticKeywordCache[kgcNextValSelect         ]={};
        }
        this->parent->init();
    }
};

SqlSuitableKeyWord::SqlSuitableKeyWord(QObject *parent):QObject{parent}
{
    Q_UNUSED(parent)
    this->p = new SqlSuitableKeyWordPvt{this};
}

SqlSuitableKeyWord::SqlSuitableKeyWord(QSqlDriver::DbmsType driver, QObject *parent):QObject{parent}
{
    Q_UNUSED(parent)
    this->p = new SqlSuitableKeyWordPvt(this, driver);
}

SqlSuitableKeyWord::SqlSuitableKeyWord(QList<QSqlDriver::DbmsType> drivers, QObject *parent):QObject{parent}
{
    Q_UNUSED(parent)
    this->p = new SqlSuitableKeyWordPvt(this, drivers);
}

SqlSuitableKeyWord::~SqlSuitableKeyWord()
{
}

SqlSuitableKeyWord &SqlSuitableKeyWord::operator =(const SqlSuitableKeyWord &o)
{

    auto keyWord=&o;
    if(keyWord==nullptr)
        return *this;

    p->drivers=o.p->drivers;
    p->staticKeywordCache=p->staticKeywordCache;
    return *this;
}

bool SqlSuitableKeyWord::isValid()
{

    return p->isValid();
}

QString SqlSuitableKeyWord::parserCommand(int command)
{
    auto commands=this->commands();
    auto cmdName=commands.value(command);
    return cmdName.trimmed();
}

QStringList SqlSuitableKeyWord::parserCommand(int command, const ModelInfo *modelInfo, const QVariant &value)
{
    const auto groupingTypes=QOrm::TypeUtil::keywordGroupingTypes();
    QStringList RETURN;
    if(command==kgcInsertInto){
        QVariantList list;
        switch (value.typeId()) {
        case QMetaType::QVariantList:
        case QMetaType::QStringList:
        {
            list=value.toList();
            break;
        }
        default:
            list.append(value);
            break;
        }

        QVariantList listRecords;
        for(auto &v:list){
            switch (value.typeId()) {
            case QMetaType::QVariantHash:
            case QMetaType::QVariantMap:
            {
                listRecords.append(v);
                break;
            }
            default:
                listRecords.append(QVariantHash{});//deve gerar erro
                break;
            }
        }

        if(!listRecords.isEmpty()){
            auto fields=modelInfo->propertyTableList();
            auto values=this->formatValues(fields, listRecords);
            if(!values.isEmpty()){
                auto v1=this->parserCommand(kgcInsertInto).arg(modelInfo->tableNameFull(),fields.join(QStringLiteral(",")));
                auto v2=this->parserCommand(kgcValues).arg(values.join(QStringLiteral(",")))+QStringLiteral(";");
                RETURN.append(v1+v2);
            }
        }
    }

    else if(command==kgcUpdate || command==kgcUpdateSet || command==kgcUpdateSetValues){
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
                for(auto &v:propertyTableList){
                    fields.append(QStringLiteral("%1=u.%1").arg(v));
                }
                tableFieldsSet=fields.join(QStringLiteral(","));
            }

            if(!modelInfo->tablePk().isEmpty()){
                QStringList fieldsWhere;
                for(auto &v:modelInfo->tablePk()){
                    fieldsWhere.append(QStringLiteral("u.%1 = s.%1").arg(v));
                }
                tableWhere=QStringLiteral("where %1").arg(fieldsWhere.join(QStringLiteral("and")));
            }

            tableFromValues=this->parserCommand(kgcFromValues).arg(values.join(QStringLiteral(",")),"s",propertyTableList.join(QStringLiteral(",")));
            auto v1=this->parserCommand(kgcUpdateSet).arg(tableNameFull+QStringLiteral(" as u"), tableFieldsSet, tableFromValues, tableWhere, "/*default*/");
            RETURN.append(v1);
        }
    }
    else if(command==kgcDelete || command==kgcDeleteFrom || command==kgcDeleteFromUsing){
        auto listRecords=value;
        switch (listRecords.typeId()) {
        case QMetaType::QVariantList:
        case QMetaType::QStringList:
        {
            auto l=listRecords.toStringList();
            listRecords=l.size()==1?QVariant(l.first()):l;
            break;
        }
        default:
            break;
        }

        auto values=this->formatValues(modelInfo->tablePk(), listRecords);
        auto tableNameFull=modelInfo->propertyTableList();

        if(!values.isEmpty()){
            auto tablePk=modelInfo->tablePk();
            auto tableName=modelInfo->tableNameFull();
            QString tableFromValues;
            QString tableWhere;

            if(!modelInfo->tablePk().isEmpty()){
                QStringList fieldsWhere;
                for(auto &v:modelInfo->tablePk()){
                    fieldsWhere.append(QStringLiteral("d.%1 = s.%1").arg(v));
                }
                tableWhere=QStringLiteral("where %1").arg(fieldsWhere.join(QStringLiteral("and")));
            }

            tableFromValues=QStringLiteral("(%1) as s").arg(this->parserCommand(kgcSelectValues).arg(QStringLiteral("*"), values.join(QStringLiteral(",")), QStringLiteral("s"), tablePk.join(QStringLiteral(","))));
            auto v1=this->parserCommand(kgcDeleteFromUsing).arg(tableName+QStringLiteral(" as d"), tableFromValues, tableWhere);
            RETURN.append(v1);
        }
    }
    else if(command==kgcTruncateTable || command==kgcTruncateTableCacade){
        auto tableName=modelInfo->tableNameFull();
        auto scommand=this->parserCommand(command).arg(tableName);
        RETURN.append(scommand);
    }
    else if(command==kgcNextValSelect){
        auto vMap=value.toHash();
        auto seqName=vMap.value(QByteArrayLiteral("name")).toString().trimmed();
        auto scommand=this->parserCommand(command).arg(seqName);
        RETURN.append(scommand);
    }
    else if(groupingTypes.contains(command)){
        auto vMap=value.toHash();
        auto value=this->formatValue(vMap.value(QByteArrayLiteral("value")));
        auto name=vMap.value(QByteArrayLiteral("name")).toString().trimmed();
        auto nameAs=vMap.value(QByteArrayLiteral("nameAs")).toString().trimmed();
        auto defaultValue=this->formatValue(vMap.value(QByteArrayLiteral("defaultValue")));
        value=value.isEmpty()?name:value;
        auto scommand=this->parserCommand(command);
        if(scommand.contains(QByteArrayLiteral("%3")))
            scommand=scommand.arg(value, defaultValue, nameAs);
        else if(scommand.contains(QByteArrayLiteral("%2")))
            scommand=scommand.arg(value, nameAs);
        else if(scommand.contains(QByteArrayLiteral("%1")))
            scommand=scommand.arg(value);
        else
            scommand={};

        if(!scommand.isEmpty())
            RETURN.append(scommand);
    }

    return RETURN;
}

KeywordCache &SqlSuitableKeyWord::commands()
{

    return p->staticKeywordCache;
}

bool SqlSuitableKeyWord::parse(const QVariant &v, QByteArray &script)
{
    Q_UNUSED(v)
    Q_UNUSED(script)
    return false;
}

SqlSuitableKeyWord &SqlSuitableKeyWord::parser(const QSqlDatabase &db)
{
    if(!db.isValid() || !db.isOpen())
        oWarning()<<QByteArrayLiteral("Invalid QSqlDatabase");
    return SqlSuitableKeyWord::parser(db.driver());
}

SqlSuitableKeyWord &SqlSuitableKeyWord::parser(const QSqlDriver *driver)
{
    return SqlSuitableKeyWord::parser(driver->dbmsType());
}

SqlSuitableKeyWord &SqlSuitableKeyWord::parser(const QSqlDriver::DbmsType &driver)
{
    auto &_driver=(*staticSqlSuitableKeyWord)[driver];
    return *_driver;
}

SqlSuitableKeyWord*SqlSuitableKeyWord::parserMaker(SqlSuitableKeyWord*parser)
{
    if(parser!=nullptr)
        parser->p->init();
    return parser;
}

QList<QSqlDriver::DbmsType>&SqlSuitableKeyWord::drivers()const
{

    return p->drivers;
}

void SqlSuitableKeyWord::setDriver(QSqlDriver::DbmsType value)
{

    if(p->drivers.contains(value))
        p->drivers.append(value);
}

void SqlSuitableKeyWord::setDrivers(QList<QSqlDriver::DbmsType> value)
{

    p->drivers=value;
    p->init();
}

QString SqlSuitableKeyWord::formatValue(const QVariant &v)
{
    if(!v.isValid())
        return QStringLiteral("null");

    auto typeId=v.typeId();

    if(typeId>=QMetaType::User)
        return QString::number(v.toLongLong());

    switch (typeId) {
    case QMetaType::Int:
    case QMetaType::UInt:
    case QMetaType::LongLong:
    case QMetaType::ULongLong:
        return QString::number(v.toLongLong());
    case QMetaType::Double:
        return QString::number(v.toDouble(),'f',6);
    case QMetaType::Bool:
        return v.toString();
    case QMetaType::QString:
    case QMetaType::QByteArray:
    case QMetaType::QBitArray:
    case QMetaType::QChar:
    {
        auto s=v.toString().trimmed();
        return QStringLiteral("'")+s.replace(QStringLiteral("'"),QStringLiteral("''"))+QStringLiteral("'");
    }
    case QMetaType::QDateTime:
    {
        auto d=v.toDateTime();
        d = (d.date()>=*__d1900)?d:*__dt1900;
        return QStringLiteral("'")+d.toString(*format_date_time)+QStringLiteral("'");
    }
    case QMetaType::QDate:
    {
        auto d=v.toDate();
        d = (d>=*__d1900)?d:*__d1900;
        return QStringLiteral("'")+d.toString(*format_date)+QStringLiteral("'");
    }
    case QMetaType::QTime:
        return QStringLiteral("'")+v.toTime().toString(*format_time)+QStringLiteral("'");
    case QMetaType::QUuid:
    {
        auto u = v.toUuid();
        return u.isNull()?QStringLiteral("null"):(QStringLiteral("'")+u.toByteArray()+QStringLiteral("'"));
    }
    case QMetaType::QUrl:
    {
        auto u = v.toUrl();
        return u.isEmpty()?QStringLiteral("null"):(QStringLiteral("'")+u.toString()+QStringLiteral("'"));
    }
    case QMetaType::QVariantList:
    case QMetaType::QStringList:
    {
        QStringList ls;
        for(auto &vv : v.toList()){
            if(vv.isNull() || !vv.isValid()){
                continue;
            }

            switch (vv.typeId()) {
            case QMetaType::QString:
            case QMetaType::QByteArray:
            case QMetaType::QBitArray:
            case QMetaType::QChar:
            {
                const auto ss=vv.toString().trimmed();
                ls.append(QStringLiteral("'") + ss + QStringLiteral("'"));
                continue;
            }
            case QMetaType::QUuid:{
                const auto ss=vv.toUuid().toString();
                ls.append(QStringLiteral("'") + ss + QStringLiteral("'"));
                continue;
            }

            case QMetaType::QUrl:{
                const auto ss=vv.toUrl().toString();
                ls.append(QStringLiteral("'") + ss + QStringLiteral("'"));
                continue;
            }
            default:
                ls.append(vv.toString().trimmed());
            }
        }
        return ls.isEmpty()?QStringLiteral("null"):ls.join(QStringLiteral(","));
    }
    default:
        return QStringLiteral("null");
    }
}

QString SqlSuitableKeyWord::formatParameter(const QVariant &v)
{
    if(!v.isValid())
        return QStringLiteral("null");

    auto typeId=v.typeId();
    switch (typeId) {
    case QMetaType::Int:
    case QMetaType::UInt:
    case QMetaType::LongLong:
    case QMetaType::ULongLong:
        return QString::number(v.toLongLong());
    case QMetaType::Double:
        return QString::number(v.toDouble(),'f',6);
    case QMetaType::Bool:
        return v.toString();
    case QMetaType::QString:
    case QMetaType::QByteArray:
    case QMetaType::QBitArray:
    case QMetaType::QChar:{
        auto s=v.toString().trimmed();
        return QStringLiteral("'")+s.replace(QStringLiteral("'"),QStringLiteral("\'"))+QStringLiteral("'");
    }
    case QMetaType::QDateTime:
    {
        auto d=v.toDateTime();
        d = (d.date()>=*__d1900)?d:*__dt1900;
        return QStringLiteral("'")+d.toString(*format_date_time)+QStringLiteral("'");
    }
    case QMetaType::QDate:
    {
        auto d=v.toDate();
        d = (d>=*__d1900)?d:*__d1900;
        return QStringLiteral("'")+d.toString(*format_date)+QStringLiteral("'");
    }
    case QMetaType::QTime:
        return QStringLiteral("'")+v.toTime().toString(*format_date)+QStringLiteral("'");
    case QMetaType::QUuid:
    {
        auto u = v.toUuid();
        return u.isNull()?QStringLiteral("null"):(QStringLiteral("'")+u.toByteArray()+QStringLiteral("'"));
    }
    case QMetaType::QUrl:
    {
        auto u = v.toUrl();
        return u.isEmpty()?QStringLiteral("null"):(QStringLiteral("'")+u.toString()+QStringLiteral("'"));
    }
    case QMetaType::QVariantList:
    case QMetaType::QStringList:
    {
        QStringList ls;
        for(auto &vv:v.toList()){
            if(vv.isValid())
                ls.append(this->formatParameter(vv));
        }
        return ls.isEmpty()?QStringLiteral("null"):ls.join(QStringLiteral(","));
    }
    default:
        return QStringLiteral("null");
    }
}

QStringList SqlSuitableKeyWord::formatValues(const QStringList &field, const QVariant &values)
{
    auto fieldFormat=field;

    QVariantList vList;
    switch (values.typeId()) {
    case QMetaType::QVariantList:
    case QMetaType::QStringList:
        vList=values.toList();
        break;
    default:
        vList.append(values);
    }

    if(fieldFormat.isEmpty()){
        if(!vList.isEmpty()){
            auto map = vList.first().toHash();
            QHashIterator<QString, QVariant> i(map);
            while (i.hasNext()) {
                i.next();
                fieldFormat.append(i.key());
            }
        }
    }

    QStringList RETURN;

    if(!fieldFormat.isEmpty()){
        for(auto &i : vList){
            auto map=i.toHash();
            QStringList row;
            for(auto &f:fieldFormat){
                auto v=map.value(f);
                auto s=this->formatValue(v);
                row.append(s);
            }
            RETURN.append(QStringLiteral("(%1)").arg(row.join(QStringLiteral(","))));
        }
    }

    return RETURN;
}

QStringList SqlSuitableKeyWord::formatValues(const QVariant &value)
{
    QVariantList values;

    switch (value.typeId()) {
    case QMetaType::QVariantList:
    case QMetaType::QStringList:
        values=value.toList();
        break;
    case QMetaType::QVariantMap:
    case QMetaType::QVariantHash:
        values.append(value.toHash().values());
        break;
    default:
        values.append(value);
    }

    QStringList RETURN;

    for(auto &v:values){
        RETURN.append(this->formatValue(v));
    }

    return RETURN;
}

QStringList SqlSuitableKeyWord::formatValuesSet(const QStringList &field, const QVariant &value)
{
    QStringList RETURN;
    QVariantList values;

    switch (value.typeId()) {
    case QMetaType::QVariantList:
    case QMetaType::QStringList:
        values=value.toList();
        break;
    case QMetaType::QVariantMap:
    case QMetaType::QVariantHash:
        values.append(value);
        break;
    default:
        values.append(value);
        break;
    }

    for(auto &v:values){
        auto vHash=v.toHash();
        for(auto &f:field){
            auto v=vHash.value(f);
            auto s=this->formatValue(v);
            RETURN.append(QStringLiteral("%1=%2").arg(f, s));
        }
    }

    return RETURN;
}

QStringList SqlSuitableKeyWord::parserCallProcedure(const QVariant &value)
{
    auto mapObject=value.toHash();
    auto database = mapObject.value(QStringLiteral("database")).toString().trimmed();
    auto schema   = mapObject.value(QStringLiteral("schema")).toString().trimmed();
    auto object   = mapObject.value(QStringLiteral("object"));
    auto s_values = this->formatValues(mapObject.value("values")).join(QStringLiteral(","));
    const auto &modelInfo=QOrm::ModelInfo::from(object);
    if(object.isValid()){
        object=modelInfo.tableNameFull();
    }

    if(this->drivers().contains(QSqlDriver::MSSqlServer)){
        QString s_database=database.isEmpty()?"":QStringLiteral("USE %1").arg(database);
        QString s_object=object.toString();
        s_object+=database.isEmpty()?"":(s_object+QStringLiteral("."));
        s_object+=schema.isEmpty()?"":(schema+QStringLiteral("."));
        s_object+=s_object.isEmpty()?"":s_object;
        return QStringList{QStringLiteral("%1; { call %2(%3) }").arg(s_database,s_object, s_values)};
    }

    if(this->drivers().contains(QSqlDriver::PostgreSQL)){
        QString s_object=object.toString();
        s_object+=schema.isEmpty()?"":(schema+QStringLiteral("."));
        s_object+=s_object.isEmpty()?"":s_object;
        return QStringList{QStringLiteral("select %1(%2)").arg(s_object, s_values)};
    }

    return {};
}

QStringList SqlSuitableKeyWord::parserCallFunction(const QVariant &value)
{
    auto mapObject=value.toHash();

    auto schema   = mapObject.value(QStringLiteral("schema")).toString().trimmed();
    auto object   = mapObject.value(QStringLiteral("object"));
    auto s_values = this->formatValues(mapObject.value("values")).join(QStringLiteral(","));
    const auto &modelInfo=QOrm::ModelInfo::from(object);
    if(object.isValid()){
        object=modelInfo.tableNameFull();
    }

    if(this->drivers().contains(QSqlDriver::PostgreSQL) || this->drivers().contains(QSqlDriver::MSSqlServer)){
        QString s_object=object.toString();
        s_object+=schema.isEmpty()?"":(schema+QStringLiteral("."));
        s_object+=s_object.isEmpty()?"":s_object;
        return QStringList{QStringLiteral("select %1(%2)").arg(s_object, s_values)};
    }

    return {};
}

QStringList SqlSuitableKeyWord::parserCallFunctionTable(const QVariant &value)
{
    auto mapObject=value.toHash();
    auto schema   = mapObject.value(QStringLiteral("schema")).toString().trimmed();
    auto object   = mapObject.value(QStringLiteral("object"));
    auto s_values = this->formatValues(mapObject.value("values")).join(QStringLiteral(","));
    const auto &modelInfo=QOrm::ModelInfo::from(object);
    if(object.isValid()){
        object=modelInfo.tableNameFull();
    }

    if(this->drivers().contains(QSqlDriver::PostgreSQL) || this->drivers().contains(QSqlDriver::MSSqlServer)){
        QString s_object=object.toString();
        s_object+=schema.isEmpty()?"":(schema+QStringLiteral("."));
        s_object+=s_object.isEmpty()?"":s_object;
        return QStringList{QStringLiteral("select * from %1(%2)").arg(s_object, s_values)};
    }

    return {};
}

void SqlSuitableKeyWord::init()
{

}

}

