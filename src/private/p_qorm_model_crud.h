#pragma once

#include <QVariant>
#include <QVariantHash>
#include "./p_qorm_model_crud_body.h"
#include "./p_qorm_model_action_method.h"
#include "../qorm_model_dto_options.h"
#include "../qorm_model_crud_request.h"
#include "../qorm_model_dao.h"
#include "../qorm_model_dto.h"

namespace QOrm {
typedef std::function<ResultValue&(QOrm::ObjectDb*controller, const QVariant&vBody)> CRUDBodyActionMethod;
typedef QOrm::ModelAction CRUDAction;
}

namespace PrivateQOrm{

    /**
     * @brief The CRUDBase class
     */
    class Q_ORM_EXPORT CRUDBase : public QOrm::ObjectDb
    {
        Q_OBJECT
    public:

        QORM_DECLARE_DTO_TYPE(dftNormalForm)
        QORM_DECLARE_DTO_LAYOUT(dflVerticalControls)

        /**
         * @brief CRUDBase
         * @param parent
         */
        Q_INVOKABLE explicit CRUDBase(QObject *parent = nullptr);

        /**
         * @brief CRUDBase
         * @param vCrud
         * @param parent
         */
        explicit CRUDBase(const QVariant &vBody, QObject *parent = nullptr);


        /**
         * @brief ~CRUDBase
         */
        Q_INVOKABLE ~CRUDBase();

        /**
         * @brief options
         * @return
         */
        QOrm::ModelDtoOptions&options();

        /**
         * @brief options
         * @return
         */
        CRUDBase &setOptions(const QOrm::ModelDtoOptions &options);

        /**
         * @brief makeDefault
         * @return
         */
        virtual CRUDBase&makeDefault();

        /**
         * @brief unMakeDefault
         * @return
         */
        virtual CRUDBase&unMakeDefault();


        /**
         * @brief type
         * @return
         */
        virtual QVariant type()const;
        virtual CRUDBase&type(const QVariant &value);
//        Q_INVOKABLE virtual QVariant defaultType()const;

        /**
         * @brief type
         * @return
         */
        virtual QVariant layout()const;
        virtual CRUDBase&layout(const QVariant &value);
//        Q_INVOKABLE virtual QVariant defaultLayout()const;

        /**
         * @brief crudName
         * @return
         */
        virtual QByteArray crudName()const;
        virtual CRUDBase&crudName(const QVariant &value);

        /**
         * @brief crudDescription
         * @return
         */
        virtual QByteArray crudDescription()const;
        virtual CRUDBase&crudDescription(const QVariant &value);

        /**
         * @brief dto
         * @return
         */
        virtual QOrm::ModelDto&dto();

        /**
         * @brief modelInfo
         * @return
         */
        virtual const QOrm::ModelInfo &modelInfo();

        /**
         * @brief crudBody
         * @return
         */
        virtual CRUDBase &crudBody(const QVariant &v);

        /**
         * @brief strategy
         * @return
         */
        virtual QOrm::CRUDStrategy&strategy()const;
        virtual CRUDBase&strategy(const QVariant&strategy);

        /**
         * @brief source
         * @param value
         * @return
         */
        virtual QVariant source()const;
        virtual CRUDBase&source(const QVariant&value);

        /**
         * @brief crudify
         * @return
         */
        virtual ResultValue &crudify();

        /**
         * @brief actionSearch
         * @param action
         * @return
         */
        virtual CRUDBase&actionSearch(QOrm::ModelAction&action);
        virtual CRUDBase&actionInsert(QOrm::ModelAction&action);
        virtual CRUDBase&actionUpsert(QOrm::ModelAction&action);
        virtual CRUDBase&actionUpdate(QOrm::ModelAction&action);
        virtual CRUDBase&actionRemove(QOrm::ModelAction&action);
        virtual CRUDBase&actionDeactivate(QOrm::ModelAction&action);

    protected:

        /**
         * @brief search
         * @return
         */
        virtual ResultValue &search();
        virtual ResultValue &search(const QVariant&value);


        /**
         * @brief insert
         * @param value
         * @return
         */
        virtual ResultValue &insert();
        virtual ResultValue &insert(const QVariant&value);

        /**
         * @brief update
         * @param value
         * @return
         */
        virtual ResultValue &update();
        virtual ResultValue &update(const QVariant&value);

        /**
         * @brief upsert
         * @param value
         * @return
         */
        virtual ResultValue &upsert();
        virtual ResultValue &upsert(const QVariant&value);

        /**
         * @brief remove
         * @param value
         * @return
         */
        virtual ResultValue &remove();
        virtual ResultValue &remove(const QVariant&value);

        /**
         * @brief deactivate
         * @return
         */
        virtual ResultValue &deactivate();
        virtual ResultValue &deactivate(const QVariant&value);

        /**
         * @brief onBefore
         * @param method
         * @return
         */
        CRUDBase&onBefore(QOrm::CRUDBodyActionMethod method);
        CRUDBase&onSuccess(QOrm::CRUDBodyActionMethod method);
        CRUDBase&onFailed(QOrm::CRUDBodyActionMethod method);


        virtual ResultValue &canActionSearch();
        virtual ResultValue &canActionInsert();
        virtual ResultValue &canActionUpsert();
        virtual ResultValue &canActionUpdate();
        virtual ResultValue &canActionRemove();
        virtual ResultValue &canActionDeactivate();

    protected:
        virtual ResultValue&doBofore();
        virtual ResultValue&doSuccess();
        virtual ResultValue&doFailed();
    private:
        void*p=nullptr;
    };

}

