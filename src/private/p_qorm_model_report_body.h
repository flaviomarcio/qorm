#pragma once

#include <QVariant>
#include <QVariantHash>

#include "../qorm_global.h"

class Q_ORM_EXPORT ReportBody:public QVariantHash
{
public:
    explicit ReportBody(const QVariant &other=QVariant());
    explicit ReportBody(const QVariant &strategy, const QVariant &source);
    virtual ~ReportBody();

    virtual const QVariant &strategy();
    virtual const QVariant &source();
    virtual bool isStrategy(const QVariant&v);
};

Q_DECLARE_METATYPE(ReportBody)
Q_DECLARE_METATYPE(ReportBody*)
