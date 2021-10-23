#include "./qorm_report_item.h"

namespace QOrm {

#define dPvt()\
    auto&p = *reinterpret_cast<ReportItemPrv*>(this->p)

class ReportItemPrv{
public:
    QString name;
    QStringList route;
    QStringList strategyRunner;
    QVariantList strategyPublic;
    QVariantHash strategyList;
    QVariantHash sourceList;

    ReportItem*parent=nullptr;
    explicit ReportItemPrv(ReportItem*parent){
        this->parent=parent;
    }
    virtual ~ReportItemPrv(){
    }


    ResultValue &isValid(){
        return this->parent->lr();
    }


    void setBody(const QVariant&v){
        QVariantHash vMap;
        QHashIterator<QString, QVariant> i(v.toHash());
        while (i.hasNext()) {
            i.next();
            auto k=i.key().toLower().replace(qsl("-"),qsl_null);
            auto v=i.value();
            vMap.insert(k, v);
        }

        this->name=vMap.value(qsl("name")).toString();
        this->route=vMap.value(qsl("route")).toStringList();
        this->strategyRunner=vMap.value(qsl("strategyrunner")).toStringList();
        this->strategyPublic=vMap.value(qsl("strategypublic")).toList();
        this->strategyList=vMap.value(qsl("strategylist")).toHash();
        this->sourceList=vMap.value(qsl("sourcelist")).toHash();

    }
};

ReportItem::ReportItem(QObject *parent) : QStm::Object(parent)
{
    this->p = new ReportItemPrv(this);
}

ReportItem::ReportItem(const QVariant &itemBody, QObject *parent):QStm::Object(parent)
{
    this->p = new ReportItemPrv(this);
    dPvt();
    p.setBody(itemBody);
}

ReportItem::~ReportItem()
{
    dPvt();
    delete&p;
}

ReportItem &ReportItem::operator=(const QVariant &v)
{
    dPvt();
    p.setBody(v);
    return*this;
}

ResultValue &ReportItem::isValid()
{
    dPvt();
    return p.isValid();
}

QString ReportItem::name() const
{
    dPvt();
    return p.name;
}

ReportItem &ReportItem::setName(const QString &value)
{
    dPvt();
    p.name=value;
    return*this;
}

QStringList ReportItem::route() const
{
    dPvt();
    return p.route;
}

ReportItem&ReportItem::setRoute(const QVariant &value)
{
    dPvt();
    auto&vList=p.route;
    vList.clear();
    for(auto&v:value.toStringList())
        vList<<v.toLower();
    return*this;
}

QStringList ReportItem::strategyRunner() const
{
    dPvt();
    return p.strategyRunner;
}

ReportItem&ReportItem::setStrategyRunner(const QVariant &value)
{
    dPvt();
    auto&vList=p.strategyRunner;
    vList.clear();
    for(auto&v:value.toStringList())
        vList<<v.toLower();
    return*this;
}

QVariantList ReportItem::strategyPublic() const
{
    dPvt();
    return p.strategyPublic;
}

ReportItem&ReportItem::setStrategyPublic(const QVariant &value)
{
    dPvt();
    p.strategyPublic = value.toList();
    return*this;
}

QVariantHash ReportItem::strategyList() const
{
    dPvt();
    return p.strategyList;
}

ReportItem&ReportItem::setStrategyList(const QVariantHash &value)
{
    dPvt();
    auto&vList=p.strategyList;
    vList.clear();
    QHashIterator<QString, QVariant> i(value);
    while (i.hasNext()) {
        i.next();
        vList.insert(i.key().toLower(), i.value());
    }
    return*this;
}

QVariantHash ReportItem::sourceList() const
{
    dPvt();
    return p.sourceList;
}

ReportItem&ReportItem::setSourceList(const QVariantHash &value)
{
    dPvt();
    auto&vList=p.sourceList;
    vList.clear();
    QHashIterator<QString, QVariant> i(value);
    while (i.hasNext()) {
        i.next();
        vList.insert(i.key().toLower(), i.value());
    }
    return*this;
}

}
