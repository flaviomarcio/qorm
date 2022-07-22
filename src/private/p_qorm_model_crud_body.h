#pragma once

#include <QVariant>
#include <QVariantHash>
#include "../qorm_global.h"
#include "../qorm_model_crud_types.h"

//!
//! \brief The CRUDBody class
//!
class Q_ORM_EXPORT CRUDBody:public QVariantHash
{
public:
    //!
    //! \brief CRUDBody
    //! \param other
    //!
    explicit CRUDBody(const QVariant &other={});

    //!
    //! \brief CRUDBody
    //! \param strategy
    //! \param source
    //!
    explicit CRUDBody(const QVariant &strategy, const QVariant &source);

    //!
    //! \brief CRUDBody
    //! \param strategy
    //! \param source
    //!
    explicit CRUDBody(const QOrm::CRUDStrategy strategy, const QVariant &source);

    //!
    //! \brief strategy
    //! \return
    //!
    virtual QOrm::CRUDStrategy strategy() const;

    //!
    //! \brief source
    //! \return
    //!
    virtual const QVariant source()const;

    //!
    //! \brief items
    //! \return
    //!
    virtual const QVariant items()const;

    //!
    //! \brief isStrategy
    //! \param v
    //! \return
    //!
    virtual bool isStrategy(const QVariant &v) const;
};

Q_DECLARE_METATYPE(CRUDBody)
Q_DECLARE_METATYPE(CRUDBody*)
