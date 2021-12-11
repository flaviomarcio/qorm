#pragma once

#include "./private/p_qorm_model_crud.h"

namespace QOrm{

//!
//! \brief The CRUD class
//!
template <class T=QOrm::Model>
class CRUD : public PrivateQOrm::CRUDBase
{
public:
    //!
    //! \brief CRUD
    //! \param parent
    //!
    Q_INVOKABLE explicit CRUD(QObject *parent = nullptr) : PrivateQOrm::CRUDBase(parent), p_dao(this), p_model(this){
        this->init();
    }

    //!
    //! \brief CRUD
    //! \param crudBody
    //! \param parent
    //!
    explicit CRUD(const QVariant&crudBody, QObject *parent = nullptr)
        :
        PrivateQOrm::CRUDBase(crudBody, parent)
        , p_dao(this)
        , p_model(this)
    {
        this->init();
    }

    //!
    //! \brief crudify
    //! \return
    //!
    virtual ResultValue &crudify(){
        this->p_dto.type(this->type());
        this->p_dto.layout(this->layout());
        return PrivateQOrm::CRUDBase::crudify();
    }

private:
    ModelDao<T> p_dao;
    T p_model;
    QOrm::ModelDto&p_dto=this->dto();

    //!
    //! \brief init
    //!
    void init(){
        this->dto().initDescriptors(&p_model);
        const auto&modelInfo=p_dao.modelRef;
        this->crudName(modelInfo.modelName()).crudDescription(modelInfo.modelDescription());
    }
protected:
    //!
    //! \brief modelInfo
    //! \return
    //!
    virtual const QOrm::ModelInfo&modelInfo()
    {
        const auto&modelInfo=this->p_model.modelInfo();
        return modelInfo;
    }

    //!
    //! \brief search
    //! \param model
    //! \return
    //!
    virtual ResultValue &search(const T&model)
    {
        auto value=model.toMapPKValues();
        VariantUtil util;
        if(!this->options().searchOnEmptyFilter() && util.vIsEmpty(value))
            return this->lr();

        if(!this->p_dao.recordList(value))
            return this->lr(this->p_dao.lr());

        auto vList=this->p_dao.lr().resultList();
        vList=this->p_model.toList(vList);
        return this->lr(vList);
    }

    //!
    //! \brief search
    //! \param value
    //! \return
    //!
    virtual ResultValue &search(const QVariant&value)
    {
        VariantUtil util;
        if(!this->options().searchOnEmptyFilter() && util.vIsEmpty(value))
            return this->lr();

        if(!this->p_dao.recordList(value))
            return this->lr(this->p_dao.lr());

        auto vList=this->p_dao.lr().resultList();
        vList=this->p_model.toList(vList);
        return this->lr(vList);
    }

    //!
    //! \brief search
    //! \return
    //!
    virtual ResultValue &search()
    {
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
                if(header==nullptr)
                    continue;
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
        auto v=map.buildVariant();
        return this->search(v);
    }

    //!
    //! \brief insert
    //! \param model
    //! \return
    //!
    virtual ResultValue &insert(T&model)
    {
        if(!model.isValid())
            return this->lr(model.lr());

        model.autoMakeUuid();

        if(model.isEmptyPK() && !model.uuidSet())
            return this->lr(model.lr());

        if(!this->p_dao.insert(model))
            return this->lr(this->p_dao.lr());

        return this->lr(model.toHash());
    }

    //!
    //! \brief insert
    //! \param value
    //! \return
    //!
    virtual ResultValue &insert(const QVariant&value)
    {
        T model(this, value);
        return this->insert(model);
    }

    //!
    //! \brief insert
    //! \return
    //!
    virtual ResultValue &insert()
    {
        return this->insert(this->source());
    }

    //!
    //! \brief update
    //! \param model
    //! \return
    //!
    virtual ResultValue &update(T&model)
    {
        if(!model.isValid())
            return this->lr(model.lr());

        if(!this->p_dao.update(model))
            return this->lr(this->p_dao.lr());

        return this->lr(model.toHash());
    }

    //!
    //! \brief update
    //! \param value
    //! \return
    //!
    virtual ResultValue &update(const QVariant&value)
    {
        T model(this, value);
        return this->update(model);
    }

    //!
    //! \brief update
    //! \return
    //!
    virtual ResultValue &update()
    {
        return this->update(this->source());
    }

    //!
    //! \brief upsert
    //! \param model
    //! \return
    //!
    virtual ResultValue &upsert(T&model)
    {
        model.autoMakeUuid();

        if(model.isEmptyPK() && !model.uuidSet())
            return this->lr(model.lr());

        if(!model.isValid())
            return this->lr(model.lr());

        if(!this->p_dao.upsert(model))
            return this->lr(this->p_dao.lr());

        return this->lr(model.toHash());
    }

    //!
    //! \brief upsert
    //! \param value
    //! \return
    //!
    virtual ResultValue &upsert(const QVariant&value)
    {
        T model(this, value);
        return this->upsert(model);
    }

    //!
    //! \brief upsert
    //! \return
    //!
    virtual ResultValue &upsert()
    {
        return this->upsert(this->source());
    }

    //!
    //! \brief remove
    //! \param value
    //! \return
    //!
    virtual ResultValue &remove(T&value)
    {
        if(!this->p_dao.remove(value))
            return this->lr(this->p_dao.lr());
        else
            return this->lr().clear();
    }

    //!
    //! \brief remove
    //! \param value
    //! \return
    //!
    virtual ResultValue &remove(const QVariant&value)
    {
        T model(this, value);
        return this->remove(model);
    }

    //!
    //! \brief remove
    //! \return
    //!
    virtual ResultValue &remove(){
        return this->remove(this->source());
    }

    //!
    //! \brief deactivate
    //! \param model
    //! \return
    //!
    virtual ResultValue &deactivate(T&model)
    {
        if(model.isEmptyPK())
            return this->lr();

        if(!this->p_dao.lock(model))
            return this->lr(this->p_dao.lr(model.toHash()));

        if(!model.isValid())
            return this->lr(this->p_dao.lr(model.toHash()));

        if(!this->p_dao.deactivate(model))
            return this->lr(this->p_dao.lr(model.toHash()));

        return this->lr(model.toHash());
    }

    //!
    //! \brief deactivate
    //! \param value
    //! \return
    //!
    virtual ResultValue &deactivate(const QVariant&value)
    {
        T model(this, value);
        return this->deactivate(model);
    }

    //!
    //! \brief deactivate
    //! \return
    //!
    virtual ResultValue &deactivate()
    {
        return this->deactivate(this->source());
    }

    //!
    //! \brief onBefore
    //! \param method
    //! \return
    //!
    auto&onBefore(CRUDBodyActionMethod method)
    {
        auto obj=PrivateQOrm::CRUDBase::onBefore(method);
        return*(dynamic_cast<CRUD<T>*>(obj));
    }

    //!
    //! \brief onSuccess
    //! \param method
    //! \return
    //!
    auto&onSuccess(CRUDBodyActionMethod method)
    {
        auto obj=PrivateQOrm::CRUDBase::onSuccess(method);
        return*(dynamic_cast<CRUD<T>*>(obj));
    }

    //!
    //! \brief onFailed
    //! \param method
    //! \return
    //!
    auto&onFailed(CRUDBodyActionMethod method)
    {
        auto obj=PrivateQOrm::CRUDBase::onFailed(method);
        return*(dynamic_cast<CRUD<T>*>(obj));
    }
};

}
