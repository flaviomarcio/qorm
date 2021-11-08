#include "./qorm_model_crud_block.h"
#include "./qstm_util_variant.h"

namespace QOrm {

    #define dPvt()\
        auto&p = *reinterpret_cast<CRUDBlockPvt*>(this->p)

    class CRUDBlockPvt{
    public:
        ModelDtoOptions options;
        QOrm::ModelDtoResultInfo resultInfo;
        CRUDBlock*parent=nullptr;
        QVariant crudBody;
        QMap<QByteArray, PrivateQOrm::CRUDBase*> crudMap;
        QList<PrivateQOrm::CRUDBase*> crudList;

        explicit CRUDBlockPvt(CRUDBlock*parent):options(parent){
            this->parent=parent;
            //options.setSearchOnEmptyFilter(false);
        }
        virtual ~CRUDBlockPvt(){
        }
    };

    CRUDBlock::CRUDBlock(QObject *parent):QOrm::ObjectDb(parent)
    {
        this->p = new CRUDBlockPvt(this);
    }

    CRUDBlock::CRUDBlock(const QVariant &crudBody, QObject *parent):QOrm::ObjectDb(parent)
    {
        this->p = new CRUDBlockPvt(this);
        dPvt();
        p.crudBody=crudBody;
    }

    CRUDBlock::~CRUDBlock()
    {
        dPvt();
        delete&p;
    }

    ModelDtoOptions &CRUDBlock::options()
    {
        dPvt();
        return p.options;
    }

    ModelDtoResultInfo &CRUDBlock::resultInfo()
    {
        dPvt();
        return p.resultInfo;
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
        dPvt();
        return p.crudBody;
    }

    CRUDBlock &CRUDBlock::crudBody(const QVariant &v)
    {
        dPvt();
        p.crudBody=v;
        return*this;
    }

    CRUDBlock&CRUDBlock::makeBlock(CRUDBlock&crudBlock, const QVariant &crudBody)
    {
        return crudBlock.crudBody(crudBody);
    }

    CRUDBlock &CRUDBlock::clear()
    {
        dPvt();
        p.crudMap.clear();
        for(auto&v:p.crudMap)
            this->remove(v);
        p.crudMap.clear();
        return*this;
    }

    CRUDBlock &CRUDBlock::insert(PrivateQOrm::CRUDBase *crud)
    {
        dPvt();
        this->remove(crud);
        if(crud!=nullptr){
            p.crudMap.insert(crud->crudName(), crud);
            p.crudList<<crud;
        }
        return*this;
    }

    CRUDBlock &CRUDBlock::remove(PrivateQOrm::CRUDBase *crud)
    {
        dPvt();
        if(p.crudMap.contains(crud->crudName())){
            auto _crud=p.crudMap.take(crud->crudName());
            if(_crud!=nullptr && _crud->parent()==this){
                p.crudList.removeOne(_crud);
                delete _crud;
            }
        }
        return*this;
    }

    CRUDBlock &CRUDBlock::remove(const QByteArray &crudName)
    {
        dPvt();
        auto crud=p.crudMap.value(crudName);
        return this->remove(crud);
    }

    ResultValue &CRUDBlock::crudify()
    {
        dPvt();
        Q_DECLARE_VU;
        if(p.crudMap.isEmpty())
            return this->lr().clear().setBadRequest(tr("crud block is empty"));
        else{
            QVariantList __return;
            QVariantHash ___hash, crudHash;
            CRUDBody crudBody(p.crudBody);

            {

                auto vCrudSource=crudBody.source().toHash();
                if(vCrudSource.contains(qsl("crud"))){
                    auto vList=vCrudSource[qsl("crud")].toList();
                    for(auto&v:vList){
                        auto vMap=v.toHash();
                        auto crudName=vMap[qsl("id")].toString().toLower().trimmed();
                        crudHash.insert(crudName, vMap);
                    }
                }
            }

            for(auto&crud:p.crudList){
                const auto crudName=crud->crudName().toLower();
                crud->setOptions(p.options);
                crud->setResultInfo(p.resultInfo);
                crud->unMakeDefault();
                QVariantList crudList;
                QVariant crudSource;
                CRUDBody crudItem(crudBody);

                if(!crudHash.isEmpty()){
                    crudSource=crudHash.value(crudName);
                }

                if(!crudHash.contains(crudName)){
                    crudList<<crudBody;
                }
                else if(vu.vIsList(crudSource)){
                    for(auto&v:crudSource.toList())
                        crudList<<v;
                }
                else if(vu.vIsMap(crudSource)){
                    auto vMap=crudSource.toHash();
                    if(vMap.contains(qsl("id")) && vMap.contains(qsl("items"))){
                        auto list=vMap[qsl("items")].toList();
                        for(auto&v:list)
                            crudList<<CRUDBody(crudBody.strategy(), v);
                    }
                    else{
                        crudList<<crudBody;
                    }
                }
                else{
                    crudList<<crudBody;
                }


                auto makeItem=[&__return](PrivateQOrm::CRUDBase*crud, const QVariant&crudSource){
                    CRUDBody crudRecord(crudSource);

                    QVariantHash lastCrud=__return.isEmpty()?QVariantHash():__return.last().toHash();
                    if(lastCrud.isEmpty())
                        crudRecord=CRUDBody(crudSource);
                    else{
                        QVariantHash record, vHash;
                        if(!lastCrud.contains(qsl("crud")))
                            record=lastCrud;
                        else{
                            auto crud=lastCrud[qsl("crud")].toList();
                            if(!crud.isEmpty()){
                                vHash = crud.first().toHash();
                                if(vHash.contains(qsl("items")))
                                    record=vHash;
                                else{
                                    crud=vHash[qsl("items")].toList();
                                    record=crud.isEmpty()?QVariantHash():crud.first().toHash();
                                }
                            }
                        }
                        const auto&modelInfo=crud->modelInfo();
                        auto crudSourceFields=modelInfo.toForeign(crudRecord.source(), record);
                        crudRecord=CRUDBody(crudRecord.strategy(), crudSourceFields);
                    }
                    return crudRecord;
                };


                if(crudBody.isStrategy(QOrm::CRUDStrategy::Remove) || crudBody.isStrategy(QOrm::CRUDStrategy::Deactivate)){
                    auto crudListCopy=crudList;
                    while(!crudListCopy.isEmpty()){
                        const auto&crudSource=crudListCopy.takeLast();
                        CRUDBody crudMaked(crudSource);
                        if(!crudListCopy.isEmpty()){
                            crudMaked=makeItem(crud, crudSource);
                        }
                        if(!crud->crudBody(crudMaked).crudify())
                            return this->lr(crud->lr());
                        else
                            __return<<crud->lr().resultVariant();
                    }
                }
                else {
                    for(auto&crudSource:crudList){
                        auto crudMaked=makeItem(crud, crudSource);
                        if(!crud->crudBody(crudMaked).crudify())
                            return this->lr(crud->lr());
                        else{
                            if(___hash.isEmpty())
                                ___hash=crud->lr().resultHash();
                            __return<<crud->lr().resultVariant();
                        }
                    }
                }

            }
            ___hash[qsl("crud")]=__return;
            return this->lr(___hash);
        }
    }

}
