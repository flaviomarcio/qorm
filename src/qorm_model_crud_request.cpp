#include "./qorm_model_crud_request.h"
#include "../../qstm/src/qstm_util_variant.h"
#include <QJsonDocument>

namespace QOrm {


CRUDRequest::CRUDRequest(const QVariant &other) : QVariantHash{other.toHash()}
{

}

CRUDRequest::~CRUDRequest()
{

}

CRUDRequest &CRUDRequest::setBody(const QVariant &v)
{
    auto h = v.toHash();
    const auto &__path = h[QStringLiteral("path")].toString().trimmed().toLower();
    const auto &__header = h[QStringLiteral("header")].toHash();
    const auto &__method = h[QStringLiteral("method")].toString().trimmed().toLower();
    const auto &__source = h[QStringLiteral("source")];

    CRUDRequest __return;
    __return[QStringLiteral("path")] = __path;
    __return[QStringLiteral("header")] = __header;
    __return[QStringLiteral("method")] = __method;
    __return[QStringLiteral("source")] = __source;
    return *this;
}

CRUDRequest CRUDRequest::fromMap(const QVariant &v)
{
    CRUDRequest r;
    return r.setBody(v);
}

CRUDRequest CRUDRequest::fromRequest(const QVariant &v)
{
    auto h = v.toHash();
    const auto &__path = h[QStringLiteral("requestPath")].toString().trimmed().toLower();
    const auto &__header = h[QStringLiteral("requestHeader")].toHash();
    const auto &__method = h[QStringLiteral("requestMethod")].toString().trimmed().toLower();
    const auto &__body = h[QStringLiteral("requestBody")];
    const auto &__parameter = h[QStringLiteral("requestParameter")].toHash();

    CRUDRequest __return;
    __return[QStringLiteral("header")] = __header;
    __return[QStringLiteral("path")] = __path;
    __return[QStringLiteral("method")] = __method;
    if (!__parameter.isEmpty())
        __return[QStringLiteral("source")] = __parameter;
    else
        __return[QStringLiteral("source")] = __body;
    return __return;
}

const QVariantHash CRUDRequest::header()
{
    auto &v = *dynamic_cast<QVariantHash *>(this);
    return v[__func__].toHash();
}

const QString CRUDRequest::path()
{
    auto &v = *dynamic_cast<QVariantHash *>(this);
    return v[__func__].toString();
}

const QString CRUDRequest::method()
{
    auto &v = *dynamic_cast<QVariantHash *>(this);
    return v[__func__].toString();
}

const QVariant CRUDRequest::source()
{
    const auto &v = *dynamic_cast<QVariantHash *>(this);
    return v[__func__];
}

CRUDStrategy CRUDRequest::strategy()
{
    auto strategy = this->method().trimmed();
    if (!strategy.isEmpty()) {
        return QOrm::Undefined;
    }

    QVariant vFy = strategy;
    if (QMetaTypeUtilString.contains(vFy.typeId())) {
        vFy = vFy.toString().toLower();
        vFy = __stringToStrategy.value(vFy.toString());
    }
    vFy = __listToStrategy.contains(vFy) ? vFy : QOrm::Undefined;
    return QOrm::CRUDStrategy(vFy.toInt());
}

bool CRUDRequest::isStrategy(const CRUDStrategy v)
{
    auto s = this->strategy();
    return s == v;
}

} // namespace QOrm
