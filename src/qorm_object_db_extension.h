#pragma once

#include <QSqlDatabase>
#include "./qorm_global.h"

namespace QOrm {
class ObjectDbExtensionPvt;

//!
//! \brief The ObjectDbExtension class
//!
class Q_ORM_EXPORT ObjectDbExtension
{
public:
    //!
    //! \brief ObjectDbExtension
    //! \param parent
    //!
    explicit ObjectDbExtension(QObject *parent = nullptr);

    //!
    //! \brief ~ObjectDbExtension
    //!
    virtual ~ObjectDbExtension();

    //!
    //! \brief connectionId
    //! \return
    //!
    QByteArray connectionId() const;

    //!
    //! \brief connection
    //! \return
    //!
    QSqlDatabase connection() const;

    //!
    //! \brief setConnection
    //! \param connection
    //! \return
    //!
    bool setConnection(const QSqlDatabase &connection);

    //!
    //! \brief setConnection
    //! \param connectionId
    //! \return
    //!
    bool setConnection(const QString &connectionId);


private:
    ObjectDbExtensionPvt *p = nullptr;
};


} // namespace QOrm
