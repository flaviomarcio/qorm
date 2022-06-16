#include "./qorm_controller.h"

namespace QOrm {

class ControllerPvt:public QObject
{
public:
    QHash<QString, ConnectionPool *> localConnection;
    explicit ControllerPvt(QObject *object=nullptr):QObject{} { Q_UNUSED(object) }
    virtual ~ControllerPvt()
    {
        auto vHash = this->localConnection;
        this->localConnection.clear();
        QHashIterator<QString, ConnectionPool *> i(vHash);
        while (i.hasNext()) {
            i.next();
            i.value()->finish(i.key());
        }
    }
};

Controller::Controller(QObject *parent) : ObjectDb{parent}
{
    this->p = new ControllerPvt{this};
}

Controller::Controller(const QSqlDatabase &connection, QObject *parent)
    : ObjectDb(connection, parent)
{
    this->p = new ControllerPvt{this};
}

Controller::~Controller()
{

}

bool Controller::dbConnect(QObject *objectConnection)
{
    auto connectionPool = dynamic_cast<ConnectionPool *>(objectConnection);
    if (connectionPool != nullptr)
        return this->dbConnect(*connectionPool);

    auto connectionManager = dynamic_cast<ConnectionManager *>(objectConnection);
    if (connectionManager == nullptr)
        return false;

    connectionPool = &connectionManager->pool();
    if (connectionPool == nullptr)
        return false;
    return this->dbConnect(*connectionPool);
}

bool Controller::dbConnect(ConnectionManager &connectionManager)
{
    auto &pool = connectionManager.pool();
    return this->dbConnect(pool);
}

bool Controller::dbConnect(ConnectionPool &connectionPool)
{
    QSqlDatabase db;
    if (!connectionPool.get(db))
        return false;

    p->localConnection.insert(db.connectionName(), &connectionPool);
    return this->setConnection(db);
}

bool Controller::dbConnect(const QSqlDatabase &db)
{
    return this->setConnection(db);
}

bool Controller::dbConnect(const QString &connectionName)
{
    auto db = QSqlDatabase::database(connectionName);
    return this->dbConnect(db);
}

} // namespace QOrm
