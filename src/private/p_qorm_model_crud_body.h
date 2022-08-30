#pragma once

#include <QVariant>
#include <QVariantHash>
#include "../qorm_global.h"
#include "../qorm_model_crud_types.h"

namespace QOrm {
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

    explicit CRUDBody(const QVariant &strategy, const QVariant &expressions, const QVariant &source);

    //!
    //! \brief strategy
    //! \return
    //!
    CRUDTypes::Strategy strategy() const;

    //!
    //! \brief expressions
    //! \return
    //!
    QVariant expressions()const;

    //!
    //! \brief source
    //! \return
    //!
    QVariant source()const;

    //!
    //! \brief items
    //! \return
    //!
    QVariantList items()const;

    //!
    //! \brief data
    //! \return
    //!
    QVariant data()const;

    //!
    //! \brief pageList
    //! \return
    //!
    QVariantList pagesList()const;

    //!
    //! \brief pagesHash
    //! \return
    //!
    QVariantHash pagesHash()const;

    //!
    //! \brief itemsList
    //! \return
    //!
    QVariantList itemsList()const;

    //!
    //! \brief isStrategy
    //! \param v
    //! \return
    //!
    virtual bool isStrategy(const QVariant &v);

    //!
    //! \brief resultInfo
    //! \return
    //!
    QVariant resultInfo() const;
};

}

typedef QOrm::CRUDBody CRUDBody;
