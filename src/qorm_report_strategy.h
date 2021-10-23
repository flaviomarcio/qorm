#pragma once

#include "./qorm_object.h"

namespace QOrm {

class Q_ORM_EXPORT ReportStrategy : public QStm::Object
{
public:
    Q_INVOKABLE explicit ReportStrategy(QObject *parent = nullptr);
    Q_INVOKABLE explicit ReportStrategy(const QVariantHash&strategy, QObject *parent = nullptr);
    Q_INVOKABLE ~ReportStrategy();

    virtual ResultValue &isValid();

    Q_INVOKABLE virtual QVariantHash strategy() const;
    virtual ReportStrategy &setStrategy(const QVariant &value);

    Q_INVOKABLE virtual QString name() const;
    virtual ReportStrategy &setName(const QString &value);

    Q_INVOKABLE virtual QVariantHash cache() const;
    virtual ReportStrategy &setCache(const QVariantHash &value);

    Q_INVOKABLE virtual QVariantHash connection() const;
    virtual ReportStrategy &setConnection(const QVariantHash &value);

    Q_INVOKABLE virtual QVariantHash header() const;
    virtual ReportStrategy &setHeader(const QVariantHash &value);

    Q_INVOKABLE virtual QVariantHash filter() const;
    virtual ReportStrategy &setFilter(const QVariantHash &value);

    Q_INVOKABLE virtual QVariantHash chart() const;
    virtual ReportStrategy &setChart(const QVariantHash &value);

    Q_INVOKABLE virtual QVariant source() const;
    virtual ReportStrategy &setSource(const QVariant &value);

    Q_INVOKABLE virtual QVariant data() const;
    virtual ReportStrategy &setData(const QVariant &value);

private:
    void*p=nullptr;
};

}
