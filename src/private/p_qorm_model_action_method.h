#pragma once

#include "./p_qorm_model_crud_body.h"
#include "../qorm_model_dao.h"

namespace QOrm {
    typedef std::function<ResultValue*(QOrm::ObjectDb*controller, const QVariant&vBody)> ModelActionMethod;

    class Q_ORM_EXPORT ModelAction : public QOrm::ObjectDb
    {
    public:
        Q_PROPERTY(QVariant Strategy READ strategy WRITE strategy )

        /**
         * @brief ModelAction
         * @param parent
         */
        Q_INVOKABLE explicit ModelAction(QObject *parent = nullptr);

        virtual ResultValue&action(const QVariant&vSource);

        Q_INVOKABLE ~ModelAction();

        virtual ModelAction&onActionBefore(ModelActionMethod action);
        virtual ModelAction&onAction(ModelActionMethod action);
        virtual ModelAction&onActionAfter(ModelActionMethod action);

    private:
        void*p=nullptr;
    };

}

