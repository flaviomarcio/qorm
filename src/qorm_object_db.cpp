#include "./qorm_object_db.h"
#include <QMetaProperty>
#include <QMetaMethod>
#include <QMetaObject>

namespace PrivateOrm {
Q_GLOBAL_STATIC_WITH_ARGS(QByteArray,connectionIdMethod,("connectionid"));
}


namespace QOrm {

static const auto&connectionIdMethod=*PrivateOrm::connectionIdMethod;

#define dPvt()\
    auto&p = *reinterpret_cast<ObjectDbPvt*>(this->p)\

class ObjectDbPvt:public QObject{
public:
    VariantUtil vUtil;
    DateUtil dtUtil;
    DoubleUtil dbUtil;
    QObject*parent=nullptr;
    QByteArray ___connectionId;
    explicit ObjectDbPvt(QObject*parent):QObject(parent){
        this->parent=parent;
    }

    static QSqlDatabase invokeMethodConnection(QObject*objectCheck){
        auto metaObject=objectCheck->metaObject();
        for(int mi = 0; mi < metaObject->methodCount(); ++mi) {
            auto method = metaObject->method(mi);
            if(method.parameterCount()>0)
                continue;

            if(method.name().toLower() != connectionIdMethod)
                continue;

            QByteArray _textBytes;
            QString _textStr;
            QGenericReturnArgument invokeReturn;

            if(method.returnType()==QVariant::String)
                invokeReturn=Q_RETURN_ARG(QString, _textStr);
            else
                invokeReturn=Q_RETURN_ARG(QByteArray, _textBytes);

            if(!method.invoke(objectCheck, Qt::DirectConnection, invokeReturn)){
#ifdef Q_ORM_LOG_SUPER_VERBOSE
                sWarning()<<qbl("Invalid invoke ")<<method.name()<<qbl(" to object:")<<objectCheck->metaObject()->className();
#endif
                return QSqlDatabase();
            }
            else{
                auto connectionId = ((method.returnType()==QVariant::String)?_textStr.toUtf8():_textBytes).trimmed();
                auto connection=connectionId.isEmpty()?QSqlDatabase():QSqlDatabase::database(connectionId);
                if(!connection.isValid() || !connection.isOpen())
                    return QSqlDatabase();
                return connection;
            }
        }
        return QSqlDatabase();
    };

    QSqlDatabase connectionGet(){
        if(!this->___connectionId.isEmpty())
            return QSqlDatabase::database(this->___connectionId);
        QSqlDatabase connection;
        auto parentConnection = this->parent;
        if(parentConnection==nullptr)
            return connection;

        QVector<QObject*> parentVector;
        auto loopObject = parentConnection;
        while(loopObject!=nullptr){
            parentVector<<loopObject;
            loopObject=loopObject->parent();
        }
        for(auto&invokeObject:parentVector){
            connection=this->invokeMethodConnection(invokeObject);
            if(!connection.isOpen())
                continue;
            return connection;
        }
        return QSqlDatabase();
    }

    QByteArray connectionId()const{
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
