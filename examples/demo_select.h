#pragma once

#include "./qorm_object_db.h"

namespace QOrm {

class Q_ORM_EXPORT ControllerSELECT : public QOrm::ObjectDb
{
    Q_OBJECT
public:
    Q_INVOKABLE explicit ControllerSELECT(QObject *parent = nullptr):QOrm::ObjectDb(parent){
    }
    Q_INVOKABLE ~ControllerSELECT(){
    }
};

}
