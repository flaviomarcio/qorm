#include "./p_qorm_sql_suitable_builder.h"
#include <QSqlError>

namespace QOrm{

SqlSuitableBuilderPvt::SqlSuitableBuilderPvt(QObject *parent):QObject{parent}
{
    this->query=(dynamic_cast<Query*>(parent));
    this->combineSelect.q=this->query;
}

QSqlDriver::DbmsType SqlSuitableBuilderPvt::sqlDriver() const
{
    if(this->query!=nullptr){
        auto db=this->query->connection();
        return db.driver()->dbmsType();
    }
    return QSqlDriver::UnknownDbms;
}

SqlSuitableBuilderPvt::~SqlSuitableBuilderPvt()
{
}

QByteArray SqlSuitableBuilderPvt::makeQuery()
{
    return QByteArray();
}

SqlSuitableKeyWord &SqlSuitableBuilderPvt::parser()
{
    return SqlSuitableKeyWord::parser(this->sqlDriver());
}

QList<SqlParserCommand *> SqlSuitableBuilderPvt::strategyList()
{
    QList<SqlParserCommand*> strategy={
     &this->select
    ,&this->insert
    ,&this->update
    ,&this->upsert
    ,&this->remove
    ,&this->procedure
    ,&this->function
    ,&this->structure
    ,&this->combineSelect
    };
    return strategy;
}

void SqlSuitableBuilderPvt::clear()
{
    for(auto &v:this->strategyList())
        v->clear();
}

QSqlDatabase SqlSuitableBuilderPvt::connection()
{
    return QSqlDatabase::database(this->connectionName);
}

void SqlSuitableBuilderPvt::setConnection(const QString &db)
{
    this->connectionName=QSqlDatabase::database(db).connectionName();
}

void SqlSuitableBuilderPvt::setConnection(const QSqlDatabase &db)
{
    this->connectionName=db.connectionName();
}

bool SqlSuitableBuilderPvt::canBuild()
{
    for(auto &strategy:this->strategyList())
        if(strategy->isChanged())
            return true;
    return false;
}

bool SqlSuitableBuilderPvt::build()
{
    this->prepareIgnored=false;
    for(auto &strategy:this->strategyList()){
        if(strategy==nullptr)
            continue;

        if(!strategy->isChanged())
            continue;

        if(!strategy->makeObject())
            continue;

        auto parser=&this->parser();

        this->preparedQuery.clear();
        if(parser==nullptr)
            return (&this->parser())!=nullptr;


        auto lst=strategy->toScript(*parser);
        for(auto &v:lst)
            this->preparedQuery.append(v);

        if(strategy->ignorePrepare()){
            this->prepareIgnored=strategy->ignorePrepare();
            continue;
        }

        auto command=lst.join(' ') + QStringLiteral(";");
        {
            auto &p=*static_cast<QueryPvt*>(query->p);
            if(!p.sqlQuery.prepare(command)){
                qDebug()<<p.sqlQuery.lastError().text();
                qDebug()<<command;
                return false;
            }
        }
    }
    return !this->preparedQuery.isEmpty();
}

}
