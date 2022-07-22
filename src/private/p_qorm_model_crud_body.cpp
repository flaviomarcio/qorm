#include "./p_qorm_model_crud_body.h"

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

const QVariant CRUDBody::source() const
{
    return (*this)[__func__];
}

const QVariant CRUDBody::items() const
{
    auto v=this->contains("source")?this->value("source"):*this;
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
        if(vHash.contains("source"))
            v=vHash.value("source");
    }
    return vList;
}

bool CRUDBody::isStrategy(const QVariant &v) const
{
    const qlonglong strategy = this->strategy();
    return (strategy == v.toInt());
}
