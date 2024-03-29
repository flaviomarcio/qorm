#pragma once

//#include "./qorm_sql_suitable_types.h"
#include <QObject>
#include "./qorm_global.h"
#include <QSqlDatabase>

namespace QOrm {
class SqlSuitableValuePvt;
//!
//! \brief The SqlSuitableValue class
//!
class Q_ORM_EXPORT SqlSuitableValue:public QObject
{
    Q_OBJECT
public:
    //!
    //! \brief The Format struct
    //!
    struct Format
    {
    public:
        //!
        //! \brief timesTamp
        //! \return
        //!
        QString timesTamp() const;

        //!
        //! \brief date
        //! \return
        //!
        QString date() const;

        //!
        //! \brief time
        //! \return
        //!
        QString time() const;

        //!
        //! \brief doublePrecision
        //! \return
        //!
        int doublePrecision() const;

        //!
        //! \brief currencyPrecision
        //! \return
        //!
        int currencyPrecision() const;
    };

    //!
    //! \brief SqlSuitableValue
    //! \param db
    //!
    explicit SqlSuitableValue(QSqlDatabase db, QObject *parent);

    //!
    //! \brief SqlSuitableValue
    //!
    Q_INVOKABLE explicit SqlSuitableValue(QObject *parent=nullptr);

    //!
    //! \brief toUuid
    //! \param v
    //! \return
    //!
    QString toUuid(const QVariant &v);

    //!
    //! \brief toUuid
    //! \param v
    //! \return
    //!
    QString toUuid(const QUuid &v);

    //!
    //! \brief toUrl
    //! \param v
    //! \return
    //!
    QString toUrl(const QUrl &v);

    //!
    //! \brief toInt
    //! \param v
    //! \return
    //!
    QString toInt(const qlonglong &v);

    //!
    //! \brief toLng
    //! \param v
    //! \return
    //!
    QString toLng(const qlonglong &v);

    //!
    //! \brief toDbl
    //! \param v
    //! \return
    //!
    QString toDbl(const double &v);

    //!
    //! \brief toCur
    //! \param v
    //! \return
    //!
    QString toCur(const double &v);

    //!
    //! \brief toStr
    //! \param v
    //! \return
    //!
    QString toStr(const QString &v);

    //!
    //! \brief toStr
    //! \param v
    //! \return
    //!
    QString toStr(const QByteArray &v);

    //!
    //! \brief toStrPure
    //! \param v
    //! \return
    //!
    QString toStrPure(const QString &v);

    //!
    //! \brief toDatMin
    //! \param v
    //! \return
    //!
    QString toDatMin(const QDateTime &v);

    //!
    //! \brief toDatMax
    //! \param v
    //! \return
    //!
    QString toDatMax(const QDateTime &v);

    //!
    //! \brief toDat
    //! \param v
    //! \return
    //!
    QString toDat(const QVariant &v);

    //!
    //! \brief toDat
    //! \param v
    //! \return
    //!
    QString toDat(const QDate &v);

    //!
    //! \brief toDat
    //! \param v
    //! \return
    //!
    QString toDat(const QDateTime &v);

    //!
    //! \brief toTim
    //! \param v
    //! \return
    //!
    QString toTim(const QTime &v);

    //!
    //! \brief toTim
    //! \param v
    //! \return
    //!
    QString toTim(const QDateTime &v);

    //!
    //! \brief toBoo
    //! \param v
    //! \return
    //!
    QString toBoo(const bool &v);

    //!
    //! \brief toVar
    //! \param v
    //! \return
    //!
    QString toVar(const QVariant &v);

    //!
    //! \brief toVar
    //! \param v
    //! \param vType
    //! \return
    //!
    QString toVar(const QVariant &v, const int &vType);

    //!
    //! \brief toLikeLR
    //! \param v
    //! \return
    //!
    static QString toLikeLR(const QVariant &v);

    //!
    //! \brief toLikeL
    //! \param v
    //! \return
    //!
    static QString toLikeL(const QVariant &v);

    //!
    //! \brief toLikeR
    //! \param v
    //! \return
    //!
    static QString toLikeR(const QVariant &v);

    //!
    //! \brief connection
    //! \return
    //!
    QSqlDatabase connection();

    //!
    //! \brief setConnection
    //! \param db
    //! \return
    //!
    SqlSuitableValue &setConnection(const QSqlDatabase &db);

    //!
    //! \brief format
    //! \return
    //!
    static Format &format();

private:
    SqlSuitableValuePvt *p = nullptr;
};

} // namespace QOrm
