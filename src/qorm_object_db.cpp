#include "./qorm_object_db.h"
#include "./qorm_const.h"
#include "./qorm_macro.h"
#include <QMetaMethod>
#include <QMetaObject>
#include <QMetaProperty>

namespace QOrm {

class ObjectDbPvt : public QObject
{
public:
    QObject *parent = nullptr;
    QByteArray ___connectionId;
    explicit ObjectDbPvt(QObject *parent) : QObject{parent} { this->parent = parent; }

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

            auto makeArg=[&method, &_textStr, &_textBytes](){
                switch (method.returnType()) {
                case QMetaType::QString:
                    return Q_RETURN_ARG(QString, _textStr);
                default:
                    return Q_RETURN_ARG(QByteArray, _textBytes);
                }
            };
            auto invokeReturn=makeArg();

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
        if (!this->___connectionId.isEmpty()) {
            auto connection = QSqlDatabase::database(this->___connectionId);
            if (connection.isOpen())
                return connection;
        }

        QSqlDatabase connection;
        auto parentConnection = this->parent;
        if (parentConnection == nullptr)
            return {};

        QVector<QObject *> parentVector;
        auto loopObject = parentConnection;
        while (loopObject != nullptr) {
            parentVector << loopObject;
            loopObject = loopObject->parent();
        }
        for (auto &invokeObject : parentVector) {
            if (!this->invokeMethodConnection(invokeObject, connection))
                continue;

            return connection;
        }
        return {};
    }

    QByteArray connectionId() const { return this->___connectionId; }
};

ObjectDb::ObjectDb(QObject *parent) : QStm::Object{parent}
{
    this->p = new ObjectDbPvt{this};
}

ObjectDb::ObjectDb(const QSqlDatabase &connection, QObject *parent) : QOrm::Object{parent}
{
    this->p = new ObjectDbPvt{this};
    p->___connectionId = connection.connectionName().toUtf8();
}

QSqlDatabase ObjectDb::connection() const
{
    return p->connectionGet();
}

bool ObjectDb::setConnection(const QSqlDatabase &connection)
{
    if (connection.isValid() && connection.isOpen())
        p->___connectionId = connection.connectionName().toUtf8();
    else
        p->___connectionId.clear();
    return !p->___connectionId.isEmpty();
}

bool ObjectDb::setConnection(const QString &connectionName)
{
    auto connection = QSqlDatabase::database(connectionName);
    return this->setConnection(connection);
}

QByteArray ObjectDb::connectionId() const
{
    return p->connectionId();
}

} // namespace QOrm
