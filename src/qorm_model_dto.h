#pragma once

#include "./private/p_qorm_model_controls.h"

namespace QOrm {
class ModelDtoPvt;
//!
//! \brief The ModelDto class
//!
class Q_ORM_EXPORT ModelDto : public QStm::Object
{
    Q_OBJECT
public:

    QORM_DECLARE_FORM_ENUMS

    QORM_DECLARE_DTO_TYPE(FormType::CustomForm)
    QORM_DECLARE_DTO_LAYOUT(FormLayout::Vertical)

    //!
    //! \brief ModelDto
    //! \param parent
    //!
    Q_INVOKABLE explicit ModelDto(QObject *parent = nullptr);

    //!
    //! \brief ModelDto
    //! \param rows
    //! \param parent
    //!
    Q_INVOKABLE explicit ModelDto(const ResultValue &rows, QObject *parent = nullptr);

    //!
    //! \brief ~ModelDto
    //!
    ~ModelDto();

    //!
    //! \brief uuid
    //! \return
    //!
    virtual QUuid &uuid() const;

    //!
    //! \brief uuid
    //! \param v
    //! \return
    //!
    virtual ModelDto &uuid(const QUuid &value);

    //!
    //! \brief setId
    //! \param v
    //! \return
    //!
    virtual ModelDto &setUuid(const QUuid &v);

    //!
    //! \brief name
    //! \return
    //!
    virtual QString name() const;

    //!
    //! \brief name
    //! \param value
    //! \return
    //!
    virtual ModelDto &name(const QVariant &value);

    //!
    //! \brief setName
    //! \param v
    //! \return
    //!
    virtual ModelDto &setName(const QVariant &value);

    //!
    //! \brief type
    //! \return
    //!
    virtual FormType type() const;

    //!
    //! \brief type
    //! \param v
    //! \return
    //!
    virtual ModelDto &type(const FormType &v);

    //!
    //! \brief setType
    //! \param v
    //! \return
    //!
    virtual ModelDto &setType(const FormType &v);

    //!
    //! \brief layout
    //! \return
    //!
    virtual FormLayout layout() const;

    //!
    //! \brief layout
    //! \param v
    //! \return
    //!
    virtual ModelDto &layout(const FormLayout &v);

    //!
    //! \brief setLayout
    //! \param v
    //! \return
    //!
    virtual ModelDto &setLayout(const FormLayout &v);

    //!
    //! \brief text
    //! \return
    //!
    virtual QString text() const;

    //!
    //! \brief text
    //! \param v
    //! \return
    //!
    virtual ModelDto &text(const QVariant &v);

    //!
    //! \brief setText
    //! \param v
    //! \return
    //!
    virtual ModelDto &setText(const QVariant &v);

    //!
    //! \brief sort
    //! \return
    //!
    virtual QVariantHash sort() const;

    //!
    //! \brief sort
    //! \param v
    //! \return
    //!
    virtual ModelDto &sort(const QVariant &v);

    //!
    //! \brief setSort
    //! \param v
    //! \return
    //!
    virtual ModelDto &setSort(const QVariant &v);

    //!
    //! \brief settings
    //! \param setting
    //! \return
    //!
    virtual ModelDto &settings(const QVariant &setting);

    //!
    //! \brief controls
    //! \return
    //!
    virtual ModelDtoControls &controls();

    //!
    //! \brief headers
    //! \return
    //!
    virtual ModelDtoHeaders<ModelDtoControls> &headers();

    //!
    //! \brief filters
    //! \return
    //!
    virtual ModelDtoFilters<ModelDtoControls> &filters();

    //!
    //! \brief Host
    //! \return
    //!
    virtual Host &host();

    //!
    //! \brief Host
    //! \return
    //!
    virtual ModelDto &host(const Host &host);
    virtual ModelDto &host(const QVariant &host);

    //!
    //! \brief EndPoints
    //! \return
    //!
    virtual EndPoints &endPoints();

    //!
    //! \brief items
    //! \return
    //!
    virtual ModelDtoItems<ModelDtoControls> &items();

    //!
    //! \brief items
    //! \param v
    //! \return
    //!
    virtual ModelDtoControls &items(const QVariant &v);

    //!
    //! \brief items
    //! \param lr
    //! \return
    //!
    virtual ModelDtoControls &items(const ResultValue &lr);

    //!
    //! \brief resultInfo
    //! \return
    //!
    virtual QStm::ResultInfo &resultInfo();

    //!
    //! \brief resultInfo
    //! \param v
    //! \return
    //!
    virtual ModelDtoControls &resultInfo(const QVariant &v);

    //!
    //! \brief setResultInfo
    //! \param resultInfo
    //! \return
    //!
    virtual ModelDtoControls &setResultInfo(const QStm::ResultInfo &resultInfo);

    //!
    //! \brief setValue
    //! \param v
    //! \return
    //!
    virtual ModelDto &setValue(const QVariant &v);

    //!
    //! \brief setValue
    //! \param lr
    //! \return
    //!
    virtual ModelDto &setValue(const ResultValue &lr);

    //!
    //! \brief clear
    //! \return
    //!
    virtual ModelDto &clear();

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

    //!
    //! \brief initDescriptors
    //! \param object
    //! \return
    //!
    virtual ModelDto &initDescriptors(QObject *object);

private:
    ModelDtoPvt *p = nullptr;
};

} // namespace QOrm
