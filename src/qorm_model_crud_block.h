#pragma once

#include "./qorm_model_crud.h"

#define QORM_CRUD_BLOCK_MAKER(crud_block)\
QOrm::CRUDBlock crud_block(this);\
QOrm::CRUDBlock::makeBlock(crud_block, vBody)

namespace QOrm {

//!
//! \brief The CRUDBlock class
//!
class Q_ORM_EXPORT CRUDBlock:public QOrm::ObjectDb
{
    Q_OBJECT
public:
    //!
    //! \brief CRUDBlock
    //! \param parent
    //!
    Q_INVOKABLE explicit CRUDBlock(QObject *parent=nullptr);

    //!
    //! \brief CRUDBlock
    //! \param crudBody
    //! \param parent
    //!
    explicit CRUDBlock(const QVariant &crudBody, QObject *parent=nullptr);

    //!
    //! \brief ~CRUDBlock
    //!
    virtual ~CRUDBlock();

    //!
    //! \brief options
    //! \return
    //!
    virtual QOrm::ModelDtoOptions&options();

    //!
    //! \brief resultInfo
    //! \return
    //!
    virtual QStm::ResultInfo&resultInfo();

    //!
    //! \brief operator +=
    //! \param crud
    //! \return
    //!
    CRUDBlock&operator+=(PrivateQOrm::CRUDBase*crud);

    //!
    //! \brief operator -=
    //! \param crud
    //! \return
    //!
    CRUDBlock&operator-=(PrivateQOrm::CRUDBase*crud);

    //!
    //! \brief operator <<
    //! \param crud
    //! \return
    //!
    CRUDBlock&operator<<(PrivateQOrm::CRUDBase*crud);

    //!
    //! \brief crudBody
    //! \return
    //!
    Q_INVOKABLE virtual QVariant crudBody()const;

    //!
    //! \brief crudBody
    //! \param v
    //! \return
    //!
    virtual CRUDBlock&crudBody(const QVariant&v);

    //!
    //! \brief makeBlock
    //! \param crudBlock
    //! \param crudBody
    //! \return
    //!
    static CRUDBlock &makeBlock(CRUDBlock &crudBlock, const QVariant&crudBody);

    //!
    //! \brief clear
    //! \return
    //!
    virtual CRUDBlock&clear();

    //!
    //! \brief insert
    //! \param crud
    //! \return
    //!
    virtual CRUDBlock&insert(PrivateQOrm::CRUDBase*crud);

    //!
    //! \brief remove
    //! \param crud
    //! \return
    //!
    virtual CRUDBlock&remove(PrivateQOrm::CRUDBase*crud);

    //!
    //! \brief remove
    //! \param crudName
    //! \return
    //!
    virtual CRUDBlock&remove(const QByteArray&crudName);

    //!
    //! \brief crudify
    //! \return
    //!
    virtual ResultValue &crudify();

private:
    void*p=nullptr;
};

}
