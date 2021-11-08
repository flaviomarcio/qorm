#pragma once

#include "../qorm_sql_suitable_types.h"
#include "./private/p_qorm_model_dto_items.h"
#include "./private/p_qorm_model_dto_header.h"
#include "./private/p_qorm_model_dto_filter.h"
#include "./private/p_qorm_model_dto_link.h"
#include "./private/p_qorm_model_dto_crud.h"
#include "../qorm_model_dto_resullt_info.h"

namespace QOrm {

class Q_ORM_EXPORT ModelDtoControls : public QStm::Object
{
    Q_OBJECT
public:

    /**
     * @brief ModelRESTControl
     * @param parent
     */
    Q_INVOKABLE explicit ModelDtoControls(QObject *parent = nullptr);

    /**
     * @brief ~ModelRESTControl
     */
    Q_INVOKABLE ~ModelDtoControls();

    //!
    //! \brief resultInfo
    //! \return
    //!
    virtual ModelDtoResultInfo&resultInfo();
    virtual ModelDtoControls&setResultInfo(const ModelDtoResultInfo&resultInfo);

    /**
     * @brief name
     * @return
     */
    virtual QString id()const;
    virtual ModelDtoControls&id(const QVariant &v);

    /**
     * @brief type
     * @return
     */
    virtual QVariant type() const;
    virtual ModelDtoControls&type(const QVariant &v);
    virtual ModelDtoControls&setType(const QVariant &v);


    /**
     * @brief layout
     * @return
     */
    virtual QVariant layout() const;
    virtual ModelDtoControls&layout(const QVariant &v);
    virtual ModelDtoControls&setLayout(const QVariant &v);

    /**
     * @brief sort
     * @return
     */
    virtual QVariantHash sort() const;
    virtual ModelDtoControls&sort(const QVariant &v);
    virtual ModelDtoControls&setSort(const QVariant &v);

    /**
     * @brief text
     * @return
     */
    virtual QString text()const;
    virtual ModelDtoControls&text(const QVariant &v);

    /**
     * @brief settings
     * @param setting
     * @return
     */
    virtual ModelDtoControls&settings(const QVariant&setting);

    /**
     * @brief outPutStyle
     * @return
     */
    virtual DtoOutPutStyle &outPutStyle() const;

    /**
     * @brief outPutStyle
     * @param value
     * @return
     */
    virtual ModelDtoControls &outPutStyle(const QOrm::DtoOutPutStyle &value);

    /**
     * @brief setOutPutStyle
     * @param value
     * @return
     */
    virtual ModelDtoControls &setOutPutStyle(const QOrm::DtoOutPutStyle &value);

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
     * @brief crud
     * @return
     */
    virtual ModelDtoCrud<ModelDtoControls>&crud();

    /**
     * @brief items
     * @return
     */
    virtual ModelDtoItems<ModelDtoControls> &items();
    virtual ModelDtoControls &items(const QVariant&v);
    virtual ModelDtoControls &items(const ResultValue&lr);

    /**
     * @brief setValue
     * @param v
     * @return
     */
    virtual ModelDtoControls &setValue(const QVariant&v);
    virtual ModelDtoControls &setValue(const ResultValue&lr);

    /**
     * @brief clear
     * @return
     */
    virtual ModelDtoControls&clear();

    /**
     * @brief toOutput
     * @return
     */
    virtual ResultValue&o();
    virtual ResultValue&toOutput();

private:
    void*p=nullptr;
};

}
