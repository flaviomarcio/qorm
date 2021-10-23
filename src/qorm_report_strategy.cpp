#include "./qorm_report_strategy.h"

namespace QOrm {

#define dPvt()\
    auto&p = *reinterpret_cast<ReportStrategyPrv*>(this->p)

class ReportStrategyPrv{
public:
    QString name;
    QVariantHash cache;
    QVariantHash connection;
    QVariantHash header;
    QVariantHash filter;
    QVariantHash chart;
    QVariant source;
    QVariant data;
    ReportStrategy*parent=nullptr;
    explicit ReportStrategyPrv(ReportStrategy*parent){
        this->parent=parent;
    }
    virtual ~ReportStrategyPrv(){
    }

    QVariantHash strategy(){
        QVariantHash map={{"name", name}, {"connection", connection}, {"header", header}, {"filter", filter}, {"source", source}, {"data", data}};
        return map;
    }

    void setStrategy(const QVariantHash&map){
        this->cache=map.value("cache").toHash();
        this->connection=map.value("connection").toHash();
        this->header=map.value("header").toHash();
        this->filter=map.value("filter").toHash();
        this->source=map.value("source");
        this->data=map.value("data");
    }

    ResultValue &isValid(){

        if(!this->header.isEmpty())
            return this->parent->lr()=false;
        else
            return this->parent->lr();
    }
};

ReportStrategy::ReportStrategy(QObject *parent) : QStm::Object(parent)
{
    this->p = new ReportStrategyPrv(this);
}

ReportStrategy::ReportStrategy(const QVariantHash &strategy, QObject *parent):QStm::Object(parent)
{
    this->p = new ReportStrategyPrv(this);
    dPvt();
    p.setStrategy(strategy);
}

ReportStrategy::~ReportStrategy()
{
    dPvt();
    delete&p;
}

ResultValue &ReportStrategy::isValid()
{
    dPvt();
    return p.isValid();
}

QVariantHash ReportStrategy::strategy() const
{
    dPvt();
    return p.strategy();
}

ReportStrategy&ReportStrategy::setStrategy(const QVariant &value)
{
    dPvt();
    p.setStrategy(value.toHash());
    return*this;
}

QString ReportStrategy::name() const
{
    dPvt();
    return p.name;
}

ReportStrategy &ReportStrategy::setName(const QString &value)
{
    dPvt();
    p.name=value;
    return*this;
}

QVariantHash ReportStrategy::cache() const
{
    dPvt();
    return p.cache;
}

ReportStrategy&ReportStrategy::setCache(const QVariantHash &value)
{
    dPvt();
    p.cache=value;
    return*this;
}

QVariantHash ReportStrategy::connection() const
{
    dPvt();
    return p.connection;
}

ReportStrategy&ReportStrategy::setConnection(const QVariantHash &value)
{
    dPvt();
    p.connection = value;
    return*this;
}

QVariantHash ReportStrategy::header() const
{
    dPvt();
    return p.header;
}

ReportStrategy&ReportStrategy::setHeader(const QVariantHash &value)
{
    dPvt();
    p.header = value;
    return*this;
}

QVariantHash ReportStrategy::filter() const
{
    dPvt();
    return p.filter;
}

ReportStrategy&ReportStrategy::setFilter(const QVariantHash &value)
{
    dPvt();
    p.filter = value;
    return*this;
}

QVariantHash ReportStrategy::chart() const
{
    dPvt();
    return p.chart;
}

ReportStrategy &ReportStrategy::setChart(const QVariantHash &value)
{
    dPvt();
    p.chart = value;
    return*this;
}

QVariant ReportStrategy::source() const
{
    dPvt();
    return p.source;
}

ReportStrategy &ReportStrategy::setSource(const QVariant &value)
{
    dPvt();
    p.source = value;
    return*this;
}

QVariant ReportStrategy::data() const
{
    dPvt();
    return p.source;
}

ReportStrategy&ReportStrategy::setData(const QVariant &value)
{
    dPvt();
    p.data = value;
    return*this;
}

}
