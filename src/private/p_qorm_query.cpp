#include "./p_qorm_query.h"
#include "../qorm_startup.h"
#include "../qorm_model.h"
#include "../qorm_const.h"
#include "../qorm_macro.h"

namespace QOrm{
static bool static_log_register=
#ifdef QT_DEBUG
true;
#else
false;
#endif
Q_GLOBAL_STATIC(QString, static_log_dir);

static void static_log_dir_clear(const QString &ormLogDir)
{
    QStringList dir_found;
    QStringList dir_rm_file;
    dir_found.append(ormLogDir);
    while(!dir_found.isEmpty()){
        auto scanDir = dir_found.takeFirst();
        dir_rm_file.append(scanDir);
        QDir dir(scanDir);
        if(!dir.exists(scanDir))
            continue;

        dir.setFilter(QDir::AllDirs);
        for(auto &scanInDir:dir.entryList()){
            if(scanInDir==QStringLiteral(".") || scanInDir==QStringLiteral(".."))
                continue;

            auto dir=QStringLiteral("%1/%2").arg(scanDir, scanInDir);
            dir_rm_file.append(dir);
            dir_found.append(dir);
        }
    }

    auto ext=QStringList{QByteArrayLiteral("*.*")};
    for(auto &sdir:dir_rm_file){
        QDir scanDir(sdir);
        if(!scanDir.exists())
            continue;
        scanDir.setFilter(QDir::Drives | QDir::Files);
        scanDir.setNameFilters(ext);
        for(auto &dirFile : scanDir.entryList()){
            auto fileName=sdir+QByteArrayLiteral("/")+dirFile;
            QFile::remove(fileName);
        }
    }
}

static void static_log_init_dir()
{
    auto env = QString{getenv(QByteArrayLiteral("Q_LOG_ENABLED"))}.trimmed();
#ifdef QT_DEBUG
    static_log_register = env.isEmpty()?true :QVariant(env).toBool();
#else
    static_log_register = env.isEmpty()?false:QVariant(env).toBool();
#endif
    if(!static_log_register)
        return;

    static const auto log_local_name=QString{__PRETTY_FUNCTION__}.split(QStringLiteral("::")).first().replace(QStringLiteral("void "), "").split(QStringLiteral(" ")).last();
    *static_log_dir=QStringLiteral("%1/%2/%3").arg(QDir::homePath(), log_local_name, qApp->applicationName());

    QDir dir(*static_log_dir);
    if(!dir.exists(*static_log_dir))
        dir.mkpath(*static_log_dir);

    if(dir.exists(*static_log_dir))
        static_log_dir_clear(*static_log_dir);
}

Q_ORM_STARTUP_FUNCTION(static_log_init_dir);

QueryPvt::QueryPvt(Query *parent, const QSqlDatabase &db) : QObject{parent}, sqlBuilder(parent)
{
    this->query=parent;
    auto currentName=QThread::currentThread()->objectName().trimmed();
    if(currentName.isEmpty())
        currentName=QString::number(qlonglong(QThread::currentThreadId()),16);
    this->fileLog=QStringLiteral("%1/%2.sql").arg(*static_log_dir, QString::number(qlonglong(QThread::currentThreadId()),16));
    this->connectionName=db.isOpen()?db.connectionName():QString{};
    this->sqlQuery=QSqlQuery{db};
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
    if(!static_log_register)
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
    if(!static_log_register)
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
