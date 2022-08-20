#pragma once

#include "./private/p_qorm_model_crud.h"
#include "./private/p_qorm_model_info.h"
#include "./qorm_model_dao.h"

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
    {
        this->init();
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
        this->init();
    }

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
        if(modelInfo.descritor()==nullptr)
            return {};
        return true;
    }

    //!
    //! \brief host
    //! \return
    //!
    Host &host()
    {
        return p_dao.host();
    }

    T &model()const
    {
        this->p_model;
    }

private:
    ModelDao<T> p_dao;
    T p_model;
    QOrm::ModelDto &p_dto=this->dto();

    //!
    //! \brief init
    //!
    void init()
    {
        p_dto.initDescriptors(&p_model);
        const auto &modelInfo=this->modelInfo();
        this->name(modelInfo.name()).description(modelInfo.description());
    }
protected:

    //!
    //! \brief insert
    //! \param model
    //! \return
    //!
    virtual ResultValue &create(T &model)
    {
        Q_UNUSED(model)
        return this->lr();
    }

    //!
    //! \brief insert
    //! \param value
    //! \return
    //!
    virtual ResultValue &create(const QVariant &value)
    {
        T model(this, value);
        return this->create(model);
    }

    //!
    //! \brief insert
    //! \return
    //!
    virtual ResultValue &create()
    {
        return this->create(this->source());
    }

    //!
    //! \brief search
    //! \param model
    //! \return
    //!
    virtual ResultValue &search(const T &model)
    {
        auto value=model.toPKValues();
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
    virtual ResultValue &search(const QVariant &value)
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
        static const auto __operator="operator";
        static const auto __format="format";

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
        for(auto&v:vList){
            auto mapSource=v.toHash();
            T model(mapSource);
            SearchParameters map;
            if(!mapSource.isEmpty()){
                const QOrm::ModelInfo &modelInfo=this->modelInfo();
                const auto &propertyShortVsTable=modelInfo.propertyShortVsTable();
                auto vHash=model.toHash();
                QHashIterator<QString, QVariant> i(vHash);
                while (i.hasNext()) {
                    i.next();
                    const auto header=&this->p_dto.headers().item(i.key());
                    if(header)
                        continue;
                    auto vHash=header->filtrableStrategy().toHash();
                    auto keywordOperator=vHash.value(__operator);
                    QString format=vHash.value(__format).toString().trimmed();
                    QVariant v_value;
                    if(format.contains(QStringLiteral("%1")))
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
            if(!this->search(map.buildVariant()))
                return this->lr();

            for(auto&v:this->lr().resultList())
                vListReturn.append(v);
        }
        return this->lr(vListReturn);
    }

    //!
    //! \brief insert
    //! \param model
    //! \return
    //!
    virtual ResultValue &insert(T &model)
    {
        if(!model.isValid())
            return this->lr(model.lr());

        model.autoMakeUuid();
        model.uuidSet();
        model.datetimeSet();

        if(model.isEmptyPK() && !model.uuidSet())
            return this->lr(model.lr());

        if(!this->p_dao.insert(model))
            return this->lr(this->p_dao.lr());
        else
            this->generatedRecords().append(model.toHash());

        return this->lr(model.toHash());
    }

    //!
    //! \brief insert
    //! \param value
    //! \return
    //!
    virtual ResultValue &insert(const QVariant &value)
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
    virtual ResultValue &update(T &model)
    {
        model.uuidSet();
        model.datetimeSet();
        if(!model.isValid())
            return this->lr(model.lr());

        if(!this->p_dao.update(model))
            return this->lr(this->p_dao.lr());
        else
            this->generatedRecords().append(model.toHash());

        return this->lr(model.toHash());
    }

    //!
    //! \brief update
    //! \param value
    //! \return
    //!
    virtual ResultValue &update(const QVariant &value)
    {
        T model(this, value);
        if(!this->p_dao.reload(model))
            return this->lr(this->p_dao.lr());

        model.mergeFrom(value);
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

    //!
    //! \brief upsert
    //! \param value
    //! \return
    //!
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
        for(auto&v:vList){
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
    virtual ResultValue &remove(T &value)
    {
        if(!this->p_dao.remove(value))
            return this->lr(this->p_dao.lr());

        return this->lr().clear();
    }

    //!
    //! \brief remove
    //! \param value
    //! \return
    //!
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

    //!
    //! \brief deactivate
    //! \param model
    //! \return
    //!
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
    auto &onBefore(CRUDBodyActionMethod method)
    {
        auto obj=PrivateQOrm::CRUDBase::onBefore(method);
        return*(dynamic_cast<CRUD<T>*>(obj));
    }

    //!
    //! \brief onSuccess
    //! \param method
    //! \return
    //!
    auto &onSuccess(CRUDBodyActionMethod method)
    {
        auto obj=PrivateQOrm::CRUDBase::onSuccess(method);
        return*(dynamic_cast<CRUD<T>*>(obj));
    }

    //!
    //! \brief onFailed
    //! \param method
    //! \return
    //!
    auto &onFailed(CRUDBodyActionMethod method)
    {
        auto obj=PrivateQOrm::CRUDBase::onFailed(method);
        return*(dynamic_cast<CRUD<T>*>(obj));
    }
};

}
