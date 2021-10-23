#pragma once

#include "./qorm_object.h"

namespace QOrm {

class Q_ORM_EXPORT ReportItem : public QStm::Object
{
public:

    Q_PROPERTY(QString name READ name WRITE setName )
    Q_PROPERTY(QStringList route READ route WRITE setRoute )
    Q_PROPERTY(QVariantList strategyRunner READ strategyRunner WRITE setStrategyRunner )
    Q_PROPERTY(QVariantList strategyPublic READ strategyPublic WRITE setStrategyPublic )
    Q_PROPERTY(QVariantHash strategyList READ strategyList WRITE setStrategyList )
    Q_PROPERTY(QVariantHash sourceList READ sourceList WRITE setSourceList )


    Q_INVOKABLE explicit ReportItem(QObject *parent = nullptr);
    Q_INVOKABLE explicit ReportItem(const QVariant&itemBody, QObject *parent = nullptr);
    Q_INVOKABLE ~ReportItem();

    ReportItem&operator=(const QVariant&v);

    Q_INVOKABLE virtual ResultValue &isValid();

    Q_INVOKABLE virtual QString name() const;
    virtual ReportItem &setName(const QString &value);

    Q_INVOKABLE virtual QStringList route() const;
    virtual ReportItem &setRoute(const QVariant &value);

    Q_INVOKABLE virtual QStringList strategyRunner() const;
    virtual ReportItem &setStrategyRunner(const QVariant &value);

    Q_INVOKABLE virtual QVariantList strategyPublic() const;
    virtual ReportItem &setStrategyPublic(const QVariant &value);

    Q_INVOKABLE virtual QVariantHash strategyList() const;
    virtual ReportItem &setStrategyList(const QVariantHash &value);

    Q_INVOKABLE virtual QVariantHash sourceList() const;
    virtual ReportItem &setSourceList(const QVariantHash &value);

private:
    void*p=nullptr;
};

}
