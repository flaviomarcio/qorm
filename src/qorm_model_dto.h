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

class Q_ORM_EXPORT ModelDto : public QStm::Object
{
    Q_OBJECT
public:

    QORM_DECLARE_DTO_TYPE(dftReportForm)
    QORM_DECLARE_DTO_LAYOUT(dflVerticalControls)

    /**
     * @brief ModelDto
     * @param parent
     */
    Q_INVOKABLE explicit ModelDto(QObject *parent = nullptr);

    /**
     * @brief ModelDto
     * @param rows
     * @param parent
     */
    Q_INVOKABLE explicit ModelDto(const ResultValue&rows, QObject *parent = nullptr);

    /**
     * @brief ~ModelDto
     */
    Q_INVOKABLE ~ModelDto();

    /**
     * @brief id
     * @return
     */
    virtual QString id() const;
    virtual ModelDto&id(const QVariant &v);
    virtual ModelDto&setId(const QVariant &v);


    /**
     * @brief type
     * @return
     */
    virtual QVariant type() const;
    virtual ModelDto&type(const QVariant &v);
    virtual ModelDto&setType(const QVariant &v);


    /**
     * @brief layout
     * @return
     */
    virtual QVariant layout() const;
    virtual ModelDto&layout(const QVariant &v);
    virtual ModelDto&setLayout(const QVariant &v);

    /**
     * @brief text
     * @return
     */
    virtual QString text() const;
    virtual ModelDto&text(const QVariant &v);
    virtual ModelDto&setText(const QVariant &v);

    /**
     * @brief sort
     * @return
     */
    virtual QVariantHash sort() const;
    virtual ModelDto&sort(const QVariant &v);
    virtual ModelDto&setSort(const QVariant &v);

    /**
     * @brief settings
     * @param setting
     * @return
     */
    virtual ModelDto&settings(const QVariant&setting);

    /**
     * @brief rest
     * @return
     */
    virtual ModelDtoControls&controls();

    /**
     * @brief headers
     * @return
     */
    virtual ModelDtoHeaders<ModelDtoControls>&headers();

    /**
     * @brief filters
     * @return
     */
    virtual ModelDtoFilters<ModelDtoControls>&filters();

    /**
     * @brief links
     * @return
     */
    virtual ModelDtoLinks<ModelDtoControls>&links();

    /**
     * @brief items
     * @return
     */
    virtual ModelDtoItems<ModelDtoControls> &items();
    virtual ModelDtoControls &items(const QVariant&v);
    virtual ModelDtoControls &items(const ResultValue&lr);

    //!
    //! \brief resultInfo
    //! \return
    //!
    virtual ModelDtoResultInfo &resultInfo();
    virtual ModelDtoControls &resultInfo(const QVariant&v);
    virtual ModelDtoControls &setResultInfo(const ModelDtoResultInfo&resultInfo);



    /**
     * @brief setValue
     * @param v
     * @return
     */
    virtual ModelDto &setValue(const QVariant&v);
    virtual ModelDto &setValue(const ResultValue&lr);

    /**
     * @brief clear
     * @return
     */
    virtual ModelDto&clear();

    /**
     * @brief toOutput
     * @return
     */
    virtual ResultValue&o();
    virtual ResultValue&toOutput();

    /**
     * @brief initDescriptors
     * @param model
     * @return
     */
    virtual ModelDto&initDescriptors(QObject *object);

private:
    void*p=nullptr;
};

}
