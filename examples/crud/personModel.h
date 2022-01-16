#pragma once

#include <QOrm>

class PersonModel : public QOrm::Model
{
    Q_OBJECT
public:
    QORM_MODEL(PersonModel)
    QORM_DECLARE_MODEL_DESCRIPTION(tr("Person"))
    QORM_DECLARE_TABLE(tab000,_,person)
    QORM_DECLARE_TABLE_PRIMARY_KEY(uuid)
public:
    QORM_DECLARE_PROPERTY(QUuid  , uuid    , uuidGenerator() )
    QORM_DECLARE_PROPERTY(QString, name    , QString()       )
    QORM_DECLARE_PROPERTY(bool   , enabled , false           )
public:
    QORM_MODEL_CONSTRUCTOR(PersonModel, QOrm::Model)
};

QORM_DECLARE_CRUD(PersonModel)
