#include "./p_qorm_sql_suitable_parser_strategy.h"
#include "../qorm_sql_suitable_builder.h"
#include "../qorm_query.h"

#define dQuery\
    auto&q=*reinterpret_cast<Query*>(this->q)

namespace QOrm {

    SqlParserCombineSelect::SqlParserCombineSelect(const QVariant &v):SqlParserCommand(v){
    }

    SqlParserCombineSelect::SqlParserCombineSelect():SqlParserCommand(){
    }

    SqlParserCombineSelect::~SqlParserCombineSelect()
    {
    }

    SqlParserCombineSelect &SqlParserCombineSelect::unionMerge(){
        return this->setV(QOrm::kgcUnion);
    }

    SqlParserCombineSelect &SqlParserCombineSelect::unionAll(){
        return this->setV(QOrm::kgcUnionAll);
    }

    SqlParserCombineSelect &SqlParserCombineSelect::intersect(){
        return this->setV(QOrm::kgcIntersect);
    }

    SqlParserCombineSelect &SqlParserCombineSelect::intersectAll(){
        return this->setV(QOrm::kgcIntersectAll);
    }

    SqlParserCombineSelect &SqlParserCombineSelect::except(){
        return this->setV(QOrm::kgcExcept);
    }

    SqlParserCombineSelect &SqlParserCombineSelect::exceptAll(){
        return this->setV(QOrm::kgcExceptAll);
    }

    SqlParserCombineSelect &SqlParserCombineSelect::setV(int t)
    {
        auto v=SqlParserCommand(t);
        this->setValue(v);
        dQuery;
        q.prepareCache();
        return*this;
    }



}

