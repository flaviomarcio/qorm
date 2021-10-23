#pragma once

#include "./private/p_qorm_model_crud.h"

namespace QOrm{

    /**
     * @brief The CRUD class
     */
    template <class T=QOrm::Model>
    class CRUD : public PrivateQOrm::CRUDBase
    {
    public:
        Q_INVOKABLE explicit CRUD(QObject *parent = nullptr) : PrivateQOrm::CRUDBase(parent), p_dao(this), p_model(this){
            this->init();
        }

        Q_INVOKABLE explicit CRUD(const QVariant&crudBody, QObject *parent = nullptr)
            :
            PrivateQOrm::CRUDBase(crudBody, parent)
            , p_dao(this)
            , p_model(this)
        {
            this->init();
        }

        virtual ResultValue &crudify(){
            this->p_dto.type(this->type());
            this->p_dto.layout(this->layout());
            return PrivateQOrm::CRUDBase::crudify();
        }

    private:
        ModelDao<T> p_dao;
        T p_model;
        QOrm::ModelDto&p_dto=this->dto();
        void init(){
            this->dto().initDescriptors(&p_model);
            const auto&modelInfo=p_dao.modelRef;
            this->crudName(modelInfo.modelName()).crudDescription(modelInfo.modelDescription());
        }
    protected:
        virtual const QOrm::ModelInfo&modelInfo(){
            const auto&modelInfo=this->p_model.modelInfo();
            return modelInfo;
        }

        /**
         * @brief search
         * @param value
         * @return
         */
        virtual ResultValue &search(const T&model){
            auto value=model.toMapPKValues();
            VariantUtil util;
            if(!this->options().searchOnEmptyFilter() && util.vIsEmpty(value))
                return this->lr();
            else if(!this->p_dao.recordList(value))
                return this->lr(this->p_dao.lr());
            else{
                auto vList=this->p_dao.lr().resultList();
                vList=this->p_model.toList(vList);
                return this->lr(vList);
            }
        }
        virtual ResultValue &search(const QVariant&value){
            VariantUtil util;
            if(!this->options().searchOnEmptyFilter() && util.vIsEmpty(value))
                return this->lr();
            else if(!this->p_dao.recordList(value))
                return this->lr(this->p_dao.lr());
            else{
                auto vList=this->p_dao.lr().resultList();
                vList=this->p_model.toList(vList);
                return this->lr(vList);
            }
        }
        virtual ResultValue &search(){
            T model(this->source());
            auto mapSource=this->source().toHash();
            SearchParameters map;
            if(!mapSource.isEmpty()){
                const QOrm::ModelInfo&modelInfo=p_dao.modelRef;
                const auto&propertyShortVsTable=modelInfo.propertyShortVsTable();
                QHashIterator<QString, QVariant> i(model.toHash());
                while (i.hasNext()) {
                    i.next();
                    auto header=this->p_dto.headers().get(i.key());
                    if(header!=nullptr){
                        auto vHash=header->filtrableStrategy();
                        auto keywordOperator=vHash.value(qsl("operator"));
                        QString format=vHash.value(qsl("format")).toString().trimmed();
                        QVariant v_value;
                        if(format.contains(qsl("%1")))
                            v_value=format.arg(i.value().toString());
                        else
                            v_value=i.value();
                        if(mapSource.contains(i.key())){
                            auto v_key=propertyShortVsTable.value(i.key());
                            auto a=SqlParserItem::createObject(v_key);
                            auto b=SqlParserItem::createValue(v_value);
                            map.insert(a, b, keywordOperator);
                        }
                    }
                }
            }
            auto v=map.buildVariant();
            return this->search(v);
        }


        /**
         * @brief insert
         * @param value
         * @return
         */
        virtual ResultValue &insert(T&model){
            if(!model.isValid())
                return this->lr(model.lr());
            else if(!this->p_dao.insert(model))
                return this->lr(this->p_dao.lr());
            else
                return this->lr(model.toHash());
        }
        virtual ResultValue &insert(const QVariant&value){
            T model(this, value);
            return this->insert(model);
        }
        virtual ResultValue &insert(){
            return this->insert(this->source());
        }

        /**
         * @brief update
         * @param value
         * @return
         */

        virtual ResultValue &update(T&model){
            if(!model.isValid())
                return this->lr(model.lr());
            else if(!this->p_dao.update(model))
                return this->lr(this->p_dao.lr());
            else
                return this->lr(model.toHash());
        }
        virtual ResultValue &update(const QVariant&value){
            T model(this, value);
            return this->update(model);
        }
        virtual ResultValue &update(){
            return this->update(this->source());
        }

        /**
         * @brief upsert
         * @param value
         * @return
         */
        virtual ResultValue &upsert(T&model){
            if(!model.uuidSet())
                return this->lr(model.lr());
            else if(!model.isValid())
                return this->lr(model.lr());
            else if(!this->p_dao.upsert(model))
                return this->lr(this->p_dao.lr());
            else
                return this->lr(model.toHash());
        }
        virtual ResultValue &upsert(const QVariant&value){
            T model(this, value);
            return this->upsert(model);
        }
        virtual ResultValue &upsert(){
            return this->upsert(this->source());
        }

        /**
         * @brief remove
         * @param value
         * @return
         */
        virtual ResultValue &remove(T&value){
            if(!this->p_dao.remove(value))
                return this->lr(this->p_dao.lr());
            else
                return this->lr().clear();
        }
        virtual ResultValue &remove(const QVariant&value){
            T model(this, value);
            return this->remove(model);
        }
        virtual ResultValue &remove(){
            return this->remove(this->source());
        }

        /**
         * @brief deactivate
         * @param model
         * @return
         */
        virtual ResultValue &deactivate(T&model){
            if(model.isEmptyPK())
                return this->lr();
            else if(!this->p_dao.lock(model))
                return this->lr(this->p_dao.lr(model.toHash()));
            else if(!model.isValid())
                return this->lr(this->p_dao.lr(model.toHash()));
            else if(!this->p_dao.deactivate(model))
                return this->lr(this->p_dao.lr(model.toHash()));
            else
                return this->lr(model.toHash());
        }
        virtual ResultValue &deactivate(const QVariant&value){
            T model(this, value);
            return this->deactivate(model);
        }
        virtual ResultValue &deactivate(){
            return this->deactivate(this->source());
        }

        auto&onBefore(CRUDBodyActionMethod method){
            auto obj=PrivateQOrm::CRUDBase::onBefore(method);
            return*(dynamic_cast<CRUD<T>*>(obj));
        }
        auto&onSuccess(CRUDBodyActionMethod method){
            auto obj=PrivateQOrm::CRUDBase::onSuccess(method);
            return*(dynamic_cast<CRUD<T>*>(obj));
        }
        auto&onFailed(CRUDBodyActionMethod method){
            auto obj=PrivateQOrm::CRUDBase::onFailed(method);
            return*(dynamic_cast<CRUD<T>*>(obj));
        }
    };

}
