#pragma once

#include <QVariant>
#include <QVariantHash>
#include "./p_qorm_model_crud_body.h"
#include "./p_qorm_model_action_method.h"
#include "../qorm_model_dto_options.h"
#include "../qorm_model_crud_request.h"
#include "../qorm_model_dao.h"
#include "../qorm_model_dto.h"

namespace QOrm {
typedef std::function<ResultValue&(QOrm::ObjectDb*controller, const QVariant&vBody)> CRUDBodyActionMethod;
typedef QOrm::ModelAction CRUDAction;
}

namespace PrivateQOrm{

//!
//! \brief The CRUDBase class
//!
class Q_ORM_EXPORT CRUDBase : public QOrm::ObjectDb
{
    Q_OBJECT
public:
    QORM_DECLARE_DTO_TYPE(dftNormalForm)
    QORM_DECLARE_DTO_LAYOUT(dflVerticalControls)

    //!
    //! \brief CRUDBase
    //! \param parent
    //!
    Q_INVOKABLE explicit CRUDBase(QObject *parent = nullptr);

    //!
    //! \brief CRUDBase
    //! \param vBody
    //! \param parent
    //!
    explicit CRUDBase(const QVariant &vBody, QObject *parent = nullptr);

    //!
    //! \brief ~CRUDBase
    //!
    Q_INVOKABLE ~CRUDBase();

    //!
    //! \brief options
    //! \return
    //!
    QOrm::ModelDtoOptions&options();

    //!
    //! \brief setOptions
    //! \param options
    //! \return
    //!
    CRUDBase &setOptions(const QOrm::ModelDtoOptions &options);

    //!
    //! \brief resultInfo
    //! \return
    //!
    QStm::ResultInfo&resultInfo();

    //!
    //! \brief setResultInfo
    //! \param resultInfo
    //! \return
    //!
    CRUDBase &setResultInfo(const QStm::ResultInfo&resultInfo);

    //!
    //! \brief makeDefault
    //! \return
    //!
    virtual CRUDBase&makeDefault();

    //!
    //! \brief unMakeDefault
    //! \return
    //!
    virtual CRUDBase&unMakeDefault();

    //!
    //! \brief type
    //! \return
    //!
    virtual QVariant type()const;

    //!
    //! \brief type
    //! \param value
    //! \return
    //!
    virtual CRUDBase&type(const QVariant &value);

    //!
    //! \brief layout
    //! \return
    //!
    virtual QVariant layout()const;

    //!
    //! \brief layout
    //! \param value
    //! \return
    //!
    virtual CRUDBase&layout(const QVariant &value);

    //!
    //! \brief crudName
    //! \return
    //!
    virtual QByteArray crudName()const;

    //!
    //! \brief crudName
    //! \param value
    //! \return
    //!
    virtual CRUDBase&crudName(const QVariant &value);

    //!
    //! \brief crudDescription
    //! \return
    //!
    virtual QByteArray crudDescription()const;

    //!
    //! \brief crudDescription
    //! \param value
    //! \return
    //!
    virtual CRUDBase&crudDescription(const QVariant &value);

    //!
    //! \brief dto
    //! \return
    //!
    virtual QOrm::ModelDto&dto();

    //!
    //! \brief modelInfo
    //! \return
    //!
    virtual const QOrm::ModelInfo &modelInfo();

    //!
    //! \brief crudBody
    //! \param v
    //! \return
    //!
    virtual CRUDBase &crudBody(const QVariant &v);

    //!
    //! \brief strategy
    //! \return
    //!
    virtual QOrm::CRUDStrategy&strategy()const;

    //!
    //! \brief strategy
    //! \param strategy
    //! \return
    //!
    virtual CRUDBase&strategy(const QVariant&strategy);

    //!
    //! \brief source
    //! \return
    //!
    virtual QVariant source()const;

    //!
    //! \brief source
    //! \param value
    //! \return
    //!
    virtual CRUDBase&source(const QVariant&value);

    //!
    //! \brief crudify
    //! \return
    //!
    virtual ResultValue &crudify();

    //!
    //! \brief actionNulls
    //! \return
    //!
    //! remove actions
    virtual CRUDBase&actionNulls();

    //!
    //! \brief actionSearch
    //! \param action
    //! \return
    //!
    virtual CRUDBase&actionSearch(QOrm::ModelAction&action);

    //!
    //! \brief actionInsert
    //! \param action
    //! \return
    //!
    virtual CRUDBase&actionInsert(QOrm::ModelAction&action);

    //!
    //! \brief actionUpsert
    //! \param action
    //! \return
    //!
    virtual CRUDBase&actionUpsert(QOrm::ModelAction&action);

    //!
    //! \brief actionUpdate
    //! \param action
    //! \return
    //!
    virtual CRUDBase&actionUpdate(QOrm::ModelAction&action);

    //!
    //! \brief actionRemove
    //! \param action
    //! \return
    //!
    virtual CRUDBase&actionRemove(QOrm::ModelAction&action);

    //!
    //! \brief actionDeactivate
    //! \param action
    //! \return
    //!
    virtual CRUDBase&actionDeactivate(QOrm::ModelAction&action);

protected:

    //!
    //! \brief search
    //! \return
    //!
    virtual ResultValue &search();

    //!
    //! \brief search
    //! \param value
    //! \return
    //!
    virtual ResultValue &search(const QVariant&value);

    //!
    //! \brief insert
    //! \return
    //!
    virtual ResultValue &insert();

    //!
    //! \brief insert
    //! \param value
    //! \return
    //!
    virtual ResultValue &insert(const QVariant&value);

    //!
    //! \brief update
    //! \return
    //!
    virtual ResultValue &update();

    //!
    //! \brief update
    //! \param value
    //! \return
    //!
    virtual ResultValue &update(const QVariant&value);

    //!
    //! \brief upsert
    //! \return
    //!
    virtual ResultValue &upsert();

    //!
    //! \brief upsert
    //! \param value
    //! \return
    //!
    virtual ResultValue &upsert(const QVariant&value);

    //!
    //! \brief remove
    //! \return
    //!
    virtual ResultValue &remove();

    //!
    //! \brief remove
    //! \param value
    //! \return
    //!
    virtual ResultValue &remove(const QVariant&value);

    //!
    //! \brief deactivate
    //! \return
    //!
    virtual ResultValue &deactivate();

    //!
    //! \brief deactivate
    //! \param value
    //! \return
    //!
    virtual ResultValue &deactivate(const QVariant&value);

    //!
    //! \brief onBefore
    //! \param method
    //! \return
    //!
    CRUDBase&onBefore(QOrm::CRUDBodyActionMethod method);

    //!
    //! \brief onSuccess
    //! \param method
    //! \return
    //!
    CRUDBase&onSuccess(QOrm::CRUDBodyActionMethod method);

    //!
    //! \brief onFailed
    //! \param method
    //! \return
    //!
    CRUDBase&onFailed(QOrm::CRUDBodyActionMethod method);

    //!
    //! \brief canActionSearch
    //! \return
    //!
    virtual ResultValue &canActionSearch();

    //!
    //! \brief canActionInsert
    //! \return
    //!
    virtual ResultValue &canActionInsert();

    //!
    //! \brief canActionUpsert
    //! \return
    //!
    virtual ResultValue &canActionUpsert();

    //!
    //! \brief canActionUpdate
    //! \return
    //!
    virtual ResultValue &canActionUpdate();

    //!
    //! \brief canActionRemove
    //! \return
    //!
    virtual ResultValue &canActionRemove();

    //!
    //! \brief canActionDeactivate
    //! \return
    //!
    virtual ResultValue &canActionDeactivate();

protected:
    //!
    //! \brief doBofore
    //! \return
    //!
    virtual ResultValue&doBofore();

    //!
    //! \brief doSuccess
    //! \return
    //!
    virtual ResultValue&doSuccess();

    //!
    //! \brief doFailed
    //! \return
    //!
    virtual ResultValue&doFailed();

private:
    void*p=nullptr;
};

}

