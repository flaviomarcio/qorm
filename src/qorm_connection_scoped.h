#pragma once

#include "./qorm_global.h"
#include <QSqlDatabase>

namespace QOrm {
class ConnectionScopedPvt;
//!
//! \brief The ConnectionScoped class
//!
class Q_ORM_EXPORT ConnectionScoped : public QObject
{
    Q_OBJECT
public:
    //!
    //! \brief ConnectionScoped
    //! \param parent
    //!
    Q_INVOKABLE explicit ConnectionScoped(QObject *parent = nullptr);
    Q_INVOKABLE explicit ConnectionScoped(const QSqlDatabase &connection, QObject *parent = nullptr);
    Q_INVOKABLE explicit ConnectionScoped(const QVariantHash &settings, QObject *parent = nullptr);

    //!
    //! \brief connectionId
    //! \return
    //!
    static QByteArray connectionId();
    static QSqlDatabase connection();

    bool transaction();
    bool commit();
    bool rollback();

private:
    ConnectionScopedPvt *p=nullptr;
};

} // namespace QOrm
