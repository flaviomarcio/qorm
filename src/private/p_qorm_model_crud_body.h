#pragma once

#include <QVariant>
#include <QVariantHash>

#include "../qorm_model_crud_types.h"

class Q_ORM_EXPORT CRUDBody:public QVariantHash
{
public:
    explicit CRUDBody(const QVariant &other=QVariant());
    explicit CRUDBody(const QVariant &strategy, const QVariant &source);
    explicit CRUDBody(const QOrm::CRUDStrategy strategy, const QVariant &source);
    virtual ~CRUDBody();

    virtual QOrm::CRUDStrategy strategy() const;
    virtual const QVariant &source();
    virtual bool isStrategy(const QVariant&v) const;
};

Q_DECLARE_METATYPE(CRUDBody)
Q_DECLARE_METATYPE(CRUDBody*)
