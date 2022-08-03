#include "./qorm_model_crud_block.h"
#include "../../qstm/src/qstm_util_variant.h"
#include "./private/p_qorm_model_crud_body.h"

namespace QOrm {

class CRUDBlockPvt:public QObject{
public:
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
        static auto __items="items";
        auto strategy=crudBody.strategy();
        CRUDBody __return;
        switch (strategy) {
        case QOrm::CRUDStrategy::Remove:
        case QOrm::CRUDStrategy::Deactivate:
        case QOrm::CRUDStrategy::Search:{
            if(!returns.isEmpty()){
                QVariantHash lastReturn=returns.last().toHash();
                QVariant vPK=lastReturn.contains(__items)?lastReturn.value(__items):lastReturn;
                __return=CRUDBody{strategy, crud->dao().toPrepareSearch(crud->modelInfo(), vPK)};
                break;
            }
            __return=CRUDBody{strategy, crud->dao().toPrepareSearch(crud->modelInfo(), crudBody.source())};
            break;
        }
        case QOrm::CRUDStrategy::Insert:
        case QOrm::CRUDStrategy::Update:
        case QOrm::CRUDStrategy::Upsert:{
            if(!returns.isEmpty()){
                QVariantHash lastReturn=returns.last().toHash();
                QVariant vPK=lastReturn.contains(__items)?lastReturn.value(__items):lastReturn;
                __return = CRUDBody{strategy, crud->dao().toPrepareForeignWrapper(crud->modelInfo(), crudBody.items(), vPK)};
                break;
            }
            __return = crudBody;
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
    static auto __mode=QStringLiteral("mode");
    static auto __fake=QStringLiteral("fake");
    static auto __uuid=QStringLiteral("uuid");
    static auto __resultInfo=QStringLiteral("resultInfo");
    static auto __expressions=QStringLiteral("expressions");
    static auto __pages=QStringLiteral("pages");
    static auto __items=QStringLiteral("items");
    static auto __type=QStringLiteral("type");

    this->clean();
    Q_DECLARE_VU;
    if(p->crudMap.isEmpty())
        return this->lr().setBadRequest(tr("crud block is empty"));

    QVariantList __return;


    QVariantHash crudPages;
    CRUDBody crudBody{p->crudBody};
    auto vCrudSource=crudBody.source().toHash();

    for(auto &crud:p->crudList){
        crud->setOptions(p->options);
        crud->setResultInfo(p->resultInfo);
        crud->host().setValues(&p->host);
    }


//    if(vCrudSource.contains(__mode) && vCrudSource.value(__mode).toString().toLower()==__fake){
//        for(auto &crud:p->crudList){
//            switch (crudBody.strategy()) {
//            case QOrm::CRUDStrategy::Search:{
//                if(!crud->isValid())
//                    continue;
//                break;
//            }
//            default:
//                break;
//            };
//            __return.append(crud->crudifyFake().resultVariant());
//        }
//    }
//    else{

        if(vCrudSource.contains(__resultInfo))
            this->resultInfo().setValues(vCrudSource.value(__resultInfo));

        if(vCrudSource.contains(__expressions)){
            auto expression=vCrudSource.value(__expressions);
            crudBody=CRUDBody{crudBody.strategy(), expression};
            vCrudSource.clear();
        }

        if(vCrudSource.contains(__pages)){
            auto vList=vCrudSource[__pages].toList();
            for(auto &v:vList){
                auto vHash=v.toHash();
                QVariant vCrudBody;
                if(vHash.contains(__items)){
                    auto vItems=vHash.value(__items).toList();
                    vCrudBody=vItems;
                }
                else{
                    vCrudBody=vHash;
                }
                auto crudUuid=vHash[__uuid].toUuid().toString();
                crudPages.insert(crudUuid, vCrudBody);
            }
        }

        for(auto &crud:p->crudList){

            switch (crudBody.strategy()) {
            case QOrm::CRUDStrategy::Search:{
                if(!crud->isValid())
                    continue;
                break;
            }
            default:
                break;
            };

            CRUDBody crudItem{};

            if(crudPages.contains(crud->uuid().toString()))
                crudItem=CRUDBody{crudBody.strategy(), crudPages.value(crud->uuid().toString())};
            else
                crudItem=crudBody;

            if(!crud->crudBody(p->reMakeCRUDBody(__return, crud, crudItem)).crudify())
                return this->lr(crud->lr());

            __return.append(crud->lr().resultVariant());
        }
//    }



    if(__return.isEmpty())
        return this->lr().clear();

    static auto metaEnum = QMetaEnum::fromType<FormType>();
    auto typeName=metaEnum.valueToKey(this->defaultType());

    return this->lr(QVariantHash{{__resultInfo, this->resultInfo().toHash()}, {__type, typeName}, {__pages,__return}});
}

}


