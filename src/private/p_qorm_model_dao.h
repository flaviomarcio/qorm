#pragma once

#include "../qorm_object_db.h"
#include "../qorm_sql_suitable_value.h"
#include "./p_qorm_sql_suitable_parser_strategy_options.h"
#include "./p_qorm_model_info.h"
#include <QVariant>

namespace PrivateQOrm{
class ModelDaoPvt;
//!
//! \brief The ModelDao class
//!
class Q_ORM_EXPORT ModelDao : public QOrm::ObjectDb
{
    Q_OBJECT
public:
    //!
    //! \brief ModelDao
    //! \param parent
    //!
    Q_INVOKABLE explicit ModelDao(QObject *parent = nullptr);

    //!
    //! \brief ~ModelDao
    //!
    ~ModelDao();

    //!
    //! \brief variantToParameters
    //! \param modelRef
    //! \param value
    //! \return
    //!
    QVariant variantToParameters(const QOrm::ModelInfo&modelRef, const QVariant &value)const;

    //!
    //! \brief suitableValue
    //! \return
    //!
    QOrm::SqlSuitableValue&suitableValue();
private:
    ModelDaoPvt *p=nullptr;
};

}

