#pragma once

#include "./qorm_object_db.h"
#include "./qorm_model_dto_options.h"
#include "./private/p_qorm_model_dto_host.h"
#include "./private/p_qorm_model_crud.h"

#define QORM_CRUD_BLOCK_MAKER(crud_block)\
QOrm::CRUDBlock crud_block(this);\
QOrm::CRUDBlock::makeBlock(crud_block, vBody)

namespace QOrm {
class CRUDBlockPvt;
//!
//! \brief The CRUDBlock class
//!
class Q_ORM_EXPORT CRUDBlock:public QOrm::ObjectDb
{
    Q_OBJECT
public:
    QORM_MODEL_DECLARE_FORM_ENUMS
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
    //! \brief type
    //! \return
    //!
    QVariant type() const;
    CRUDBlock &setType(const FormType &newType);
    CRUDBlock &resetType();

    //!
    //! \brief host
    //! \return
    //!
    Host &host();
    CRUDBlock &host(const Host &value);
    CRUDBlock &host(const QVariant &value);

    //!
    //! \brief options
    //! \return
    //!
    virtual QOrm::ModelDtoOptions &options();

    //!
    //! \brief resultInfo
    //! \return
    //!
    virtual QStm::ResultInfo &resultInfo();

    //!
    //! \brief operator +=
    //! \param crud
    //! \return
    //!
    CRUDBlock &operator+=(PrivateQOrm::CRUDBase *crud);

    //!
    //! \brief operator -=
    //! \param crud
    //! \return
    //!
    CRUDBlock &operator-=(PrivateQOrm::CRUDBase *crud);

    //!
    //! \brief operator <<
    //! \param crud
    //! \return
    //!
    CRUDBlock &operator<<(PrivateQOrm::CRUDBase *crud);

    //!
    //! \brief crudBody
    //! \return
    //!
    Q_INVOKABLE virtual QVariant &crudBody()const;

    //!
    //! \brief crudBody
    //! \param v
    //! \return
    //!
    virtual CRUDBlock &crudBody(const QVariant &v);

    //!
    //! \brief makeBlock
    //! \param crudBlock
    //! \param crudBody
    //! \return
    //!
    static CRUDBlock &makeBlock(CRUDBlock &crudBlock, const QVariant &crudBody);

    //!
    //! \brief clear
    //! \return
    //!
    virtual CRUDBlock &clear();

    //!
    //! \brief clean
    //! \return
    //!
    virtual CRUDBlock &clean();

    //!
    //! \brief append
    //! \param crud
    //! \return
    //!
    virtual CRUDBlock &append(PrivateQOrm::CRUDBase *crud);

    //!
    //! \brief remove
    //! \param crud
    //! \return
    //!
    virtual CRUDBlock &remove(PrivateQOrm::CRUDBase *crud);

    //!
    //! \brief remove
    //! \param crudName
    //! \return
    //!
    virtual CRUDBlock &remove(const QUuid &crudUuid);

    //!
    //! \brief generatedRecords
    //! \return
    //!
    const QVariantHash &generatedRecords() const;

    //!
    //! \brief generatedRecords
    //! \param uuid
    //! \return
    //!
    const QVariantList generatedRecords(const QUuid &uuid) const;

    //!
    //! \brief generatedRecords
    //! \param modelInfo
    //! \return
    //!
    const QVariantList generatedRecords(const ModelInfo &modelInfo) const;

    //!
    //! \brief crudify
    //! \return
    //!
    virtual ResultValue &crudify();

private:
    CRUDBlockPvt *p=nullptr;
};

}
