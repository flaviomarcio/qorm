#pragma once

#include <QVariant>
#include <QVariantHash>
#include <QVariantHash>

#include "./qorm_global.h"
#include "./qorm_model_crud_types.h"

namespace QOrm {

#define ___CRUDRequest__isStrategy(v)\
bool is##v(){return isStrategy(v);};

//!
//! \brief The CRUDRequest class
//!
class Q_ORM_EXPORT CRUDRequest:public QVariantHash
{
public:
    //!
    //! \brief CRUDRequest
    //! \param other
    //!
    explicit CRUDRequest(const QVariant &other=QVariant());

    //!
    //! \brief ~CRUDRequest
    //!
    virtual ~CRUDRequest();

    //!
    //! \brief setBody
    //! \param v
    //! \return
    //!
    Q_INVOKABLE virtual CRUDRequest &setBody(const QVariant&v);

    //!
    //! \brief fromMap
    //! \param v
    //! \return
    //!
    static CRUDRequest fromMap(const QVariant&v);

    //!
    //! \brief fromRequest
    //! \param v
    //! \return
    //!
    static CRUDRequest fromRequest(const QVariant&v);

    //!
    //! \brief header
    //! \return
    //!
    virtual const QVariantHash header();

    //!
    //! \brief path
    //! \return
    //!
    virtual const QString path();

    //!
    //! \brief method
    //! \return
    //!
    virtual const QString method();

    //!
    //! \brief source
    //! \return
    //!
    virtual const QVariant source();

    //!
    //! \brief strategy
    //! \return
    //!
    virtual CRUDStrategy strategy();

    //!
    //! \brief isStrategy
    //! \param v
    //! \return
    //!
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
