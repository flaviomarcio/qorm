#pragma once

#include "../qorm_sql_suitable_types.h"
#include "../../../qstm/src/qstm_result_info.h"
#include "./p_qorm_model_dto_items.h"
#include "./p_qorm_model_dto_header.h"
#include "./p_qorm_model_dto_filter.h"
#include "./p_qorm_model_dto_link.h"
#include "./p_qorm_model_dto_crud.h"

namespace QOrm {

//!
//! \brief The ModelDtoControls class
//!
class Q_ORM_EXPORT ModelDtoControls : public QStm::Object
{
    Q_OBJECT
public:

    //!
    //! \brief ModelDtoControls
    //! \param parent
    //!
    Q_INVOKABLE explicit ModelDtoControls(QObject *parent = nullptr);
    ~ModelDtoControls();

    //!
    //! \brief resultInfo
    //! \return
    //!
    virtual QStm::ResultInfo&resultInfo();
    virtual ModelDtoControls&setResultInfo(const QStm::ResultInfo&resultInfo);

    //!
    //! \brief id
    //! \return
    //!
    virtual QString id()const;
    virtual ModelDtoControls&id(const QVariant &v);

    //!
    //! \brief type
    //! \return
    //!
    virtual FormType type() const;
    virtual ModelDtoControls&type(const FormType &v);
    virtual ModelDtoControls&setType(const FormType &v);


    //!
    //! \brief layout
    //! \return
    //!
    virtual FormLayout &layout() const;
    virtual ModelDtoControls&layout(const FormLayout &v);
    virtual ModelDtoControls&setLayout(const FormLayout &v);

    //!
    //! \brief sort
    //! \return
    //!
    virtual QVariantHash sort() const;
    virtual ModelDtoControls&sort(const QVariant &v);
    virtual ModelDtoControls&setSort(const QVariant &v);

    //!
    //! \brief text
    //! \return
    //!
    virtual QString text()const;
    virtual ModelDtoControls&text(const QVariant &v);

    //!
    //! \brief settings
    //! \param setting
    //! \return
    //!
    virtual ModelDtoControls&settings(const QVariant&setting);

    /**
     * @brief outPutStyle
     * @return
     */
    virtual DtoOutPutStyle &outPutStyle() const;

    //!
    //! \brief outPutStyle
    //! \param value
    //! \return
    //!
    virtual ModelDtoControls &outPutStyle(const DtoOutPutStyle &value);

    /**
     * @brief setOutPutStyle
     * @param value
     * @return
     */
    virtual ModelDtoControls &setOutPutStyle(const DtoOutPutStyle &value);

    //!
    //! \brief headers
    //! \return
    //!
    virtual ModelDtoHeaders<ModelDtoControls>&headers();

    /**
     * @brief filters
     * @return
     */
    virtual ModelDtoFilters<ModelDtoControls>&filters();

    //!
    //! \brief links
    //! \return
    //!
    virtual ModelDtoLinks<ModelDtoControls>&links();

    //!
    //! \brief crud
    //! \return
    //!
    virtual ModelDtoCrud<ModelDtoControls>&crud();

    //!
    //! \brief items
    //! \return
    //!
    virtual ModelDtoItems<ModelDtoControls> &items();
    virtual ModelDtoControls &items(const QVariant&v);
    virtual ModelDtoControls &items(const ResultValue&lr);

    //!
    //! \brief setValue
    //! \param v
    //! \return
    //!
    virtual ModelDtoControls &setValue(const QVariant&v);
    virtual ModelDtoControls &setValue(const ResultValue&lr);

    //!
    //! \brief clear
    //! \return
    //!
    virtual ModelDtoControls&clear();

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

private:
    void*p=nullptr;
};

}
