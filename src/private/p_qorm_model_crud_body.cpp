#include "./p_qorm_model_crud_body.h"
#include "../qorm_model_crud_types.h"
#include <QStm>



namespace QOrm {

static const auto __pages="pages";
static const auto __items="items";
static const auto __uuid="uuid";
static const auto __expressions="expressions";
static const auto __source="source";
static const auto __custom_source="customSource";
static const auto __strategy="strategy";
static const auto __resultInfo="resultInfo";

CRUDBody::CRUDBody(const QVariant &other) : QVariantHash(other.toHash()) {}

CRUDBody::CRUDBody(const QVariant &strategy, const QVariant &source): QVariantHash{{__strategy, strategy}, {__source, source}}{}

CRUDBody::CRUDBody(const QVariant &strategy, const QVariant &expressions, const QVariant &source)
: QVariantHash{
                {__strategy, strategy},
                {__expressions, expressions},
                {__source, source}
              }
{

}

CRUDTypes::Strategy CRUDBody::strategy()const
{
    auto vStrategy=this->value(__strategy);
    QStm::MetaEnum<QOrm::CRUDTypes::Strategy> e;
    if(!e.canType(vStrategy))
        return QOrm::CRUDTypes::Strategy::Custom;
    return e.type();
}

QString CRUDBody::strategyName() const
{
    auto vStrategy=this->value(__strategy).toString().trimmed();
    QStm::MetaEnum<QOrm::CRUDTypes::Strategy> e;
    if(!e.canType(vStrategy))
        return vStrategy;
    return e.name();
}

bool CRUDBody::canStrategy(const QVariant &strategy)
{
    auto strategyValue=this->value(__strategy);

    QStm::MetaEnum<QOrm::CRUDTypes::Strategy> e(strategyValue);
    if(e.equal(strategy))
        return true;

    if(strategy.toString().trimmed().toLower()==strategyValue.toString().trimmed().toLower())
        return true;

    return false;
}

QVariant CRUDBody::expressions() const
{
    Q_DECLARE_VU;
    if(!this->contains(__expressions))
        return {};
    return vu.toVariant(this->value(__expressions));
}

QVariant CRUDBody::source() const
{
    if(!this->contains(__source))
        return {};
    return this->value(__source);
}

QVariant CRUDBody::customSource() const
{
    if(!this->contains(__custom_source))
        return {};
    return this->value(__custom_source);
}

QVariantList CRUDBody::items() const
{
    auto v=this->contains(__source)?this->value(__source):QVariant{};
    if(!v.isValid() || v.isNull())
        return {};

    QVariantList vList;
    switch (v.typeId()) {
    case QMetaType::QVariantList:
    case QMetaType::QStringList:
        vList=v.toList();
        break;
    case QMetaType::QVariantHash:
    case QMetaType::QVariantMap:
        vList.append(v);
        break;
    default:
        break;
    }
    for(auto &v:vList){
        auto vHash=v.toHash();
        if(vHash.contains(__source))
            v=vHash.value(__source);
    }
    return vList;
}

QVariant CRUDBody::data() const
{
    QStm::VariantUtil vu;
    auto v=this->value(__expressions);
    if(!vu.vIsEmpty(v))
        return v;

    v=this->value(__source);
    if(!vu.vIsEmpty(v))
        return v;

    return {};
}

QVariantList CRUDBody::pagesList() const
{
    auto vCrudSource=this->source().toHash();
    if(vCrudSource.contains(__pages))
        return vCrudSource.value(__pages).toList();
    return {};
}

QVariantHash CRUDBody::pagesHash() const
{
    QVariantHash __return;
    auto pagesList=this->pagesList();
    for(auto &v:pagesList){
        auto vHash=v.toHash();
        QVariant vCrudBody;
        if(vHash.contains(__items))
            vCrudBody=vHash.value(__items).toList();
        else
            vCrudBody=vHash;
        auto crudUuid=vHash.value(__uuid).toUuid().toString();
        __return.insert(crudUuid, vCrudBody);
    }
    return __return;
}

QVariantList CRUDBody::itemsList() const
{
    QVariantList __return;

    auto pagesList=this->pagesList();
    for(auto &v:pagesList){
        auto vHash=v.toHash();
        QVariant vCrudBody;
        if(vHash.contains(__items))
            vCrudBody=vHash.value(__items).toList();
        else
            vCrudBody=vHash;
        __return.append(vCrudBody);
    }
    return __return;
}

bool CRUDBody::isStrategy(const QVariant &v)
{
    const auto strategy = this->strategy();
    return (strategy == v.toInt());
}

bool CRUDBody::isStrategyModify()
{
    auto strategy=this->strategy();
    if
        (
        strategy==QOrm::CRUDTypes::Custom
        ||
        strategy==QOrm::CRUDTypes::Upsert
        ||
        strategy==QOrm::CRUDTypes::Remove
        )
        return true;

    return false;
}

QVariant CRUDBody::resultInfo() const
{
    return this->value(__resultInfo);
}

}
