#include "./p_qorm_query.h"
#include "../qorm_model.h"
#include "../qorm_const.h"
#include "../qorm_macro.h"
#include "../qorm_log.h"

namespace QOrm{
static const auto __sql="sql";
static const auto __query="query";

QueryPvt::QueryPvt(Query *parent, const QSqlDatabase &db) : QObject{parent}, sqlBuilder(parent)
{
    this->query=parent;
    this->connectionName=db.isOpen()?db.connectionName():QString{};
    this->sqlQuery=QSqlQuery{db};
    this->fileLog=logFile(__sql, __query);
}

bool QueryPvt::clearCache()
{
    this->preperedQuery.clear();
    return true;
}

bool QueryPvt::prepare()
{
    this->close();
    {//recuperando conexao
        QSqlDatabase connection;

        if(this->connectionName.isEmpty())
            connection=this->query->connection();
        else
            connection=QSqlDatabase::database(this->connectionName);

        if(!connection.isOpen()){
#ifdef Q_ORM_LOG_SUPER_VERBOSE
            oWarning()<<QByteArrayLiteral("QSqlDatabase is not opened");
#endif
            return false;
        }
        this->sqlQuery.finish();
        this->sqlQuery.clear();
        this->sqlQuery=QSqlQuery{connection};
    }

    if(!this->sqlBuilder.build()){
        this->sqlBuilder.clear();
        this->preperedQuery.clear();
        return false;
    }

    this->sqlBuilder.clear();
    this->preperedQuery=this->sqlBuilder.preparedQuery();
    return true;
}

bool QueryPvt::prepareExec()
{
    this->clearCache();
    this->prepareExecCache();
    return true;
}

bool QueryPvt::prepareExecCache()
{
    if(!this->prepare()){
        this->writeLog(this->sqlQuery.executedQuery(), this->sqlQuery.lastError());
#if Q_ORM_LOG
        oWarning()<<this->sqlQuery.executedQuery();
        oWarning()<<this->sqlQuery.lastError();
#endif
        this->sqlError=this->sqlQuery.lastError();
        return false;
    }

    this->writeLog(this->sqlQuery.executedQuery(), {});
    return true;
}

bool QueryPvt::exec(const QVariant &command)
{
    auto scriptCommand=command.toString().trimmed();
    if(!scriptCommand.isEmpty()){
        if(!this->sqlQuery.prepare(scriptCommand)){
            this->writeLog(this->sqlQuery.executedQuery(), this->sqlQuery.lastError());
#if Q_ORM_LOG
            oWarning()<<this->sqlQuery.executedQuery();
            oWarning()<<this->sqlQuery.lastError();
#endif
            this->sqlError=this->sqlQuery.lastError();
            return false;
        }
        this->preperedQuery.clear();
        this->preperedQuery.append(scriptCommand);
    }

    if(!this->sqlQuery.exec()){
        this->writeLog(this->sqlQuery.executedQuery(), this->sqlQuery.lastError());
#if Q_ORM_LOG
        oWarning()<<this->sqlQuery.executedQuery();
        oWarning()<<this->sqlQuery.lastError();
#endif
        this->sqlError=this->sqlQuery.lastError();
        return false;
    }

    this->writeLogFinish(this->sqlQuery.lastError());
    return true;
}

bool QueryPvt::initNext()
{
    if(!this->internalNext)
        return this->next();
    return this->internalNext;
}

bool QueryPvt::next()
{
    auto r=this->sqlQuery.next();
    this->internalNext=true;
    if(r)
        this->sqlRecord=this->sqlQuery.record();
    return r;
}

void QueryPvt::close()
{
    this->sqlRecord={};
    this->sqlError={};
    this->internalNext=false;
}

void QueryPvt::clear()
{
    this->sqlBuilder.clear();
    this->sqlRecord={};
    this->sqlError={};
    this->sqlQuery.finish();
    this->sqlQuery.clear();
    this->sqlQuery=QSqlQuery();
    this->internalNext=false;
}

void QueryPvt::makeSqlRecord(QSqlRecord &sqlRecord)
{
    this->sqlRecord=sqlRecord;
    this->sqlQueryFields.clear();
    for (int col = 0; col < this->sqlRecord.count(); ++col)
        this->sqlQueryFields<<this->sqlRecord.fieldName(col).toLower().trimmed();
}

bool QueryPvt::makeModelMetaObject(QMetaObject &metaObject)
{
    if(&this->modelMetaObject == &metaObject)
        return true;

    if(!Model::staticMetaObject.inherits(&this->modelMetaObject))
        return false;

    this->modelMetaObject=metaObject;
    return true;
}

void QueryPvt::writeLog(const QString &scriptSQL, const QSqlError &error)
{
    if(!logRegister())
        return;

    if(scriptSQL.isEmpty())
        return;

    QFile file(this->fileLog);

    QString connectionLine;
    if(!file.exists()){
        auto connection=QSqlDatabase::database(this->connectionName);
        static const auto format=QStringLiteral("driver=%1, hostname=%2, port=%3, userName=%4, connectOptions=%5");
        connectionLine = format.arg(connection.driverName(),
                                    connection.hostName(),
                                    QString::number(connection.port()),
                                    connection.userName(),
                                    connection.connectOptions()
                                    );
    }

    if (!file.open(QIODevice::Append | QIODevice::WriteOnly | QIODevice::Text))
        return;

    QTextStream outText(&file);
    if(!connectionLine.isEmpty()){
        auto logSeparator = QStringLiteral("--").rightJustified(connectionLine.length(),'-');
        outText << logSeparator << QStringLiteral("\n");
        outText << QStringLiteral("|")+connectionLine << QStringLiteral("\n");
        outText << logSeparator << "\n";
    }
    static auto timeFormat=QStringLiteral("hh:mm:ss.zzz");
    static auto qStart=QStringLiteral("| query start\n");
    outText << QStringLiteral("|")+QDateTime::currentDateTime().toString(timeFormat)+qStart;
    outText << scriptSQL<<QStringLiteral("\n");
    if(error.isValid())
        outText << error.text()<<QStringLiteral("\n");
    file.flush();
    file.close();
}

void QueryPvt::writeLogFinish(const QSqlError &error)
{
    if(!logRegister())
        return;

    QFile file(this->fileLog);
    if (!file.open(QIODevice::Append | QIODevice::WriteOnly | QIODevice::Text))
        return;

    QTextStream outText(&file);
    if(error.isValid())
        outText << error.text()<<QByteArrayLiteral("\n");
    static auto timeFormat=QStringLiteral("hh:mm:ss.zzz");
    static auto qFinish=QByteArrayLiteral("| query finish\n\n");
    outText << QByteArrayLiteral("|")+QDateTime::currentDateTime().toString(timeFormat)+qFinish;
    file.flush();
    file.close();
}

}
