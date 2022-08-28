#include "./p_qorm_model_crud.h"
#include "../qorm_model_dto.h"
#include <QJsonDocument>
#include "./p_qorm_model_crud_body.h"
#include "./p_qorm_sql_suitable_parser_item.h"
//#include "../qorm_query.h"
//#include "../qorm_transaction_scope.h"

namespace PrivateQOrm {

static const auto __success = "success";
static const auto __action = "action";
static const auto __source = "source";
static const auto __method = "method";
static const auto __strategy = "strategy";
static const auto __resuliInfo = "resultInfo";
static const auto __canAction = "canAction";
static const auto __before = "before";
static const auto __failed = "failed";

class CRUDBasePvt:public QObject{
public:
    QStm::MetaEnum<CRUDBase::FormType> type=CRUDBase::RegisterForm;
    QOrm::ModelDtoOptions options;    
    QOrm::Host host;
    QUuid uuid;
    QByteArray name;
    QByteArray description;
    ModelDao dao;
    QOrm::ModelDto dto;
    QHash<QByteArray, QOrm::ModelAction*> actions;
    QHash<QByteArray, QOrm::CRUDBodyActionMethod> actionMethod;
    QStm::MetaEnum<QOrm::CRUDTypes::Strategy> strategy=QOrm::CRUDTypes::Search;
    QVariant source;
    QVariantList generatedRecords;
    CRUDBase*parent=nullptr;

    explicit CRUDBasePvt(CRUDBase*parent):QObject{parent}, options{parent}, dao{parent}, dto{parent}
    {
        this->parent=parent;
    }

    QVariant parserSearch(const QVariant &v)
    {
        if(!v.isValid() || v.isNull())
            return {};

        QVariantList vList;
        switch (v.typeId()) {
        case QMetaType::QVariantList:
            vList=v.toList();
            break;
        case QMetaType::QVariantHash:
        case QMetaType::QVariantMap:
            vList.append(v);
            break;
        default:
            return v;
        }
        if(vList.isEmpty())
            return {};

        //this.parent->dao()

        //const auto &modelInfo = this->parent->modelInfo();
        for(auto&v:vList){
            auto vHash=v.toHash();
            QHashIterator <QString, QVariant> i(vHash);
            while(i.hasNext()){
                i.next();
                auto vA=QOrm::SqlParserItem::from(i.key());
                auto vB=QOrm::SqlParserItem::from(i.value());

                if(vA.isValue()){

                }

            }
        }

        return vList.size()==1?vList.first():vList;

    }

    auto &doModelAction(const QString &methodName)
    {
        auto method=this->actionMethod.value(methodName.toUtf8());
        if(method==nullptr)
            return this->parent->lr();
        return this->parent->lr(method(this->parent, this->source));
    }

    auto &actionNothing(QOrm::ObjectDb *crudController, const QVariant &vBody)
    {
        Q_UNUSED(crudController)
        Q_UNUSED(vBody)
        return*this->parent;
    }

    void set_crud(const QVariant &crud)
    {
        auto vCrud=CRUDBody(crud);
        auto vStrategy=[&vCrud](){
            QVariant v;
            if(vCrud.contains(__method))
                v=vCrud[__method];
            else if(vCrud.contains(__strategy))
                v=vCrud[__strategy];
            return v;
        };

        auto vSource=[&vCrud](){
            QVariant v;
            if(vCrud.contains(__source))
                v=vCrud[__source];
            return v;
        };
        this->strategySet(vStrategy());
        this->sourceSet(vSource());
        this->parent->lr().resultInfo().fromVar(vCrud.value(__resuliInfo));
    }

    void sourceSet(const QVariant &source)
    {
        switch (source.typeId()) {
        case QMetaType::QString:
        case QMetaType::QByteArray:
        {
            auto vSource=QJsonDocument::fromJson(source.toByteArray()).toVariant();
            this->source=vSource;
            break;
        }
        default:
            this->source=source;
        }
    }

    void strategySet(const QVariant &strategy)
    {
        this->strategy=strategy;
    }
};


CRUDBase::CRUDBase(QObject *parent) : QOrm::ObjectDb{parent}
{
    this->p = new CRUDBasePvt{this};

    p->set_crud(CRUDBody());
}

CRUDBase::CRUDBase(const QVariant &vBody, QObject *parent):QOrm::ObjectDb{parent}
{
    this->p = new CRUDBasePvt{this};

    p->set_crud(vBody);
}

bool CRUDBase::isValid()const
{
    auto descriptor=this->modelInfo().descritor();
    return (descriptor!=nullptr);
}

const QOrm::ModelInfo &CRUDBase::modelInfo() const
{
    static QOrm::ModelInfo __return;
    return __return;
}

ModelDao &CRUDBase::dao()
{
    return p->dao;
}

QOrm::Host &CRUDBase::host()
{
    return p->host;
}

const QVariant CRUDBase::type() const
{
    return p->type.name();
}

CRUDBase &CRUDBase::setType(const FormType &newType)
{
    if (p->type == newType)
        return *this;
    p->type = newType;
    emit typeChanged();
    return *this;
}

CRUDBase &CRUDBase::resetType()
{
    return setType({});
}

QOrm::ModelDtoOptions &CRUDBase::options()
{
    return p->options;
}

CRUDBase &CRUDBase::setOptions(const QOrm::ModelDtoOptions &options)
{
    p->options=options;
    return *this;
}

QStm::ResultInfo &CRUDBase::resultInfo()
{
    return this->lr().resultInfo();
}

CRUDBase &CRUDBase::setResultInfo(const QStm::ResultInfo &resultInfo)
{
    this->lr().resultInfo().fromHash(resultInfo.toHash());
    return *this;
}

CRUDBase &CRUDBase::clear()
{
    p->generatedRecords.clear();
    return *this;
}

CRUDBase &CRUDBase::clean()
{
    p->generatedRecords.clear();
    return *this;
}

const QUuid &CRUDBase::uuid()
{
    Q_DECLARE_VU;
    if(p->uuid.isNull())
        p->uuid=vu.toMd5Uuid(this->name());
    return p->uuid;
}

const QByteArray &CRUDBase::name()
{
    if(p->name.trimmed().isEmpty())
        p->name=this->metaObject()->className();
    return p->name;
}

CRUDBase &CRUDBase::name(const QVariant &value)
{
    p->name=value.toByteArray().trimmed();
    return *this;
}

QByteArray &CRUDBase::description() const
{
    if(p->description.trimmed().isEmpty())
        p->description=this->metaObject()->className();
    return p->description;
}

CRUDBase &CRUDBase::description(const QVariant &value)
{
    p->description=value.toByteArray().trimmed();
    return *this;
}

QOrm::ModelDto &CRUDBase::dto()
{
    return p->dto;
}

CRUDBase &CRUDBase::crudBody(const QVariant &v)
{
    p->set_crud(v);
    return *this;
}

QOrm::CRUDTypes::Strategy CRUDBase::strategy()const
{
    return p->strategy.type();
}

CRUDBase &CRUDBase::strategy(const QVariant &strategy)
{
    p->strategySet(strategy);
    return *this;
}

QVariant &CRUDBase::source() const
{
    switch (p->source.typeId()) {
    case QMetaType::QString:
    case QMetaType::QByteArray:
    case QMetaType::QChar:
    case QMetaType::QBitArray:{
        auto v=p->source.toString().trimmed();
        p->source=v.isEmpty()?QVariant{}:v;
        break;
    }
    case QMetaType::QVariantList:
    case QMetaType::QStringList:{
        auto v=p->source.toList();
        p->source=v.isEmpty()?QVariant{}:v;
        break;
    }
    case QMetaType::QVariantHash:
    case QMetaType::QVariantMap:{
        auto v=p->source.toHash();
        p->source=v.isEmpty()?QVariant{}:v;
        break;
    }
    default:
        break;
    }
    return p->source;
}

CRUDBase &CRUDBase::source(const QVariant &value)
{
    p->source=value;
    return *this;
}

QVariantList &CRUDBase::generatedRecords() const
{
    return p->generatedRecords;
}

ResultValue &CRUDBase::crudify()
{
    p->dto.setResultInfo(this->resultInfo());
    auto strategy=this->strategy();
    switch (strategy) {
    case QOrm::CRUDTypes::Create:
        return this->canActionCreate();
    case QOrm::CRUDTypes::Search:
        return this->canActionSearch();
    case QOrm::CRUDTypes::Insert:
        return this->canActionInsert();
    case QOrm::CRUDTypes::Update:
        return this->canActionUpdate();
    case QOrm::CRUDTypes::Upsert:
        return this->canActionUpsert();
    case QOrm::CRUDTypes::Remove:
        return this->canActionRemove();
    case QOrm::CRUDTypes::Deactivate:
        return this->canActionDeactivate();
    default:
        return this->lr().setValidation(tr("Invalid strategy"));
    }
}

CRUDBase &CRUDBase::actionsNulls()
{
    qDeleteAll(p->actions);
    p->actions.clear();
    return *this;
}

CRUDBase &CRUDBase::actionsSearch()
{
    static auto __actionSearch=QT_STRINGIFY2(actionSearch);
    auto keys=p->actions.keys();
    for(auto &name : keys){
        if(name==__actionSearch)
            continue;
        p->actions.insert(name, nullptr);
    }
    return *this;
}

CRUDBase &CRUDBase::actionSearch(QOrm::ModelAction &action)
{
    action.setCrudBase(this);
    p->actions[__func__]=&action;
    return *this;
}

CRUDBase &CRUDBase::actionCreate(QOrm::ModelAction &action)
{
    action.setCrudBase(this);
    p->actions[__func__]=&action;
    return *this;
}

CRUDBase &CRUDBase::actionInsert(QOrm::ModelAction &action)
{
    action.setCrudBase(this);
    p->actions[__func__]=&action;
    return *this;
}

CRUDBase &CRUDBase::actionUpsert(QOrm::ModelAction &action)
{
    action.setCrudBase(this);
    p->actions[__func__]=&action;
    return *this;
}

CRUDBase &CRUDBase::actionUpdate(QOrm::ModelAction &action)
{
    action.setCrudBase(this);
    p->actions[__func__]=&action;
    return *this;
}

CRUDBase &CRUDBase::actionInsertUpsertUpdate(QOrm::ModelAction &action)
{
    action.setCrudBase(this);
    p->actions[QT_STRINGIFY2(actionInsert)]=&action;
    p->actions[QT_STRINGIFY2(actionUpdate)]=&action;
    p->actions[QT_STRINGIFY2(actionUpsert)]=&action;
    return *this;
}

CRUDBase &CRUDBase::actionRemove(QOrm::ModelAction &action)
{
    action.setCrudBase(this);
    p->actions[__func__]=&action;
    return *this;
}

CRUDBase &CRUDBase::actionDeactivate(QOrm::ModelAction &action)
{
    action.setCrudBase(this);
    p->actions[__func__]=&action;
    return *this;
}

ResultValue &CRUDBase::create()
{
    return this->lr().setNotImplemented();
}

ResultValue &CRUDBase::create(const QVariant &value)
{
    Q_UNUSED(value)
    return this->lr().setNotImplemented();
}

ResultValue &CRUDBase::search()
{
    return this->lr().setNotImplemented();
}

ResultValue &CRUDBase::search(const QVariant &value)
{
    Q_UNUSED(value)
    return this->lr().setNotImplemented();
}

ResultValue &CRUDBase::insert()
{
    return this->lr().setNotImplemented();
}

ResultValue &CRUDBase::insert(const QVariant &value)
{
    Q_UNUSED(value)
    return this->lr().setNotImplemented();
}

ResultValue &CRUDBase::update()
{
    return this->lr().setNotImplemented();
}

ResultValue &CRUDBase::update(const QVariant &value)
{
    Q_UNUSED(value)
    return this->lr().setNotImplemented();
}

ResultValue &CRUDBase::upsert()
{
    return this->lr().setNotImplemented();
}

ResultValue &CRUDBase::upsert(const QVariant &value)
{
    Q_UNUSED(value)
    return this->lr().setNotImplemented();
}

ResultValue &CRUDBase::remove()
{
    return this->lr().setNotImplemented();
}

ResultValue &CRUDBase::remove(const QVariant &value)
{
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
    p->actionMethod.insert(__before, method);
    return *this;
}

CRUDBase &CRUDBase::onSuccess(QOrm::CRUDBodyActionMethod method)
{
    p->actionMethod.insert(__success, method);
    return *this;
}

CRUDBase &CRUDBase::onFailed(QOrm::CRUDBodyActionMethod method)
{
    p->actionMethod.insert(__failed, method);
    return *this;
}

ResultValue &CRUDBase::canActionCreate()
{
    Q_DECLARE_VU;

    static const auto name=QByteArray{__func__}.replace(__canAction, __action);

    QVariantHash vSource;
    {
        switch (this->source().typeId()) {
        case QMetaType::QVariantList:
        case QMetaType::QStringList:
        {
            auto vList=this->source().toList();
            if(!vList.isEmpty())
                vSource=vList.first().toHash();
            break;
        }
        case QMetaType::QVariantHash:
        case QMetaType::QVariantMap:
            vSource=this->source().toHash();
            break;
        default:
            break;
        }
        if(vSource.contains(__source))
            vSource=vSource.value(__source).toHash();
    }

    auto act=p->actions.value(name);
    auto &lr=(act==nullptr)?this->create():act->action(vSource);
    if(!lr)
        return this->lr(lr);
    auto v=lr.resultVariant();
    return this->lr(p->dto
                    .host(p->host)
                    .items(v).o());
}

ResultValue &CRUDBase::canActionSearch()
{
    Q_DECLARE_VU;

    static const auto name=QByteArray{__func__}.replace(__canAction, __action);

    QVariantHash vSource;
    {
        switch (this->source().typeId()) {
        case QMetaType::QVariantList:
        case QMetaType::QStringList:
        {
            auto vList=this->source().toList();
            if(!vList.isEmpty())
                vSource=vList.first().toHash();
            break;
        }
        case QMetaType::QVariantHash:
        case QMetaType::QVariantMap:
            vSource=this->source().toHash();
            break;
        default:
            break;
        }
        if(vSource.contains(__source))
            vSource=vSource.value(__source).toHash();
    }

    QVariant v;
    if(this->options().searchOnEmptyFilter() || !vu.vIsEmpty(vSource)){
        auto act=p->actions.value(name);
        auto &lr=(act==nullptr)?this->search():act->action(vSource);
        if(!lr)
            return this->lr(lr);
        v=lr.resultVariant();
    }
    return this->lr(p->dto
                    .uuid(this->uuid())//crud uuid
                    .host(p->host)
                    .items(v).o());
}

ResultValue &CRUDBase::canActionInsert()
{
    static const auto name=QByteArray{__func__}.replace(__canAction, __action);
    auto act=p->actions.value(name);
    QVariant v;
    if(this->source().isValid() && !this->source().isNull()){
        auto &lr=(act==nullptr)?this->insert():act->action(this->source());
        v=lr.resultVariant();
    }
    return this->lr(p->dto
                    .uuid(this->uuid())//crud uuid
                    .host(p->host)
                    .items(v).o());
}

ResultValue &CRUDBase::canActionUpsert()
{
    static const auto name=QByteArray{__func__}.replace(__canAction, __action);
    auto act=p->actions.value(name);
    QVariant v;
    if(this->source().isValid() && !this->source().isNull()){
        auto &lr=(act==nullptr)?this->upsert():act->action(this->source());
        v=lr.resultVariant();
    }
    return this->lr(p->dto
                    .uuid(this->uuid())//crud uuid
                    .host(p->host)
                    .items(v).o());
}

ResultValue &CRUDBase::canActionUpdate()
{
    static const auto name=QByteArray{__func__}.replace(__canAction, __action);
    auto act=p->actions.value(name);
    QVariant v;
    if(this->source().isValid() && !this->source().isNull()){
        auto &lr=(act==nullptr)?this->update():act->action(this->source());
        v=lr.resultVariant();
    }
    return this->lr(p->dto
                    .uuid(this->uuid())//crud uuid
                    .host(p->host)
                    .items(v).o());
}

ResultValue &CRUDBase::canActionRemove()
{
    static const auto name=QByteArray{__func__}.replace(__canAction, __action);
    auto act=p->actions.value(name);
    QVariant v;
    if(this->source().isValid() && !this->source().isNull()){
        auto &lr=(act==nullptr)?this->remove():act->action(this->source());
        v=lr.resultVariant();
        return this->lr(v);
    }
    return this->lr().clear();
}

ResultValue &CRUDBase::canActionDeactivate()
{
    static const auto name=QByteArray{__func__}.replace(__canAction, __action);
    auto act=p->actions.value(name);
    QVariant v;
    if(this->source().isValid() && !this->source().isNull()){
        auto &lr=(act==nullptr)?this->deactivate():act->action(this->source());
        v=lr.resultVariant();
        return this->lr(v);
    }
    return this->lr().clear();
}

ResultValue &CRUDBase::doBofore()
{
    return p->doModelAction(__before);
}

ResultValue &CRUDBase::doSuccess()
{
    return p->doModelAction(__success);
}

ResultValue &CRUDBase::doFailed()
{
    return p->doModelAction(__failed);
}

}
