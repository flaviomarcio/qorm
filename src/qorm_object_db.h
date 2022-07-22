#pragma once

#include <QSqlDatabase>
#include "./qorm_global.h"
#include "./qorm_object.h"
//#include "./qorm_types.h"

namespace QOrm {
class ObjectDbPvt;

//!
//! \brief The ObjectDb class
//!
class Q_ORM_EXPORT ObjectDb : public QOrm::Object
{
    Q_OBJECT
public:
    //!
    //! \brief ObjectDb
    //! \param parent
    //!
    Q_INVOKABLE explicit ObjectDb(QObject *parent = nullptr);
    explicit ObjectDb(const QSqlDatabase &connection, QObject *parent = nullptr);


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
    Q_INVOKABLE virtual QByteArray connectionId() const;

private:
    ObjectDbPvt *p = nullptr;
};

} // namespace QOrm
