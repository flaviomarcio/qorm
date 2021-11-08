#include "./p_qorm_model_report.h"
#include "../qorm_query.h"
#include "../qorm_transaction_scope.h"
#include <QJsonDocument>

namespace PrivateQOrm {


#define dPvt()\
    auto&p = *reinterpret_cast<ModelReportBasePvt*>(this->p)


class ModelReportBasePvt{
public:
    QOrm::ModelDtoOptions options;
    QByteArray reportName;
    QByteArray reportDescription;
    QOrm::ModelDto dto;
    QHash<QByteArray, QOrm::ModelAction*> actions;
    QHash<QByteArray, QOrm::ReportActionMethod> actionMethod;
    QVariant strategy;
    QVariant source;
    ModelReportBase*parent=nullptr;

    explicit ModelReportBasePvt(ModelReportBase*parent):options(parent),dto(parent){
        this->parent=parent;
        dto.setType(dftNormalForm);
    }
    virtual ~ModelReportBasePvt(){
    }
    auto&doModelAction(const QString&methodName){
        auto method=this->actionMethod.value(methodName.toUtf8());
        if(method==nullptr)
            return this->parent->lr();
        else
            return this->parent->lr(method(this->parent, this->source));
    }

    auto&actionNothing(QOrm::ObjectDb*controller, const QVariant&vBody){
        Q_UNUSED(controller)
        Q_UNUSED(vBody)
        return*this->parent;
    }

    void set_report(const QVariant&v){
        auto vObject=ReportBody(v);
        auto vStrategy=[&vObject](){
            QVariant v;
            if(vObject.contains(qsl("method")))
                v=vObject[qsl("method")];
            else if(vObject.contains(qsl("strategy")))
                v=vObject[qsl("strategy")];
            return v;
        };

        auto vSource=[&vObject](){
            QVariant v;
            if(vObject.contains(qsl("source")))
                v=vObject[qsl("source")];
            return v;
        };
        this->strategy_set(vStrategy());
        this->source_set(vSource());
    }

    void source_set(const QVariant&source){
        if(source.typeId()==source.String || source.typeId()==source.ByteArray || source.typeId()==source.Char || source.typeId()==source.BitArray){
            auto vSource=QJsonDocument::fromJson(source.toByteArray()).toVariant();
            this->source=vSource;
        }
        else{
            this->source=source;
        }
    }

    void strategy_set(const QVariant&strategy){
        this->strategy=strategy;
    }
};


ModelReportBase::ModelReportBase(QObject *parent) : QOrm::ObjectDb(parent)
{
    this->p = new ModelReportBasePvt(this);
    dPvt();
    p.set_report(ReportBody());
}

ModelReportBase::ModelReportBase(const QVariant &reportBody, QObject *parent):QOrm::ObjectDb(parent)
{
    this->p = new ModelReportBasePvt(this);
    dPvt();
    p.set_report(reportBody);
}

ModelReportBase::~ModelReportBase(){
    dPvt();
    delete&p;
}

QOrm::ModelDtoOptions &ModelReportBase::options()
{
    dPvt();
    return p.options;
}

ModelReportBase &ModelReportBase::setOptions(const QOrm::ModelDtoOptions &options)
{
    dPvt();
    p.options=options;
    return*this;
}

ModelReportBase &ModelReportBase::makeDefault()
{
    this->dto().headers().makeDefault();
    return *this;
}

ModelReportBase &ModelReportBase::unMakeDefault()
{
    this->dto().headers().unMakeDefault();
    return *this;
}

QVariant ModelReportBase::type() const
{
    dPvt();
    QVariant type=p.dto.type();
    if(type.isNull() && !type.isValid()){
        type=this->defaultType();
    }
    return type;
}

ModelReportBase &ModelReportBase::type(const QVariant &value)
{
    dPvt();
    p.dto.setType(value);
    return*this;
}

QVariant ModelReportBase::layout() const
{
    dPvt();
    QVariant type=p.dto.layout();
    if(type.isNull() && !type.isValid()){
        type=this->defaultLayout();
    }
    return type;
}

ModelReportBase &ModelReportBase::layout(const QVariant &value)
{
    dPvt();
    p.dto.setLayout(value);
    return*this;
}

QByteArray ModelReportBase::reportName() const
{
    dPvt();
    if(p.reportName.trimmed().isEmpty())
        return this->metaObject()->className();
    else
        return p.reportName;
}

ModelReportBase &ModelReportBase::reportName(const QVariant &value)
{
    dPvt();
    p.reportName=value.toByteArray().trimmed();
    return*this;
}

QByteArray ModelReportBase::reportDescription() const
{
    dPvt();
    if(p.reportDescription.trimmed().isEmpty())
        return this->metaObject()->className();
    else
        return p.reportDescription;
}

ModelReportBase &ModelReportBase::reportDescription(const QVariant &value)
{
    dPvt();
    p.reportDescription=value.toByteArray().trimmed();
    return*this;
}

QOrm::ModelDto &ModelReportBase::dto()
{
    dPvt();
    return p.dto;
}

const QOrm::ModelInfo &ModelReportBase::modelInfo()
{
    static const QOrm::ModelInfo __return;
    return __return;
}

ModelReportBase&ModelReportBase::reportBody(const QVariant&v)
{
    dPvt();
    p.set_report(v);
    return*this;
}

QVariant ModelReportBase::strategy()const
{
    dPvt();
    return p.strategy;
}

ModelReportBase &ModelReportBase::strategy(const QVariant &strategy)
{
    dPvt();
    p.strategy_set(strategy);
    return*this;
}

QVariant ModelReportBase::source() const{
    dPvt();
    return p.source;
}

ModelReportBase &ModelReportBase::source(const QVariant &value)
{
    dPvt();
    p.source=value;
    return*this;
}

ResultValue &ModelReportBase::reportfy()
{
    return this->canActionSearch();
}

ModelReportBase &ModelReportBase::actionSearch(QOrm::ModelAction &action)
{
    dPvt();
    p.actions[__func__]=&action;
    return*this;
}

ResultValue &ModelReportBase::search(){
    return this->lr().setNotImplemented();
}

ResultValue &ModelReportBase::search(const QVariant &value)
{
    Q_UNUSED(value)
    return this->lr().setNotImplemented();
}

ModelReportBase &ModelReportBase::onBefore(QOrm::ReportActionMethod method)
{
    dPvt();
    p.actionMethod[qbl("bofore")]=method;
    return*this;
}

ModelReportBase &ModelReportBase::onSuccess(QOrm::ReportActionMethod method)
{
    dPvt();
    p.actionMethod[qbl("success")]=method;
    return*this;
}

ModelReportBase &ModelReportBase::onFailed(QOrm::ReportActionMethod method)
{
    dPvt();
    p.actionMethod[qbl("failed")]=method;
    return*this;
}

ResultValue &ModelReportBase::canActionSearch()
{
    VariantUtil vu;
    dPvt();
    static auto name=QByteArray(__func__).replace(qbl("canAction"), qbl("action"));
    QVariant v;
    if(this->options().searchOnEmptyFilter() || !vu.vIsEmpty(this->source())){
        auto&act=p.actions[name];
        auto&lr=(act==nullptr)?this->search():act->action(this->source());\
        v=lr.resultVariant();
    }
    return this->lr(p.dto.id(this->reportName()).text(this->reportDescription()).items(v).o());

}

ResultValue &ModelReportBase::doBofore()
{
    dPvt();
    return p.doModelAction(tr("bofore"));
}

ResultValue &ModelReportBase::doSuccess()
{
    dPvt();
    return p.doModelAction(tr("success"));
}

ResultValue &ModelReportBase::doFailed()
{
    dPvt();
    return p.doModelAction(tr("failed"));
}


}
