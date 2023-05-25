#include "./qorm_sql_suitable_builder.h"
#include "./private/p_qorm_sql_suitable_builder.h"

namespace QOrm {

SqlSuitableBuilder::SqlSuitableBuilder(Query*parent):ObjectDb{parent}
{
    this->p = new SqlSuitableBuilderPvt{parent};
}

SqlSuitableBuilder::~SqlSuitableBuilder()
{
}

SqlParserInsert &SqlSuitableBuilder::insert()
{
    return p->insert;
}

SqlParserUpdate &SqlSuitableBuilder::update()
{

    return p->update;
}

SqlParserUpsert &SqlSuitableBuilder::upsert()
{

    return p->upsert;
}

SqlParserRemove &SqlSuitableBuilder::remove()
{

    return p->remove;
}

SqlParserSelect &SqlSuitableBuilder::select()
{

    return p->select;
}

SqlParserProcedure &SqlSuitableBuilder::procedure()
{

    return p->procedure;
}

SqlParserFunction &SqlSuitableBuilder::function()
{

    return p->function;
}

SqlParserStructure &SqlSuitableBuilder::structure()
{

    return p->structure;
}

SqlParserCombineSelect &SqlSuitableBuilder::combineSelect()
{

    return p->combineSelect;
}

bool SqlSuitableBuilder::build()
{

    if(!p->canBuild())
        return true;

    if(!p->build())
        return false;

    return true;
}

QStringList &SqlSuitableBuilder::preparedQuery() const
{
    return p->preparedQuery;
}

bool SqlSuitableBuilder::prepareIgnored() const
{
    return p->prepareIgnored;
}

void SqlSuitableBuilder::clear()
{

    p->clear();
}

}
