#pragma once

#include "../qorm_query.h"
#include "../qorm_sql_suitable_builder.h"
#include "../qorm_object_db.h"
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
        Q_INVOKABLE explicit SqlSuitableBuilderPvt(QObject *parent = nullptr):QObject(parent){
            this->query=(dynamic_cast<Query*>(parent));
            this->combineSelect.q=this->query;
        }

        QSqlDriver::DbmsType sqlDriver()const{
            if(this->query!=nullptr){
                auto db=this->query->connection();
                return db.driver()->dbmsType();
            }
            return QSqlDriver::UnknownDbms;
        }

        Q_INVOKABLE ~SqlSuitableBuilderPvt(){
        }

        QByteArray makeQuery(){
            return QByteArray();
        }

        SqlSuitableKeyWord&parser(){
            return SqlSuitableKeyWord::parser(this->sqlDriver());
        }

        QList<SqlParserCommand*> strategyList(){
            QList<SqlParserCommand*> strategy;
            strategy<<&this->select;
            strategy<<&this->insert;
            strategy<<&this->update;
            strategy<<&this->upsert;
            strategy<<&this->remove;
            strategy<<&this->procedure;
            strategy<<&this->function;
            strategy<<&this->structure;
            strategy<<&this->combineSelect;
            return strategy;
        }


        void clear(){
            for(auto&v:this->strategyList()){
                v->clear();
            }
        }


    public:
        QSqlDatabase connection(){
            return QSqlDatabase::database(this->connectionName);
        }

        void setConnection(const QString&db){
            this->connectionName=QSqlDatabase::database(db).connectionName();
        }

        void setConnection(const QSqlDatabase&db){
            this->connectionName=db.connectionName();
        }

    public:

        bool canBuild(){
            for(auto&strategy:this->strategyList())
                if(strategy->isChanged())
                    return true;
            return false;
        }

        bool build(){
            for(auto&strategy:this->strategyList()){
                if(strategy==nullptr)
                    continue;

                if(!strategy->isChanged())
                    continue;

                if(!strategy->makeObject())
                    continue;

                auto parser=&this->parser();

                this->_build.clear();
                if(parser==nullptr){
                    return (&this->parser())!=nullptr;
                }

                auto lst=strategy->toScript(*parser);
                for(auto&v:lst)
                    this->_build<<v;

                if(strategy->ignorePrepare())
                    continue;

                auto command=lst.join(' ') + qsl(";");
                if(!this->query->sqlQuery().prepare(command))
                    return false;
            }
            return !this->_build.isEmpty();
        }
    private:
        QString connectionName;
    };

}

