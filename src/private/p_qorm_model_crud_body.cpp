#include "./p_qorm_model_crud_body.h"

CRUDBody::CRUDBody(const QVariant &other):QVariantHash(other.toHash())
{

}

CRUDBody::CRUDBody(const QVariant &strategy, const QVariant &source):QVariantHash({{qsl("strategy"), strategy}, {qsl("source"), source}})
{

}

CRUDBody::CRUDBody(const QOrm::CRUDStrategy strategy, const QVariant &source):QVariantHash({{qsl("strategy"), strategy}, {qsl("source"), source}})
{

}

CRUDBody::~CRUDBody()
{
}

QOrm::CRUDStrategy CRUDBody::strategy()const
{
    static auto varName=QStringList()<<__func__<<qsl("method");
    auto&vHash=*this;
    for(auto&vName:varName){
        const QVariant&v=vHash[vName];
        if(v.isValid()){
            auto vName=v.toString().toLower();
            if(QOrm::__stringToStrategy.contains(vName)){
                auto v=QOrm::__stringToStrategy.value(vName);
                auto t=QOrm::CRUDStrategy(v.toUInt());
                return t;
            }
        }
    }
    return QOrm::CRUDStrategy::Undefined;
}

const QVariant &CRUDBody::source()
{
    return(*this)[__func__];
}

bool CRUDBody::isStrategy(const QVariant &v) const
{
    const qlonglong strategy=this->strategy();
    return (strategy==v.toInt());

}
