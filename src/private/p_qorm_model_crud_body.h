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

    //!
    //! \brief CRUDBody
    //! \param strategy
    //! \param expressions
    //! \param source
    //!
    explicit CRUDBody(const QVariant &strategy, const QVariant &expressions, const QVariant &source);

    //!
    //! \brief strategy
    //! \return
    //!
    CRUDTypes::Strategy strategy() const;

    //!
    //! \brief strategyName
    //! \return
    //!
    QString strategyName() const;

    //!
    //! \brief canStrategy
    //! \param strategy
    //! \return
    //!
    bool canStrategy(const QVariant &strategy);

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
    //! \brief customSource
    //! \return
    //!
    QVariant customSource()const;

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
    //! \brief isValid
    //! \return
    //!
    virtual bool isValid() const;

    //!
    //! \brief isStrategy
    //! \param v
    //! \return
    //!
    virtual bool isStrategy(const QVariant &v) const;

    //!
    //! \brief isStrategyModify
    //! \return
    //!
    virtual bool isStrategyModify();

    //!
    //! \brief resultInfo
    //! \return
    //!
    QVariant resultInfo() const;
};

}

typedef QOrm::CRUDBody CRUDBody;
