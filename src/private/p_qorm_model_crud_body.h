#pragma once

#include <QVariant>
#include <QVariantHash>

#include "../qorm_model_crud_types.h"

//!
class Q_ORM_EXPORT CRUDBody:public QVariantHash
{
public:
    //!
    //! \brief CRUDBody
    //! \param other
    //!
    explicit CRUDBody(const QVariant &other=QVariant());

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
    //! \brief ~CRUDBody
    //!
    virtual ~CRUDBody();

    //!
    //! \brief strategy
    //! \return
    //!
    virtual QOrm::CRUDStrategy strategy() const;

    //!
    //! \brief source
    //! \return
    //!
    virtual const QVariant &source();

    //!
    //! \brief isStrategy
    //! \param v
    //! \return
    //!
    virtual bool isStrategy(const QVariant&v) const;
};

Q_DECLARE_METATYPE(CRUDBody)
Q_DECLARE_METATYPE(CRUDBody*)
