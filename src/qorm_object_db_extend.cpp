#include "./qorm_object_db_extension.h"
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
    explicit ObjectDbExtensionPvt(QObject *parent, const QByteArray &connectionId)
        :
        parent{parent},
        connectionId{connectionId}
    {
    }

    bool invokeMethodConnection(QObject *objectCheck, QSqlDatabase &outConnection)
    {
        static QByteArray connectionIdMethod(QByteArrayLiteral("connectionid"));
        auto metaObject = objectCheck->metaObject();
        for (int i = 0; i < metaObject->methodCount(); ++i) {
            auto method = metaObject->method(i);

            if (method.parameterCount() > 0)
                continue;

            if (method.name().toLower() != connectionIdMethod)
                continue;

            QByteArray _textBytes;
            QString _textStr;

#if QT_VERSION >= QT_VERSION_CHECK(6, 5, 0)
            QMetaMethodReturnArgument invokeReturn;
#else
            QGenericReturnArgument invokeReturn;
#endif

            switch (method.returnType()) {
            case QMetaType::QString:
                invokeReturn=Q_RETURN_ARG(QString, _textStr);
                break;
            default:
                invokeReturn=Q_RETURN_ARG(QByteArray, _textBytes);
                break;
            }

            if (!method.invoke(objectCheck, Qt::DirectConnection, invokeReturn)) {
#ifdef Q_ORM_LOG_SUPER_VERBOSE
                oWarning() << QByteArrayLiteral("Invalid invoke ") << method.name() << QByteArrayLiteral(" to object:") << objectCheck->metaObject()->className();
#endif
                return {};
            }

            QByteArray connectionId;
            switch (method.returnType()) {
            case QMetaType::QString:
                connectionId = _textStr.toUtf8().trimmed();
                break;
            default:
                connectionId = _textBytes.trimmed();
            }

            if (connectionId.isEmpty())
                return {};

            auto connection = QSqlDatabase::database(connectionId);
            if (!connection.isValid() || !connection.isOpen())
                return {};

            outConnection = connection;
            return true;
        }
        return {};
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
        return {};
    }
};

ObjectDbExtension::ObjectDbExtension(QObject *parent):
    p(new ObjectDbExtensionPvt{parent, {}})
{
}

ObjectDbExtension::ObjectDbExtension(const QSqlDatabase &connection, QObject *parent):
    p(new ObjectDbExtensionPvt{parent, connection.connectionName().toUtf8()})
{
}

ObjectDbExtension::~ObjectDbExtension()
{
    delete p;
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
    return this->setConnection(connection);
}

QByteArray &ObjectDbExtension::connectionId() const
{
    return p->connectionId;
}

} // namespace QOrm
