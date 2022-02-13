#pragma once

#include "../qorm_sql_suitable_builder.h"
#include "../qorm_object_db.h"
#include "../qorm_model.h"
#include "../qorm_query.h"
#include <QThread>
#include <QDir>
#include <QMutex>
#include <QtSql/QSqlRecord>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include <QCoreApplication>

namespace QOrm{


class QueryPvt:public QObject{
public:
    Query*query=nullptr;
    QMetaObject modelMetaObject;
    SqlSuitableBuilder sqlBuilder;
    QSqlQuery sqlQuery;
    QStringList preperedQuery;
    QStringList sqlQueryFields;
    QSqlRecord sqlRecord;
    QSqlError sqlError;
    QString connectionName;

    QOrm::SqlSuitableKeyWord*parser=nullptr;

    QString fileLog;
    bool internalNext=false;
    explicit QueryPvt(Query *parent, const QSqlDatabase&db);

    bool clearCache();

    bool prepare();

    bool prepareExec();

    bool prepareExecCache();

    bool exec(const QVariant&command);

    bool initNext();

    bool next();

    void close();

    void clear();

    void makeSqlRecord(QSqlRecord&sqlRecord);

    bool makeModelMetaObject(QMetaObject&metaObject);

    void writeLog(const QString&scriptSQL, const QSqlError&error);

    void writeLogFinish(const QSqlError&error);

};

}

