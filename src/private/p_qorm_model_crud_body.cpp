#include "./p_qorm_model_crud_body.h"

static auto __expression="expression";
static auto __source="source";

CRUDBody::CRUDBody(const QVariant &other) : QVariantHash(other.toHash()) {}

CRUDBody::CRUDBody(const QVariant &strategy, const QVariant &source)
    : QVariantHash{{QStringLiteral("strategy"), strategy}, {QStringLiteral("source"), source}}
{}

CRUDBody::CRUDBody(const QOrm::CRUDStrategy strategy, const QVariant &source)
    : QVariantHash{{QStringLiteral("strategy"), strategy}, {QStringLiteral("source"), source}}
{}

QOrm::CRUDStrategy CRUDBody::strategy() const
{
    static auto varName = QStringList{__func__, QStringLiteral("method")};
    auto &vHash = *this;
    for (auto &vName : varName) {
        const QVariant &v = vHash[vName];
        if (v.isNull() || !v.isValid())
            continue;
        auto name = v.toString().toLower();
        if (!QOrm::__stringToStrategy.contains(name))
            continue;

        auto vStrategy = QOrm::__stringToStrategy.value(name);
        auto tStrategy = QOrm::CRUDStrategy(vStrategy.toUInt());
        return tStrategy;
    }
    return QOrm::CRUDStrategy::Undefined;
}

const QVariant CRUDBody::expression() const
{
    auto v=this->value(__source);
    if(!v.isValid())
        return this->value(__expression);
    auto vHash=v.toHash();
    return vHash.value(__expression);
}


const QVariant CRUDBody::source() const
{
    return (*this)[__func__];
}

const QVariant CRUDBody::items() const
{
    static auto __source="source";
    auto v=this->contains(__source)?this->value(__source):*this;
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

bool CRUDBody::isStrategy(const QVariant &v) const
{
    const qlonglong strategy = this->strategy();
    return (strategy == v.toInt());
}
