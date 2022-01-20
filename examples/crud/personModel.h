#pragma once

#include <QOrm>

//!
//! \brief The PersonModel class
//!
//! inherits of QOrm::Model
class PersonModel : public QOrm::Model
{
    Q_OBJECT
public:
    QORM_MODEL(PersonModel)//make default methods for PersonModel
    QORM_DECLARE_MODEL_DESCRIPTION(tr("Person"))//description
    QORM_DECLARE_TABLE_SCHEMA()//declare schema name in database
    QORM_DECLARE_TABLE(tab000,_,person)//table in database
    QORM_DECLARE_TABLE_PRIMARY_KEY(uuid)//inform PK name
    QORM_DECLARE_TABLE_FOREIGN_KEY(parent_uuid)//declare FK
    QORM_DESCRIPTOR_ORDERBY(name)//declare default order by
public:
    QORM_DECLARE_PROPERTY(QUuid  , uuid         , uuidGenerator()   )//declare field and default value
    QORM_DECLARE_PROPERTY(QDate  , dt_record    , QDate()           )//declare field and default value
    QORM_DECLARE_PROPERTY(QUuid  , parent_uuid  , QUuid()           )//declare field and default value
    QORM_DECLARE_PROPERTY(QString, name         , QString()         )//declare field and default value
    QORM_DECLARE_PROPERTY(bool   , enabled      , false             )//declare field and default value
public:
    QORM_MODEL_CONSTRUCTOR(PersonModel, QOrm::Model)//contructors for PersonModel
};

//REGISTER MODEL AND DECLARE CRUD AND DAO
QORM_DECLARE_CRUD(PersonModel)
