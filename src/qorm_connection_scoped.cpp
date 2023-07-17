#include "./qorm_connection_scoped.h"
#include "./qorm_connection_pool.h"
#include <QVariantHash>
#include <QThread>
#include <QMutex>

#define canConvert(CLASS)

namespace QOrm {

typedef QHash<QThread*, QByteArray> ConnectionScopedCollection;

Q_GLOBAL_STATIC(QMutex, connectionScopedLocker);
Q_GLOBAL_STATIC(ConnectionScopedCollection, connectionScopedCollection)

class ConnectionScopedPvt:public QObject
{
public:
    QOrm::ConnectionPool pool;
    QSqlDatabase connection;
    explicit ConnectionScopedPvt(QObject*parent):QObject{parent}
    {
    }
    explicit ConnectionScopedPvt(QObject *parent, QObject *objectTarget, const QSqlDatabase &connectionTarget, const QVariantHash &settingTarget)
        :QObject{parent}
    {
        if(connectionScopedCollection->contains(QThread::currentThread()))
            return;

        if(connectionTarget.isValid())
            pool.from(connectionTarget);
        else if(!settingTarget.isEmpty())
            pool.from(settingTarget);
        else if(objectTarget)
            pool.from(objectTarget);

        if(!pool.get(this->connection))
            qFatal("Invalid connection");

        scopedAdd(this->connection.connectionName());


    }

    static void scopedAdd(QString connectionName)
    {
        QMutexLocker<QMutex> locker(connectionScopedLocker);
        connectionScopedCollection->insert(QThread::currentThread(), connectionName.toUtf8());
    }

    static QByteArray scopedGet()
    {
        QMutexLocker<QMutex> locker(connectionScopedLocker);
        return connectionScopedCollection->value(QThread::currentThread());
    }

    static void scopedRemove()
    {
        QMutexLocker<QMutex> locker(connectionScopedLocker);
        connectionScopedCollection->remove(QThread::currentThread());
    }

    ~ConnectionScopedPvt()
    {
        if(connection.isValid())
            return;
        scopedRemove();
        pool.finish(connection);
    }
};

ConnectionScoped::ConnectionScoped(QObject *parent)
    : QObject{parent}, p{new ConnectionScopedPvt{this,parent, {},{}}}
{
}

ConnectionScoped::ConnectionScoped(const QSqlDatabase &connection, QObject *parent)
    : QObject{parent}, p{new ConnectionScopedPvt{this,parent,connection,{}}}
{
}

ConnectionScoped::ConnectionScoped(const QVariantHash &setting, QObject *parent)
    : QObject{parent}, p{new ConnectionScopedPvt{this,parent,{},setting}}
{
}

QByteArray ConnectionScoped::connectionId()
{
    return ConnectionScopedPvt::scopedGet();
}

QSqlDatabase ConnectionScoped::connection()
{
    auto connectionId=ConnectionScopedPvt::scopedGet();
    return connectionId.isEmpty()
        ?QSqlDatabase()
               :QSqlDatabase::database(connectionId);
}

bool ConnectionScoped::transaction()
{
    return p->connection.transaction();
}

bool ConnectionScoped::commit()
{
    return p->connection.commit();
}

bool ConnectionScoped::rollback()
{
    return p->connection.rollback();
}

} // namespace QOrm
