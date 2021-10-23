#pragma once

#include "./qorm_sql_suitable_types.h"

namespace QOrm {

class SqlSuitableValue
{
public:

    struct Format{
    public:
        QString timesTamp() const;
        QString date() const;
        QString time() const;
        int doublePrecision() const;
        int currencyPrecision() const;
    };

    explicit SqlSuitableValue(QSqlDatabase db);
    explicit SqlSuitableValue();

    virtual ~SqlSuitableValue();

    QString toUuid(const QVariant&v);
    QString toUuid(const QUuid&v);
    QString toUrl(const QUrl&v);
    QString toInt(const qlonglong&v);
    QString toLng(const qlonglong&v);
    QString toDbl(const double&v);
    QString toCur(const double&v);
    QString toStr(const QString&v);
    QString toStr(const QByteArray&v);
    QString toStrPure(const QString&v);
    QString toDatMin(const QDateTime&v);
    QString toDatMax(const QDateTime&v);
    QString toDat(const QVariant&v);
    QString toDat(const QDate&v);
    QString toDat(const QDateTime&v);
    QString toTim(const QTime&v);
    QString toTim(const QDateTime&v);
    QString toBoo(const bool&v);
    QString toVar(const QVariant&v);
    QString toVar(const QVariant &v, const QVariant::Type&vType);

    static QString toLikeLR(const QVariant &v);
    static QString toLikeL(const QVariant &v);
    static QString toLikeR(const QVariant &v);


    QSqlDatabase connection();
    SqlSuitableValue&setConnection(const QSqlDatabase&db);

    static Format&format();

private:
    void*p=nullptr;
};

}
