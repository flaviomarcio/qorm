#pragma once

#include <QVariant>
#include <QVariantHash>
#include "../qorm_global.h"

//!
//! \brief The ReportBody class
//!
class Q_ORM_EXPORT ReportBody:public QVariantHash
{
public:
    //!
    //! \brief ReportBody
    //! \param other
    //!
    explicit ReportBody(const QVariant &other=QVariant());

    //!
    //! \brief ReportBody
    //! \param strategy
    //! \param source
    //!
    explicit ReportBody(const QVariant &strategy, const QVariant &source);

    //!
    //! \brief ~ReportBody
    //!
    virtual ~ReportBody();

    //!
    //! \brief strategy
    //! \return
    //!
    virtual const QVariant &strategy();

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
    virtual bool isStrategy(const QVariant&v);
};

Q_DECLARE_METATYPE(ReportBody)
Q_DECLARE_METATYPE(ReportBody*)
