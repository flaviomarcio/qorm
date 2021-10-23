#include "./p_qorm_model_crud.h"
#include "../qorm_query.h"
#include "../qorm_transaction_scope.h"
#include <QJsonDocument>

namespace PrivateQOrm {


#define dPvt()\
    auto&p = *reinterpret_cast<CRUDBasePvt*>(this->p)


class CRUDBasePvt{
public:
    QOrm::ModelDtoOptions options;
    QByteArray crudName;
    QByteArray crudDescription;
    QOrm::ModelDto dto;
    QHash<QByteArray, QOrm::ModelAction*> actions;
    QHash<QByteArray, QOrm::CRUDBodyActionMethod> actionMethod;
    QOrm::CRUDStrategy strategy=QOrm::Undefined;
    QVariant source;
    CRUDBase*parent=nullptr;

    explicit CRUDBasePvt(CRUDBase*parent):options(parent),dto(parent){
        this->parent=parent;
        dto.setType(dftNormalForm);
    }
    virtual ~CRUDBasePvt(){
    }
    auto&doModelAction(const QString&methodName){
        auto method=this->actionMethod.value(methodName.toUtf8());
        if(method==nullptr)
            return this->parent->lr();
        else
            return this->parent->lr(method(this->parent, this->source));
    }

    auto&actionNothing(QOrm::ObjectDb*crudController, const QVariant&vBody){
        Q_UNUSED(crudController)
        Q_UNUSED(vBody)
        return*this->parent;
    }

    void set_crud(const QVariant&crud){
        auto vCrud=CRUDBody(crud);
        auto vStrategy=[&vCrud](){
            QVariant v;
            if(vCrud.contains(qsl("method")))
                v=vCrud[qsl("method")];
            else if(vCrud.contains(qsl("strategy")))
                v=vCrud[qsl("strategy")];
            return v;
        };

        auto vSource=[&vCrud](){
            QVariant v;
            if(vCrud.contains(qsl("source")))
                v=vCrud[qsl("source")];
            return v;
        };
        this->strategy_set(vStrategy());
        this->source_set(vSource());
    }

    void source_set(const QVariant&source){
        if(source.type()==source.String || source.type()==source.ByteArray || source.type()==source.Char || source.type()==source.BitArray){
            auto vSource=QJsonDocument::fromJson(source.toByteArray()).toVariant();
            this->source=vSource;
        }
        else{
            this->source=source;
        }
    }

    void strategy_set(const QVariant&strategy){
        if(!strategy.isValid() || strategy.isNull()){
            this->strategy=QOrm::Undefined;
        }
        else{
            QVariant vFy=strategy;
            if(vFy.type()==vFy.String || vFy.type()==vFy.ByteArray || vFy.type()==vFy.Char || vFy.type()==vFy.BitArray){
                vFy=vFy.toString().toLower();
                vFy=QOrm::__stringToStrategy.value(vFy.toString());
            }
            vFy=QOrm::__listToStrategy.contains(vFy)?vFy:QOrm::Undefined;
            this->strategy=QOrm::CRUDStrategy(vFy.toInt());
        }
    }
};


CRUDBase::CRUDBase(QObject *parent) : QOrm::ObjectDb(parent)
{
    this->p = new CRUDBasePvt(this);
    dPvt();
    p.set_crud(CRUDBody());
}

CRUDBase::CRUDBase(const QVariant &vBody, QObject *parent):QOrm::ObjectDb(parent)
{
    this->p = new CRUDBasePvt(this);
    dPvt();
    p.set_crud(vBody);
}

CRUDBase::~CRUDBase(){
    dPvt();
    delete&p;
}

QOrm::ModelDtoOptions &CRUDBase::options()
{
    dPvt();
    return p.options;
}

CRUDBase &CRUDBase::setOptions(const QOrm::ModelDtoOptions &options)
{
    dPvt();
    p.options=options;
    return*this;
}

CRUDBase &CRUDBase::makeDefault()
{
    this->dto().headers().makeDefault();
    return *this;
}

CRUDBase &CRUDBase::unMakeDefault()
{
    this->dto().headers().unMakeDefault();
    return *this;
}

QVariant CRUDBase::type() const
{
    dPvt();
    QVariant type=p.dto.type();
    if(type.isNull() && !type.isValid()){
        type=this->defaultType();
    }
    return type;
}

CRUDBase &CRUDBase::type(const QVariant &value)
{
    dPvt();
    p.dto.setType(value);
    return*this;
}

QVariant CRUDBase::layout() const
{
    dPvt();
    QVariant type=p.dto.layout();
    if(type.isNull() && !type.isValid()){
        type=this->defaultLayout();
    }
    return type;
}

CRUDBase &CRUDBase::layout(const QVariant &value)
{
    dPvt();
    p.dto.setLayout(value);
    return*this;
}

QByteArray CRUDBase::crudName() const
{
    dPvt();
    if(p.crudName.trimmed().isEmpty())
        return this->metaObject()->className();
    else
        return p.crudName;
}

CRUDBase &CRUDBase::crudName(const QVariant &value)
{
    dPvt();
    p.crudName=value.toByteArray().trimmed();
    return*this;
}

QByteArray CRUDBase::crudDescription() const
{
    dPvt();
    if(p.crudDescription.trimmed().isEmpty())
        return this->metaObject()->className();
    else
        return p.crudDescription;
}

CRUDBase &CRUDBase::crudDescription(const QVariant &value)
{
    dPvt();
    p.crudDescription=value.toByteArray().trimmed();
    return*this;
}

QOrm::ModelDto &CRUDBase::dto()
{
    dPvt();
    return p.dto;
}

const QOrm::ModelInfo &CRUDBase::modelInfo()
{
    static const QOrm::ModelInfo __return;
    return __return;
}

CRUDBase&CRUDBase::crudBody(const QVariant&v)
{
    dPvt();
    p.set_crud(v);
    return*this;
}

QOrm::CRUDStrategy&CRUDBase::strategy()const
{
    dPvt();
    return p.strategy;
}

CRUDBase &CRUDBase::strategy(const QVariant &strategy)
{
    dPvt();
    p.strategy_set(strategy);
    return*this;
}

QVariant CRUDBase::source() const{
    dPvt();
    return p.source;
}

CRUDBase &CRUDBase::source(const QVariant &value)
{
    dPvt();
    p.source=value;
    return*this;
}

ResultValue &CRUDBase::crudify()
{
    //QOrm::TransactionScope transaction(this);
    auto strategy=this->strategy();
    if(strategy==QOrm::Search)
        return this->canActionSearch();
    else if(strategy==QOrm::Insert)
        return this->canActionInsert();
    else if(strategy==QOrm::Update)
        return this->canActionUpdate();
    else if(strategy==QOrm::Upsert)
        return this->canActionUpsert();
    else if(strategy==QOrm::Remove)
        return this->canActionRemove();
    else if(strategy==QOrm::Deactivate)
        return this->canActionDeactivate();
    else
        return this->lr().setValidation(tr("Invalid strategy"));
}

CRUDBase &CRUDBase::actionSearch(QOrm::ModelAction &action)
{
    dPvt();
    p.actions[__func__]=&action;
    return*this;
}

CRUDBase &CRUDBase::actionInsert(QOrm::ModelAction &action)
{
    dPvt();
    p.actions[__func__]=&action;
    return*this;
}

CRUDBase &CRUDBase::actionUpsert(QOrm::ModelAction &action)
{
    dPvt();
    p.actions[__func__]=&action;
    return*this;
}

CRUDBase &CRUDBase::actionUpdate(QOrm::ModelAction &action)
{
    dPvt();
    p.actions[__func__]=&action;
    return*this;
}

CRUDBase &CRUDBase::actionRemove(QOrm::ModelAction &action)
{
    dPvt();
    p.actions[__func__]=&action;
    return*this;
}

CRUDBase &CRUDBase::actionDeactivate(QOrm::ModelAction &action)
{
    dPvt();
    p.actions[__func__]=&action;
    return*this;
}

ResultValue &CRUDBase::search(){
    return this->lr().setNotImplemented();
}

ResultValue &CRUDBase::search(const QVariant &value){
    Q_UNUSED(value)
    return this->lr().setNotImplemented();
}

ResultValue &CRUDBase::insert(){
    return this->lr().setNotImplemented();
}

ResultValue &CRUDBase::insert(const QVariant &value){
    Q_UNUSED(value)
    return this->lr().setNotImplemented();
}

ResultValue &CRUDBase::update(){
    return this->lr().setNotImplemented();
}

ResultValue &CRUDBase::update(const QVariant &value){
    Q_UNUSED(value)
    return this->lr().setNotImplemented();
}

ResultValue &CRUDBase::upsert(){
    return this->lr().setNotImplemented();
}

ResultValue &CRUDBase::upsert(const QVariant &value){
    Q_UNUSED(value)
    return this->lr().setNotImplemented();
}

ResultValue &CRUDBase::remove(){
    return this->lr().setNotImplemented();
}

ResultValue &CRUDBase::remove(const QVariant &value){
    Q_UNUSED(value)
    return this->lr().setNotImplemented();
}

ResultValue &CRUDBase::deactivate()
{
    return this->lr().setNotImplemented();
}

ResultValue &CRUDBase::deactivate(const QVariant &value)
{
    Q_UNUSED(value)
    return this->lr().setNotImplemented();
}

CRUDBase &CRUDBase::onBefore(QOrm::CRUDBodyActionMethod method)
{
    dPvt();
    p.actionMethod[qbl("bofore")]=method;
    return*this;
}

CRUDBase &CRUDBase::onSuccess(QOrm::CRUDBodyActionMethod method)
{
    dPvt();
    p.actionMethod[qbl("success")]=method;
    return*this;
}

CRUDBase &CRUDBase::onFailed(QOrm::CRUDBodyActionMethod method)
{
    dPvt();
    p.actionMethod[qbl("failed")]=method;
    return*this;
}

ResultValue &CRUDBase::canActionSearch()
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
    return this->lr(p.dto.id(this->crudName()).text(this->crudDescription()).items(v).o());

}

ResultValue &CRUDBase::canActionInsert()
{
    dPvt();
    static auto name=QByteArray(__func__).replace(qbl("canAction"), qbl("action"));
    auto&act=p.actions[name];
    auto&lr=(act==nullptr)?this->insert():act->action(this->source());
    return this->lr(lr);
}

ResultValue &CRUDBase::canActionUpsert()
{
    dPvt();
    static auto name=QByteArray(__func__).replace(qbl("canAction"), qbl("action"));
    auto&act=p.actions[name];
    auto&lr=(act==nullptr)?this->upsert():act->action(this->source());
    return this->lr(lr);
}

ResultValue &CRUDBase::canActionUpdate()
{
    dPvt();
    static auto name=QByteArray(__func__).replace(qbl("canAction"), qbl("action"));
    auto&act=p.actions[name];
    auto&lr=(act==nullptr)?this->update():act->action(this->source());
    return this->lr(lr);
}

ResultValue &CRUDBase::canActionRemove()
{
    dPvt();
    static auto name=QByteArray(__func__).replace(qbl("canAction"), qbl("action"));
    auto&act=p.actions[name];
    auto&lr=(act==nullptr)?this->remove():act->action(this->source());
    return this->lr(lr);
}

ResultValue &CRUDBase::canActionDeactivate()
{
    dPvt();
    static auto name=QByteArray(__func__).replace(qbl("canAction"), qbl("action"));
    auto&act=p.actions[name];
    auto&lr=(act==nullptr)?this->deactivate():act->action(this->source());
    return this->lr(lr);
}

ResultValue &CRUDBase::doBofore()
{
    dPvt();
    return p.doModelAction(tr("bofore"));
}

ResultValue &CRUDBase::doSuccess()
{
    dPvt();
    return p.doModelAction(tr("success"));
}

ResultValue &CRUDBase::doFailed()
{
    dPvt();
    return p.doModelAction(tr("failed"));
}


}
