#pragma once

#include <QThread>//manter por conta do log

#define ORM_STRINGFY(name)\
QT_STRINGIFY2(#name)

#define QORM_OBJECT(v)

#define Q_ORM_CRUD_PROPERTY_LIST QVector<QString>({"type","id","resultInfo","layout","links","title","items","filters","headers"})

#define QORM_DECLARE_MODEL_CONST(ModelName, ModelAlias)\
namespace QOrm {\
    static QMetaObject ModelAlias = ModelName::staticMetaObject;\
    static const QOrm::ModelInfo &ModelAlias##Detail = QOrm::Model::from(ModelAlias);\
}

#define qmo(object)object##::staticMetaObject

#define QORM_DECLARE_DTO_TYPE(_MACRO_VALUE_)\
    static FormType defaultType(){\
        static const FormType ___return=_MACRO_VALUE_;\
        return ___return;\
    }

#define QORM_DECLARE_DTO_LAYOUT(_MACRO_VALUE_)\
    static FormLayout defaultLayout(){\
        static const FormLayout ___return=_MACRO_VALUE_;\
        return ___return;\
    }

#define QORM_DECLARE_CRU_TYPE(_MACRO_VALUE_)\
    QORM_DECLARE_DTO_TYPE(_MACRO_VALUE_)

#define QORM_DECLARE_CRUD_LAYOUT(_MACRO_VALUE_)\
    QORM_DECLARE_DTO_LAYOUT(_MACRO_VALUE_)

#define QORM_DECLARE_KEY_WORD(SSKClass)\
    static SqlSuitableKeyWord*SSKClass##Parser = nullptr;\
    {\
        auto &keywork=SSKClass##Parser;\
        if(keywork==nullptr)\
            keywork=SqlSuitableKeyWord::parserMaker(new SSKClass());\
    }

#define QORM_CHECK_CONNECTION()                                                 \
{                                                                               \
    auto __db=this->connection();                                               \
    if(!__db.isValid()){                                                        \
        auto msg=QStringLiteral("Invalid QSqlDatabase");                        \
        oWarning()<< msg;                                                       \
        this->rq().co().setInternalServerError();                               \
        return QVariant(msg);                                                   \
    }                                                                           \
                                                                                \
    if(!__db.isOpen()){                                                         \
        auto msg=QStringLiteral("Connection %1 is not open").arg(__db.connectionName());   \
        oWarning()<< msg;                                                       \
        this->rq().co().setInternalServerError();                               \
        return QVariant(msg);                                                   \
    }                                                                           \
}                                                                               \

#define QORM_CONNECTION_SUPPORT() \
private:\
    QByteArray ____connectionId;\
public:\
    Q_INVOKABLE virtual QSqlDatabase connection()const{\
        return QSqlDatabase::database(____connectionId);\
    }\
    Q_INVOKABLE virtual bool setConnection(const QSqlDatabase &connection){\
        ____connectionId=connection.connectionName().toUtf8();\
        return this->connection().isValid() && this->connection().isOpen();\
    }\
    Q_INVOKABLE virtual bool setConnection(const QString &connectionId){\
        ____connectionId=connectionId.toUtf8();\
        return this->connection().isValid() && this->connection().isOpen();\
    }\
    Q_INVOKABLE virtual QByteArray connectionId()const{\
        return ____connectionId;\
    }\
    Q_INVOKABLE virtual bool connectionClose()const{\
        this->connection().close();\
        return !this->connection().isOpen();\
    }\
    Q_INVOKABLE virtual void connectionClear(){\
        ____connectionId=QByteArrayLiteral("");\
    }\
    Q_INVOKABLE bool connectionFinish(){\
        if(this->connection().isValid()){\
            this->connection().close();\
            QSqlDatabase::removeDatabase(____connectionId);\
        }\
        this->____connectionId=QByteArrayLiteral("");\
        return true;\
    }

#define Q_V_SET(v)auto v=vOrmMap.value(#v)

#define Q_V_SET_UUID(v)auto v=vu.toUuid(vOrmMap.value(#v))\

#define Q_V_SET_BYTE(v)auto v=vu.toByteArray(vOrmMap.value(#v))\

#define Q_V_SET_STRING(v)auto v=vu.toStr(vOrmMap.value(#v))\

#define Q_V_SET_LONGLONG(v)auto v=vu.toLongLong(vOrmMap.value(#v))\

#define Q_V_SET_INT(v)auto v=vu.toInt(vOrmMap.value(#v))\

#define Q_V_SET_DATE(v)auto v=vu.toDat(vOrmMap.value(#v));

#define Q_V_SET_DATETIME(v)auto v=vu.toDateTime(vOrmMap.value(#v));

#define Q_V_SET_TIME(v)auto v=vu.toTime(vOrmMap.value(#v));

#define Q_V_SET_DOUBLE(v)auto v=vu.toDouble(vOrmMap.value(#v));

#define Q_V_SET_MAP(v)auto v=vu.toMap(vOrmMap.value(#v));

#define Q_V_SET_HASH(v)auto v=vu.toHash(vOrmMap.value(#v));

#define Q_V_SET_LIST(v)auto v=vu.toList(vOrmMap.value(#v));

#define Q_V_SET_STRINGLIST(v)auto v=vu.toStringList(vOrmMap.value(#v));

#define QORM_DTO_DECLARE_HEADER(propertyName, params)\
Q_PROPERTY(QVariantHash header_propertyName READ get_##propertyName)\
public:\
static const QVariantHash header_get_##propertyName(){\
    static const QVariantHash v(params);\
    return v;\
}

#define QORM_DTO_DECLARE_FILTER(propertyName, params)\
Q_PROPERTY(QVariantHash header_propertyName READ get_##propertyName)\
public:\
static const QVariantHash filter_get_##propertyName(){\
    static const QVariantHash v(params);\
    return v;\
}

#define ___o_party_1 QString::number(qlonglong(QThread::currentThreadId()))+QStringLiteral(":")

#define ___o_party_2 QString(__PRETTY_FUNCTION__).replace(QStringLiteral("virtual"),QLatin1String("")).split(QStringLiteral("(")).first().trimmed()+QStringLiteral(":")

#define oDebug()qDebug()<<___o_party_1<<___o_party_2

#define oCritical()qCritical()<<___o_party_1<<___o_party_2

#define oInfo()qInfo()<<___o_party_1<<___o_party_2

#define oWarning()qWarning()<<___o_party_1<<___o_party_2

#define oDebugMethodStart() qDebug()<<___o_party_1<<___o_party_2<<QStringLiteral(":start")

#define oDebugMethodFinish()qDebug()<<___o_party_1<<___o_party_2<<QStringLiteral(":finish")


