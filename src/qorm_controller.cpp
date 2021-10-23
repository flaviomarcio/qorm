#include "./qorm_controller.h"

namespace QOrm {

#define dPvt()\
    auto&p = *reinterpret_cast<ControllerPvt*>(this->p)

class ControllerPvt{
public:
    QHash<QString, ConnectionPool*>localConnection;
    explicit ControllerPvt(QObject*object){
        Q_UNUSED(object)
    }
    virtual ~ControllerPvt(){
        {
            auto map=this->localConnection;
            this->localConnection.clear();
            QHashIterator<QString, ConnectionPool*> i(map);
            while (i.hasNext()) {
                i.next();
                i.value()->finish(i.key());
            }
        }
    }
};

Controller::Controller(QObject *parent) : ObjectDb(parent)
{
    this->p=new ControllerPvt(this);
}

Controller::Controller(const QSqlDatabase &connection, QObject *parent) : ObjectDb(connection, parent)
{
    this->p=new ControllerPvt(this);
}

Controller::~Controller()
{
    dPvt();delete&p;
}

bool Controller::dbConnect(QObject *objectConnection)
{
    auto connectionPool=dynamic_cast<ConnectionPool*>(objectConnection);
    if(connectionPool==nullptr){
        auto connectionManager=dynamic_cast<ConnectionManager*>(objectConnection);
        if(connectionManager!=nullptr){
            connectionPool=&connectionManager->pool();
            if(connectionPool!=nullptr){
                return this->dbConnect(*connectionPool);
            }
        }
    }
    return false;
}

bool Controller::dbConnect(ConnectionManager &connectionManager)
{
    auto&pool=connectionManager.pool();
    return this->dbConnect(pool);
}

bool Controller::dbConnect(ConnectionPool &connectionPool)
{
    QSqlDatabase db;
    if(connectionPool.get(db)){
        dPvt();
        p.localConnection.insert(db.connectionName(), &connectionPool);
        return this->setConnection(db);
    }
    return false;
}

bool Controller::dbConnect(const QSqlDatabase &db)
{
    return this->setConnection(db);
}

bool Controller::dbConnect(const QString &connectionName)
{
    auto db=QSqlDatabase::database(connectionName);
    return this->dbConnect(db);
}

}
