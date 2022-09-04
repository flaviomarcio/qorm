#pragma once

#include "../../../qstm/src/qstm_result_info.h"
#include "./p_qorm_model_dto_host.h"
#include "./p_qorm_model_dto_endpoints.h"
#include "../qorm_model_field_descriptors.h"

namespace QOrm {
class ModelDtoControlsPvt;
//!
//! \brief The ModelDtoControls class
//!
class Q_ORM_EXPORT ModelDtoControls : public QStm::Object
{
    Q_OBJECT
public:

    QORM_MODEL_DECLARE_FORM_ENUMS

    //!
    //! \brief ModelDtoControls
    //! \param parent
    //!
    Q_INVOKABLE explicit ModelDtoControls(QObject *parent = nullptr);

    //!
    //! \brief fields
    //! \return
    //!
    ModelFieldDescriptors &fields();

    //!
    //! \brief resultInfo
    //! \return
    //!
    virtual QStm::ResultInfo &resultInfo();
    virtual ModelDtoControls &setResultInfo(const QStm::ResultInfo&resultInfo);

    //!
    //! \brief uuid
    //! \return
    //!
    virtual QUuid &uuid() const;
    virtual ModelDtoControls &uuid(const QUuid &uuid);

    //!
    //! \brief description
    //! \return
    //!
    virtual QString &description() const;

    //!
    //! \brief sort
    //! \return
    //!
    virtual QVariantHash &sort() const;

    //!
    //! \brief headers
    //! \return
    //!
    virtual ModelFieldCollection &headers();

    //!
    //! \brief filters
    //! \return
    //!
    virtual ModelFieldCollection &filters();

    //!
    //! \brief host
    //! \return
    //!
    virtual const Host &host() const;
    virtual const ModelDtoControls &host(const Host &value);
    virtual const ModelDtoControls &host(const QVariant &value);
    virtual const ModelDtoControls &setHost(const Host &value);
    virtual const ModelDtoControls &setHost(const QVariant &value);

    //!
    //! \brief endPoints
    //! \return
    //!
    virtual EndPoints &endPoints();

    //!
    //! \brief endPoint
    //! \return
    //!
    virtual EndPoint &endPoint();

    //!
    //! \brief items
    //! \return
    //!
    virtual const QVariantList &items() const;
    virtual ModelDtoControls &items(const QVariant &v);
    virtual ModelDtoControls &items(const ResultValue &lr);

    //!
    //! \brief setValue
    //! \param v
    //! \return
    //!
    virtual ModelDtoControls &setValue(const QVariant &v);
    virtual ModelDtoControls &setValue(const ResultValue &lr);

    //!
    //! \brief clear
    //! \return
    //!
    virtual ModelDtoControls &clear();

    //!
    //! \brief o
    //! \return
    //!
    virtual ResultValue &o();

    //!
    //! \brief toOutput
    //! \return
    //!
    virtual ResultValue &toOutput();
private:
    ModelDtoControlsPvt *p=nullptr;
};

}
