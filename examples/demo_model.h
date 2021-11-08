#pragma once

#include "./qorm_model.h"

class DemoModel : public QOrm::Model
{
    Q_OBJECT
public:
    QORM_MODEL(DemoModel)
    QORM_DECLARE_TABLE(reg08aab,_,product_departament)
public:
    QORM_DECLARE_PROPERTY(QUuid      , uuid             , uuidGenerator() )
    QORM_DECLARE_PROPERTY(QString    , name             , QString()       )
    QORM_DECLARE_PROPERTY(bool       , enabled          , false           )
public:
    QORM_MODEL_CONSTRUCTOR(DemoModel, QOrm::Model)
};

QORM_DECLARE_CRUD(DemoModel)
