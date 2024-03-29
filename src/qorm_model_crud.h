#pragma once

#include "./private/p_qorm_model_crud.h"
#include "./private/p_qorm_model_info.h"
#include "./qorm_model_dao.h"
#include "../../qstm/src/qstm_util_variant.h"
#include "../../qstm/src/qstm_meta_enum.h"

namespace QOrm{

const auto __operator="operator";

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
    Q_INVOKABLE explicit CRUD(QObject *parent = nullptr)
      :
        PrivateQOrm::CRUDBase{parent}
      , p_dao{this}
      , p_model{this}
    {

    }

    //!
    //! \brief CRUD
    //! \param crudBody
    //! \param parent
    //!
    explicit CRUD(const QVariant &crudBody, QObject *parent = nullptr)
        :
          PrivateQOrm::CRUDBase{crudBody, parent}
        , p_dao{this}
        , p_model{this}
    {

    }

    //!
    //! \brief model
    //! \return
    //!
    T &model()const
    {
        this->p_model;
    }

    //!
    //! \brief modelInfo
    //! \return
    //!
    virtual const QOrm::ModelInfo &modelInfo()const
    {
        return p_dao.modelInfo();
    }

    //!
    //! \brief isValid
    //! \return
    //!
    virtual bool isValid()const
    {
        const ModelInfo &modelInfo=this->modelInfo();
        return modelInfo.propertyDescriptors().isEmpty();
    }

    //!
    //! \brief actionStart
    //! \return
    //!
    virtual ActionStart actionStart()const
    {
        static const auto __actionStart="actionStart";

        if(!this->isValid())
            return asCREATE;

        const ModelInfo &modelInfo=this->modelInfo();
        const auto &propertyDescriptors=modelInfo.propertyDescriptors();

        QStm::MetaEnum<ActionStart> action=propertyDescriptors.value(__actionStart);
        return action.type();
    }

    virtual CRUDBase &setResultInfo(const QStm::ResultInfo &resultInfo)
    {
        this->p_dao.lr().resultInfo(resultInfo);
        PrivateQOrm::CRUDBase::setResultInfo(resultInfo);
        return *this;
    }

    //!
    //! \brief host
    //! \return
    //!
    const QOrm::Host &host()const
    {
        return this->dto().host();
    }

    //!
    //! \brief setHost
    //! \param newHost
    //! \return
    //!
    CRUDBase &setHost(const QOrm::Host &newHost)
    {
        this->dto().host(newHost);
        return CRUDBase::setHost(newHost);
    }

    //!
    //! \brief beforeCrudify
    //! \return
    //!
    virtual bool beforeCrudify()
    {
        this->dto().initDescriptors(&p_model);
        const auto &modelInfo=this->modelInfo();
        this->name(modelInfo.name()).description(modelInfo.description());
        return true;
    }

private:
    ModelDao<T> p_dao;
    T p_model;
protected:

    //!
    //! \brief init
    //! \param model
    //! \return
    //!
    virtual ResultValue &init(T &model)
    {
        Q_UNUSED(model)
        auto value=model.toPKValuesValid();
        return this->init(value);
    }
    virtual ResultValue &init(const QVariant &value)
    {
        Q_DECLARE_VU;
        if(!this->options().searchOnEmptyFilter() && vu.vIsEmpty(value))
            return this->lr();

        if(!this->p_dao.recordCount(value))
            return this->lr(this->p_dao.lr());

        return this->lr(this->p_dao.lr().resultVariant());
    }
    virtual ResultValue &init()
    {
        return this->init(this->source());
    }

    //!
    //! \brief create
    //! \param model
    //! \return
    //!
    virtual ResultValue &create(T &model)
    {
        Q_UNUSED(model)
        return this->lr();
    }
    virtual ResultValue &create(const QVariant &value)
    {
        T model(this, value);
        return this->create(model);
    }
    virtual ResultValue &create()
    {
        return this->create(this->source());
    }

    //!
    //! \brief edit
    //! \param model
    //! \return
    //!
    virtual ResultValue &edit(const T &model)
    {
        return this->search(model);
    }
    virtual ResultValue &edit(const QVariant &value)
    {
        return this->search(value);
    }
    virtual ResultValue &edit()
    {
        return this->lr(this->search());
    }

    //!
    //! \brief search
    //! \param model
    //! \return
    //!
    virtual ResultValue &search(const T &model)
    {
        auto value=model.toPKValuesValid();
        Q_DECLARE_VU;
        if(!this->options().searchOnEmptyFilter() && vu.vIsEmpty(value))
            return this->lr();

        if(!this->p_dao.recordList(value))
            return this->lr(this->p_dao.lr());

        auto vList=this->p_dao.lr().resultList();
        vList=this->p_model.toList(vList);
        return this->lr(vList);
    }
    virtual ResultValue &search(const QVariant &value)
    {
        Q_DECLARE_VU;
        if(!this->options().searchOnEmptyFilter() && vu.vIsEmpty(value))
            return this->lr();

        if(!this->p_dao.recordList(value))
            return this->lr(this->p_dao.lr());

        auto vList=this->p_dao.lr().resultList();
        vList=this->p_model.toList(vList);
        return this->lr(vList);
    }
    virtual ResultValue &search()
    {
        QVariantList vList;
        switch (this->source().typeId()) {
        case QMetaType::QVariantList:
        case QMetaType::QStringList:
            vList=this->source().toList();
            break;
        case QMetaType::QVariantHash:
        case QMetaType::QVariantMap:
            vList.append(this->source());
            break;
        default:
            vList.append(QVariant{});
            break;
        }

        QVariantList vListReturn;
        for(auto &v:vList){
            auto filters=this->modelInfo().extractProperties(v.toHash());
            if(!this->search(filters))
                return this->lr();

            for(auto &v:this->lr().resultList())
                vListReturn.append(v);
        }
        return this->lr(vListReturn);
    }

    //!
    //! \brief upsert
    //! \param model
    //! \return
    //!
    virtual ResultValue &upsert(T &model)
    {
        model.autoMakeUuid();
        model.uuidSet();
        model.datetimeSet();
        if(model.isEmptyPK())
            return this->lr(model.lr());

        if(!model.isValid())
            return this->lr(model.lr());

        if(!this->p_dao.upsert(model))
            return this->lr(this->p_dao.lr());
        else
            this->generatedRecords().append(model.toHash());

        return this->lr(model.toHash());
    }
    virtual ResultValue &upsert(const QVariant &value)
    {
        QVariantList vList;
        switch (value.typeId()) {
        case QMetaType::QVariantList:
        case QMetaType::QStringList:
            vList=value.toList();
            break;
        default:
            vList.append(value);
            break;
        }
        for(auto &v:vList){
            T model(this, v);
            if(!this->p_dao.reload(model) && this->lr().isNotOk())
                return this->lr(this->p_dao.lr());
            model.mergeFrom(v);
            if(!this->upsert(model))
                return this->lr();
            v=model.toHash();
        }
        return this->lr(vList);
    }
    virtual ResultValue &upsert()
    {
        return this->upsert(this->source());
    }

    //!
    //! \brief remove
    //! \param value
    //! \return
    //!
    virtual ResultValue &remove(T &value)
    {
        auto &modelInfo=this->modelInfo();

        if(!modelInfo.propertyDeactivateField().isEmpty()){
            if(!this->p_dao.deactivate(value))
                return this->lr(this->p_dao.lr());
        }
        else{
            if(!this->p_dao.remove(value))
                return this->lr(this->p_dao.lr());
        }

        return this->lr().clear();
    }
    virtual ResultValue &remove(const QVariant &value)
    {
        T model(this, value);
        return this->remove(model);
    }

    //!
    //! \brief remove
    //! \return
    //!
    virtual ResultValue &remove()
    {
        return this->remove(this->source());
    }
    virtual ResultValue &deactivate(T &model)
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
    virtual ResultValue &deactivate(const QVariant &value)
    {
        T model(this, value);
        return this->deactivate(model);
    }
    virtual ResultValue &deactivate()
    {
        return this->deactivate(this->source());
    }

    //!
    //! \brief onBefore
    //! \param method
    //! \return
    //!
    auto &onBefore(CRUDBodyActionMethod method)
    {
        auto obj=PrivateQOrm::CRUDBase::onBefore(method);
        return *(dynamic_cast<CRUD<T>*>(obj));
    }

    //!
    //! \brief onSuccess
    //! \param method
    //! \return
    //!
    auto &onSuccess(CRUDBodyActionMethod method)
    {
        auto obj=PrivateQOrm::CRUDBase::onSuccess(method);
        return *(dynamic_cast<CRUD<T>*>(obj));
    }

    //!
    //! \brief onFailed
    //! \param method
    //! \return
    //!
    auto &onFailed(CRUDBodyActionMethod method)
    {
        auto obj=PrivateQOrm::CRUDBase::onFailed(method);
        return *(dynamic_cast<CRUD<T>*>(obj));
    }
};

}
