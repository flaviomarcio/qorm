#include "./p_qorm_model_report.h"
#include "./p_qorm_model_report_body.h"
#include <QJsonDocument>

namespace PrivateQOrm {

class ModelReportBasePvt:public QObject{
public:
    QOrm::ModelDtoOptions options;
    QUuid uuid;
    QByteArray name;
    QByteArray description;
    QOrm::ModelDto dto;
    QHash<QByteArray, QOrm::ModelAction*> actions;
    QHash<QByteArray, QOrm::ModelActionMethod> actionMethod;
    QVariant strategy;
    QVariant source;
    ModelReportBase*parent=nullptr;

    explicit ModelReportBasePvt(ModelReportBase*parent):QObject{parent}, options{parent}, dto{parent}
    {
        this->parent=parent;
    }

    auto &doModelAction(const QString &methodName)
    {
        auto method=this->actionMethod.value(methodName.toUtf8());
        if(method==nullptr)
            return this->parent->lr();
        return this->parent->lr(method(this->parent, this->source));
    }

    auto &actionNothing(QOrm::ObjectDb *controller, const QVariant &vBody)
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
            if(vObject.contains(QStringLiteral("method")))
                v=vObject[QStringLiteral("method")];
            else if(vObject.contains(QStringLiteral("strategy")))
                v=vObject[QStringLiteral("strategy")];
            return v;
        };

        auto vSource=[&vObject](){
            QVariant v;
            if(vObject.contains(QStringLiteral("source")))
                v=vObject[QStringLiteral("source")];
            return v;
        };
        this->strategy_set(vStrategy());
        this->source_set(vSource());
    }

    void source_set(const QVariant &source)
    {
        switch (source.typeId()) {
        case QMetaType::QString:
            this->source=QJsonDocument::fromJson(source.toByteArray()).toVariant();
            return;
        case QMetaType::QByteArray:
            this->source=QJsonDocument::fromJson(source.toByteArray()).toVariant();
            return;
        default:
            this->source=source;
        }
    }

    void strategy_set(const QVariant &strategy)
    {
        this->strategy=strategy;
    }
};


ModelReportBase::ModelReportBase(QObject *parent) : QOrm::ObjectDb{parent}
{
    this->p = new ModelReportBasePvt{this};

    p->set_report(ReportBody{});
}

ModelReportBase::ModelReportBase(const QVariant &reportBody, QObject *parent):QOrm::ObjectDb{parent}
{
    this->p = new ModelReportBasePvt{this};

    p->set_report(reportBody);
}

QOrm::ModelDtoOptions &ModelReportBase::options()
{
    return p->options;
}

ModelReportBase &ModelReportBase::setOptions(const QOrm::ModelDtoOptions &options)
{
    p->options=options;
    return *this;
}

ModelReportBase::FormType ModelReportBase::type() const
{
    return FormType(p->dto.type());
}

ModelReportBase &ModelReportBase::type(const FormType &value)
{
    p->dto.setType(QOrm::ModelDto::FormType(value));
    return *this;
}

QUuid &ModelReportBase::uuid() const
{
    Q_DECLARE_VU;
    if(p->uuid.isNull() && !p->name.isEmpty())
        p->uuid=vu.toMd5Uuid(this->name());
    return p->uuid;
}

ModelReportBase &ModelReportBase::uuid(const QUuid &value)
{
    p->uuid=value;
    return *this;
}

QByteArray ModelReportBase::name() const
{
    if(p->name.trimmed().isEmpty())
        return this->metaObject()->className();
    return p->name;
}

ModelReportBase &ModelReportBase::name(const QVariant &value)
{
    p->name=value.toByteArray().trimmed();
    return *this;
}

QByteArray ModelReportBase::description() const
{
    if(p->description.trimmed().isEmpty())
        return this->metaObject()->className();
    return p->description;
}

ModelReportBase &ModelReportBase::description(const QVariant &value)
{
    p->description=value.toByteArray().trimmed();
    return *this;
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

ModelReportBase &ModelReportBase::reportBody(const QVariant &v)
{
    p->set_report(v);
    return *this;
}

QVariant ModelReportBase::strategy()const
{
    return p->strategy;
}

ModelReportBase &ModelReportBase::strategy(const QVariant &strategy)
{
    p->strategy_set(strategy);
    return *this;
}

QVariant ModelReportBase::source() const{

    return p->source;
}

ModelReportBase &ModelReportBase::source(const QVariant &value)
{
    p->source=value;
    return *this;
}

ResultValue &ModelReportBase::reportfy()
{
    return this->canActionSearch();
}

ModelReportBase &ModelReportBase::actionSearch(QOrm::ModelAction &action)
{
    p->actions[__func__]=&action;
    return *this;
}

ResultValue &ModelReportBase::search()
{
    return this->lr().setNotImplemented();
}

ResultValue &ModelReportBase::search(const QVariant &value)
{
    Q_UNUSED(value)
    return this->lr().setNotImplemented();
}

ModelReportBase &ModelReportBase::onBefore(QOrm::ModelActionMethod method)
{
    p->actionMethod[QByteArrayLiteral("bofore")]=method;
    return *this;
}

ModelReportBase &ModelReportBase::onSuccess(QOrm::ModelActionMethod method)
{
    p->actionMethod[QByteArrayLiteral("success")]=method;
    return *this;
}

ModelReportBase &ModelReportBase::onFailed(QOrm::ModelActionMethod method)
{
    p->actionMethod[QByteArrayLiteral("failed")]=method;
    return *this;
}

ResultValue &ModelReportBase::canActionSearch()
{
    VariantUtil vu;

    static auto name=QByteArray{__func__}.replace(QByteArrayLiteral("canAction"), QByteArrayLiteral("action"));
    QVariant v;
    if(this->options().searchOnEmptyFilter() || !vu.vIsEmpty(this->source())){
        auto &act=p->actions[name];
        auto &lr=(act==nullptr)?this->search():act->action(this->source());\
        v=lr.resultVariant();
    }
    return this->lr(p->dto
                    .name(this->uuid())
                    .name(this->name())
                    .text(this->description())
                    .items(v)
                    .o()
                    );

}

ResultValue &ModelReportBase::doBofore()
{
    return p->doModelAction(QStringLiteral("bofore"));
}

ResultValue &ModelReportBase::doSuccess()
{
    return p->doModelAction(QStringLiteral("success"));
}

ResultValue &ModelReportBase::doFailed()
{
    return p->doModelAction(QStringLiteral("failed"));
}


}
