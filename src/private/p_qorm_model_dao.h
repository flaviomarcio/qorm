#pragma once

#include "./qorm_object_db.h"
#include "./p_qorm_sql_suitable_parser_strategy_options.h"
#include "./private/p_qorm_model_info.h"
#include "../qorm_sql_suitable_value.h"
#include <QVariant>

namespace PrivateQOrm{

    class Q_ORM_EXPORT ModelDao : public QOrm::ObjectDb
    {
        Q_OBJECT
    public:
        Q_INVOKABLE explicit ModelDao(QObject *parent = nullptr);
        Q_INVOKABLE ~ModelDao();

        /**
         * @brief variantToParameters
         * @param modelRef
         * @param value
         * @return
         */
        QVariant variantToParameters(const QOrm::ModelInfo&modelRef, const QVariant&value)const;

        /**
         * @brief suitableValue
         * @return
         */
        QOrm::SqlSuitableValue&suitableValue();
    private:
        void*p=nullptr;
    };

}

