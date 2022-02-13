#pragma once

#include "../qorm_query.h"
#include "../qorm_sql_suitable_builder.h"
#include "../qorm_object_db.h"
#include "./p_qorm_query.h"
#include "./p_qorm_sql_suitable_parser_keywork.h"

namespace QOrm{

class SqlSuitableBuilderPvt:public QObject {

public:

    Query*query=nullptr;
    QStringList _build;
    SqlParserInsert insert;
    SqlParserUpdate update;
    SqlParserUpsert upsert;
    SqlParserRemove remove;
    SqlParserSelect select;
    SqlParserProcedure procedure;
    SqlParserFunction function;
    SqlParserStructure structure;
    SqlParserCombineSelect combineSelect;
    SqlSuitableKeyWord*suitable=nullptr;
    explicit SqlSuitableBuilderPvt(QObject *parent = nullptr);
    QSqlDriver::DbmsType sqlDriver()const;
    ~SqlSuitableBuilderPvt();
    QByteArray makeQuery();
    SqlSuitableKeyWord&parser();
    QList<SqlParserCommand*> strategyList();

    void clear();

public:
    QSqlDatabase connection();
    void setConnection(const QString&db);
    void setConnection(const QSqlDatabase&db);

public:
    bool canBuild();
    bool build();
private:
    QString connectionName;
};

}

