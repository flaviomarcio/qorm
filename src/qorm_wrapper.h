#pragma once

#include "./qstm_result.h"

namespace QOrm {

//!
//! \brief The Wrapper class
//!
class Wrapper
{
public:
    //!
    //! \brief Wrapper
    //! \param v
    //!
    explicit Wrapper(const QVariant&v);

    //!
    //! \brief Wrapper
    //! \param v
    //!
    explicit Wrapper(ResultValue &v);

    //!
    //! \brief ~Wrapper
    //!
    ~Wrapper();

    //!
    //! \brief w
    //! \param propertySrc
    //! \param propertyDestine
    //! \return
    //!
    Wrapper&w(const QString &propertySrc, const QString &propertyDestine);

    //!
    //! \brief w
    //! \param propertySrc
    //! \return
    //!
    Wrapper&w(const QString &propertySrc);

    //!
    //! \brief clear
    //! \return
    //!
    virtual Wrapper&clear();

    //!
    //! \brief v
    //! \return
    //!
    virtual QVariant &v() const;
private:
    void*p=nullptr;
};

}
