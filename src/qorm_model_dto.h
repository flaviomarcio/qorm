#pragma once

#include "./private/p_qorm_model_controls.h"

enum DTOFormType{
    dftNormalForm=0,
    dftAddressForm=1,
    dftReportForm=1
};

enum DTOFormLayout{
    dflVerticalControls=0,
    dflHorizontalControls=0
};

namespace QOrm {

//!
//! \brief The ModelDto class
//!
class Q_ORM_EXPORT ModelDto : public QStm::Object
{
    Q_OBJECT
public:

    QORM_DECLARE_DTO_TYPE(dftReportForm)
    QORM_DECLARE_DTO_LAYOUT(dflVerticalControls)

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
    Q_INVOKABLE explicit ModelDto(const ResultValue&rows, QObject *parent = nullptr);

    //!
    //! \brief ~ModelDto
    //!
    ~ModelDto();

    //!
    //! \brief id
    //! \return
    //!
    virtual QString id() const;

    //!
    //! \brief id
    //! \param v
    //! \return
    //!
    virtual ModelDto&id(const QVariant &v);

    //!
    //! \brief setId
    //! \param v
    //! \return
    //!
    virtual ModelDto&setId(const QVariant &v);

    //!
    //! \brief type
    //! \return
    //!
    virtual QVariant type() const;

    //!
    //! \brief type
    //! \param v
    //! \return
    //!
    virtual ModelDto&type(const QVariant &v);

    //!
    //! \brief setType
    //! \param v
    //! \return
    //!
    virtual ModelDto&setType(const QVariant &v);

    //!
    //! \brief layout
    //! \return
    //!
    virtual QVariant layout() const;

    //!
    //! \brief layout
    //! \param v
    //! \return
    //!
    virtual ModelDto&layout(const QVariant &v);

    //!
    //! \brief setLayout
    //! \param v
    //! \return
    //!
    virtual ModelDto&setLayout(const QVariant &v);

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
    virtual ModelDto&text(const QVariant &v);

    //!
    //! \brief setText
    //! \param v
    //! \return
    //!
    virtual ModelDto&setText(const QVariant &v);

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
    virtual ModelDto&sort(const QVariant &v);

    //!
    //! \brief setSort
    //! \param v
    //! \return
    //!
    virtual ModelDto&setSort(const QVariant &v);

    //!
    //! \brief settings
    //! \param setting
    //! \return
    //!
    virtual ModelDto&settings(const QVariant&setting);

    //!
    //! \brief controls
    //! \return
    //!
    virtual ModelDtoControls&controls();

    //!
    //! \brief headers
    //! \return
    //!
    virtual ModelDtoHeaders<ModelDtoControls>&headers();

    //!
    //! \brief filters
    //! \return
    //!
    virtual ModelDtoFilters<ModelDtoControls>&filters();

    //!
    //! \brief links
    //! \return
    //!
    virtual ModelDtoLinks<ModelDtoControls>&links();

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
    virtual ModelDtoControls &items(const QVariant&v);

    //!
    //! \brief items
    //! \param lr
    //! \return
    //!
    virtual ModelDtoControls &items(const ResultValue&lr);

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
    virtual ModelDtoControls &resultInfo(const QVariant&v);

    //!
    //! \brief setResultInfo
    //! \param resultInfo
    //! \return
    //!
    virtual ModelDtoControls &setResultInfo(const QStm::ResultInfo&resultInfo);

    //!
    //! \brief setValue
    //! \param v
    //! \return
    //!
    virtual ModelDto &setValue(const QVariant&v);

    //!
    //! \brief setValue
    //! \param lr
    //! \return
    //!
    virtual ModelDto &setValue(const ResultValue&lr);

    //!
    //! \brief clear
    //! \return
    //!
    virtual ModelDto&clear();

    //!
    //! \brief o
    //! \return
    //!
    virtual ResultValue&o();

    //!
    //! \brief toOutput
    //! \return
    //!
    virtual ResultValue&toOutput();

    //!
    //! \brief initDescriptors
    //! \param object
    //! \return
    //!
    virtual ModelDto&initDescriptors(QObject *object);

private:
    void*p=nullptr;
};

}
