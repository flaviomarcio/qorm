#include "./p_qorm_model_report.h"
#include "../qorm_query.h"
#include "../qorm_transaction_scope.h"
#include <QJsonDocument>

namespace PrivateQOrm {


#define dPvt()\
    auto &p = *reinterpret_cast<ModelReportBasePvt*>(this->p)


class ModelReportBasePvt{
public:
    QOrm::ModelDtoOptions options;
    QByteArray reportName;
    QByteArray reportDescription;
    QOrm::ModelDto dto;
    QHash<QByteArray, QOrm::ModelAction*> actions;
    QHash<QByteArray, QOrm::ModelActionMethod> actionMethod;
    QVariant strategy;
    QVariant source;
    ModelReportBase*parent=nullptr;

    explicit ModelReportBasePvt(ModelReportBase*parent):options(parent),dto(parent)
    {
        this->parent=parent;
    }
    virtual ~ModelReportBasePvt(){
    }
    auto &doModelAction(const QString &methodName)
    {
        auto method=this->actionMethod.value(methodName.toUtf8());
        if(method==nullptr)
            return this->parent->lr();
        return this->parent->lr(method(this->parent, this->source));
    }

    auto &actionNothing(QOrm::ObjectDb*controller, const QVariant &vBody)
    {
        Q_UNUSED(controller)
        Q_UNUSED(vBody)
        return*this->parent;
    }

    void set_report(const QVariant &v)
    {
        auto vObject=ReportBody{v};
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

    void source_set(const QVariant&source)
    {
        switch (qTypeId(source)) {
        case QMetaType_QString:
            this->source=QJsonDocument::fromJson(source.toByteArray()).toVariant();
            return;
        case QMetaType_QByteArray:
            this->source=QJsonDocument::fromJson(source.toByteArray()).toVariant();
            return;
        default:
            this->source=source;
        }
    }

    void strategy_set(const QVariant&strategy)
    {
        this->strategy=strategy;
    }
};


ModelReportBase::ModelReportBase(QObject *parent) : QOrm::ObjectDb(parent)
{
    this->p = new ModelReportBasePvt{this};

    p->set_report(ReportBody{});
}

ModelReportBase::ModelReportBase(const QVariant &reportBody, QObject *parent):QOrm::ObjectDb(parent)
{
    this->p = new ModelReportBasePvt{this};

    p->set_report(reportBody);
}

ModelReportBase::~ModelReportBase()
{
    delete p;
}

QOrm::ModelDtoOptions &ModelReportBase::options()
{

    return p->options;
}

ModelReportBase &ModelReportBase::setOptions(const QOrm::ModelDtoOptions &options)
{

    p->options=options;
    return*this;
}

ModelReportBase::FormType ModelReportBase::type() const
{

    return FormType(p->dto.type());
}

ModelReportBase &ModelReportBase::type(const FormType &value)
{

    p->dto.setType(QOrm::ModelDto::FormType(value));
    return*this;
}

QOrm::ModelDto::FormLayout ModelReportBase::layout() const
{

    return QOrm::ModelDto::FormLayout(p->dto.layout());
}

ModelReportBase &ModelReportBase::layout(const FormLayout &value)
{

    p->dto.setLayout(QOrm::ModelDto::FormLayout(value));
    return*this;
}

QByteArray ModelReportBase::reportName() const
{

    if(p->reportName.trimmed().isEmpty())
        return this->metaObject()->className();
    return p->reportName;
}

ModelReportBase &ModelReportBase::reportName(const QVariant &value)
{

    p->reportName=value.toByteArray().trimmed();
    return*this;
}

QByteArray ModelReportBase::reportDescription() const
{

    if(p->reportDescription.trimmed().isEmpty())
        return this->metaObject()->className();
    return p->reportDescription;
}

ModelReportBase &ModelReportBase::reportDescription(const QVariant &value)
{

    p->reportDescription=value.toByteArray().trimmed();
    return*this;
}

QOrm::ModelDto &ModelReportBase::dto()
{

    return p->dto;
}

const QOrm::ModelInfo &ModelReportBase::modelInfo()
{
    static const QOrm::ModelInfo __return;
    return __return;
}

ModelReportBase&ModelReportBase::reportBody(const QVariant &v)
{

    p->set_report(v);
    return*this;
}

QVariant ModelReportBase::strategy()const
{

    return p->strategy;
}

ModelReportBase &ModelReportBase::strategy(const QVariant &strategy)
{

    p->strategy_set(strategy);
    return*this;
}

QVariant ModelReportBase::source() const{

    return p->source;
}

ModelReportBase &ModelReportBase::source(const QVariant &value)
{

    p->source=value;
    return*this;
}

ResultValue &ModelReportBase::reportfy()
{
    return this->canActionSearch();
}

ModelReportBase &ModelReportBase::actionSearch(QOrm::ModelAction &action)
{

    p->actions[__func__]=&action;
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

ModelReportBase &ModelReportBase::onBefore(QOrm::ModelActionMethod method)
{

    p->actionMethod[qbl("bofore")]=method;
    return*this;
}

ModelReportBase &ModelReportBase::onSuccess(QOrm::ModelActionMethod method)
{

    p->actionMethod[qbl("success")]=method;
    return*this;
}

ModelReportBase &ModelReportBase::onFailed(QOrm::ModelActionMethod method)
{

    p->actionMethod[qbl("failed")]=method;
    return*this;
}

ResultValue &ModelReportBase::canActionSearch()
{
    VariantUtil vu;

    static auto name=QByteArray{__func__}.replace(qbl("canAction"), qbl("action"));
    QVariant v;
    if(this->options().searchOnEmptyFilter() || !vu.vIsEmpty(this->source())){
        auto &act=p->actions[name];
        auto &lr=(act==nullptr)?this->search():act->action(this->source());\
        v=lr.resultVariant();
    }
    return this->lr(p->dto
                    .uuid(this->reportName())
                    .name(this->reportName())
                    .text(this->reportDescription())
                    .items(v)
                    .o()
                    );

}

ResultValue &ModelReportBase::doBofore()
{

    return p->doModelAction(qsl("bofore"));
}

ResultValue &ModelReportBase::doSuccess()
{

    return p->doModelAction(qsl("success"));
}

ResultValue &ModelReportBase::doFailed()
{

    return p->doModelAction(qsl("failed"));
}


}
