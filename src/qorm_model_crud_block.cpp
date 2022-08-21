#include "./qorm_model_crud_block.h"
#include "../../qstm/src/qstm_util_variant.h"
#include "../../qstm/src/qstm_meta_enum.h"
#include "./private/p_qorm_model_crud_body.h"

namespace QOrm {

static auto __resultInfo=QStringLiteral("resultInfo");
static auto __pages=QStringLiteral("pages");
static auto __type=QStringLiteral("type");
static auto __items="items";

class CRUDBlockPvt:public QObject{
public:
    QStm::MetaEnum<CRUDBlock::FormType> type=CRUDBlock::RegisterForm;
    ModelDtoOptions options;
    QStm::ResultInfo resultInfo;
    CRUDBlock *parent=nullptr;
    QVariant crudBody;
    QMap<QString, PrivateQOrm::CRUDBase *> crudMap;
    QList<PrivateQOrm::CRUDBase *> crudList;
    QVariantHash generatedRecords;
    Host host;
    explicit CRUDBlockPvt(CRUDBlock*parent):QObject{parent}, options{parent}
    {
        this->parent=parent;
    }

    static CRUDBody reMakeCRUDBody(const QVariantList &returns, PrivateQOrm::CRUDBase *crud, CRUDBody crudBody)
    {
        auto strategy=crudBody.strategy();
        CRUDBody __return;
        switch (strategy) {
        case QOrm::CRUDTypes::Strategy::Create:
        case QOrm::CRUDTypes::Strategy::Remove:
        case QOrm::CRUDTypes::Strategy::Deactivate:
        case QOrm::CRUDTypes::Strategy::Search:{
            if(returns.isEmpty()){//prepara primeira consulta
                __return=CRUDBody{strategy, crud->dao().toPrepareSearch(crud->modelInfo(), crudBody.data())};
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
        case QOrm::CRUDTypes::Strategy::Insert:
        case QOrm::CRUDTypes::Strategy::Update:
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

CRUDBlock::CRUDBlock(QObject *parent):QOrm::ObjectDb{parent}
{
    this->p = new CRUDBlockPvt{this};
}

CRUDBlock::CRUDBlock(const QVariant &crudBody, QObject *parent):QOrm::ObjectDb{parent}
{
    this->p = new CRUDBlockPvt{this};

    p->crudBody=crudBody;
}

QVariant CRUDBlock::type() const
{
    return p->type.name();
}

CRUDBlock &CRUDBlock::setType(const FormType &newType)
{
    if (p->type == newType)
        return *this;
    p->type = newType;
    return *this;
}

CRUDBlock &CRUDBlock::resetType()
{
    return setType(RegisterForm);
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
    return p->resultInfo;
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
    return*this;
}

CRUDBlock &CRUDBlock::makeBlock(CRUDBlock &crudBlock, const QVariant &crudBody)
{
    return crudBlock.crudBody(crudBody);
}

CRUDBlock &CRUDBlock::clear()
{
    p->generatedRecords.clear();
    p->crudMap.clear();
    return*this;
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
    return*this;
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
    return*this;
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

ResultValue &CRUDBlock::crudify()
{
    this->clean();

    if(p->crudMap.isEmpty())
        return this->lr().setBadRequest(tr("crud block is empty"));

    if(p->crudList.isEmpty())
        return this->lr().setBadRequest(tr("crud list is empty"));

    Q_DECLARE_VU;

    QVariantList __return;
    const CRUDBody crudBody{p->crudBody};

    this->resultInfo().setValues(crudBody.resultInfo());
    auto crudPages=crudBody.pagesHash();

    for(auto &crud:p->crudList){
        crud->setOptions(p->options);
        crud->setResultInfo(p->resultInfo);
        crud->host().setValues(&p->host);
    }

    for(auto &crud:p->crudList){

        CRUDBody crudItem{};

        if(crudPages.contains(crud->uuid().toString()))
            crudItem=CRUDBody{crudBody.strategy(), crudPages.value(crud->uuid().toString())};
        else
            crudItem=crudBody;

        if(!crud->crudBody(p->reMakeCRUDBody(__return, crud, crudItem)).crudify())
            return this->lr(crud->lr());

        __return.append(crud->lr().resultVariant());
    }

    if(__return.isEmpty())
        return this->lr().clear();

    return this->lr(QVariantHash{{__resultInfo, this->resultInfo().toHash()}, {__pages, __return}, {__type, this->type()}});
}

}


