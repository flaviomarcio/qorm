#include "./qorm_object_db_extension.h"
#include "./qorm_connection_scoped.h"
#include "./qorm_const.h"
#include "./qorm_macro.h"
#include <QMetaMethod>
#include <QMetaObject>
#include <QMetaProperty>

namespace QOrm {

class ObjectDbExtensionPvt
{
public:
    QObject *parent = nullptr;
    QByteArray connectionId;
    explicit ObjectDbExtensionPvt(QObject *parent): parent{parent}
    {
    }

    virtual ~ObjectDbExtensionPvt()
    {
    }

    bool invokeMethodConnection(QObject *objectCheck, QSqlDatabase &outConnection)
    {
        if(objectCheck==nullptr)
            return {};

        static QByteArray connectionIdMethod(QByteArrayLiteral("connectionid"));
        auto metaObject = objectCheck->metaObject();

        QMetaMethod objectMethod;
        for (int i = 0; i < metaObject->methodCount(); ++i) {
            auto method = metaObject->method(i);

            if (method.parameterCount() > 0)
                continue;

            if (method.methodType() != QMetaMethod::Method)
                continue;

            auto returnType=QMetaType::Type(method.returnType());
            if (returnType != QMetaType::QString && returnType != QMetaType::QByteArray)
                continue;

            if (method.name().toLower() != connectionIdMethod)
                continue;

            objectMethod=method;
            break;
        }

        if(!objectMethod.isValid()){
            return {};
        }

        QByteArray returnValue;

        auto invokeReturn = Q_RETURN_ARG(QByteArray, returnValue);

        if (!objectMethod.invoke(objectCheck, Qt::DirectConnection, invokeReturn)) {
#ifdef Q_ORM_LOG_SUPER_VERBOSE
            oWarning() << QStringLiteral("method:[%1::%2]: error on invoke").arg(objectCheck->metaObject()->className(), objectMethod.name());
#endif
            return {};
        }

        if (returnValue.isEmpty())
            return {};

        auto connection = QSqlDatabase::database(returnValue);
        if (!connection.isValid() || !connection.isOpen())
            return {};

        outConnection = connection;
        return true;
    };

    QSqlDatabase connectionGet()
    {
        auto connection = this->connectionId.isEmpty()
                              ?QSqlDatabase{}
                              :QSqlDatabase::database(this->connectionId);
        if (connection.isOpen())
            return connection;

        auto parentConnection = this->parent;
        if (parentConnection == nullptr)
            return {};

        QVector<QObject *> parentVector;
        auto loopObject = parentConnection;
        while (loopObject != nullptr) {
            parentVector.append(loopObject);
            loopObject = loopObject->parent();
        }
        for (auto &invokeObject : parentVector) {
            if (!this->invokeMethodConnection(invokeObject, connection))
                continue;
            return connection;
        }
        connection=QOrm::ConnectionScoped::connection();
        if (!connection.isValid() || !connection.isOpen())
            return {};

        return connection;
    }
};

ObjectDbExtension::ObjectDbExtension(QObject *parent)
    : p{new ObjectDbExtensionPvt{parent}}
{
}

ObjectDbExtension::~ObjectDbExtension()
{
    delete p;
}

QByteArray ObjectDbExtension::connectionId() const
{
    return p->connectionId;
}

QSqlDatabase ObjectDbExtension::connection() const
{
    return p->connectionGet();
}

bool ObjectDbExtension::setConnection(const QSqlDatabase &connection)
{
    if (connection.isValid() && connection.isOpen())
        p->connectionId = connection.connectionName().toUtf8();
    else
        p->connectionId.clear();
    return !p->connectionId.isEmpty();
}

bool ObjectDbExtension::setConnection(const QString &connectionName)
{
    auto connection = QSqlDatabase::database(connectionName);
    if (connection.isValid() && connection.isOpen())
        p->connectionId = connection.connectionName().toUtf8();
    else
        p->connectionId.clear();
    return !p->connectionId.isEmpty();
}



} // namespace QOrm
