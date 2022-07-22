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
    Host host;
    explicit CRUDBlockPvt(CRUDBlock*parent):QObject{parent}, options{parent}
    {
        this->parent=parent;
    }
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
    return this->insert(crud);
}

CRUDBlock &CRUDBlock::operator-=(PrivateQOrm::CRUDBase *crud)
{
    return this->remove(crud);
}

CRUDBlock &CRUDBlock::operator<<(PrivateQOrm::CRUDBase *crud)
{
    return this->insert(crud);
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
    p->crudMap.clear();
    for(auto &v:p->crudMap)
        this->remove(v);
    p->crudMap.clear();
    return*this;
}

CRUDBlock &CRUDBlock::insert(PrivateQOrm::CRUDBase *crud)
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

ResultValue &CRUDBlock::crudify()
{
    Q_DECLARE_VU;
    if(p->crudMap.isEmpty())
        return this->lr().clear().setBadRequest(tr("crud block is empty"));

    QVariantList __return;
    QVariantHash crudPages;
    CRUDBody crudBody{p->crudBody};

    {
        auto vCrudSource=crudBody.source().toHash();
        if(vCrudSource.contains(QStringLiteral("pages"))){
            auto vList=vCrudSource[QStringLiteral("pages")].toList();
            for(auto &v:vList){
                auto vHash=v.toHash();
                QVariant vCrudBody;
                if(vHash.contains(QStringLiteral("items"))){
                    auto vItems=vHash.value(QStringLiteral("items")).toList();
                    vCrudBody=vItems;
                }
                else{
                    vCrudBody=vHash;
                }
                auto crudUuid=vHash[QStringLiteral("uuid")].toUuid().toString();
                crudPages.insert(crudUuid, vCrudBody);
            }
        }
    }

    for(auto &crud:p->crudList){
        const auto crudUuid=crud->uuid().toString();
        crud->setOptions(p->options);
        crud->setResultInfo(p->resultInfo);
        crud->host().setValues(&p->host);

        CRUDBody crudItem{};

        if(crudPages.contains(crudUuid))
            crudItem=CRUDBody{crudBody.strategy(), crudPages.value(crudUuid)};
        else
            crudItem=crudBody;

        auto makeItem=[&__return](PrivateQOrm::CRUDBase *crud, CRUDBody crudBody){
            auto strategy=crudBody.strategy();

            if(!__return.isEmpty()){
                auto lastReturn = __return.last().toHash();
                static auto __items="items";
                if(lastReturn.contains(__items)){
                    auto v = lastReturn.value(__items);
                    v=crud->dao().toPrepareForeignWrapper(crud->modelInfo(), {}, v);
                    crudBody=CRUDBody{strategy, v};
                }
            }

            switch (strategy) {
            case QOrm::CRUDStrategy::Remove:
            case QOrm::CRUDStrategy::Deactivate:
            case QOrm::CRUDStrategy::Search:
                return CRUDBody{strategy, crud->dao().toPrepareSearch(crud->modelInfo(), crudBody.items())};
            case QOrm::CRUDStrategy::Insert:
            case QOrm::CRUDStrategy::Update:
            case QOrm::CRUDStrategy::Upsert:
                return crudBody;
            default:
                return CRUDBody{};
            }

            switch (strategy) {
            case QOrm::CRUDStrategy::Remove:
            case QOrm::CRUDStrategy::Deactivate:
            case QOrm::CRUDStrategy::Search:
            {
                break;
            }
            case QOrm::CRUDStrategy::Insert:
            case QOrm::CRUDStrategy::Update:
            case QOrm::CRUDStrategy::Upsert:
                return crudBody;
            default:
                return CRUDBody{};
            }

            switch (strategy) {
            case QOrm::CRUDStrategy::Remove:
            case QOrm::CRUDStrategy::Deactivate:
            case QOrm::CRUDStrategy::Search:
                return CRUDBody{strategy, crud->dao().toPrepareSearch(crud->modelInfo(), crudBody.items())};
            case QOrm::CRUDStrategy::Insert:
            case QOrm::CRUDStrategy::Update:
            case QOrm::CRUDStrategy::Upsert:
                return crudBody;
            default:
                return CRUDBody{};
            }
        };

        auto crudMaked=makeItem(crud, crudItem);
        if(!crud->crudBody(crudMaked).crudify())
            return this->lr(crud->lr());
        __return.append(crud->lr().resultVariant());
    }

    if(__return.isEmpty())
        return this->lr().clear();

    auto vPage=__return.first().toHash();
    auto type=vPage.value(QStringLiteral("type"));
    return this->lr(QVariantHash{{QStringLiteral("type"), type}, {QStringLiteral("pages"),__return}});
}

}


