#pragma once

#include <QVariant>
#include <QVariantHash>
#include <QVariantHash>

#include "./qorm_global.h"
#include "./qorm_model_crud_types.h"

namespace QOrm {

#define ___CRUDRequest__isStrategy(v)\
    bool is##v(){return isStrategy(v);};


    class Q_ORM_EXPORT CRUDRequest:public QVariantHash
    {
    public:
        explicit CRUDRequest(const QVariant &other=QVariant());
        virtual ~CRUDRequest();

        virtual Q_INVOKABLE CRUDRequest &setBody(const QVariant&v);

        static CRUDRequest fromMap(const QVariant&v);
        static CRUDRequest fromRequest(const QVariant&v);


        virtual const QVariantHash header();

        /**
         * @brief method
         * @return
         */
        virtual const QString path();

        /**
         * @brief method
         * @return
         */
        virtual const QString method();

        /**
         * @brief source
         * @return
         */
        virtual const QVariant source();

        /**
         * @brief strategy
         * @return
         */
        virtual CRUDStrategy strategy();
        virtual bool isStrategy(const CRUDStrategy v);
        ___CRUDRequest__isStrategy(Search)
        ___CRUDRequest__isStrategy(Insert)
        ___CRUDRequest__isStrategy(Upsert)
        ___CRUDRequest__isStrategy(Update)
        ___CRUDRequest__isStrategy(Remove)
        ___CRUDRequest__isStrategy(Deactivate)

    private:
        void*p=nullptr;
    };

}

Q_DECLARE_METATYPE(QOrm::CRUDRequest)
Q_DECLARE_METATYPE(QOrm::CRUDRequest*)
