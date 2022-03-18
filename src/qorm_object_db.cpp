#include "./qorm_object_db.h"
#include <QMetaProperty>
#include <QMetaMethod>
#include <QMetaObject>

namespace QOrm {

#define dPvt()\
    auto&p = *reinterpret_cast<ObjectDbPvt*>(this->p)\

class ObjectDbPvt:public QObject{
public:
    VariantUtil vUtil;
    DateUtil dtUtil;
    DoubleUtil dbUtil;
    QObject*parent=nullptr;
    QByteArray ___connectionId;
    explicit ObjectDbPvt(QObject*parent):QObject{parent}
    {
        this->parent=parent;
    }

    bool invokeMethodConnection(QObject*objectCheck, QSqlDatabase &outConnection)
    {
        static QByteArray connectionIdMethod(qbl("connectionid"));
        auto metaObject=objectCheck->metaObject();
        for(int i = 0; i < metaObject->methodCount(); ++i) {
            auto method = metaObject->method(i);

            if(method.parameterCount()>0)
                continue;

            if(method.name().toLower() != connectionIdMethod)
                continue;

            QByteArray _textBytes;
            QString _textStr;
            QGenericReturnArgument invokeReturn;

            switch (method.returnType()) {
            case QMetaType_QString:
                invokeReturn=Q_RETURN_ARG(QString, _textStr);
                break;
            default:
                invokeReturn=Q_RETURN_ARG(QByteArray, _textBytes);
            }

            if(!method.invoke(objectCheck, Qt::DirectConnection, invokeReturn)){
#ifdef Q_ORM_LOG_SUPER_VERBOSE
                sWarning()<<qbl("Invalid invoke ")<<method.name()<<qbl(" to object:")<<objectCheck->metaObject()->className();
#endif
                return {};
            }

            QByteArray connectionId;
            switch (method.returnType()){
            case QMetaType_QString:
                connectionId=_textStr.toUtf8().trimmed();
                break;
            default:
                connectionId=_textBytes.trimmed();
            }

            if(connectionId.isEmpty())
                return {};

            auto connection=QSqlDatabase::database(connectionId);
            if(!connection.isValid() || !connection.isOpen())
                return {};

            outConnection=connection;
            return true;
        }
        return {};
    };

    QSqlDatabase connectionGet()
    {
        if(!this->___connectionId.isEmpty()){
            auto connection=QSqlDatabase::database(this->___connectionId);
            if(connection.isOpen())
                return connection;
        }

        QSqlDatabase connection;
        auto parentConnection = this->parent;
        if(parentConnection==nullptr)
            return {};

        QVector<QObject*> parentVector;
        auto loopObject = parentConnection;
        while(loopObject!=nullptr){
            parentVector<<loopObject;
            loopObject=loopObject->parent();
        }
        for(auto&invokeObject:parentVector){

            if(!this->invokeMethodConnection(invokeObject, connection))
                continue;

            return connection;
        }
        return {};
    }

    QByteArray connectionId()const
    {
        return this->___connectionId;
    }


};

ObjectDb::ObjectDb(QObject *parent):QStm::Object(parent)
{
    this->p = new ObjectDbPvt(this);
}

ObjectDb::ObjectDb(const QSqlDatabase &connection, QObject *parent):QStm::Object(parent)
{
    this->p = new ObjectDbPvt(this);
    dPvt();
    p.___connectionId=connection.connectionName().toUtf8();
}

ObjectDb::~ObjectDb()
{
}

QSqlDatabase ObjectDb::connection() const
{
    dPvt();
    return p.connectionGet();
}

bool ObjectDb::setConnection(const QSqlDatabase&connection)
{
    dPvt();
    if(connection.isValid() && connection.isOpen())
        p.___connectionId=connection.connectionName().toUtf8();
    else
        p.___connectionId.clear();

    return !p.___connectionId.isEmpty();
}

bool ObjectDb::setConnection(const QString&connectionName)
{
    auto connection=QSqlDatabase::database(connectionName);
    return this->setConnection(connection);
}

QByteArray ObjectDb::connectionId() const
{
    dPvt();
    return p.connectionId();
}

VariantUtil &ObjectDb::vu()
{
    dPvt();
    p.vUtil.clear();
    return p.vUtil;
}

VariantUtil &ObjectDb::vu(const QVariant &v)
{
    dPvt();
    if(v.isValid())
        p.vUtil.setValue(v);
    else
        p.vUtil.clear();
    return p.vUtil;
}

VariantUtil &ObjectDb::vUtil()
{
    dPvt();
    p.vUtil.clear();
    return p.vUtil;
}

VariantUtil &ObjectDb::vUtil(const QVariant&v)
{
    dPvt();
    if(v.isValid())
        p.vUtil.setValue(v);
    else
        p.vUtil.clear();
    return p.vUtil;
}

DateUtil &ObjectDb::dtUtil()
{
    dPvt();
    p.dtUtil.clear();
    return p.dtUtil;
}

DateUtil &ObjectDb::dtUtil(const QVariant &v)
{
    dPvt();
    if(v.isValid())
        p.vUtil.setValue(v);
    else
        p.vUtil.clear();
    return p.dtUtil;
}

DateUtil &ObjectDb::dtu()
{
    dPvt();
    p.dtUtil.clear();
    return p.dtUtil;
}

DateUtil &ObjectDb::dtu(const QVariant &v)
{
    dPvt();
    if(v.isValid())
        p.vUtil.setValue(v);
    else
        p.vUtil.clear();
    return p.dtUtil;
}

DoubleUtil &ObjectDb::dbUtil()
{
    dPvt();
    p.dbUtil.clear();
    return p.dbUtil;
}

DoubleUtil &ObjectDb::dbUtil(const QVariant &v)
{
    dPvt();
    if(v.isValid())
        p.dbUtil.setValue(v);
    else
        p.dbUtil.clear();
    return p.dbUtil;
}

DoubleUtil &ObjectDb::dbu()
{
    dPvt();
    p.dbUtil.clear();
    return p.dbUtil;
}

DoubleUtil &ObjectDb::dbu(const QVariant &v)
{
    dPvt();
    if(v.isValid())
        p.dbUtil.setValue(v);
    else
        p.dbUtil.clear();
    return p.dbUtil;
}

}
