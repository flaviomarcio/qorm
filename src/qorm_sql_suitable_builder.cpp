#include "./qorm_sql_suitable_builder.h"
#include "./private/p_qorm_sql_suitable_builder.h"

namespace QOrm {

#define dPvt()\
    auto&p = *reinterpret_cast<SqlSuitableBuilderPvt*>(this->p)

SqlSuitableBuilder::SqlSuitableBuilder(Query*parent):ObjectDb(parent){
    this->p = new SqlSuitableBuilderPvt(parent);
}

SqlSuitableBuilder::~SqlSuitableBuilder()
{
    dPvt();delete&p;
}

SqlParserInsert &SqlSuitableBuilder::insert()
{
    dPvt();
    return p.insert;
}

SqlParserUpdate &SqlSuitableBuilder::update()
{
    dPvt();
    return p.update;
}

SqlParserUpsert &SqlSuitableBuilder::upsert()
{
    dPvt();
    return p.upsert;
}

SqlParserRemove &SqlSuitableBuilder::remove()
{
    dPvt();
    return p.remove;
}

SqlParserSelect &SqlSuitableBuilder::select()
{
    dPvt();
    return p.select;
}

SqlParserProcedure &SqlSuitableBuilder::procedure()
{
    dPvt();
    return p.procedure;
}

SqlParserFunction &SqlSuitableBuilder::function()
{
    dPvt();
    return p.function;
}

SqlParserStructure &SqlSuitableBuilder::structure()
{
    dPvt();
    return p.structure;
}

SqlParserCombineSelect &SqlSuitableBuilder::combineSelect()
{
    dPvt();
    return p.combineSelect;
}

bool SqlSuitableBuilder::build()
{
    dPvt();
    if(!p.canBuild())
        return true;
    else if(!p.build())
        return false;
    else
        return true;
}

QStringList &SqlSuitableBuilder::preparedQuery() const
{
    dPvt();
    return p._build;
}

void SqlSuitableBuilder::clear()
{
    dPvt();
    p.clear();
}

}
