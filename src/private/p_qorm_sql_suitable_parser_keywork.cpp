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
            staticKeywordCache[kgcAll                   ]=qbl("All "                                  ).trimmed().toLower();
            staticKeywordCache[kgcAlter                 ]=qbl("Alter "                                ).trimmed().toLower();
            staticKeywordCache[kgcAnd                   ]=qbl("And "                                  ).trimmed().toLower();
            staticKeywordCache[kgcAny                   ]=qbl("%1 Any(%2) "                           ).trimmed().toLower();
            staticKeywordCache[kgcAs                    ]=qbl("As "                                   ).trimmed().toLower();
            staticKeywordCache[kgcAsc                   ]=qbl("Asc "                                  ).trimmed().toLower();
            staticKeywordCache[kgcAvg                   ]=qbl("Avg(%1) "                              ).trimmed().toLower();
            staticKeywordCache[kgcAvgAs                 ]=qbl("Avg(%1) as %2 "                        ).trimmed().toLower();
            staticKeywordCache[kgcBetween               ]=qbl("%1 Between %2 and %3 "                 ).trimmed().toLower();
            staticKeywordCache[kgcNotBetween            ]=qbl("not %1 Between %2 and %3 "             ).trimmed().toLower();
            staticKeywordCache[kgcBigger                ]=qbl("%1 > %2 "                              ).trimmed().toLower();
            staticKeywordCache[kgcCascade               ]=qbl("cascade "                              ).trimmed().toLower();
            staticKeywordCache[kgcCase                  ]=qbl("Case "                                 ).trimmed().toLower();
            staticKeywordCache[kgcCheck                 ]=qbl("Check "                                ).trimmed().toLower();
            staticKeywordCache[kgcClustered             ]=qbl("Clustered "                            ).trimmed().toLower();
            staticKeywordCache[kgcConstraint            ]=qbl("Constraint "                           ).trimmed().toLower();
            staticKeywordCache[kgcCount                 ]=qbl("Count(%1) "                            ).trimmed().toLower();
            staticKeywordCache[kgcCountAs               ]=qbl("Count(%1) as %2 "                      ).trimmed().toLower();
            staticKeywordCache[kgcCreate                ]=qbl("Create "                               ).trimmed().toLower();
            staticKeywordCache[kgcCreateOrReplace       ]=qbl("create or Replace "                    ).trimmed().toLower();
            staticKeywordCache[kgcDataBase              ]=qbl("DataBase "                             ).trimmed().toLower();
            staticKeywordCache[kgcDeclare               ]=qbl("Declare "                              ).trimmed().toLower();
            staticKeywordCache[kgcDefault               ]=qbl("Default "                              ).trimmed().toLower();
            staticKeywordCache[kgcDelete                ]=qbl("delete "                               ).trimmed().toLower();
            staticKeywordCache[kgcDeleteFrom            ]=qbl("delete from %1 "                       ).trimmed().toLower();
            staticKeywordCache[kgcDeleteFromUsing       ]=qbl("delete from %1 using %2 %3 "           ).trimmed().toLower();
            staticKeywordCache[kgcDesc                  ]=qbl("Desc "                                 ).trimmed().toLower();
            staticKeywordCache[kgcDistinct              ]=qbl("Distinct "                             ).trimmed().toLower();
            staticKeywordCache[kgcDrop                  ]=qbl("Drop "                                 ).trimmed().toLower();
            staticKeywordCache[kgcEnd                   ]=qbl("End "                                  ).trimmed().toLower();
            staticKeywordCache[kgcEqual                 ]=qbl("%1 = %2 "                              ).trimmed().toLower();
            staticKeywordCache[kgcNotEqual              ]=qbl("%1 <> %2 "                             ).trimmed().toLower();
            staticKeywordCache[kgcEqualBigger           ]=qbl("%1 >= %2 "                             ).trimmed().toLower();
            staticKeywordCache[kgcEqualMinor            ]=qbl("%1 <= %2 "                             ).trimmed().toLower();
            staticKeywordCache[kgcExists                ]=qbl("exists "                               ).trimmed().toLower();
            staticKeywordCache[kgcForeignKey            ]=qbl("Foreign Key "                          ).trimmed().toLower();
            staticKeywordCache[kgcFrom                  ]=qbl("from "                                 ).trimmed().toLower();
            staticKeywordCache[kgcFromValues            ]=qbl("from(values %1 )as %2 (%3) "           ).trimmed().toLower();
            staticKeywordCache[kgcFull                  ]=qbl("Full "                                 ).trimmed().toLower();
            staticKeywordCache[kgcFunction              ]=qbl("Function "                             ).trimmed().toLower();
            staticKeywordCache[kgcFunction              ]=qbl("Function "                             ).trimmed().toLower();
            staticKeywordCache[kgcGroupBy               ]=qbl("Group by "                             ).trimmed().toLower();
            staticKeywordCache[kgcHaving                ]=qbl("Having "                               ).trimmed().toLower();
            staticKeywordCache[kgcIfExists              ]=qbl("if exists "                            ).trimmed().toLower();
            staticKeywordCache[kgcIn                    ]=qbl("%1 In (%2) "                           ).trimmed().toLower();
            staticKeywordCache[kgcIndex                 ]=qbl("Index "                                ).trimmed().toLower();
            staticKeywordCache[kgcInOut                 ]=qbl("not %1 in (%2) "                       ).trimmed().toLower();
            staticKeywordCache[kgcInsert                ]=qbl("Insert "                               ).trimmed().toLower();
            staticKeywordCache[kgcInsertInto            ]=qbl("Insert into %1(%2) "                   ).trimmed().toLower();
            staticKeywordCache[kgcIsNull                ]=qbl(""                                      ).trimmed().toLower();
            staticKeywordCache[kgcIsNullCheckValue      ]=qbl(""                                      ).trimmed().toLower();
            staticKeywordCache[kgcValueIsNull           ]=qbl("%1 is null "                           ).trimmed().toLower();
            staticKeywordCache[kgcValueIsNotNull        ]=qbl("not %1 is null "                       ).trimmed().toLower();
            staticKeywordCache[kgcJoin                  ]=qbl("Join "                                 ).trimmed().toLower();
            staticKeywordCache[kgcJoinFull              ]=qbl("Full Join "                            ).trimmed().toLower();
            staticKeywordCache[kgcJoinInner             ]=qbl("Inner Join "                           ).trimmed().toLower();
            staticKeywordCache[kgcJoinLeft              ]=qbl("Left Join "                            ).trimmed().toLower();
            staticKeywordCache[kgcJoinRigth             ]=qbl("Rigth Join "                           ).trimmed().toLower();
            staticKeywordCache[kgcKey                   ]=qbl("Key "                                  ).trimmed().toLower();
            staticKeywordCache[kgcLeft                  ]=qbl("Left "                                 ).trimmed().toLower();
            staticKeywordCache[kgcILike                 ]=qbl("%1 like %2 "                           ).trimmed().toLower();
            staticKeywordCache[kgcLike                  ]=qbl("%1 like %2 "                           ).trimmed().toLower();
            staticKeywordCache[kgcSoundexEqual          ]=qbl("soundex(%1) = soundex(%2)"             ).trimmed().toLower();
            staticKeywordCache[kgcLimit                 ]=qbl("Limit %1 "                             ).trimmed().toLower();
            staticKeywordCache[kgcOffset                ]=qbl("offset %1 "                            ).trimmed().toLower();
            staticKeywordCache[kgcMax                   ]=qbl("Max(%1) "                              ).trimmed().toLower();
            staticKeywordCache[kgcMaxAs                 ]=qbl("Max(%1) as %2 "                        ).trimmed().toLower();
            staticKeywordCache[kgcMin                   ]=qbl("Min(%1) "                              ).trimmed().toLower();
            staticKeywordCache[kgcMinAs                 ]=qbl("Min(%1) as %2 "                        ).trimmed().toLower();
            staticKeywordCache[kgcMinor                 ]=qbl("%1 < %2 "                              ).trimmed().toLower();
            staticKeywordCache[kgcNot                   ]=qbl("Not "                                  ).trimmed().toLower();
            staticKeywordCache[kgcNotExists             ]=qbl("not Exists "                           ).trimmed().toLower();
            staticKeywordCache[kgcNotNull               ]=qbl("Not Null "                             ).trimmed().toLower();
            staticKeywordCache[kgcNull                  ]=qbl("Null "                                 ).trimmed().toLower();
            staticKeywordCache[kgcOn                    ]=qbl("On "                                   ).trimmed().toLower();
            staticKeywordCache[kgcOr                    ]=qbl("or "                                   ).trimmed().toLower();
            staticKeywordCache[kgcOrderBy               ]=qbl("Order by "                             ).trimmed().toLower();
            staticKeywordCache[kgcJoinOuter             ]=qbl("Outer Join "                           ).trimmed().toLower();
            staticKeywordCache[kgcPivot                 ]=qbl("Pivot "                                ).trimmed().toLower();
            staticKeywordCache[kgcPrimary               ]=qbl("Primary "                              ).trimmed().toLower();
            staticKeywordCache[kgcPrimaryKey            ]=qbl("Primary Key "                          ).trimmed().toLower();
            staticKeywordCache[kgcProcedure             ]=qbl("Procedure "                            ).trimmed().toLower();
            staticKeywordCache[kgcReplace               ]=qbl("Replace "                              ).trimmed().toLower();
            staticKeywordCache[kgcRigth                 ]=qbl("Rigth "                                ).trimmed().toLower();
            staticKeywordCache[kgcRowNumber             ]=qbl("RowNumber "                            ).trimmed().toLower();
            staticKeywordCache[kgcSchema                ]=qbl("Schema "                               ).trimmed().toLower();
            staticKeywordCache[kgcSelect                ]=qbl("Select "                               ).trimmed().toLower();
            staticKeywordCache[kgcSelectTop             ]=qbl("Select Top %1 "                        ).trimmed().toLower();
            staticKeywordCache[kgcSelectDistinct        ]=qbl("Select distinct "                      ).trimmed().toLower();
            staticKeywordCache[kgcSelectDistinctTop     ]=qbl("Select distinct top %1 "               ).trimmed().toLower();
            staticKeywordCache[kgcSelectForExists       ]=qbl("Select null "                          ).trimmed().toLower();
            staticKeywordCache[kgcSelectValues          ]=qbl("select %1 from(values(%2))as %3(%4) "  ).trimmed().toLower();
            staticKeywordCache[kgcSet                   ]=qbl("Set "                                  ).trimmed().toLower();
            staticKeywordCache[kgcSum                   ]=qbl("sum(%1) "                              ).trimmed().toLower();
            staticKeywordCache[kgcSumAs                 ]=qbl("sum(%1) as %2 "                        ).trimmed().toLower();
            staticKeywordCache[kgcTable                 ]=qbl("Table "                                ).trimmed().toLower();
            staticKeywordCache[kgcTableTemp             ]=qbl("Temp Table "                           ).trimmed().toLower();
            staticKeywordCache[kgcTemp                  ]=qbl("Temp "                                 ).trimmed().toLower();
            staticKeywordCache[kgcThen                  ]=qbl("Then "                                 ).trimmed().toLower();
            staticKeywordCache[kgcTruncate              ]=qbl("truncate "                             ).trimmed().toLower();
            staticKeywordCache[kgcTruncateTable         ]=qbl("truncate table %1 "                    ).trimmed().toLower();
            staticKeywordCache[kgcTruncateTableCacade   ]=qbl("truncate table %1 cascade "            ).trimmed().toLower();
            staticKeywordCache[kgcUnion                 ]=qbl("union "                                ).trimmed().toLower();
            staticKeywordCache[kgcUnionAll              ]=qbl("union all "                            ).trimmed().toLower();
            staticKeywordCache[kgcIntersect             ]=qbl("intersect "                            ).trimmed().toLower();
            staticKeywordCache[kgcIntersectAll          ]=qbl("intersect all "                        ).trimmed().toLower();
            staticKeywordCache[kgcExcept                ]=qbl("Except "                               ).trimmed().toLower();
            staticKeywordCache[kgcExceptAll             ]=qbl("Except all "                           ).trimmed().toLower();
            staticKeywordCache[kgcUnique                ]=qbl("unique "                               ).trimmed().toLower();
            staticKeywordCache[kgcUpdate                ]=qbl("update "                               ).trimmed().toLower();
            staticKeywordCache[kgcUpdateSet             ]=qbl("update %1 set %2 %3 %4 %5 "            ).trimmed().toLower();
            staticKeywordCache[kgcUpdateSetValues       ]=qbl(" "                                     ).trimmed().toLower();
            staticKeywordCache[kgcUpsertSet             ]=qbl(" "                                     ).trimmed().toLower();
            staticKeywordCache[kgcUsing                 ]=qbl("Using "                                ).trimmed().toLower();
            staticKeywordCache[kgcValues                ]=qbl("values %1 "                            ).trimmed().toLower();
            staticKeywordCache[kgcView                  ]=qbl("view "                                 ).trimmed().toLower();
            staticKeywordCache[kgcWhen                  ]=qbl("when "                                 ).trimmed().toLower();
            staticKeywordCache[kgcWhere                 ]=qbl("where "                                ).trimmed().toLower();
            staticKeywordCache[kgcSetApplicationName    ]=qbl(" "                                     ).trimmed().toLower();
            staticKeywordCache[kgcSetSearchPath         ]=qbl(" "                                     ).trimmed().toLower();
            staticKeywordCache[kgcNextVal               ]=QByteArray();
            staticKeywordCache[kgcNextValSelect         ]=QByteArray();
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
    auto keyWord=&o;
    if(keyWord==nullptr)
        return *this;

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
        if(qTypeId(value)==QMetaType_QVariantList)
            list=value.toList();
        else
            list<<value;

        QVariantList listRecords;
        for(auto&v:list){
            if(qTypeId(v)==QMetaType_QVariantMap || qTypeId(v)==QMetaType_QVariantHash)
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
        if(qTypeId(listRecords)==QMetaType_QVariantList){
            auto l=listRecords.toList();
            listRecords=l.size()==1?l.first():l;
        }
        else if(qTypeId(listRecords)==QMetaType_QStringList){
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
    if(!v.isValid())
        return qsl("null");

    auto __type=qTypeId(v);
    if(__type==QMetaType_Int || __type==QMetaType_UInt)
        return QString::number(v.toInt());

    if(__type==QMetaType_LongLong || __type==QMetaType_ULongLong)
        return QString::number(v.toLongLong());

    if(__type>=QMetaType_User)
        return QString::number(v.toLongLong());

    if(__type==QMetaType_Double)
        return QString::number(v.toDouble(),'f',6);

    if(__type==QMetaType_Bool)
        return v.toString();

    if(__type==QMetaType_QString || __type==QMetaType_QByteArray){
        auto s=v.toString().trimmed();
        return qsl("'")+s.replace(qsl("'"),qsl("''"))+qsl("'");
    }

    if(__type==QMetaType_QBitArray || __type==QMetaType_QChar)
        return v.toString();

    if(__type==QMetaType_QDateTime){
        auto d=v.toDateTime();
        d = (d.date()>=__d1900)?d:__dt1900;
        return qsl("'")+d.toString(format_date_time)+qsl("'");
    }

    if(__type==QMetaType_QDate){
        auto d=v.toDate();
        d = (d>=__d1900)?d:__d1900;
        return qsl("'")+d.toString(format_date)+qsl("'");
    }

    if(__type==QMetaType_QTime)
        return qsl("'")+v.toTime().toString(format_date)+qsl("'");

    if(__type==QMetaType_QUuid){
        auto u = v.toUuid();
        return u.isNull()?qsl("null"):(qsl("'")+u.toByteArray()+qsl("'"));
    }

    if(__type==QMetaType_QUrl){
        auto u = v.toUrl();
        return u.isEmpty()?qsl("null"):(qsl("'")+u.toString()+qsl("'"));
    }

    if(__type==QMetaType_QVariantList || __type==QMetaType_QStringList){
        QStringList ls;
        for(auto&vv : v.toList()){
            if(vv.isNull() || !vv.isValid()){
                continue;
            }

            if(qTypeId(vv)==QMetaType_QString || qTypeId(vv)==QMetaType_QByteArray || qTypeId(vv)==QMetaType_QChar || qTypeId(vv)==QMetaType_QBitArray){
                const auto ss=vv.toString().trimmed();
                ls.append(qsl("'") + ss + qsl("'"));
                continue;
            }

            if(qTypeId(vv)==QMetaType_QUuid){
                const auto ss=vv.toUuid().toString();
                ls.append(qsl("'") + ss + qsl("'"));
                continue;
            }

            if(qTypeId(vv)==QMetaType_QUrl){
                const auto ss=vv.toUrl().toString();
                ls.append(qsl("'") + ss + qsl("'"));
                continue;
            }

            ls.append(vv.toString().trimmed());
        }
        if(!ls.isEmpty())
            return ls.join(qsl(","));
        return qsl("null");
    }

    return qsl("undefined_sql_type");
}

QString SqlSuitableKeyWord::formatParameter(const QVariant &v)
{
    QString __return=qsl("null");
    if(v.isValid()){
        auto __type=qTypeId(v);
        if(__type==QMetaType_Int || __type==QMetaType_UInt)
            __return=QString::number(v.toInt());
        else if(__type==QMetaType_LongLong || __type==QMetaType_ULongLong)
            __return=QString::number(v.toLongLong());
        else if(__type==QMetaType_Double)
            __return=QString::number(v.toDouble(),'f',6);
        else if(__type==QMetaType_Bool)
            __return=v.toString();
        else if(__type==QMetaType_QString || __type==QMetaType_QByteArray){
            auto s=v.toString().trimmed();
            __return=qsl("'")+s.replace(qsl("'"),qsl("\'"))+qsl("'");
        }
        else if(__type==QMetaType_QBitArray || __type==QMetaType_QChar){
            auto s=v.toString().trimmed();
            __return=qsl("'")+s.replace(qsl("'"),qsl("\'"))+qsl("'");
        }
        else if(__type==QMetaType_QDateTime){
            auto d=v.toDateTime();
            d = (d.date()>=__d1900)?d:__dt1900;
            __return=qsl("'")+d.toString(format_date_time)+qsl("'");
        }
        else if(__type==QMetaType_QDate){
            auto d=v.toDate();
            d = (d>=__d1900)?d:__d1900;
            __return=qsl("'")+d.toString(format_date)+qsl("'");
        }
        else if(__type==QMetaType_QTime)
            return qsl("'")+v.toTime().toString(format_date)+qsl("'");
        else if(__type==QMetaType_QUuid){
            auto u = v.toUuid();
            __return=u.isNull()?qsl("null"):(qsl("'")+u.toByteArray()+qsl("'"));
        }
        else if(__type==QMetaType_QUrl){
            auto u = v.toUrl();
            __return=u.isEmpty()?qsl("null"):(qsl("'")+u.toString()+qsl("'"));
        }
        else if(__type==QMetaType_QVariantList || __type==QMetaType_QStringList){
            QStringList ls;
            if(__type==QMetaType_QStringList){
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
    if(qTypeId(values)==QMetaType_QVariantList)
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

    if(qTypeId(value)==QMetaType_QVariantList || qTypeId(value)==QMetaType_QStringList)
        values=value.toList();
    else if(qTypeId(value)==QMetaType_QVariantMap || qTypeId(value)==QMetaType_QVariantHash)
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

    if(qTypeId(value)==QMetaType_QVariantList)
        values=value.toList();
    else if(qTypeId(value)==QMetaType_QVariantMap || qTypeId(value)==QMetaType_QVariantHash)
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
        return QStringList{qsl("%1; { call %2(%3) }").arg(s_database,s_object, s_values)};
    }

    if(this->drivers().contains(QSqlDriver::PostgreSQL)){
        QString s_object=object.toString();
        s_object+=schema.isEmpty()?qsl_null:(schema+qsl("."));
        s_object+=s_object.isEmpty()?qsl_null:s_object;
        return QStringList{qsl("select %1(%2)").arg(s_object, s_values)};
    }

    return {};
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
        return QStringList{qsl("select %1(%2)").arg(s_object, s_values)};
    }

    return {};
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
        return QStringList{qsl("select * from %1(%2)").arg(s_object, s_values)};
    }

    return {};
}

void SqlSuitableKeyWord::init()
{

}

}

