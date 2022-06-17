#pragma once

#include "./qorm_global.h"
#include <QObject>

namespace QOrm {
class ModelDtoOptionsPvt;
//!
//! \brief The ModelDtoOptions class
//!
class Q_ORM_EXPORT ModelDtoOptions : public QObject
{
    Q_OBJECT
public:
    //!
    //! \brief ModelDtoOptions
    //! \param parent
    //!
    Q_INVOKABLE explicit ModelDtoOptions(QObject *parent = nullptr);

    //!
    //! \brief operator =
    //! \param v
    //! \return
    //!
    ModelDtoOptions &operator=(const ModelDtoOptions &v);

    //!
    //! \brief searchOnEmptyFilter
    //! \return
    //!
    Q_INVOKABLE virtual bool searchOnEmptyFilter() const;

    //!
    //! \brief setSearchOnEmptyFilter
    //! \param value
    //!
    Q_INVOKABLE virtual void setSearchOnEmptyFilter(bool value);

private:
    ModelDtoOptionsPvt *p = nullptr;
};

} // namespace QOrm
