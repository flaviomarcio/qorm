#pragma once

#include <QVariant>
#include <QVariantHash>
#include "./p_qorm_model_action_method.h"
#include "./p_qorm_model_info.h"
#include "../qorm_model_dto.h"
#include "../qorm_model_dto_options.h"


namespace PrivateQOrm{
class ModelReportBasePvt;
//TODO CREATE DOCUMENTATION
class Q_ORM_EXPORT ModelReportBase : public QOrm::ObjectDb
{
    Q_OBJECT
public:
    QORM_MODEL_DECLARE_FORM_ENUMS
    QORM_DECLARE_DTO_TYPE(FormType::ReportForm)

    //!
    //! \brief ModelReportBase
    //! \param parent
    //!
    Q_INVOKABLE explicit ModelReportBase(QObject *parent = nullptr);

    //!
    //! \brief ModelReportBase
    //! \param reportBody
    //! \param parent
    //!
    explicit ModelReportBase(const QVariant &reportBody, QObject *parent = nullptr);

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
    ModelReportBase &setOptions(const QOrm::ModelDtoOptions &options);

    //!
    //! \brief type
    //! \return
    //!
    virtual FormType type()const;
    virtual ModelReportBase &type(const FormType &value);

    //!
    //! \brief reportName
    //! \return
    //!
    virtual QUuid &uuid()const;
    virtual ModelReportBase &uuid(const QUuid &value);

    //!
    //! \brief reportName
    //! \return
    //!
    virtual QByteArray name()const;
    virtual ModelReportBase &name(const QVariant &value);

    //!
    //! \brief reportDescription
    //! \return
    //!
    virtual QByteArray description()const;
    virtual ModelReportBase &description(const QVariant &value);

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
    //! \brief reportBody
    //! \param v
    //! \return
    //!
    virtual ModelReportBase &reportBody(const QVariant &v);

    //!
    //! \brief strategy
    //! \return
    //!
    virtual QVariant strategy()const;
    virtual ModelReportBase &strategy(const QVariant &strategy);

    //!
    //! \brief source
    //! \return
    //!
    virtual QVariant source()const;
    virtual ModelReportBase &source(const QVariant &value);

    //!
    //! \brief reportfy
    //! \return
    //!
    virtual ResultValue &reportfy();

    //!
    //! \brief actionSearch
    //! \param action
    //! \return
    //!
    virtual ModelReportBase &actionSearch(QOrm::ModelAction&action);

protected:

    //!
    //! \brief search
    //! \return
    //!
    virtual ResultValue &search();
    virtual ResultValue &search(const QVariant &value);

    //!
    //! \brief onBefore
    //! \param method
    //! \return
    //!
    ModelReportBase &onBefore(QOrm::ModelActionMethod method);

    //!
    //! \brief onSuccess
    //! \param method
    //! \return
    //!
    ModelReportBase &onSuccess(QOrm::ModelActionMethod method);

    //!
    //! \brief onFailed
    //! \param method
    //! \return
    //!
    ModelReportBase &onFailed(QOrm::ModelActionMethod method);

    //!
    //! \brief canActionSearch
    //! \return
    //!
    virtual ResultValue &canActionSearch();

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
    ModelReportBasePvt *p=nullptr;
};

}

