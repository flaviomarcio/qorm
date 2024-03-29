#include "./qorm_model_crud_block.h"
#include "../../qstm/src/qstm_util_variant.h"
#include "./private/p_qorm_model_crud_body.h"

namespace QOrm {

static auto __resultInfo="resultInfo";
static auto __pages="pages";
static auto __type="type";
static auto __items="items";

class CRUDBlockPvt:public QObject{
public:
    CRUDBlock *parent=nullptr;
    QByteArray owner;
    QVariant type;
    ModelDtoOptions options;
    QVariant crudBody;
    QMap<QString, PrivateQOrm::CRUDBase *> crudMap;
    QList<PrivateQOrm::CRUDBase *> crudList;
    QVariantHash generatedRecords;
    Host host;
    explicit CRUDBlockPvt(CRUDBlock*parent):QObject{parent}, parent{parent}, options{parent}
    {
    }

    static CRUDBody reMakeCRUDBody(const QVariantList &returns, PrivateQOrm::CRUDBase *crud, CRUDBody crudBody)
    {
        auto strategy=crudBody.strategy();
        CRUDBody __return;

        switch (strategy) {
        case QOrm::CRUDTypes::Strategy::Custom:
            return crudBody;
//        case QOrm::CRUDTypes::Strategy::Init:
//        {
//            switch (crud->actionStart()) {
//            case PrivateQOrm::CRUDBase::asCREATE:
//                strategy=QOrm::CRUDTypes::Create;
//                break;
//            case PrivateQOrm::CRUDBase::asSEARCH:
//                strategy=QOrm::CRUDTypes::Search;
//                break;
//            default:
//                break;
//            }
//        }
        default:
            break;
        }


//        switch (strategy) {
//        case QOrm::CRUDTypes::Strategy::Print:
//            strategy=QOrm::CRUDTypes::Strategy::Search;
//            break;
//        default:
//            break;
//        }

        switch (strategy) {
        case QOrm::CRUDTypes::Strategy::Init:
        case QOrm::CRUDTypes::Strategy::Create:
        case QOrm::CRUDTypes::Strategy::Edit:
        case QOrm::CRUDTypes::Strategy::Remove:
        case QOrm::CRUDTypes::Strategy::Deactivate:
        case QOrm::CRUDTypes::Strategy::Search:
        case QOrm::CRUDTypes::Strategy::Print:{
            if(returns.isEmpty()){//prepara primeira consulta
                QVariant data;
                switch (strategy) {
                case QOrm::CRUDTypes::Strategy::Edit:
                case QOrm::CRUDTypes::Strategy::Search:
                case QOrm::CRUDTypes::Strategy::Print:
                    if(crudBody.expressions().isValid())
                        data=crudBody.expressions();
                    else
                        data=crudBody.data();
                    break;
                default:
                    data=crudBody.data();
                    break;
                }
                __return=CRUDBody{strategy, crud->dao().toPrepareSearch(crud->modelInfo(), data)};
            }
            else{
                QVariantHash lastReturn;
                auto pkRecord=returns.first();//primeiro registro que contem a PK principal
                switch (pkRecord.typeId()) {
                case QMetaType::QVariantList:
                {
                    auto l=pkRecord.toList();
                    if(!l.isEmpty())
                        lastReturn=pkRecord.toHash();
                    break;
                }
                default:
                    lastReturn=pkRecord.toHash();
                    break;
                }
                //por não ser a primeira consulta utilizaremos o replace da FK extraida do ultimo registro
                QVariant vPK=lastReturn.contains(__items)?lastReturn.value(__items):lastReturn;
                __return=CRUDBody{strategy, crud->dao().toPrepareForeign(crud->modelInfo(), vPK)};
            }
            break;
        }
        case QOrm::CRUDTypes::Strategy::Apply:
        case QOrm::CRUDTypes::Strategy::Execute:
        case QOrm::CRUDTypes::Strategy::Finalize:
        case QOrm::CRUDTypes::Strategy::Upsert:{
            if(returns.isEmpty()){
                __return = crudBody;//primeiro registro não deve sofrer modificacao
            }
            else{
                QVariantHash lastReturn;
                auto pkRecord=returns.first();//primeiro registro que contem a PK principal
                switch (pkRecord.typeId()) {
                case QMetaType::QVariantMap:
                case QMetaType::QVariantHash:
                    lastReturn=pkRecord.toHash();
                    break;
                case QMetaType::QVariantList:
                {
                    auto l=pkRecord.toList();
                    if(!l.isEmpty())
                        lastReturn=l.first().toHash();
                    break;
                }
                default:
                    break;
                }
                QVariant vPK=lastReturn.contains(__items)?lastReturn.value(__items):lastReturn;
                __return = CRUDBody{strategy, crud->dao().toPrepareForeignWrapper(crud->modelInfo(), crudBody.items(), vPK)};
                break;
            }
            break;
        }
        default:
            return CRUDBody{};
        }
        return __return;
    };

};

CRUDBlock::CRUDBlock(QObject *parent):QOrm::ObjectDb{parent}, p{new CRUDBlockPvt{this}}
{
}

CRUDBlock::CRUDBlock(const QVariant &crudBody, QObject *parent):QOrm::ObjectDb{parent}, p{new CRUDBlockPvt{this}}
{
    p->crudBody=crudBody;
}

QByteArray &CRUDBlock::owner() const
{
    return p->owner;
}

CRUDBlock &CRUDBlock::owner(const QVariant &newOwner)
{
    p->owner=newOwner.toByteArray();
    return *this;
}

QVariant CRUDBlock::type() const
{
    return p->type;
}

CRUDBlock &CRUDBlock::type(const QVariant &newType)
{
    if (p->type == newType)
        return *this;
    p->type = newType;
    return *this;
}

Host &CRUDBlock::host()
{
    return p->host;
}

CRUDBlock &CRUDBlock::host(const Host &value)
{
    p->host=&value;
    return *this;
}

CRUDBlock &CRUDBlock::host(const QVariant &value)
{
    p->host=value;
    return *this;
}

ModelDtoOptions &CRUDBlock::options()
{
    return p->options;
}

QStm::ResultInfo &CRUDBlock::resultInfo()
{
    return this->lr().resultInfo();
}

CRUDBlock &CRUDBlock::operator+=(PrivateQOrm::CRUDBase *crud)
{
    return this->append(crud);
}

CRUDBlock &CRUDBlock::operator-=(PrivateQOrm::CRUDBase *crud)
{
    return this->remove(crud);
}

CRUDBlock &CRUDBlock::operator<<(PrivateQOrm::CRUDBase *crud)
{
    return this->append(crud);
}

QVariant &CRUDBlock::crudBody() const
{
    return p->crudBody;
}

CRUDBlock &CRUDBlock::crudBody(const QVariant &v)
{
    p->crudBody=v;
    return *this;
}

CRUDBlock &CRUDBlock::makeBlock(CRUDBlock &crudBlock, const QVariant &crudBody)
{
    return crudBlock.crudBody(crudBody);
}

CRUDBlock &CRUDBlock::clear()
{
    p->generatedRecords.clear();
    p->crudMap.clear();
    return *this;
}

CRUDBlock &CRUDBlock::clean()
{
    p->generatedRecords.clear();
    for(auto &crud:p->crudMap)
        crud->clean();
    return *this;
}

CRUDBlock &CRUDBlock::append(PrivateQOrm::CRUDBase *crud)
{
    this->remove(crud);
    if(crud!=nullptr){
        p->crudMap.insert(crud->uuid().toString(), crud);
        p->crudList.append(crud);
    }
    return *this;
}

CRUDBlock &CRUDBlock::remove(PrivateQOrm::CRUDBase *crud)
{
    if(p->crudMap.contains(crud->uuid().toString())){
        auto _crud=p->crudMap.take(crud->uuid().toString());
        if(_crud!=nullptr && _crud->parent()==this){
            p->crudList.removeOne(_crud);
            delete _crud;
        }
    }
    return *this;
}

CRUDBlock &CRUDBlock::remove(const QUuid &crudUuid)
{
    return this->remove(p->crudMap.value(crudUuid.toString()));
}

const QVariantHash &CRUDBlock::generatedRecords() const
{
    for(auto &crud:p->crudList){
        p->generatedRecords.insert(crud->uuid().toString(), crud->generatedRecords());
    }
    return p->generatedRecords;
}

const QVariantList CRUDBlock::generatedRecords(const QUuid &uuid) const
{
    for(auto &crud:p->crudList){
        if(crud->uuid()==uuid)
            return crud->generatedRecords();
    }
    return {};
}

const QVariantList CRUDBlock::generatedRecords(const ModelInfo &modelInfo) const
{
    for(auto &crud:p->crudList){
        if(crud->modelInfo().uuid()==modelInfo.uuid())
            return crud->generatedRecords();
    }
    return {};
}

const QVariantList CRUDBlock::generatedListRecords() const
{
    QVariantList __return;
    for(auto &crud:p->crudList)
        __return.append(crud->generatedRecords());
    return __return;
}

ResultValue &CRUDBlock::crudify()
{
    this->clean();

    if(p->crudMap.isEmpty())
        return this->lr().setBadRequest(tr("crud block is empty"));

    if(p->crudList.isEmpty())
        return this->lr().setBadRequest(tr("crud list is empty"));

    Q_DECLARE_VU;

    QVariantList pageList;
    const CRUDBody crudBody{p->crudBody};

    if(!crudBody.isValid())
        return this->lr().setValidation(tr("Invalid CRUDBody"));

    auto &rInfo=this->resultInfo();
    rInfo.setValues(crudBody.resultInfo());
    auto crudPages=crudBody.pagesHash();

    for(auto &crud:p->crudList){
        crud->setHost(p->host);
        crud->setOptions(p->options);
        crud->setResultInfo(rInfo);
    }

    QVariantHash resultInfo;
    bool jump=false;

    for(auto &crud:p->crudList){

        CRUDBody crudItem{};

        if(crudPages.contains(crud->uuid().toString()))
            crudItem=CRUDBody{crudBody.strategy(), crudPages.value(crud->uuid().toString())};
        else
            crudItem=crudBody;

        if(!jump){
            if(!crud->crudBody(p->reMakeCRUDBody(pageList, crud, crudItem)).crudify())
                return this->lr(crud->lr());
        }

        if(crudBody.isStrategy(CRUDTypes::Init) || crudBody.isStrategy(CRUDTypes::Search))// não carrega os proximos
            jump=true;

        crud->resultInfo().md5Counter(crudItem.expressions());

        auto vResult=crud->lr().resultHash();
        if(resultInfo.isEmpty())
            resultInfo=vResult.value(__resultInfo).toHash();
        pageList.append(vResult);
    }

    if(pageList.isEmpty())
        return this->lr().clear();

    auto totalCount=rInfo.totalCount();
    auto page=rInfo.page();
    auto perPage=rInfo.perPage();
    rInfo.setValues(resultInfo);
    this->resultInfo()
        .page(page)
        .perPage(perPage)
        .totalCount(
            (rInfo.totalCount()<=0 && totalCount>0)
                ?totalCount
                :rInfo.totalCount()
            );


    auto crudType=this->type().toString().trimmed();
    if(crudType.isEmpty())
        crudType=Q_ORM_REGISTERFORM;

    auto __return=QVariantHash{{__resultInfo, this->resultInfo().toHash()}, {__pages, pageList}, {__type, crudType}};

    for(auto &crud:p->crudList)
        crud->clean();

    return this->lr(__return);
}

}


