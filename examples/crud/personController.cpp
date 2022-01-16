#include "./personController.h"
#include "./personModel.h"

ResultValue &PersonController::crud(const QVariant &vBody)
{
    //Person CRUD
    PersonModelCRUD crud(vBody, this);
    return this->lr(crud.crudify());//return success/error and created object
}

ResultValue &PersonController::upsert(const QVariant &v)
{
    //Person generic dao
    PersonModelGDao dao(this);
    PersonModel model(v);

    //insert or update record
    if(!dao.upsert(model))
        return this->lr();//return error

    return this->lr(model.toHash());//return success and created object
}

ResultValue &PersonController::remove(const QVariant &v)
{
    //Person generic dao
    PersonModelGDao dao(this);

    //remove record
    return this->lr(dao.remove(v));//return successful/error
}

ResultValue &PersonController::get(const QVariant &v)
{
    //Person generic dao
    PersonModelGDao dao(this);

    //get record
    return this->lr(dao.record(v));//return success/error and record
}

ResultValue &PersonController::list(const QVariant &vFilters)
{
    //Person generic dao
    PersonModelGDao dao(this);

    //get record list
    return this->lr(dao.recordList(vFilters));//return success/error and record list
}

ResultValue &PersonController::listPerson(const QVariant &v)
{
    QOrm::Query query(this);

    auto&select=query.builder().select();

    select
        .fields(PersonModelR)
        .from(PersonModelR);

    select
        .where()
        .equal(PersonModelM.parent_uuid_field(), v);

    //execute query
    if(!query.exec())
        return this->lr();//return error

    //make record list
    return this->lr(query.makeRecordList());//return successful and record list
}

ResultValue &PersonController::listPersonParent(const QVariant &v)
{
    QOrm::Query query(this);

    auto&select=query.builder().select();

    select
        .fields()
        .f(PersonModelM.uuid_field("a"))
        .f(PersonModelM.parent_uuid_field("a"))
        .f(PersonModelM.name_field("a"))
        .f(PersonModelM.enabled_field("a"));
    select
        .from(PersonModelR, "a");

    select
        .joinInner(PersonModelR).alias("b")
        .equal(PersonModelM.parent_uuid_field("b"), PersonModelM.parent_uuid_field("a"));

    select
        .where()
        .equal(PersonModelM.parent_uuid_field("a"), v);

    //execute query
    if(!query.exec())
        return this->lr();//return error

    //make record list
    return this->lr(query.makeRecordList());//return successful and record list
}

ResultValue &PersonController::listPersonCheck(const QVariant &v)
{
    //Person generic dao
    PersonModelGDao dao(this);

    //get record list
    if(!dao.recordList(v))
        return this->lr();//return error

    //recovery cache record list
    auto vList=dao.lr().resultToList();

    for(auto&v : vList){
        PersonModel model(v);//load model by cache
        if(!model.isValid())//check is valid
            return this->lr().setWarning("Invalid record");//return error
    }

    return this->lr(vList);//return successful record list
}

ResultValue &PersonController::runMultiTasks()
{
    //lamba to async update record

    auto errorUpdateDate=[](QSqlDatabase&, const QVariant&)
    {
        qWarning()<<"It\'s not OK";
        //..action to error
        return QVariant();
    };

    auto successUpdateDate=[](QSqlDatabase&, const QVariant&)
    {
        qWarning()<<"it\'s OK";
        //..action to success
        return QVariant();
    };

    auto executeUpdateDate=[](QSqlDatabase&connection, const QVariant&record){
        //Person generic dao
        PersonModelGDao dao;
        dao.setConnection(connection);//individual connection to task
        PersonModel model(record);//load body model
        model.set_dt_record(QDate::currentDate());//update dt

        if(!dao.upsert(model))//insert/update person
            return dao.lr().toHash();//record body error

        return model.toHash();//return new body object
    };

    PersonModelGDao dao(this);//dao to current connection
    auto vList=dao.recordList().resultToList();//list all personModel record

    QOrm::TaskRunner taskRunner(this);//task runner controller
    taskRunner
        .slotCount(QThread::idealThreadCount())//maximum thread in actual machine
        .values(vList)//personModel records
        .onExecute(executeUpdateDate)//lambda to async execute
        .onFailed(errorUpdateDate)//lambda to error notify
        .onSuccess(successUpdateDate)//lambda to success notify
        .start();//wait to finish

    return this->lr(taskRunner.lr());//return successful/error

}
