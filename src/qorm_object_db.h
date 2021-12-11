#pragma once

#include "./qorm_global.h"
#include "./qorm_types.h"
#include "./qstm_object.h"
#include "./qstm_result.h"
#include "./qstm_util_variant.h"
#include "./qstm_util_date.h"
#include "./qstm_util_double.h"

namespace QOrm {

//!
//! \brief The ObjectDb class
//!
class Q_ORM_EXPORT ObjectDb: public QStm::Object
{
    Q_OBJECT
public:
    //!
    //! \brief ObjectDb
    //! \param parent
    //!
    Q_INVOKABLE explicit ObjectDb(QObject *parent = nullptr);
    explicit ObjectDb(const QSqlDatabase&connection, QObject *parent = nullptr);
    virtual ~ObjectDb();

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

    QT_DEPRECATED_X("Use Q_DECLARE_VU;")
    Q_INVOKABLE virtual VariantUtil&vu();
    QT_DEPRECATED_X("Use Q_DECLARE_VU;")
    Q_INVOKABLE virtual VariantUtil&vu(const QVariant&v);
    QT_DEPRECATED_X("Use Q_DECLARE_VU;")
    Q_INVOKABLE virtual VariantUtil&vUtil();
    QT_DEPRECATED_X("Use Q_DECLARE_VU;")
    Q_INVOKABLE virtual VariantUtil&vUtil(const QVariant&v);
    QT_DEPRECATED_X("Use Q_DECLARE_VU;")
    QT_DEPRECATED_X("Use Q_DECLARE_DU;")
    Q_INVOKABLE virtual DateUtil&dtUtil();
    QT_DEPRECATED_X("Use Q_DECLARE_DU;")
    Q_INVOKABLE virtual DateUtil&dtUtil(const QVariant&v);
    QT_DEPRECATED_X("Use Q_DECLARE_DU;")
    Q_INVOKABLE virtual DateUtil&dtu();
    QT_DEPRECATED_X("Use Q_DECLARE_DU;")
    Q_INVOKABLE virtual DateUtil&dtu(const QVariant&v);
    QT_DEPRECATED_X("Use Q_DECLARE_DBU;")
    Q_INVOKABLE virtual DoubleUtil&dbUtil();
    QT_DEPRECATED_X("Use Q_DECLARE_DBU;")
    Q_INVOKABLE virtual DoubleUtil&dbUtil(const QVariant&v);
    QT_DEPRECATED_X("Use Q_DECLARE_DBU;")
    Q_INVOKABLE virtual DoubleUtil&dbu();
    QT_DEPRECATED_X("Use Q_DECLARE_DBU;")
    Q_INVOKABLE virtual DoubleUtil&dbu(const QVariant&v);

private:
    void*p=nullptr;
};

}
