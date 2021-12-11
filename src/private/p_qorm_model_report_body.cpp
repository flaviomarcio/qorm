#include "./p_qorm_model_report_body.h"

ReportBody::ReportBody(const QVariant &other):QVariantHash(other.toHash())
{

}

ReportBody::ReportBody(const QVariant &strategy, const QVariant &source):QVariantHash{{qsl("strategy"), strategy}, {qsl("source"), source}}
{

}

ReportBody::~ReportBody()
{
}

const QVariant &ReportBody::strategy()
{
    return(*this)[__func__];
}

const QVariant &ReportBody::source()
{
    return(*this)[__func__];
}

bool ReportBody::isStrategy(const QVariant &v)
{
    const QVariant strategy=this->strategy();
    return (strategy==v);

}
