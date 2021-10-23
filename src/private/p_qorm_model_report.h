#pragma once

#include <QVariant>
#include <QVariantHash>
#include "../qorm_model_dao.h"
#include "../qorm_model_dto.h"
#include "../qorm_model_dto_options.h"
#include "../qorm_model_crud_request.h"
#include "./p_qorm_model_report_body.h"
#include "./p_qorm_model_action_method.h"

namespace QOrm {

typedef std::function<ResultValue&(QOrm::ObjectDb*controller, const QVariant&vBody)> ReportActionMethod;

}


namespace PrivateQOrm{

    /**
     * @brief The ModelReportBase class
     */
    class Q_ORM_EXPORT ModelReportBase : public QOrm::ObjectDb
    {
        Q_OBJECT
    public:

        QORM_DECLARE_DTO_TYPE(dftNormalForm)
        QORM_DECLARE_DTO_LAYOUT(dflVerticalControls)

        /**
         * @brief ModelReportBase
         * @param parent
         */
        Q_INVOKABLE explicit ModelReportBase(QObject *parent = nullptr);

        /**
         * @brief ModelReportBase
         * @param vCrud
         * @param parent
         */
        explicit ModelReportBase(const QVariant &reportBody, QObject *parent = nullptr);


        /**
         * @brief ~ModelReportBase
         */
        Q_INVOKABLE ~ModelReportBase();

        /**
         * @brief options
         * @return
         */
        QOrm::ModelDtoOptions&options();

        /**
         * @brief options
         * @return
         */
        ModelReportBase &setOptions(const QOrm::ModelDtoOptions &options);

        /**
         * @brief makeDefault
         * @return
         */
        virtual ModelReportBase&makeDefault();

        /**
         * @brief unMakeDefault
         * @return
         */
        virtual ModelReportBase&unMakeDefault();


        /**
         * @brief type
         * @return
         */
        virtual QVariant type()const;
        virtual ModelReportBase&type(const QVariant &value);
//        Q_INVOKABLE virtual QVariant defaultType()const;

        /**
         * @brief type
         * @return
         */
        virtual QVariant layout()const;
        virtual ModelReportBase&layout(const QVariant &value);
//        Q_INVOKABLE virtual QVariant defaultLayout()const;

        /**
         * @brief reportName
         * @return
         */
        virtual QByteArray reportName()const;
        virtual ModelReportBase&reportName(const QVariant &value);

        /**
         * @brief reportDescription
         * @return
         */
        virtual QByteArray reportDescription()const;
        virtual ModelReportBase&reportDescription(const QVariant &value);

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
         * @brief reportBody
         * @return
         */
        virtual ModelReportBase &reportBody(const QVariant &v);

        /**
         * @brief strategy
         * @return
         */
        virtual QVariant strategy()const;
        virtual ModelReportBase&strategy(const QVariant&strategy);

        /**
         * @brief source
         * @param value
         * @return
         */
        virtual QVariant source()const;
        virtual ModelReportBase&source(const QVariant&value);

        /**
         * @brief reportfy
         * @return
         */
        virtual ResultValue &reportfy();

        /**
         * @brief actionSearch
         * @param action
         * @return
         */
        virtual ModelReportBase&actionSearch(QOrm::ModelAction&action);

    protected:

        /**
         * @brief search
         * @return
         */
        virtual ResultValue &search();
        virtual ResultValue &search(const QVariant&value);

        /**
         * @brief onBefore
         * @param method
         * @return
         */
        ModelReportBase&onBefore(QOrm::ReportActionMethod method);
        ModelReportBase&onSuccess(QOrm::ReportActionMethod method);
        ModelReportBase&onFailed(QOrm::ReportActionMethod method);


        virtual ResultValue &canActionSearch();

    protected:
        virtual ResultValue&doBofore();
        virtual ResultValue&doSuccess();
        virtual ResultValue&doFailed();
    private:
        void*p=nullptr;
    };

}

