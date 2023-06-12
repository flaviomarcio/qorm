#include "./p_qorm_model_crud.h"
#include "../qorm_model_dto.h"
#include <QJsonDocument>
#include "./p_qorm_model_crud_body.h"
//#include "./p_qorm_sql_suitable_parser_item.h"
//#include "../qorm_query.h"
//#include "../qorm_transaction_scope.h"

namespace PrivateQOrm {

static const auto __success = "success";
static const auto __action = "action";
static const auto __source = "source";
static const auto __method = "method";
static const auto __strategy = "strategy";
static const auto __resultInfo = "resultInfo";
static const auto __canAction = "canAction";
static const auto __before = "before";
static const auto __failed = "failed";
static const auto __print = "print";

#ifdef QTREFORCE_QRMK
static const auto __title="title";
static const auto __field="field";
static const auto __value="value";
#endif

class CRUDBasePvt:public QObject{
public:
    QVariant type;
    QOrm::ModelDtoOptions options;    
    QOrm::Host host;
    QUuid uuid;
    QByteArray owner;
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
#ifdef QTREFORCE_QRMK
    QRmk::Maker maker;
#endif
    explicit CRUDBasePvt(CRUDBase*parent):
        QObject{parent}
      , options{parent}
      , dao{parent}
      , dto{parent}
#ifdef QTREFORCE_QRMK
      , maker{parent}
#endif
    {
        this->parent=parent;
    }

#ifdef QTREFORCE_QRMK
    QRmk::Maker &makerPrepare()
    {
        this->maker.clear();

        auto makeFilters=[this](QRmk::Headers &headers){
            QVariantHash filters;

            {//parse filters
                const auto &modelInfo=this->parent->modelInfo();
                auto vSource=this->parent->source().toHash();
                const auto &vFields=modelInfo.propertyByFieldName();
                QHashIterator<QString,QMetaProperty> i(vFields);
                while(i.hasNext()){
                    i.next();
                    if(!vSource.contains(i.key()) && !vSource.contains(i.value().name()))
                        continue;
                    filters.insert(i.value().name(), vSource.value(i.key()));
                }
            }

            QVariantHash __return;
            QHashIterator<QString, QVariant> i(filters);
            while(i.hasNext()){
                i.next();

                const auto &key=i.key().trimmed().toLower();
                auto value=i.value();

                if(!headers.contains(key))
                    continue;

                const auto &header=headers.header(key);
                auto v=QVariantHash{{__title, header.title()}, {__field, key},{__value, value}};
                __return.insert(key, v);
            }
            return __return;
        };

        auto makeHeaders=[this](QRmk::Headers &headers)
        {
            QStringList groupingField;
            for(auto &header : this->parent->dto().headers().list()){
                headers
                        .header(header->field())
                        .title(header->title())
                        .align(header->align())
                        .field(header->field())
                        .width(header->width())
                        .visible(header->displayer())
                        .format(header->format())
                        .dataType(header->dataType());
                if(header->grouping() && !groupingField.contains(header->field()))
                    groupingField.append(header->field());
            }
            this->maker.groupingFields(groupingField);
        };

        auto makeSummary=[this](QRmk::Headers &headers)
        {
            for(auto &header : this->parent->dto().headers().list()){
                if(header->summaryMode()==header->None)
                    continue;
                headers
                        .header(header->field())
                        .format(header->format())
                        .computeMode(header->summaryMode());
            }
        };

        auto makerSignature=[](QRmk::Signatures &signatures)
        {
            Q_UNUSED(signatures)
    //            auto declaration=QStringList
    //            {
    //                    "<p>Recebi da <strong>Empresa de Serviços</strong> LTDA com CNPJ: ",
    //                    "888.888.88/0001-88, a importância total de <strong>R$ 505,82 ( QUINHENTOS E CINCO ",
    //                    "REAIS E OITENTA E DOIS CENTAVOS )</strong> valor este discriminado acima</p> "
    //            };

    //            auto local="Sant Lois";

    //            signatures
    //                    .pageArea(QRmk::Signatures::Area{"20%","30%"})
    //                    .title("Recibo")
    //                    .declaration(declaration)
    //                    .local(local);

    //            signatures
    //                    .signature("${document01}-One")
    //                    .documentType(QRmk::Signature::CNPJ)
    //                    .name("${name}");

    //            signatures
    //                    .signature("${document02}-Two")
    //                    .documentType(QRmk::Signature::CNPJ)
    //                    .name("${name}");

    //            signatures
    //                    .signature("${document03}-three")
    //                    .documentType(QRmk::Signature::CNPJ)
    //                    .name("${name}");
        };

        this->maker
                .clean()
                .headers(makeHeaders)
                .summary(makeSummary)
                .filters(makeFilters)
                .title(this->parent->description())
                .signature(makerSignature)
                .owner(this->owner)
                .make()
                ;

        return this->maker;
    }
#endif

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
        return *this->parent;
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
        this->parent->lr().resultInfo().fromVar(vCrud.value(__resultInfo));
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

    QVariant &getSource()
    {
        switch (this->source.typeId()) {
        case QMetaType::QString:
        case QMetaType::QByteArray:
        case QMetaType::QChar:
        case QMetaType::QBitArray:{
            auto v=this->source.toString().trimmed();
            this->source=v.isEmpty()?QVariant{}:v;
            break;
        }
        case QMetaType::QVariantList:
        case QMetaType::QStringList:{
            auto v=this->source.toList();
            this->source=v.isEmpty()?QVariant{}:v;
            break;
        }
        case QMetaType::QVariantHash:
        case QMetaType::QVariantMap:{
            auto v=this->source.toHash();
            this->source=v.isEmpty()?QVariant{}:v;
            break;
        }
        default:
            break;
        }
        return this->source;
    }

    ResultValue &canStrategy(const QByteArray &methodName){
        auto name=QByteArray{methodName}.replace(__canAction, __action);
        auto act=this->actions.value(name);
        if(!act)
            return parent->lr();
        auto &lr=act->action(this->getSource());
        if(!lr)
            return parent->lr();
        return parent->lr(lr.resultVariant());
    }

};


CRUDBase::CRUDBase(QObject *parent) : QOrm::ObjectDb{parent}
{
    this->p = new CRUDBasePvt{this};
    p->set_crud(CRUDBody{});
}

CRUDBase::CRUDBase(const QVariant &vBody, QObject *parent):QOrm::ObjectDb{parent}
{
    this->p = new CRUDBasePvt{this};

    p->set_crud(vBody);
}

bool CRUDBase::isValid()const
{
    return !this->modelInfo().propertyDescriptors().isEmpty();
}

CRUDBase::ActionStart CRUDBase::actionStart() const
{
    return this->asCREATE;
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

#ifdef QTREFORCE_QRMK
QRmk::Maker &CRUDBase::maker()
{
    return p->makerPrepare();
}
#endif

const QOrm::Host &CRUDBase::host() const
{
    return p->host;
}

CRUDBase &CRUDBase::host(const QOrm::Host &newHost)
{
    return this->setHost(newHost);
}

CRUDBase &CRUDBase::setHost(const QOrm::Host &newHost)
{
    p->host.setValues(&newHost);
    return *this;
}

const QVariant CRUDBase::type() const
{
    return p->type;
}

CRUDBase &CRUDBase::setType(const QVariant &newType)
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
    if(p->uuid.isNull()){
        Q_DECLARE_VU;
        const auto &modelInfo=this->modelInfo();
        p->uuid=modelInfo.tableUuid();
        if(p->uuid.isNull())
            p->uuid=vu.toMd5Uuid(this->name());
    }
    return p->uuid;
}

const QByteArray &CRUDBase::owner()
{
    return p->owner;
}

CRUDBase &CRUDBase::owner(const QVariant &value)
{
    p->owner=value.toByteArray();
    return *this;
}

const QByteArray &CRUDBase::name()
{
    if(p->name.trimmed().isEmpty())
        p->name=this->metaObject()->className();
    return p->name;
}

CRUDBase &CRUDBase::name(const QVariant &value)
{
    auto name=value.toByteArray().trimmed();
    if(p->name==name)
        return *this;
    Q_DECLARE_VU;
    p->name=name;
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
    return p->getSource();
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

bool CRUDBase::beforeCrudify()
{
    return true;
}

bool CRUDBase::afterCrudify()
{
    return true;
}

ResultValue &CRUDBase::crudify()
{
    this->clean();

    if(!this->beforeCrudify())
        return this->lr()=false;

    p->dto.setResultInfo(this->resultInfo());
    auto strategy=this->strategy();
    switch (strategy) {
    case QOrm::CRUDTypes::Custom:
        if(!this->canActionCustom())
            return this->lr();
        break;
    case QOrm::CRUDTypes::Create:
        if(!this->canActionCreate())
            return this->lr();
        break;
    case QOrm::CRUDTypes::Search:
        if(!this->canActionSearch())
            return this->lr();
        break;
    case QOrm::CRUDTypes::Upsert:
        if(!this->canActionUpsert())
            return this->lr();
        break;
    case QOrm::CRUDTypes::Remove:
        if(!this->canActionRemove())
            return this->lr();
        break;
    case QOrm::CRUDTypes::Deactivate:
        if(!this->canActionDeactivate())
            return this->lr();
        break;
    case QOrm::CRUDTypes::Apply:
        if(!this->canActionApply())
            return this->lr();
        break;
    case QOrm::CRUDTypes::Execute:
        if(!this->canActionExecute())
            return this->lr();
        break;
    case QOrm::CRUDTypes::Finalize:
        if(!this->canActionFinalize())
            return this->lr();
        break;
    case QOrm::CRUDTypes::Print:
        if(!this->canActionPrint())
            return this->lr();
        break;
    default:
        return this->lr().setValidation(tr("Invalid strategy"));
    }

    if(!this->afterCrudify())
        return this->lr()=false;

    return this->lr();
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
    p->actions.insert(__func__, &action);
    return *this;
}

CRUDBase &CRUDBase::actionCreate(QOrm::ModelAction &action)
{
    action.setCrudBase(this);
    p->actions.insert(__func__, &action);
    return *this;
}

CRUDBase &CRUDBase::actionUpsert(QOrm::ModelAction &action)
{
    action.setCrudBase(this);
    p->actions.insert(__func__, &action);
    return *this;
}

CRUDBase &CRUDBase::actionRemove(QOrm::ModelAction &action)
{
    action.setCrudBase(this);
    p->actions.insert(__func__, &action);
    return *this;
}

CRUDBase &CRUDBase::actionDeactivate(QOrm::ModelAction &action)
{
    action.setCrudBase(this);
    p->actions.insert(__func__, &action);
    return *this;
}

CRUDBase &CRUDBase::actionApply(QOrm::ModelAction &action)
{
    action.setCrudBase(this);
    p->actions.insert(__func__, &action);
    return *this;
}

CRUDBase &CRUDBase::actionExecute(QOrm::ModelAction &action)
{
    action.setCrudBase(this);
    p->actions.insert(__func__, &action);
    return *this;
}

CRUDBase &CRUDBase::actionFinalize(QOrm::ModelAction &action)
{
    action.setCrudBase(this);
    p->actions.insert(__func__, &action);
    return *this;
}

CRUDBase &CRUDBase::actionCustom(QOrm::ModelAction &action)
{
    action.setCrudBase(this);
    p->actions.insert(__func__, &action);
    return *this;
}

CRUDBase &CRUDBase::actionPrint(QOrm::ModelAction &action)
{
    action.setCrudBase(this);
    p->actions.insert(__func__, &action);
    return *this;
}

ResultValue &CRUDBase::create()
{
    return this->create({});
}

ResultValue &CRUDBase::create(const QVariant &value)
{
    Q_UNUSED(value)
    return this->lr().setNotImplemented();
}

ResultValue &CRUDBase::search()
{
    return this->upsert({});
}

ResultValue &CRUDBase::search(const QVariant &value)
{
    Q_UNUSED(value)
    return this->lr().setNotImplemented();
}

ResultValue &CRUDBase::upsert()
{
    return this->upsert({});
}

ResultValue &CRUDBase::upsert(const QVariant &value)
{
    Q_UNUSED(value)
    return this->lr().setNotImplemented();
}

ResultValue &CRUDBase::remove()
{
    return this->remove({});
}

ResultValue &CRUDBase::remove(const QVariant &value)
{
    Q_UNUSED(value)
    return this->lr().setNotImplemented();
}

ResultValue &CRUDBase::deactivate()
{
    return this->deactivate({});
}

ResultValue &CRUDBase::deactivate(const QVariant &value)
{
    Q_UNUSED(value)
    return this->lr().setNotImplemented();
}

ResultValue &CRUDBase::execute()
{
    return this->execute({});
}

ResultValue &CRUDBase::execute(const QVariant &value)
{
    Q_UNUSED(value)
    return this->lr().setNotImplemented();
}

ResultValue &CRUDBase::finalize()
{
    return this->finalize({});
}

ResultValue &CRUDBase::finalize(const QVariant &value)
{
    Q_UNUSED(value)
    return this->lr().setNotImplemented();
}

ResultValue &CRUDBase::print()
{
    return this->print(this->source());
}

ResultValue &CRUDBase::print(const QVariant &value)
{
    if(!this->search(value))
        return this->lr();
#ifdef QTREFORCE_QRMK
    p->makerPrepare()
            .items(this->lr().resultList())
            .make()
            ;

    auto fileName=this->maker().outFileName();
    if(!QFile::exists(fileName))
        return this->lr();

    return this->lr(QUrl::fromLocalFile(fileName));
#else
    return this->lr();
#endif
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
#ifdef QTREFORCE_QRMK
    p->generatedRecords=p->makerPrepare()
            .items(v)
            .makeRecords();
    v={};
#else
    p->generatedRecords=vu.toList(v);
#endif
    return this->lr(p->dto
                    .uuid(this->uuid())//crud uuid
                    .host(p->host)
                    .items(p->generatedRecords).o());
}

ResultValue &CRUDBase::canActionSearch()
{
    Q_DECLARE_VU;

    static const auto name=QByteArray{__func__}.replace(__canAction, __action).replace(__print, __action);

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
#ifdef QTREFORCE_QRMK
    p->generatedRecords=p->makerPrepare()
            .clean()
            .items(v)
            .makeRecords();
    v={};
#else
    p->generatedRecords=vu.toList(v);
#endif
    return this->lr(p->dto
                    .uuid(this->uuid())//crud uuid
                    .host(p->host)
                    .items(p->generatedRecords).o());
}

ResultValue &CRUDBase::canActionUpsert()
{
    static const auto name=QByteArray{__func__}.replace(__canAction, __action);
    auto act=p->actions.value(name);
    QVariant v;
    if(this->source().isValid() && !this->source().isNull()){
        auto &lr=(act==nullptr)?this->upsert():act->action(this->source());
        v=lr.resultVariant();
        if(!lr)
            return this->lr(lr);
    }
    Q_DECLARE_VU;
    p->generatedRecords=vu.toList(v);
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
        if(!lr)
            return this->lr(lr);
        v=lr.resultVariant();
        Q_DECLARE_VU;
        p->generatedRecords=vu.toList(v);
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
        if(!lr)
            return this->lr();
        v=lr.resultVariant();
        Q_DECLARE_VU;
        p->generatedRecords=vu.toList(v);
        return this->lr(v);
    }
    return this->lr().clear();
}

ResultValue &CRUDBase::canActionApply()
{
    return this->lr(p->canStrategy(__func__));
}

ResultValue &CRUDBase::canActionExecute()
{
    return this->lr(p->canStrategy(__func__));
}

ResultValue &CRUDBase::canActionFinalize()
{
    return this->lr(p->canStrategy(__func__));
}

ResultValue &CRUDBase::canActionPrint()
{
    static const auto name=QByteArray{__func__}.replace(__canAction, __action);
    auto act=p->actions.value(name);
    QVariant v;
    if(this->source().isValid() && !this->source().isNull()){
        auto &lr=(act==nullptr)?this->print():act->action(this->source());
        v=lr.resultVariant();
        if(!lr)
            return this->lr();
    }
    return this->lr(v);
}

ResultValue &CRUDBase::canActionCustom()
{
    return this->lr(p->canStrategy(__func__));
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
