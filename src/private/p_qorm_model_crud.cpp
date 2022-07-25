#include "./p_qorm_model_crud.h"
#include "../qorm_model_dto.h"
#include <QJsonDocument>
#include "./p_qorm_model_crud_body.h"
//#include "../qorm_query.h"
//#include "../qorm_transaction_scope.h"

namespace PrivateQOrm {

class CRUDBasePvt:public QObject{
public:
    QOrm::ModelDtoOptions options;
    QOrm::Host host;
    QUuid uuid;
    QByteArray name;
    QByteArray description;
    ModelDao dao;
    QOrm::ModelDto dto;
    QHash<QByteArray, QOrm::ModelAction*> actions;
    QHash<QByteArray, QOrm::CRUDBodyActionMethod> actionMethod;
    QOrm::CRUDStrategy strategy=QOrm::Undefined;
    QVariant source;
    QVariantList generatedRecords;
    CRUDBase*parent=nullptr;

    explicit CRUDBasePvt(CRUDBase*parent):QObject{parent}, options{parent}, dao{parent}, dto{parent}
    {
        this->parent=parent;
        dto.setType(QOrm::ModelDto::FormType(CRUDBase::defaultType()));
        dto.setLayout(QOrm::ModelDto::FormLayout(CRUDBase::defaultLayout()));
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
            if(vCrud.contains(QStringLiteral("method")))
                v=vCrud[QStringLiteral("method")];
            else if(vCrud.contains(QStringLiteral("strategy")))
                v=vCrud[QStringLiteral("strategy")];
            return v;
        };

        auto vSource=[&vCrud](){
            QVariant v;
            if(vCrud.contains(QStringLiteral("source")))
                v=vCrud[QStringLiteral("source")];
            return v;
        };
        this->strategySet(vStrategy());
        this->sourceSet(vSource());
        this->parent->lr().resultInfo().fromVar(vCrud.value(QStringLiteral("resultInfo")));
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
        if(!strategy.isValid() || strategy.isNull()){
            this->strategy=QOrm::Undefined;
            return;
        }
        QVariant vFy=strategy;
        if(QMetaTypeUtilString.contains(vFy.typeId())){
            vFy=vFy.toString().toLower();
            vFy=QOrm::__stringToStrategy.value(vFy.toString());
        }
        vFy=QOrm::__listToStrategy.contains(vFy)?vFy:QOrm::Undefined;
        this->strategy=QOrm::CRUDStrategy(vFy.toInt());
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

CRUDBase::FormType CRUDBase::type() const
{
    return FormType(p->dto.type());
}

CRUDBase &CRUDBase::type(const FormType &value)
{
    p->dto.setType(QOrm::ModelDto::FormType(value));
    return *this;
}

CRUDBase::FormLayout CRUDBase::layout() const
{
    return FormLayout(p->dto.layout());
}

CRUDBase &CRUDBase::layout(const FormLayout &value)
{
    p->dto.setLayout(QOrm::ModelDto::FormLayout(value));
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

const QOrm::ModelInfo &CRUDBase::modelInfo()
{
    static const QOrm::ModelInfo __return;
    return __return;
}

CRUDBase&CRUDBase::crudBody(const QVariant &v)
{
    p->set_crud(v);
    return *this;
}

QOrm::CRUDStrategy&CRUDBase::strategy()const
{
    return p->strategy;
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
    case QOrm::Search:
        return this->canActionSearch();
    case QOrm::Insert:
        return this->canActionInsert();
    case QOrm::Update:
        return this->canActionUpdate();
    case QOrm::Upsert:
        return this->canActionUpsert();
    case QOrm::Remove:
        return this->canActionRemove();
    case QOrm::Deactivate:
        return this->canActionDeactivate();
    default:
        return this->lr().setValidation(tr("Invalid strategy"));
    }
}

CRUDBase &CRUDBase::actionNulls()
{
    qDeleteAll(p->actions);
    p->actions.clear();
    return *this;
}

CRUDBase &CRUDBase::actionSearch(QOrm::ModelAction &action)
{
    p->actions[__func__]=&action;
    return *this;
}

CRUDBase &CRUDBase::actionInsert(QOrm::ModelAction &action)
{
    p->actions[__func__]=&action;
    return *this;
}

CRUDBase &CRUDBase::actionUpsert(QOrm::ModelAction &action)
{
    p->actions[__func__]=&action;
    return *this;
}

CRUDBase &CRUDBase::actionUpdate(QOrm::ModelAction &action)
{
    p->actions[__func__]=&action;
    return *this;
}

CRUDBase &CRUDBase::actionInsertUpsertUpdate(QOrm::ModelAction &action)
{
    p->actions[QT_STRINGIFY2(actionInsert)]=&action;
    p->actions[QT_STRINGIFY2(actionUpdate)]=&action;
    p->actions[QT_STRINGIFY2(actionUpsert)]=&action;
    return *this;
}

CRUDBase &CRUDBase::actionRemove(QOrm::ModelAction &action)
{
    p->actions[__func__]=&action;
    return *this;
}

CRUDBase &CRUDBase::actionDeactivate(QOrm::ModelAction &action)
{

    p->actions[__func__]=&action;
    return *this;
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
    p->actionMethod[QByteArrayLiteral("bofore")]=method;
    return *this;
}

CRUDBase &CRUDBase::onSuccess(QOrm::CRUDBodyActionMethod method)
{
    p->actionMethod[QByteArrayLiteral("success")]=method;
    return *this;
}

CRUDBase &CRUDBase::onFailed(QOrm::CRUDBodyActionMethod method)
{
    p->actionMethod[QByteArrayLiteral("failed")]=method;
    return *this;
}

ResultValue &CRUDBase::canActionSearch()
{
    Q_DECLARE_VU;

    static auto name=QByteArray{__func__}.replace(QByteArrayLiteral("canAction"), QByteArrayLiteral("action"));

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
        if(vSource.contains(QStringLiteral("source")))
            vSource=vSource.value(QStringLiteral("source")).toHash();
    }

    QVariant v;
    if(this->options().searchOnEmptyFilter() || !vu.vIsEmpty(vSource)){
        auto &act=p->actions[name];
        auto &lr=(act==nullptr)?this->search():act->action(vSource);
        v=lr.resultVariant();
    }
    return this->lr(p->dto
                    .host(p->host)
                    .uuid(this->uuid())
                    .name(this->name())
                    .text(this->description())
                    .items(v).o());
}

ResultValue &CRUDBase::canActionInsert()
{
    static auto name=QByteArray{__func__}.replace(QByteArrayLiteral("canAction"), QByteArrayLiteral("action"));
    auto &act=p->actions[name];
    auto &lr=(act==nullptr)?this->insert():act->action(this->source());
    return this->lr(lr);
}

ResultValue &CRUDBase::canActionUpsert()
{
    static auto name=QByteArray{__func__}.replace(QByteArrayLiteral("canAction"), QByteArrayLiteral("action"));
    auto &act=p->actions[name];
    auto &lr=(act==nullptr)?this->upsert():act->action(this->source());
    return this->lr(lr);
}

ResultValue &CRUDBase::canActionUpdate()
{
    static auto name=QByteArray{__func__}.replace(QByteArrayLiteral("canAction"), QByteArrayLiteral("action"));
    auto &act=p->actions[name];
    auto &lr=(act==nullptr)?this->update():act->action(this->source());
    return this->lr(lr);
}

ResultValue &CRUDBase::canActionRemove()
{
    static auto name=QByteArray{__func__}.replace(QByteArrayLiteral("canAction"), QByteArrayLiteral("action"));
    auto &act=p->actions[name];
    auto &lr=(act==nullptr)?this->remove():act->action(this->source());
    return this->lr(lr);
}

ResultValue &CRUDBase::canActionDeactivate()
{
    static auto name=QByteArray{__func__}.replace(QByteArrayLiteral("canAction"), QByteArrayLiteral("action"));
    auto &act=p->actions[name];
    auto &lr=(act==nullptr)?this->deactivate():act->action(this->source());
    return this->lr(lr);
}

ResultValue &CRUDBase::doBofore()
{
    return p->doModelAction(tr("bofore"));
}

ResultValue &CRUDBase::doSuccess()
{
    return p->doModelAction(tr("success"));
}

ResultValue &CRUDBase::doFailed()
{
    return p->doModelAction(tr("failed"));
}


}
