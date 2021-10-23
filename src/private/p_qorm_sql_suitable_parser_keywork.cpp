#include "./p_qorm_sql_suitable_parser_keywork.h"
#include "./p_qorm_sql_suitable_parser_keywork_ansi.h"
#include "./p_qorm_sql_suitable_parser_keywork_psq.h"
#include "./p_qorm_sql_suitable_parser_keywork_tsql.h"
#include "./p_qorm_sql_suitable_parser_keywork_sqlite.h"
#include "./p_qorm_sql_suitable_parser_keywork_oracle.h"
#include "./p_qorm_sql_suitable_parser_keywork_mysql.h"
#include "./p_qorm_model_info.h"
#include <QMutex>
#include <QStringList>
#include <QCoreApplication>

namespace PrivateQOrm {

typedef QMap<QSqlDriver::DbmsType,QOrm::SqlSuitableKeyWord*> DriverSuiteMap;
typedef QList<QOrm::SqlSuitableKeyWord*> DriverSuiteList;


Q_GLOBAL_STATIC_WITH_ARGS(QString, format_date_time, ("yyyy-MM-dd hh:mm:ss.zzz"));
Q_GLOBAL_STATIC_WITH_ARGS(QString, format_date, ("yyyy-MM-dd"));
Q_GLOBAL_STATIC_WITH_ARGS(QString, format_time, ("hh:mm:ss.zzz"));
Q_GLOBAL_STATIC_WITH_ARGS(QDate, __d1900, (QDate(1900,01,01)));
Q_GLOBAL_STATIC_WITH_ARGS(QDateTime, __dt1900, (QDateTime(QDate(1900,01,01),QTime())));

Q_GLOBAL_STATIC(QMutex, staticSqlSuitableKeyWordLocker);
Q_GLOBAL_STATIC(DriverSuiteMap, staticSqlSuitableKeyWord);
Q_GLOBAL_STATIC(DriverSuiteList, staticSqlSuitableKeyWordList);

}

namespace QOrm {

static const auto&format_date_time=*PrivateQOrm::format_date_time;
static const auto&format_date=*PrivateQOrm::format_date;
static const auto&format_time=*PrivateQOrm::format_time;
static const auto&__d1900=*PrivateQOrm::__d1900;
static const auto&__dt1900=*PrivateQOrm::__dt1900;
static auto&staticSqlSuitableKeyWordLocker=*PrivateQOrm::staticSqlSuitableKeyWordLocker;
static auto&staticSqlSuitableKeyWord=*PrivateQOrm::staticSqlSuitableKeyWord;
static auto&staticSqlSuitableKeyWordList=*PrivateQOrm::staticSqlSuitableKeyWordList;

#define dPvt()\
    auto&p = *reinterpret_cast<SqlSuitableKeyWordPvt*>(this->p)

static void deinitKeyWork(){
    auto lst=staticSqlSuitableKeyWordList;
    staticSqlSuitableKeyWord.clear();
    staticSqlSuitableKeyWordList.clear();
    qDeleteAll(lst);
}

static bool initKeyWork(){
    QORM_DECLARE_KEY_WORD(SqlSuitableKeyWordAnsi);
    QORM_DECLARE_KEY_WORD(SqlSuitableKeyWordMySql);
    QORM_DECLARE_KEY_WORD(SqlSuitableKeyWordOracle);
    QORM_DECLARE_KEY_WORD(SqlSuitableKeyWordPSql);
    QORM_DECLARE_KEY_WORD(SqlSuitableKeyWordSQLite);
    QORM_DECLARE_KEY_WORD(SqlSuitableKeyWordTSQL);
    qAddPostRoutine(deinitKeyWork); //gera loop infinito e nao deixa a aplicacao terminar
    return true;
}

//Q_COREAPP_STARTUP_FUNCTION(initKeyWork)//NAO DEVE USAR POIS QUANDO TUDO LINKADO A APLICACAO PODE LEVANTAR ANTES DO METODO TER SIDO CHAMADO
static const auto __initKeyWork=initKeyWork();


class SqlSuitableKeyWordPvt{
public:
    QList<QSqlDriver::DbmsType> drivers;
    KeywordCache staticKeywordCache;
    SqlSuitableKeyWord*parent=nullptr;

    explicit SqlSuitableKeyWordPvt(SqlSuitableKeyWord*parent){
        this->parent=parent;
    }

    explicit SqlSuitableKeyWordPvt(SqlSuitableKeyWord*parent, QSqlDriver::DbmsType driver){
        this->parent=parent;
        if(!this->drivers.contains(driver))
            this->drivers<<driver;
    }
    explicit SqlSuitableKeyWordPvt(SqlSuitableKeyWord*parent, QList<QSqlDriver::DbmsType> drivers){
        this->parent=parent;
        this->drivers=drivers;
    }
    virtual ~SqlSuitableKeyWordPvt(){

    }

    auto isValid(){
        return !this->drivers.isEmpty();
    }


    void init(){
        QMutexLocker locker(&staticSqlSuitableKeyWordLocker);
        auto vList=this->parent->drivers();
        for(auto&driver : vList){
            if(staticSqlSuitableKeyWordList.contains(this->parent))
                staticSqlSuitableKeyWordList<<this->parent;

            if(!staticSqlSuitableKeyWord.contains(driver))
                staticSqlSuitableKeyWord.insert(driver, this->parent);
        }

        {
            staticKeywordCache.insert(kgcAll                   , qbl("All "                                  ).trimmed().toLower());
            staticKeywordCache.insert(kgcAlter                 , qbl("Alter "                                ).trimmed().toLower());
            staticKeywordCache.insert(kgcAnd                   , qbl("And "                                  ).trimmed().toLower());
            staticKeywordCache.insert(kgcAny                   , qbl("%1 Any(%2) "                           ).trimmed().toLower());
            staticKeywordCache.insert(kgcAs                    , qbl("As "                                   ).trimmed().toLower());
            staticKeywordCache.insert(kgcAsc                   , qbl("Asc "                                  ).trimmed().toLower());
            staticKeywordCache.insert(kgcAvg                   , qbl("Avg(%1) "                              ).trimmed().toLower());
            staticKeywordCache.insert(kgcAvgAs                 , qbl("Avg(%1) as %2 "                        ).trimmed().toLower());
            staticKeywordCache.insert(kgcBetween               , qbl("%1 Between %2 and %3 "                 ).trimmed().toLower());
            staticKeywordCache.insert(kgcNotBetween            , qbl("not %1 Between %2 and %3 "             ).trimmed().toLower());
            staticKeywordCache.insert(kgcBigger                , qbl("%1 > %2 "                              ).trimmed().toLower());
            staticKeywordCache.insert(kgcCascade               , qbl("cascade "                              ).trimmed().toLower());
            staticKeywordCache.insert(kgcCase                  , qbl("Case "                                 ).trimmed().toLower());
            staticKeywordCache.insert(kgcCheck                 , qbl("Check "                                ).trimmed().toLower());
            staticKeywordCache.insert(kgcClustered             , qbl("Clustered "                            ).trimmed().toLower());
            staticKeywordCache.insert(kgcConstraint            , qbl("Constraint "                           ).trimmed().toLower());
            staticKeywordCache.insert(kgcCount                 , qbl("Count(%1) "                            ).trimmed().toLower());
            staticKeywordCache.insert(kgcCountAs               , qbl("Count(%1) as %2 "                      ).trimmed().toLower());
            staticKeywordCache.insert(kgcCreate                , qbl("Create "                               ).trimmed().toLower());
            staticKeywordCache.insert(kgcCreateOrReplace       , qbl("create or Replace "                    ).trimmed().toLower());
            staticKeywordCache.insert(kgcDataBase              , qbl("DataBase "                             ).trimmed().toLower());
            staticKeywordCache.insert(kgcDeclare               , qbl("Declare "                              ).trimmed().toLower());
            staticKeywordCache.insert(kgcDefault               , qbl("Default "                              ).trimmed().toLower());
            staticKeywordCache.insert(kgcDelete                , qbl("delete "                               ).trimmed().toLower());
            staticKeywordCache.insert(kgcDeleteFrom            , qbl("delete from %1 "                       ).trimmed().toLower());
            staticKeywordCache.insert(kgcDeleteFromUsing       , qbl("delete from %1 using %2 %3 "           ).trimmed().toLower());
            staticKeywordCache.insert(kgcDesc                  , qbl("Desc "                                 ).trimmed().toLower());
            staticKeywordCache.insert(kgcDistinct              , qbl("Distinct "                             ).trimmed().toLower());
            staticKeywordCache.insert(kgcDrop                  , qbl("Drop "                                 ).trimmed().toLower());
            staticKeywordCache.insert(kgcEnd                   , qbl("End "                                  ).trimmed().toLower());
            staticKeywordCache.insert(kgcEqual                 , qbl("%1 = %2 "                              ).trimmed().toLower());
            staticKeywordCache.insert(kgcNotEqual              , qbl("%1 <> %2 "                             ).trimmed().toLower());
            staticKeywordCache.insert(kgcEqualBigger           , qbl("%1 >= %2 "                             ).trimmed().toLower());
            staticKeywordCache.insert(kgcEqualMinor            , qbl("%1 <= %2 "                             ).trimmed().toLower());
            staticKeywordCache.insert(kgcExists                , qbl("exists "                               ).trimmed().toLower());
            staticKeywordCache.insert(kgcForeignKey            , qbl("Foreign Key "                          ).trimmed().toLower());
            staticKeywordCache.insert(kgcFrom                  , qbl("from "                                 ).trimmed().toLower());
            staticKeywordCache.insert(kgcFromValues            , qbl("from(values %1 )as %2 (%3) "           ).trimmed().toLower());
            staticKeywordCache.insert(kgcFull                  , qbl("Full "                                 ).trimmed().toLower());
            staticKeywordCache.insert(kgcFunction              , qbl("Function "                             ).trimmed().toLower());
            staticKeywordCache.insert(kgcFunction              , qbl("Function "                             ).trimmed().toLower());
            staticKeywordCache.insert(kgcGroupBy               , qbl("Group by "                             ).trimmed().toLower());
            staticKeywordCache.insert(kgcHaving                , qbl("Having "                               ).trimmed().toLower());
            staticKeywordCache.insert(kgcIfExists              , qbl("if exists "                            ).trimmed().toLower());
            staticKeywordCache.insert(kgcIn                    , qbl("%1 In (%2) "                           ).trimmed().toLower());
            staticKeywordCache.insert(kgcIndex                 , qbl("Index "                                ).trimmed().toLower());
            staticKeywordCache.insert(kgcInOut                 , qbl("not %1 in (%2) "                       ).trimmed().toLower());
            staticKeywordCache.insert(kgcInsert                , qbl("Insert "                               ).trimmed().toLower());
            staticKeywordCache.insert(kgcInsertInto            , qbl("Insert into %1(%2) "                   ).trimmed().toLower());
            staticKeywordCache.insert(kgcIsNull                , qbl(""                                      ).trimmed().toLower());
            staticKeywordCache.insert(kgcIsNullCheckValue      , qbl(""                                      ).trimmed().toLower());
            staticKeywordCache.insert(kgcValueIsNull           , qbl("%1 is null "                           ).trimmed().toLower());
            staticKeywordCache.insert(kgcValueIsNotNull        , qbl("not %1 is null "                       ).trimmed().toLower());
            staticKeywordCache.insert(kgcJoin                  , qbl("Join "                                 ).trimmed().toLower());
            staticKeywordCache.insert(kgcJoinFull              , qbl("Full Join "                            ).trimmed().toLower());
            staticKeywordCache.insert(kgcJoinInner             , qbl("Inner Join "                           ).trimmed().toLower());
            staticKeywordCache.insert(kgcJoinLeft              , qbl("Left Join "                            ).trimmed().toLower());
            staticKeywordCache.insert(kgcJoinRigth             , qbl("Rigth Join "                           ).trimmed().toLower());
            staticKeywordCache.insert(kgcKey                   , qbl("Key "                                  ).trimmed().toLower());
            staticKeywordCache.insert(kgcLeft                  , qbl("Left "                                 ).trimmed().toLower());
            staticKeywordCache.insert(kgcILike                 , qbl("%1 like %2 "                           ).trimmed().toLower());
            staticKeywordCache.insert(kgcLike                  , qbl("%1 like %2 "                           ).trimmed().toLower());
            staticKeywordCache.insert(kgcSoundexEqual          , qbl("soundex(%1) = soundex(%2)"             ).trimmed().toLower());
            staticKeywordCache.insert(kgcLimit                 , qbl("Limit %1 "                             ).trimmed().toLower());
            staticKeywordCache.insert(kgcOffset                , qbl("offset %1 "                            ).trimmed().toLower());
            staticKeywordCache.insert(kgcMax                   , qbl("Max(%1) "                              ).trimmed().toLower());
            staticKeywordCache.insert(kgcMaxAs                 , qbl("Max(%1) as %2 "                        ).trimmed().toLower());
            staticKeywordCache.insert(kgcMin                   , qbl("Min(%1) "                              ).trimmed().toLower());
            staticKeywordCache.insert(kgcMinAs                 , qbl("Min(%1) as %2 "                        ).trimmed().toLower());
            staticKeywordCache.insert(kgcMinor                 , qbl("%1 < %2 "                              ).trimmed().toLower());
            staticKeywordCache.insert(kgcNot                   , qbl("Not "                                  ).trimmed().toLower());
            staticKeywordCache.insert(kgcNotExists             , qbl("not Exists "                           ).trimmed().toLower());
            staticKeywordCache.insert(kgcNotNull               , qbl("Not Null "                             ).trimmed().toLower());
            staticKeywordCache.insert(kgcNull                  , qbl("Null "                                 ).trimmed().toLower());
            staticKeywordCache.insert(kgcOn                    , qbl("On "                                   ).trimmed().toLower());
            staticKeywordCache.insert(kgcOr                    , qbl("or "                                   ).trimmed().toLower());
            staticKeywordCache.insert(kgcOrderBy               , qbl("Order by "                             ).trimmed().toLower());
            staticKeywordCache.insert(kgcJoinOuter             , qbl("Outer Join "                           ).trimmed().toLower());
            staticKeywordCache.insert(kgcPivot                 , qbl("Pivot "                                ).trimmed().toLower());
            staticKeywordCache.insert(kgcPrimary               , qbl("Primary "                              ).trimmed().toLower());
            staticKeywordCache.insert(kgcPrimaryKey            , qbl("Primary Key "                          ).trimmed().toLower());
            staticKeywordCache.insert(kgcProcedure             , qbl("Procedure "                            ).trimmed().toLower());
            staticKeywordCache.insert(kgcReplace               , qbl("Replace "                              ).trimmed().toLower());
            staticKeywordCache.insert(kgcRigth                 , qbl("Rigth "                                ).trimmed().toLower());
            staticKeywordCache.insert(kgcRowNumber             , qbl("RowNumber "                            ).trimmed().toLower());
            staticKeywordCache.insert(kgcSchema                , qbl("Schema "                               ).trimmed().toLower());
            staticKeywordCache.insert(kgcSelect                , qbl("Select "                               ).trimmed().toLower());
            staticKeywordCache.insert(kgcSelectTop             , qbl("Select Top %1 "                        ).trimmed().toLower());
            staticKeywordCache.insert(kgcSelectDistinct        , qbl("Select distinct "                      ).trimmed().toLower());
            staticKeywordCache.insert(kgcSelectDistinctTop     , qbl("Select distinct top %1 "               ).trimmed().toLower());
            staticKeywordCache.insert(kgcSelectForExists       , qbl("Select null "                          ).trimmed().toLower());
            staticKeywordCache.insert(kgcSelectValues          , qbl("select %1 from(values(%2))as %3(%4) "  ).trimmed().toLower());
            staticKeywordCache.insert(kgcSet                   , qbl("Set "                                  ).trimmed().toLower());
            staticKeywordCache.insert(kgcSum                   , qbl("sum(%1) "                              ).trimmed().toLower());
            staticKeywordCache.insert(kgcSumAs                 , qbl("sum(%1) as %2 "                        ).trimmed().toLower());
            staticKeywordCache.insert(kgcTable                 , qbl("Table "                                ).trimmed().toLower());
            staticKeywordCache.insert(kgcTableTemp             , qbl("Temp Table "                           ).trimmed().toLower());
            staticKeywordCache.insert(kgcTemp                  , qbl("Temp "                                 ).trimmed().toLower());
            staticKeywordCache.insert(kgcThen                  , qbl("Then "                                 ).trimmed().toLower());
            staticKeywordCache.insert(kgcTruncate              , qbl("truncate "                             ).trimmed().toLower());
            staticKeywordCache.insert(kgcTruncateTable         , qbl("truncate table %1 "                    ).trimmed().toLower());
            staticKeywordCache.insert(kgcTruncateTableCacade   , qbl("truncate table %1 cascade "            ).trimmed().toLower());
            staticKeywordCache.insert(kgcUnion                 , qbl("union "                                ).trimmed().toLower());
            staticKeywordCache.insert(kgcUnionAll              , qbl("union all "                            ).trimmed().toLower());
            staticKeywordCache.insert(kgcIntersect             , qbl("intersect "                            ).trimmed().toLower());
            staticKeywordCache.insert(kgcIntersectAll          , qbl("intersect all "                        ).trimmed().toLower());
            staticKeywordCache.insert(kgcExcept                , qbl("Except "                               ).trimmed().toLower());
            staticKeywordCache.insert(kgcExceptAll             , qbl("Except all "                           ).trimmed().toLower());
            staticKeywordCache.insert(kgcUnique                , qbl("unique "                               ).trimmed().toLower());
            staticKeywordCache.insert(kgcUpdate                , qbl("update "                               ).trimmed().toLower());
            staticKeywordCache.insert(kgcUpdateSet             , qbl("update %1 set %2 %3 %4 %5 "            ).trimmed().toLower());
            staticKeywordCache.insert(kgcUpdateSetValues       , qbl(" "                                     ).trimmed().toLower());
            staticKeywordCache.insert(kgcUpsertSet             , qbl(" "                                     ).trimmed().toLower());
            staticKeywordCache.insert(kgcUsing                 , qbl("Using "                                ).trimmed().toLower());
            staticKeywordCache.insert(kgcValues                , qbl("values %1 "                            ).trimmed().toLower());
            staticKeywordCache.insert(kgcView                  , qbl("view "                                 ).trimmed().toLower());
            staticKeywordCache.insert(kgcWhen                  , qbl("when "                                 ).trimmed().toLower());
            staticKeywordCache.insert(kgcWhere                 , qbl("where "                                ).trimmed().toLower());
            staticKeywordCache.insert(kgcSetApplicationName    , qbl(" "                                     ).trimmed().toLower());
            staticKeywordCache.insert(kgcSetSearchPath         , qbl(" "                                     ).trimmed().toLower());
            staticKeywordCache.insert(kgcNextVal               , QByteArray());
            staticKeywordCache.insert(kgcNextValSelect         , QByteArray());
        }
        this->parent->init();
    }
};

SqlSuitableKeyWord::SqlSuitableKeyWord(QObject *parent)//:QObject(parent)
{
    Q_UNUSED(parent)
    this->p = new SqlSuitableKeyWordPvt(this);
}

SqlSuitableKeyWord::SqlSuitableKeyWord(QSqlDriver::DbmsType driver, QObject *parent)//:QObject(parent)
{
    Q_UNUSED(parent)
    this->p = new SqlSuitableKeyWordPvt(this, driver);
}

SqlSuitableKeyWord::SqlSuitableKeyWord(QList<QSqlDriver::DbmsType> drivers, QObject *parent)//:QObject(parent)
{
    Q_UNUSED(parent)
    this->p = new SqlSuitableKeyWordPvt(this, drivers);
}

SqlSuitableKeyWord::~SqlSuitableKeyWord()
{
    dPvt();delete&p;
}

SqlSuitableKeyWord &SqlSuitableKeyWord::operator =(const SqlSuitableKeyWord &o)
{
    dPvt();
    auto&pp=*static_cast<SqlSuitableKeyWordPvt*>(o.p);
    p.drivers=pp.drivers;
    p.staticKeywordCache=p.staticKeywordCache;
    return*this;
}

bool SqlSuitableKeyWord::isValid()
{
    dPvt();
    return p.isValid();
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
        if(value.type()==QVariant::List)
            list=value.toList();
        else
            list<<value;

        QVariantList listRecords;
        for(auto&v:list){
            if(v.type()==QVariant::Map || v.type()==QVariant::Hash)
                listRecords<<v;
            else
                listRecords<<QVariantHash();//deve gerar erro
        }

        if(!listRecords.isEmpty()){
            auto fields=modelInfo->propertyTableList();
            auto values=this->formatValues(fields, listRecords);
            if(!values.isEmpty()){
                auto v1=this->parserCommand(kgcInsertInto).arg(modelInfo->tableNameFull(),fields.join(qsl(",")));
                auto v2=this->parserCommand(kgcValues).arg(values.join(qsl(",")))+qsl(";");
                RETURN<<v1+v2;
            }
        }
    }

    else if(command==kgcUpdate || command==kgcUpdateSet || command==kgcUpdateSetValues){
        auto listRecords=value;
        if(listRecords.type()==QVariant::List){
            auto l=listRecords.toList();
            listRecords=l.size()==1?l.first():l;
        }
        else if(listRecords.type()==QVariant::StringList){
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
                for(auto&v:propertyTableList){
                    fields << qsl("%1=u.%1").arg(v);
                }
                tableFieldsSet=fields.join(qsl(","));
            }

            if(!modelInfo->tablePk().isEmpty()){
                QStringList fieldsWhere;
                for(auto&v:modelInfo->tablePk()){
                    fieldsWhere<<qsl("u.%1 = s.%1").arg(v);
                }
                tableWhere=qsl("where %1").arg(fieldsWhere.join(qsl("and")));
            }

            tableFromValues=this->parserCommand(kgcFromValues).arg(values.join(qsl(",")),"s",propertyTableList.join(qsl(",")));
            auto v1=this->parserCommand(kgcUpdateSet).arg(tableNameFull+qsl(" as u"), tableFieldsSet, tableFromValues, tableWhere, "/*default*/");
            RETURN<<v1;
        }
    }
    else if(command==kgcDelete || command==kgcDeleteFrom || command==kgcDeleteFromUsing){
        auto listRecords=value;
        if(listRecords.type()==QVariant::List){
            auto l=listRecords.toList();
            listRecords=l.size()==1?l.first():l;
        }
        else if(listRecords.type()==QVariant::StringList){
            auto l=listRecords.toStringList();
            listRecords=l.size()==1?QVariant(l.first()):l;
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
                for(auto&v:modelInfo->tablePk()){
                    fieldsWhere<<qsl("d.%1 = s.%1").arg(v);
                }
                tableWhere=qsl("where %1").arg(fieldsWhere.join(qsl("and")));
            }

            tableFromValues=qsl("(%1) as s").arg(this->parserCommand(kgcSelectValues).arg(qsl("*"), values.join(qsl(",")), qsl("s"), tablePk.join(qsl(","))));
            auto v1=this->parserCommand(kgcDeleteFromUsing).arg(tableName+qsl(" as d"), tableFromValues, tableWhere);
            RETURN<<v1;
        }
    }
    else if(command==kgcTruncateTable || command==kgcTruncateTableCacade){
        auto tableName=modelInfo->tableNameFull();
        auto scommand=this->parserCommand(command).arg(tableName);
        RETURN<<scommand;
    }
    else if(command==kgcNextValSelect){
        auto vMap=value.toHash();
        auto seqName=vMap.value(qbl("name")).toString().trimmed();
        auto scommand=this->parserCommand(command).arg(seqName);
        RETURN<<scommand;
    }
    else if(groupingTypes.contains(command)){
        auto vMap=value.toHash();
        auto value=this->formatValue(vMap.value(qbl("value")));
        auto name=vMap.value(qbl("name")).toString().trimmed();
        auto nameAs=vMap.value(qbl("nameAs")).toString().trimmed();
        auto defaultValue=this->formatValue(vMap.value(qbl("defaultValue")));
        value=value.isEmpty()?name:value;
        auto scommand=this->parserCommand(command);
        if(scommand.contains(qbl("%3")))
            scommand=scommand.arg(value, defaultValue, nameAs);
        else if(scommand.contains(qbl("%2")))
            scommand=scommand.arg(value, nameAs);
        else if(scommand.contains(qbl("%1")))
            scommand=scommand.arg(value);
        else
            scommand=qbl_null;

        if(!scommand.isEmpty())
            RETURN<<scommand;
    }

    return RETURN;
}

KeywordCache &SqlSuitableKeyWord::commands()
{
    dPvt();
    return p.staticKeywordCache;
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
        sWarning()<<qbl("Invalid QSqlDatabase");
    return SqlSuitableKeyWord::parser(db.driver());
}

SqlSuitableKeyWord &SqlSuitableKeyWord::parser(const QSqlDriver *driver)
{
    return SqlSuitableKeyWord::parser(driver->dbmsType());
}

SqlSuitableKeyWord &SqlSuitableKeyWord::parser(const QSqlDriver::DbmsType &driver)
{
    auto&_driver=staticSqlSuitableKeyWord[driver];
    return *_driver;
}

SqlSuitableKeyWord*SqlSuitableKeyWord::parserMaker(SqlSuitableKeyWord*parser)
{
    if(parser!=nullptr){
        auto&pp = *reinterpret_cast<SqlSuitableKeyWordPvt*>(parser->p);
        pp.init();
    }
    return parser;
}

QList<QSqlDriver::DbmsType>&SqlSuitableKeyWord::drivers()const
{
    dPvt();
    return p.drivers;
}

void SqlSuitableKeyWord::setDriver(QSqlDriver::DbmsType value)
{
    dPvt();
    if(p.drivers.contains(value))
        p.drivers<<value;
}

void SqlSuitableKeyWord::setDrivers(QList<QSqlDriver::DbmsType> value)
{
    dPvt();
    p.drivers=value;
    p.init();
}

QString SqlSuitableKeyWord::formatValue(const QVariant &v)
{
    QString __return=qsl("null");
    if(v.isValid()){
        auto __type=v.type();
        if(__type==QVariant::Int || __type==QVariant::UInt)
            __return=QString::number(v.toInt());
        else if(__type==QVariant::LongLong || __type==QVariant::ULongLong)
            __return=QString::number(v.toLongLong());
        else if(__type==QVariant::Double)
            __return=QString::number(v.toDouble(),'f',6);
        else if(__type==QVariant::Bool)
            __return=v.toString();
        else if(__type==QVariant::String || __type==QVariant::ByteArray){
            auto s=v.toString().trimmed();
            __return=qsl("'")+s.replace(qsl("'"),qsl("''"))+qsl("'");
        }
        else if(__type==QVariant::BitArray || __type==QVariant::Char)
            __return=v.toString();
        else if(__type==QVariant::DateTime){
            auto d=v.toDateTime();
            d = (d.date()>=__d1900)?d:__dt1900;
            __return=qsl("'")+d.toString(format_date_time)+qsl("'");
        }
        else if(__type==QVariant::Date){
            auto d=v.toDate();
            d = (d>=__d1900)?d:__d1900;
            __return=qsl("'")+d.toString(format_date)+qsl("'");
        }
        else if(__type==QVariant::Time)
            return qsl("'")+v.toTime().toString(format_date)+qsl("'");
        else if(__type==QVariant::Uuid){
            auto u = v.toUuid();
            __return=u.isNull()?qsl("null"):(qsl("'")+u.toByteArray()+qsl("'"));
        }
        else if(__type==QVariant::Url){
            auto u = v.toUrl();
            __return=u.isEmpty()?qsl("null"):(qsl("'")+u.toString()+qsl("'"));
        }
        else if(__type==QVariant::List || __type==QVariant::StringList){
            QStringList ls;
            for(auto&vv : v.toList()){
                if(vv.isNull() || !vv.isValid()){
                    continue;
                }
                else if(vv.type()==vv.String || vv.type()==v.ByteArray || vv.type()==v.Char || vv.type()==v.BitArray){
                    const auto ss=vv.toString().trimmed();
                    ls.append(qsl("'") + ss + qsl("'"));
                }
                else if(vv.type()==vv.Uuid){
                    const auto ss=vv.toUuid().toString();
                    ls.append(qsl("'") + ss + qsl("'"));
                }
                else if(vv.type()==vv.Url){
                    const auto ss=vv.toUrl().toString();
                    ls.append(qsl("'") + ss + qsl("'"));
                    continue;
                }
                else{
                    ls.append(vv.toString().trimmed());
                }
            }
            __return=ls.isEmpty()?qsl("null"):ls.join(qsl(","));
        }
    }

    return __return;
}

QString SqlSuitableKeyWord::formatParameter(const QVariant &v)
{
    QString __return=qsl("null");
    if(v.isValid()){
        auto __type=v.type();
        if(__type==QVariant::Int || __type==QVariant::UInt)
            __return=QString::number(v.toInt());
        else if(__type==QVariant::LongLong || __type==QVariant::ULongLong)
            __return=QString::number(v.toLongLong());
        else if(__type==QVariant::Double)
            __return=QString::number(v.toDouble(),'f',6);
        else if(__type==QVariant::Bool)
            __return=v.toString();
        else if(__type==QVariant::String || __type==QVariant::ByteArray){
            auto s=v.toString().trimmed();
            __return=qsl("'")+s.replace(qsl("'"),"\'")+qsl("'");
        }
        else if(__type==QVariant::BitArray || __type==QVariant::Char){
            auto s=v.toString().trimmed();
            __return=qsl("'")+s.replace(qsl("'"),"\'")+qsl("'");
        }
        else if(__type==QVariant::DateTime){
            auto d=v.toDateTime();
            d = (d.date()>=__d1900)?d:__dt1900;
            __return=qsl("'")+d.toString(format_date_time)+qsl("'");
        }
        else if(__type==QVariant::Date){
            auto d=v.toDate();
            d = (d>=__d1900)?d:__d1900;
            __return=qsl("'")+d.toString(format_date)+qsl("'");
        }
        else if(__type==QVariant::Time)
            return qsl("'")+v.toTime().toString(format_date)+qsl("'");
        else if(__type==QVariant::Uuid){
            auto u = v.toUuid();
            __return=u.isNull()?qsl("null"):(qsl("'")+u.toByteArray()+qsl("'"));
        }
        else if(__type==QVariant::Url){
            auto u = v.toUrl();
            __return=u.isEmpty()?qsl("null"):(qsl("'")+u.toString()+qsl("'"));
        }
        else if(__type==QVariant::List || __type==QVariant::StringList){
            QStringList ls;
            if(__type==QVariant::StringList){
                for(auto&v:v.toStringList()){
                    ls<<this->formatParameter(v);
                }
            }
            else{
                for(auto&vv:v.toList()){
                    if(vv.isValid())
                        ls<<this->formatParameter(vv);
                }
            }
            __return=ls.isEmpty()?qsl("null"):ls.join(qsl(","));
        }
    }

    return __return;
}

QStringList SqlSuitableKeyWord::formatValues(const QStringList &field, const QVariant &values)
{
    auto fieldFormat=field;

    QVariantList vList;
    if(values.type()==QVariant::List)
        vList=values.toList();
    else
        vList<<values;

    if(fieldFormat.isEmpty()){
        if(!vList.isEmpty()){
            auto map = vList.first().toHash();
            QHashIterator<QString, QVariant> i(map);
            while (i.hasNext()) {
                i.next();
                fieldFormat<<i.key();
            }
        }
    }

    QStringList RETURN;

    if(!fieldFormat.isEmpty()){
        for(auto&i : vList){
            auto map=i.toHash();
            QStringList row;
            for(auto&f:fieldFormat){
                auto v=map.value(f);
                auto s=this->formatValue(v);
                row<<s;
            }
            RETURN<<qsl("(%1)").arg(row.join(qsl(",")));
        }
    }

    return RETURN;
}

QStringList SqlSuitableKeyWord::formatValues(const QVariant &value)
{
    QVariantList values;

    if(value.type()==QVariant::List || value.type()==QVariant::StringList)
        values=value.toList();
    else if(value.type()==QVariant::Map || value.type()==QVariant::Hash)
        values<<value.toHash().values();
    else
        values<<value;

    QStringList RETURN;

    for(auto&v:values){
        RETURN<<this->formatValue(v);
    }

    return RETURN;
}

QStringList SqlSuitableKeyWord::formatValuesSet(const QStringList &field, const QVariant &value)
{
    QStringList RETURN;
    QVariantList values;

    if(value.type()==QVariant::List)
        values=value.toList();
    else if(value.type()==QVariant::Map || value.type()==QVariant::Hash)
        values<<value;

    for(auto&v:values){
        auto map=v.toHash();
        for(auto&f:field){
            auto v=map.value(f);
            auto s=this->formatValue(v);
            RETURN<<qsl("%1=%2").arg(f, s);
        }
    }

    return RETURN;
}

QStringList SqlSuitableKeyWord::parserCallProcedure(const QVariant &value)
{
    auto mapObject=value.toHash();
    auto database = mapObject.value(qsl("database")).toString().trimmed();
    auto schema   = mapObject.value(qsl("schema")).toString().trimmed();
    auto object   = mapObject.value(qsl("object"));
    auto s_values = this->formatValues(mapObject.value("values")).join(qsl(","));
    const auto&modelInfo=QOrm::ModelInfo::modelInfo(object);
    if(object.isValid()){
        object=modelInfo.tableNameFull();
    }

    if(this->drivers().contains(QSqlDriver::MSSqlServer)){
        QString s_database=database.isEmpty()?qsl_null:qsl("USE %1").arg(database);
        QString s_object=object.toString();
        s_object+=database.isEmpty()?qsl_null:(s_object+qsl("."));
        s_object+=schema.isEmpty()?qsl_null:(schema+qsl("."));
        s_object+=s_object.isEmpty()?qsl_null:s_object;
        return QStringList()<<qsl("%1; { call %2(%3) }").arg(s_database,s_object, s_values);
    }
    else if(this->drivers().contains(QSqlDriver::PostgreSQL)){
        QString s_object=object.toString();
        s_object+=schema.isEmpty()?qsl_null:(schema+qsl("."));
        s_object+=s_object.isEmpty()?qsl_null:s_object;
        return QStringList()<<qsl("select %1(%2)").arg(s_object, s_values);
    }

    return QStringList();
}

QStringList SqlSuitableKeyWord::parserCallFunction(const QVariant &value)
{
    auto mapObject=value.toHash();

    auto schema   = mapObject.value(qsl("schema")).toString().trimmed();
    auto object   = mapObject.value(qsl("object"));
    auto s_values = this->formatValues(mapObject.value("values")).join(qsl(","));
    const auto&modelInfo=QOrm::ModelInfo::modelInfo(object);
    if(object.isValid()){
        object=modelInfo.tableNameFull();
    }

    if(this->drivers().contains(QSqlDriver::PostgreSQL) || this->drivers().contains(QSqlDriver::MSSqlServer)){
        QString s_object=object.toString();
        s_object+=schema.isEmpty()?qsl_null:(schema+qsl("."));
        s_object+=s_object.isEmpty()?qsl_null:s_object;
        return QStringList()<<qsl("select %1(%2)").arg(s_object, s_values);
    }

    return QStringList();
}

QStringList SqlSuitableKeyWord::parserCallFunctionTable(const QVariant &value)
{
    auto mapObject=value.toHash();
    auto schema   = mapObject.value(qsl("schema")).toString().trimmed();
    auto object   = mapObject.value(qsl("object"));
    auto s_values = this->formatValues(mapObject.value("values")).join(qsl(","));
    const auto&modelInfo=QOrm::ModelInfo::modelInfo(object);
    if(object.isValid()){
        object=modelInfo.tableNameFull();
    }

    if(this->drivers().contains(QSqlDriver::PostgreSQL) || this->drivers().contains(QSqlDriver::MSSqlServer)){
        QString s_object=object.toString();
        s_object+=schema.isEmpty()?qsl_null:(schema+qsl("."));
        s_object+=s_object.isEmpty()?qsl_null:s_object;
        return QStringList()<<qsl("select * from %1(%2)").arg(s_object, s_values);
    }

    return QStringList();
}

void SqlSuitableKeyWord::init()
{

}

}

