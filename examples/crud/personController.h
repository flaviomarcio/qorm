#pragma once

#include <QOrm>
#include "./personModel.h"

namespace QOrm {

class Q_ORM_EXPORT personController : public QOrm::ObjectDb
{
    Q_OBJECT
public:
    Q_INVOKABLE explicit personController(QObject *parent = nullptr):QOrm::ObjectDb(parent)
    {
    }

    ~personController()
    {
    }

    ResultValue &crud(const QVariant &vBody)
    {
        PersonModelCRUD crud(vBody, this);
        return this->lr(crud.crudify());
    }

};

}
