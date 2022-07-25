#pragma once

#include <QVariant>
#include <QVariantHash>
#include "./p_qorm_model_info.h"
#include "./p_qorm_model_dao.h"
#include "./p_qorm_model_action_method.h"
#include "./p_qorm_model_dto_host.h"
#include "../qorm_model_dto_options.h"
#include "../qorm_model_dto.h"
#include "../qorm_model_crud_types.h"
//#include "./p_qorm_model_crud_body.h"
//#include "../qorm_model_crud_request.h"
//#include "../qorm_model_dao.h"

namespace QOrm {
typedef std::function<ResultValue &(QOrm::ObjectDb *controller, const QVariant &vBody)> CRUDBodyActionMethod;
typedef QOrm::ModelAction CRUDAction;
}

namespace PrivateQOrm{
class CRUDBasePvt;
//!
//! \brief The CRUDBase class
//!
class Q_ORM_EXPORT CRUDBase : public QOrm::ObjectDb
{
    Q_OBJECT
public:
    QORM_DECLARE_FORM_ENUMS

    QORM_DECLARE_DTO_TYPE(FormType::RegisterForm)
    QORM_DECLARE_DTO_LAYOUT(FormLayout::Vertical)

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
    //! \brief isValid
    //! \return
    //!
    virtual bool isValid()const;

    //!
    //! \brief modelInfo
    //! \return
    //!
    virtual const QOrm::ModelInfo &modelInfo()const;

    //!
    //! \brief dao
    //! \return
    //!
    ModelDao &dao();

    //!
    //! \brief host
    //! \return
    //!
    QOrm::Host &host();

    //!
    //! \brief options
    //! \return
    //!
    QOrm::ModelDtoOptions &options();

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
    QStm::ResultInfo &resultInfo();

    //!
    //! \brief setResultInfo
    //! \param resultInfo
    //! \return
    //!
    CRUDBase &setResultInfo(const QStm::ResultInfo &resultInfo);

    //!
    //! \brief clear
    //! \return
    //!
    CRUDBase &clear();

    //!
    //! \brief clean
    //! \return
    //!
    CRUDBase &clean();

    //!
    //! \brief type
    //! \return
    //!
    virtual FormType type()const;

    //!
    //! \brief type
    //! \param value
    //! \return
    //!
    virtual CRUDBase &type(const FormType &value);

    //!
    //! \brief layout
    //! \return
    //!
    virtual FormLayout layout()const;

    //!
    //! \brief layout
    //! \param value
    //! \return
    //!
    virtual CRUDBase &layout(const FormLayout &value);

    //!
    //! \brief uuid
    //! \return
    //!
    virtual const QUuid &uuid();

    //!
    //! \brief name
    //! \return
    //!
    virtual const QByteArray &name();

    //!
    //! \brief name
    //! \param value
    //! \return
    //!
    virtual CRUDBase &name(const QVariant &value);

    //!
    //! \brief description
    //! \return
    //!
    virtual QByteArray &description()const;

    //!
    //! \brief description
    //! \param value
    //! \return
    //!
    virtual CRUDBase &description(const QVariant &value);

    //!
    //! \brief dto
    //! \return
    //!
    virtual QOrm::ModelDto &dto();

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
    virtual QOrm::CRUDStrategy &strategy()const;

    //!
    //! \brief strategy
    //! \param strategy
    //! \return
    //!
    virtual CRUDBase &strategy(const QVariant &strategy);

    //!
    //! \brief source
    //! \return
    //!
    virtual QVariant &source()const;

    //!
    //! \brief source
    //! \param value
    //! \return
    //!
    virtual CRUDBase &source(const QVariant &value);

    //!
    //! \brief generatedRecords
    //! \return
    //!
    QVariantList &generatedRecords()const;

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
    virtual CRUDBase &actionNulls();

    //!
    //! \brief actionSearch
    //! \param action
    //! \return
    //!
    virtual CRUDBase &actionSearch(QOrm::ModelAction &action);

    //!
    //! \brief actionInsert
    //! \param action
    //! \return
    //!
    virtual CRUDBase &actionInsert(QOrm::ModelAction &action);

    //!
    //! \brief actionUpsert
    //! \param action
    //! \return
    //!
    virtual CRUDBase &actionUpsert(QOrm::ModelAction &action);

    //!
    //! \brief actionUpdate
    //! \param action
    //! \return
    //!
    virtual CRUDBase &actionUpdate(QOrm::ModelAction &action);

    //!
    //! \brief actionInsertUpsertUpdate
    //! \param action
    //! \return
    //!
    virtual CRUDBase &actionInsertUpsertUpdate(QOrm::ModelAction &action);

    //!
    //! \brief actionRemove
    //! \param action
    //! \return
    //!
    virtual CRUDBase &actionRemove(QOrm::ModelAction &action);

    //!
    //! \brief actionDeactivate
    //! \param action
    //! \return
    //!
    virtual CRUDBase &actionDeactivate(QOrm::ModelAction &action);

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
    virtual ResultValue &search(const QVariant &value);

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
    virtual ResultValue &insert(const QVariant &value);

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
    virtual ResultValue &update(const QVariant &value);

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
    virtual ResultValue &upsert(const QVariant &value);

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
    virtual ResultValue &remove(const QVariant &value);

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
    virtual ResultValue &deactivate(const QVariant &value);

    //!
    //! \brief onBefore
    //! \param method
    //! \return
    //!
    CRUDBase &onBefore(QOrm::CRUDBodyActionMethod method);

    //!
    //! \brief onSuccess
    //! \param method
    //! \return
    //!
    CRUDBase &onSuccess(QOrm::CRUDBodyActionMethod method);

    //!
    //! \brief onFailed
    //! \param method
    //! \return
    //!
    CRUDBase &onFailed(QOrm::CRUDBodyActionMethod method);

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
    virtual ResultValue &doBofore();

    //!
    //! \brief doSuccess
    //! \return
    //!
    virtual ResultValue &doSuccess();

    //!
    //! \brief doFailed
    //! \return
    //!
    virtual ResultValue &doFailed();

private:
    CRUDBasePvt *p=nullptr;
};

}

