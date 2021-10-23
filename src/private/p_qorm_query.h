#pragma once

#include "./qorm_sql_suitable_builder.h"
#include "../qorm_object_db.h"
#include "../qorm_model.h"
#include "../qorm_query.h"
#include <QThread>
#include <QDir>
#include <QMutex>
#include <QSqlRecord>
#include <QSqlError>
#include <QSqlQuery>
#include <QCoreApplication>

namespace QOrm{
#ifdef QT_DEBUG
    static bool static_log_register=true;
#else
    static bool static_log_register=false;
#endif
    static bool static_log_dir_inited=false;
    static QString static_log_dir;

    static void static_log_dir_clear(const QString&ormLogDir){
        QStringList dir_found;
        QStringList dir_rm_file;
        dir_found.append(ormLogDir);
        while(!dir_found.isEmpty()){
            auto scanDir = dir_found.takeFirst();
            dir_rm_file.append(scanDir);
            QDir dir(scanDir);
            if(dir.exists(scanDir)){
                dir.setFilter(QDir::AllDirs);
                for(auto&scanInDir:dir.entryList()){
                    if(scanInDir==qsl(".") || scanInDir==qsl(".."))
                        continue;
                    else{
                        auto dir=qsl("%1/%2").arg(scanDir, scanInDir);
                        dir_rm_file.append(dir);
                        dir_found.append(dir);
                    }
                }
            }
        }

        auto ext=QStringList()<<qbl("*.*");
        for(auto&sdir:dir_rm_file){
            QDir scanDir(sdir);
            if(scanDir.exists()){
                scanDir.setFilter(QDir::Drives | QDir::Files);
                scanDir.setNameFilters(ext);
                for(auto&dirFile : scanDir.entryList()){
                    auto fileName=sdir+qbl("/")+dirFile;
                    QFile::remove(fileName);
                }
            }
        }
    }

    static void static_log_init_dir(){
        if(!static_log_dir_inited){
            static_log_dir_inited=true;
            auto env = QString(getenv(qbl("Q_LOG_ENABLED"))).trimmed();
#ifdef QT_DEBUG
            static_log_register = env.isEmpty()?true :QVariant(env).toBool();
#else
            static_log_register = env.isEmpty()?false:QVariant(env).toBool();
#endif
            if(static_log_register){
                static const auto log_local_name=QString(__PRETTY_FUNCTION__).split(qsl("::")).first().replace(qsl("void "),qsl_null).split(qsl_space).last();
                static_log_dir=qsl("%1/%2/%3").arg(QDir::homePath(), log_local_name, qApp->applicationName());
                QDir dir(static_log_dir);
                if(!dir.exists(static_log_dir))
                    dir.mkpath(static_log_dir);
                if(dir.exists(static_log_dir))
                    static_log_dir_clear(static_log_dir);
            }
        }
    }

    Q_COREAPP_STARTUP_FUNCTION(static_log_init_dir);


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
        explicit QueryPvt(Query *parent, const QSqlDatabase&db) : QObject(parent), sqlBuilder(parent){
            this->query=parent;
            auto currentName=QThread::currentThread()->objectName().trimmed();
            if(currentName.isEmpty())
                currentName=QString::number(qlonglong(QThread::currentThreadId()),16);
            this->fileLog=qsl("%1/%2.sql").arg(static_log_dir, QString::number(qlonglong(QThread::currentThreadId()),16));
            this->connectionName=db.isOpen()?db.connectionName():QString();
        }

        bool clearCache(){
            this->preperedQuery.clear();
            return true;
        }

        bool prepare(){
            this->close();
            {//recuperando conexao
                QSqlDatabase connection;

                if(this->connectionName.isEmpty())
                    connection=this->query->connection();
                else
                    connection=QSqlDatabase::database(this->connectionName);

                if(!connection.isOpen()){
#ifdef Q_ORM_LOG_SUPER_VERBOSE
                    sWarning()<<qbl("QSqlDatabase is not opened");
#endif
                    return false;
                }
                this->sqlQuery=QSqlQuery(connection);
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

        bool prepareExec(){
            this->clearCache();
            this->prepareExecCache();
            return true;
        }

        bool prepareExecCache(){
            if(!this->prepare()){
                this->writeLog(this->sqlQuery.executedQuery(), this->sqlQuery.lastError());
#if Q_ORM_LOG
                sWarning()<<this->sqlQuery.executedQuery();
                sWarning()<<this->sqlQuery.lastError();
#endif
                this->sqlError=this->sqlQuery.lastError();
                return false;
            }

            this->writeLog(this->sqlQuery.executedQuery(), QSqlError());
            return true;
        }

        bool exec(const QVariant&command){
            auto scriptCommand=command.toString().trimmed();
            if(!scriptCommand.isEmpty()){
                if(!this->sqlQuery.prepare(scriptCommand)){
                    this->writeLog(this->sqlQuery.executedQuery(), this->sqlQuery.lastError());
#if Q_ORM_LOG
                    sWarning()<<this->sqlQuery.executedQuery();
                    sWarning()<<this->sqlQuery.lastError();
#endif
                    this->sqlError=this->sqlQuery.lastError();
                    return false;
                }
                else{
                    this->preperedQuery.clear();
                    this->preperedQuery<<scriptCommand;
                }
            }

            if(!this->sqlQuery.exec()){
                this->writeLog(this->sqlQuery.executedQuery(), this->sqlQuery.lastError());
#if Q_ORM_LOG
                sWarning()<<this->sqlQuery.executedQuery();
                sWarning()<<this->sqlQuery.lastError();
#endif
                this->sqlError=this->sqlQuery.lastError();
                return false;
            }

            this->writeLogFinish(this->sqlQuery.lastError());
            return true;
        }



        bool initNext(){
            if(!this->internalNext)
                return this->next();
            return this->internalNext;
        }

        bool next(){
            auto r=this->sqlQuery.next();
            this->internalNext=true;
            if(r)
                this->sqlRecord=this->sqlQuery.record();
            return r;
        }

        void close(){
            this->sqlRecord=QSqlRecord();
            this->sqlError=QSqlError();
            this->internalNext=false;
        }

        void clear(){
            this->sqlBuilder.clear();
            this->sqlRecord=QSqlRecord();
            this->sqlError=QSqlError();
            this->sqlQuery.finish();
            this->sqlQuery.clear();
            this->sqlQuery=QSqlQuery();
            this->internalNext=false;
        }

        void makeSqlRecord(QSqlRecord&sqlRecord){
            this->sqlRecord=sqlRecord;
            this->sqlQueryFields.clear();
            for (int col = 0; col < this->sqlRecord.count(); ++col)
                this->sqlQueryFields<<this->sqlRecord.fieldName(col).toLower().trimmed();
        }

        bool makeModelMetaObject(QMetaObject&metaObject){
            if(&this->modelMetaObject == &metaObject)
                return true;

            if(!Model::staticMetaObject.inherits(&this->modelMetaObject))
                return false;

            this->modelMetaObject=metaObject;
            return true;
        }

        void writeLog(const QString&scriptSQL, const QSqlError&error){
            if(!static_log_register)
                return;
#ifdef QT_DEBUG
            static_log_init_dir();
#endif
            if(scriptSQL.isEmpty())
                return;

            QFile file(this->fileLog);

            QString connectionLine;
            if(!file.exists()){
                auto connection=QSqlDatabase::database(this->connectionName);
                connectionLine = qsl("driver=%1, hostname=%2, port=%3, userName=%4, connectOptions=%5").arg(connection.driverName(), connection.hostName(), QString::number(connection.port()), connection.userName(), connection.connectOptions());
            }

            if (file.open(QIODevice::Append | QIODevice::WriteOnly | QIODevice::Text)){
                QTextStream outText(&file);
                if(!connectionLine.isEmpty()){
                    auto logSeparator = qsl("--").rightJustified(connectionLine.length(),'-');
                    outText << logSeparator << qsl("\n");
                    outText << qsl("|")+connectionLine << qsl("\n");
                    outText << logSeparator << "\n";
                }
                outText << qsl("|")+QDateTime::currentDateTime().toString(qsl("hh:mm:ss.zzz"))+qsl("| query start\n");
                outText << scriptSQL<<qsl("\n");
                if(error.isValid())
                    outText << error.text()<<qsl("\n");
                file.flush();
                file.close();
            }
        }

        void writeLogFinish(const QSqlError&error){
            if(!static_log_register)
                return;
            QFile file(this->fileLog);
            if (file.open(QIODevice::Append | QIODevice::WriteOnly | QIODevice::Text)){
                QTextStream outText(&file);
                if(error.isValid())
                    outText << error.text()<<qbl("\n");
                outText << qbl("|")+QDateTime::currentDateTime().toString(qbl("hh:mm:ss.zzz"))+qbl("| query finish\n\n");
                file.flush();
                file.close();
            }
        }

    };

}

