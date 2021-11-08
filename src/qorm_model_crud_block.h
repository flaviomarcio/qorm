#pragma once

#include "./qorm_model_crud.h"

#define QORM_CRUD_BLOCK_MAKER(crud_block)\
    QOrm::CRUDBlock crud_block(this);\
    QOrm::CRUDBlock::makeBlock(crud_block, vBody)

namespace QOrm {

    class Q_ORM_EXPORT CRUDBlock:public QOrm::ObjectDb
    {
        Q_OBJECT
    public:
        Q_INVOKABLE explicit CRUDBlock(QObject *parent=nullptr);
        Q_INVOKABLE explicit CRUDBlock(const QVariant &crudBody, QObject *parent=nullptr);
        Q_INVOKABLE virtual ~CRUDBlock();

        Q_INVOKABLE virtual QOrm::ModelDtoOptions&options();
        Q_INVOKABLE virtual QOrm::ModelDtoResultInfo&resultInfo();

         /**
         * @brief operator +=
         * @param v
         * @return
         */
        CRUDBlock&operator+=(PrivateQOrm::CRUDBase*crud);
        CRUDBlock&operator-=(PrivateQOrm::CRUDBase*crud);
        CRUDBlock&operator<<(PrivateQOrm::CRUDBase*crud);

        /**
         * @brief crudName
         * @return
         */
        Q_INVOKABLE virtual QVariant crudBody()const;
        virtual CRUDBlock&crudBody(const QVariant&v);

        /**
         * @brief clear
         * @return
         */
        static CRUDBlock &makeBlock(CRUDBlock &crudBlock, const QVariant&crudBody);

        /**
         * @brief clear
         * @return
         */
        virtual CRUDBlock&clear();

        /**
         * @brief insert
         * @return
         */
        virtual CRUDBlock&insert(PrivateQOrm::CRUDBase*crud);

        /**
         * @brief remove
         * @param v
         * @return
         */
        virtual CRUDBlock&remove(PrivateQOrm::CRUDBase*crud);

        /**
         * @brief remove
         * @param v
         * @return
         */
        virtual CRUDBlock&remove(const QByteArray&crudName);

        /**
         * @brief crudify
         * @return
         */
        virtual ResultValue &crudify();

    private:
        void*p=nullptr;
    };

}
