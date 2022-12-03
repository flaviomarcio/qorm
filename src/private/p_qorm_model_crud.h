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
#include "../../../qrmk/include/QRmk"

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
    QORM_MODEL_DECLARE_FORM_ENUMS

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
    //! \brief actionStart
    //! \return
    //!
    virtual ActionStart actionStart()const;

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
    //! \brief maker
    //! \return
    //!
    QRmk::Maker &maker();

    //!
    //! \brief host
    //! \return
    //!
    virtual const QOrm::Host &host()const;
    virtual CRUDBase &host(const QOrm::Host &newHost);
    virtual CRUDBase &setHost(const QOrm::Host &newHost);

    //!
    //! \brief type
    //! \return
    //!
    const QVariant type() const;
    CRUDBase &type(const FormType &newType){ return this->setType(newType);}
    CRUDBase &setType(const FormType &newType);
    CRUDBase &resetType();

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
    //! \brief uuid
    //! \return
    //!
    virtual const QUuid &uuid();

    //!
    //! \brief owner
    //! \return
    //!
    virtual const QByteArray &owner();
    virtual CRUDBase &owner(const QVariant &value);

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
    //! \brief crudBody
    //! \param v
    //! \return
    //!
    virtual CRUDBase &crudBody(const QVariant &v);

    //!
    //! \brief strategy
    //! \return
    //!
    virtual QOrm::CRUDTypes::Strategy strategy()const;

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
    //! \brief beforeCrudify
    //! \return
    //!
    virtual bool beforeCrudify();

    //!
    //! \brief afterCrudify
    //! \return
    //!
    virtual bool afterCrudify();

    //!
    //! \brief crudify
    //! \return
    //!
    virtual ResultValue &crudify();

    //!
    //! \brief actionsNulls
    //! \return
    //!
    //! remove actions
    virtual CRUDBase &actionsNulls();

    //!
    //! \brief actionsSearch
    //! \return
    //!
    virtual CRUDBase &actionsSearch();

    //!
    //! \brief actionSearch
    //! \param action
    //! \return
    //!
    virtual CRUDBase &actionSearch(QOrm::ModelAction &action);

    //!
    //! \brief actionCreate
    //! \param action
    //! \return
    //!
    virtual CRUDBase &actionCreate(QOrm::ModelAction &action);

    //!
    //! \brief actionUpsert
    //! \param action
    //! \return
    //!
    virtual CRUDBase &actionUpsert(QOrm::ModelAction &action);


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

    //!
    //! \brief actionApply
    //! \param action
    //! \return
    //!
    virtual CRUDBase &actionApply(QOrm::ModelAction &action);

    //!
    //! \brief actionExecute
    //! \param action
    //! \return
    //!
    virtual CRUDBase &actionExecute(QOrm::ModelAction &action);

    //!
    //! \brief actionFinalize
    //! \param action
    //! \return
    //!
    virtual CRUDBase &actionFinalize(QOrm::ModelAction &action);

    //!
    //! \brief actionPrint
    //! \param action
    //! \return
    //!
    virtual CRUDBase &actionPrint(QOrm::ModelAction &action);

signals:
    void typeChanged();

protected:

    //!
    //! \brief create
    //! \return
    //!
    virtual ResultValue &create();

    //!
    //! \brief create
    //! \param value
    //! \return
    //!
    virtual ResultValue &create(const QVariant &value);

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
    //! \brief execute
    //! \return
    //!
    virtual ResultValue &execute();

    //!
    //! \brief execute
    //! \param value
    //! \return
    //!
    virtual ResultValue &execute(const QVariant &value);

    //!
    //! \brief finalize
    //! \return
    //!
    virtual ResultValue &finalize();

    //!
    //! \brief finalize
    //! \param value
    //! \return
    //!
    virtual ResultValue &finalize(const QVariant &value);

    //!
    //! \brief print
    //! \return
    //!
    virtual ResultValue &print();

    //!
    //! \brief print
    //! \param value
    //! \return
    //!
    virtual ResultValue &print(const QVariant &value);

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
    //! \brief canActionCreate
    //! \return
    //!
    virtual ResultValue &canActionCreate();

    //!
    //! \brief canActionSearch
    //! \return
    //!
    virtual ResultValue &canActionSearch();

    //!
    //! \brief canActionUpsert
    //! \return
    //!
    virtual ResultValue &canActionUpsert();

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

    //!
    //! \brief canActionApply
    //! \return
    //!
    virtual ResultValue &canActionApply();

    //!
    //! \brief canActionExecute
    //! \return
    //!
    virtual ResultValue &canActionExecute();

    //!
    //! \brief canActionFinalize
    //! \return
    //!
    virtual ResultValue &canActionFinalize();

    //!
    //! \brief canActionPrint
    //! \return
    //!
    virtual ResultValue &canActionPrint();

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

