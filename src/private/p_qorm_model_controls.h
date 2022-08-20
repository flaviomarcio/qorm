#pragma once

#include "../../../qstm/src/qstm_result_info.h"
#include "./p_qorm_model_dto_host.h"
#include "./p_qorm_model_dto_endpoints.h"
#include "./p_qorm_model_dto_types.h"
#include "../qorm_model_field_descriptors.h"
#include "../qorm_types.h"

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
    virtual ModelDtoControls &uuid(const QUuid &v);
    virtual ModelDtoControls &setUuid(const QUuid &v);

    //!
    //! \brief name
    //! \return
    //!
    virtual QString &name() const;

    //!
    //! \brief name
    //! \param value
    //! \return
    //!
    virtual ModelDtoControls &name(const QVariant &value);

    //!
    //! \brief setName
    //! \param v
    //! \return
    //!
    virtual ModelDtoControls &setName(const QVariant &v);


    //!
    //! \brief type
    //! \return
    //!
    virtual FormType type() const;
    virtual ModelDtoControls &type(const QVariant &v);
    virtual ModelDtoControls &setType(const QVariant &v);

    //!
    //! \brief design
    //! \return
    //!
    virtual QVariantHash &design()const;
    virtual ModelDtoControls &design(const QVariant &v);
    virtual ModelDtoControls &setDesign(const QVariant &v);

    //!
    //! \brief sort
    //! \return
    //!
    virtual QVariantHash &sort() const;
    virtual ModelDtoControls &sort(const QVariant &v);
    virtual ModelDtoControls &setSort(const QVariant &v);

    //!
    //! \brief text
    //! \return
    //!
    virtual QString &text()const;
    virtual ModelDtoControls &text(const QVariant &v);

    //!
    //! \brief outPutStyle
    //! \return
    //!
    virtual DtoOutPutStyle &outPutStyle() const;

    //!
    //! \brief outPutStyle
    //! \param value
    //! \return
    //!
    virtual ModelDtoControls &outPutStyle(const DtoOutPutStyle &value);

    //!
    //! \brief setOutPutStyle
    //! \param value
    //! \return
    //!
    virtual ModelDtoControls &setOutPutStyle(const DtoOutPutStyle &value);

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
    virtual Host &host();

    //!
    //! \brief endpoints
    //! \return
    //!
    virtual EndPoints &endpoints();

    //!
    //! \brief endpoint
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
