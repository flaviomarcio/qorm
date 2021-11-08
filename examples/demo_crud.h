#pragma once

#include "./qorm_object_db.h"
#include "./demo_model.h"

namespace QOrm {

class Q_ORM_EXPORT ControllerCRUD : public QOrm::ObjectDb
{
    Q_OBJECT
public:
    Q_INVOKABLE explicit ControllerCRUD(QObject *parent = nullptr):QOrm::ObjectDb(parent){
    }
    Q_INVOKABLE ~ControllerCRUD(){
    }

    ResultValue &crud(const QVariant &vBody)
    {
        SpecialtiesCRUD crud(vBody, this);
        QORM_CRUD_BLOCK_MAKER(crudBlock)<<&crud;
        return this->lr(crudBlock.crudify());
    }

};

}
