#pragma once

#include "./qorm_global.h"
#include "../../qstm/src/qstm_result.h"

namespace QOrm {
class WrapperPvt;
//!
//! \brief The Wrapper class
//!
class Q_ORM_EXPORT Wrapper:public QObject
{
    Q_OBJECT
public:
    //!
    //! \brief Wrapper
    //! \param v
    //!
    explicit Wrapper(const QVariant &v);

    //!
    //! \brief Wrapper
    //! \param v
    //!
    explicit Wrapper(ResultValue &v);


    //!
    //! \brief w
    //! \param propertySrc
    //! \param propertyDestine
    //! \return
    //!
    Wrapper &w(const QString &propertySrc, const QString &propertyDestine);

    //!
    //! \brief w
    //! \param propertySrc
    //! \return
    //!
    Wrapper &w(const QString &propertySrc);

    //!
    //! \brief clear
    //! \return
    //!
    virtual Wrapper &clear();

    //!
    //! \brief v
    //! \return
    //!
    virtual QVariant &v() const;

private:
    WrapperPvt *p = nullptr;
};

} // namespace QOrm
