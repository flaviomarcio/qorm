#include <QCoreApplication>

//include of model and controller
#include "./personModel.h"
#include "./personController.h"

Q_GLOBAL_STATIC(QOrm::ConnectionManager, connectionManager);

//!
//! \brief createConnection
//! \param connection
//! \return
//! method for create connection
bool createConnection(QSqlDatabase&connection)
{
    //set settings to run service
    if(!connectionManager->setSettingsFileName(QStringLiteral(":/settings.json"))){
        qWarning()<<"invalid settings";
        return 0;
    }

    //request new connection
    if(!connectionManager->pool().get(connection))
        return false;

    return true;
}


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QSqlDatabase connection;

    //create new connection
    if(!createConnection(connection))
        return 0;//normal exit

    //controller person
    PersonController controller(connection);

    {//crud run
        QVariantHash v{};//structure crud
        qWarning()<<controller.crud(v);
    }

    //create uuid to person
    auto personUuid=QUuid::createUuid();

    {//insert/update for person
        PersonModel model;
        model.set_uuid(personUuid);
        model.set_name("Nick");
        model.set_enabled(true);
        qWarning()<<controller.upsert(model.toHash());
    }

    {//get for person
        //by uuid
        qWarning()<<controller.get(personUuid);

        //by model body
        PersonModel model(personUuid);
        qWarning()<<controller.get(model.toHash());
    }

    {//list records for person
        //by uuid
        qWarning()<<controller.list(personUuid);

        //by model body
        PersonModel model(personUuid);
        qWarning()<<controller.list(model.toHash());
    }

    {//remove for person
        //by uuid
        qWarning()<<controller.remove(personUuid);

        //by model body
        PersonModel model(personUuid);
        qWarning()<<controller.remove(model.toHash());
    }

    {//list parent for person
        //by uuid
        qWarning()<<controller.listPersonParent(personUuid);

        //by model body
        PersonModel model(personUuid);
        qWarning()<<controller.listPersonParent(model.toHash());
    }

    {//list and check for person
        //by uuid
        qWarning()<<controller.listPersonCheck(personUuid);

        //by model body
        PersonModel model(personUuid);
        qWarning()<<controller.listPersonCheck(model.toHash());
    }

    {//update dt_record for person using multi tasks connection
        qWarning()<<controller.runMultiTasks();
    }

    return a.exec();
}
