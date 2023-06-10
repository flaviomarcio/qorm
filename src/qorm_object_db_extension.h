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
    Q_INVOKABLE explicit ObjectDbExtension(QObject *parent = nullptr);

    //!
    //! \brief ObjectDbExtension
    //! \param connection
    //! \param parent
    //!
    explicit ObjectDbExtension(const QSqlDatabase &connection, QObject *parent = nullptr);

    //!
    //! \brief ~ObjectDbExtension
    //!
    virtual ~ObjectDbExtension();

    //!
    //! \brief connection
    //! \return
    //!
    Q_INVOKABLE virtual QSqlDatabase connection() const;

    //!
    //! \brief setConnection
    //! \param connection
    //! \return
    //!
    Q_INVOKABLE virtual bool setConnection(const QSqlDatabase &connection);

    //!
    //! \brief setConnection
    //! \param connectionId
    //! \return
    //!
    Q_INVOKABLE virtual bool setConnection(const QString &connectionId);

    //!
    //! \brief connectionId
    //! \return
    //!
    Q_INVOKABLE virtual QByteArray &connectionId() const;

private:
    ObjectDbExtensionPvt *p = nullptr;
};

} // namespace QOrm
