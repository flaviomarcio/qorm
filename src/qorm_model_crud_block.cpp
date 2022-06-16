#include "./qorm_model_crud_block.h"
#include "../../qstm/src/qstm_util_variant.h"

namespace QOrm {

#define dPvt()\
    auto &p = *reinterpret_cast<CRUDBlockPvt*>(this->p)

class CRUDBlockPvt:public QObject{
public:
    ModelDtoOptions options;
    QStm::ResultInfo resultInfo;
    CRUDBlock *parent=nullptr;
    QVariant crudBody;
    QMap<QByteArray, PrivateQOrm::CRUDBase *> crudMap;
    QList<PrivateQOrm::CRUDBase *> crudList;

    explicit CRUDBlockPvt(CRUDBlock*parent):QObject{parent},options{parent}
    {
        this->parent=parent;
    }

    virtual ~CRUDBlockPvt()
    {
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

CRUDBlock::~CRUDBlock()
{

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

QVariant CRUDBlock::crudBody() const
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
        p->crudMap.insert(crud->crudName(), crud);
        p->crudList<<crud;
    }
    return*this;
}

CRUDBlock &CRUDBlock::remove(PrivateQOrm::CRUDBase *crud)
{

    if(p->crudMap.contains(crud->crudName())){
        auto _crud=p->crudMap.take(crud->crudName());
        if(_crud!=nullptr && _crud->parent()==this){
            p->crudList.removeOne(_crud);
            delete _crud;
        }
    }
    return*this;
}

CRUDBlock &CRUDBlock::remove(const QByteArray &crudName)
{

    return this->remove(p->crudMap.value(crudName));
}

ResultValue &CRUDBlock::crudify()
{

    Q_DECLARE_VU;
    if(p->crudMap.isEmpty())
        return this->lr().clear().setBadRequest(tr("crud block is empty"));

    QVariantList __return;
    QVariantHash crudPages;
    CRUDBody crudBody(p->crudBody);

    {
        auto vCrudSource=crudBody.source().toHash();
        if(vCrudSource.contains(qsl("pages"))){
            auto vList=vCrudSource[qsl("pages")].toList();
            for(auto &v:vList){
                auto vHash=v.toHash();
                auto crudName=vHash[qsl("id")].toString().toLower().trimmed();
                crudPages.insert(crudName, vHash);
            }
        }
    }

    for(auto &crud:p->crudList){
        const auto crudName=crud->crudName().toLower();
        crud->setOptions(p->options);
        crud->setResultInfo(p->resultInfo);
        QVariantList crudList;
        QVariant crudSource;
        CRUDBody crudItem(crudBody);

        if(!crudPages.isEmpty())
            crudSource=crudPages.value(crudName);


        if(!crudPages.contains(crudName)){
            crudList.append(crudBody);
        }
        else if(vu.vIsList(crudSource)){
            for(auto &v:crudSource.toList())
                crudList<<v;
        }
        else if(vu.vIsMap(crudSource)){
            auto vHash=crudSource.toHash();
            if(vHash.contains(qsl("id")) && vHash.contains(qsl("items"))){
                auto list=vHash[qsl("items")].toList();
                for(auto &v:list)
                    crudList.append(CRUDBody(crudBody.strategy(), v));
            }
            else{
                crudList<<crudBody;
            }
        }
        else{
            crudList<<crudBody;
        }


        auto makeItem=[&__return](PrivateQOrm::CRUDBase *crud, const QVariant&crudSource){
            CRUDBody crudRecord(crudSource);

            auto lastCrud=__return.isEmpty()?qvh_null:__return.last().toHash();
            if(lastCrud.isEmpty())
                return CRUDBody(crudSource);

            QVariantHash vHash;
            auto record=lastCrud;
            if(lastCrud.contains(qsl("pages"))){
                auto crud=lastCrud[qsl("pages")].toList();
                if(!crud.isEmpty()){
                    vHash = crud.first().toHash();
                    if(vHash.contains(qsl("items")))
                        record=vHash;
                    else{
                        crud=vHash[qsl("items")].toList();
                        record=crud.isEmpty()?qvh_null:crud.first().toHash();
                    }
                }
            }
            const auto &modelInfo=crud->modelInfo();
            auto crudSourceFields=modelInfo.toForeign(crudRecord.source(), record);
            return CRUDBody(crudRecord.strategy(), crudSourceFields);
        };


        switch (crudBody.strategy()) {
        case QOrm::CRUDStrategy::Remove:
        case QOrm::CRUDStrategy::Deactivate:
        {
            auto crudListCopy=crudList;
            while(!crudListCopy.isEmpty()){
                const auto &crudSource=crudListCopy.takeLast();
                CRUDBody crudMaked(crudSource);
                if(!crudListCopy.isEmpty())
                    crudMaked=makeItem(crud, crudSource);

                if(!crud->crudBody(crudMaked).crudify())
                    return this->lr(crud->lr());
                __return<<crud->lr().resultVariant();
            }
            break;
        }
        default:
            for(auto &crudSource:crudList){
                auto crudMaked=makeItem(crud, crudSource);
                if(!crud->crudBody(crudMaked).crudify())
                    return this->lr(crud->lr());

                __return<<crud->lr().resultVariant();
            }
        }
    }
    switch (__return.count()) {
    case 0:
        return this->lr().clear();
    default:
        auto vPage=__return.first().toHash();
        return this->lr(qvh{{qsl("type"), vPage.value(qsl("type"))}, {qsl("pages"),__return}});
    }
}

}
