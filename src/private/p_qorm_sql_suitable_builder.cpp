#include "./p_qorm_sql_suitable_builder.h"

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

void SqlSuitableBuilderPvt::clear()
{
    for(auto &v:this->strategyList()){
        v->clear();
    }
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
    for(auto &strategy:this->strategyList()){
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
        for(auto &v:lst)
            this->_build<<v;

        if(strategy->ignorePrepare())
            continue;

        auto command=lst.join(' ') + QStringLiteral(";");
        {
            auto &p=*static_cast<QueryPvt*>(query->p);
            if(!p.sqlQuery.prepare(command))
                return false;
        }
    }
    return !this->_build.isEmpty();
}

}
