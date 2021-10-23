#pragma once

#include <QThread>
#include <QMutex>
#include "./qstm_macro.h"

#define ORM_STRINGFY(name)\
QT_STRINGIFY2(#name)

#define Q_ORM_NAME(name)\
QT_STRINGIFY2(#name)

#define QORM_OBJECT(v)\
 \

#define QORM_DECLARE_INSTANCE(instanceClassName)\
public:\
    static instanceClassName&instance(){\
        static instanceClassName*____instance=nullptr;\
        if(____instance==nullptr){\
            ____instance=new instanceClassName(nullptr);\
        }\
        return*____instance;\
    }

#define QORM_DECLARE_MODEL_CONST(ModelName, ModelAlias)\
namespace QOrm {\
    static QMetaObject ModelAlias = ModelName::staticMetaObject;\
    static const QOrm::ModelInfo&ModelAlias##Detail = QOrm::Model::modelInfo(ModelAlias);\
}

#define QORM_OBJECT_CONSTRUCTOR(ObjectName)\
public:\
    Q_INVOKABLE explicit ObjectName(QObject *parent = nullptr):QOrm::ModelDescriptor(parent){};\

#define QORM_MODEL_CONSTRUCTOR(ModelName, Model)\
    Q_INVOKABLE explicit ModelName(QObject *parent = nullptr):Model(parent){};\
    Q_INVOKABLE explicit ModelName(const QVariant&record):Model(nullptr){this->readFrom(record);};\
    Q_INVOKABLE explicit ModelName(QObject *parent, const QByteArray &record):Model(parent){this->readFrom(record);};\
    Q_INVOKABLE explicit ModelName(QObject *parent, const QVariantHash &record):Model(parent){this->readFrom(record);};\
    Q_INVOKABLE explicit ModelName(QObject *parent, const QVariant &record):Model(parent){this->readFrom(record);};\
    Q_INVOKABLE explicit ModelName(QObject *parent, const QUuid &record):Model(parent){this->readFrom(record);};\
    Q_INVOKABLE explicit ModelName(QObject *parent, QSqlQuery &record):Model(parent){this->readFrom(record);};\
    Q_INVOKABLE explicit ModelName(QObject *parent, QOrm::Query &record):Model(parent){this->readFrom(record);};\
    Q_INVOKABLE explicit ModelName(QObject *parent, const ResultValue &record):Model(parent){this->readFrom(record);};\
    Q_INVOKABLE ~ModelName(){};

#define QORM_MODEL(modelClass)\
public:\
    virtual QByteArray modelName() const{\
        QString name=this->metaObject()->className();\
        return name.split(QStringLiteral("::")).last().toLower().toUtf8();\
    }\
public:\
    static modelClass&m(){ modelClass*__m=nullptr; if(__m==nullptr) __m = new modelClass(); return*__m;}\
    auto&modelInfo()const{return QOrm::ModelInfo::modelInfo(modelClass::staticMetaObject);}\
    Q_PROPERTY(QByteArray tablePrefix READ tablePrefix)\
    Q_PROPERTY(QByteArray tablePrefixSeparator READ tablePrefixSeparator)\
    Q_PROPERTY(QByteArray tableName READ tableName)\
    Q_PROPERTY(QByteArray tablePk READ tablePk)\
    Q_PROPERTY(QByteArray tablePrimaryKey READ tablePrimaryKey)\
    Q_PROPERTY(QVariantHash tableSequence READ tableSequence)\

#define QORM_DECLARE_DESCRIPTOR(name)\
public:\
Q_INVOKABLE virtual const QMetaObject&descriptor(){\
    return name::staticMetaObject;\
}

#define QORM_DECLARE_TOPIC(v1)\
public:\
    Q_INVOKABLE virtual QVariant topic()const{\
        return QVariant(v1);\
    }

#define QORM_DECLARE_TABLE_DEACTIVATE_FIELD(name)\
public:\
Q_INVOKABLE virtual QByteArray tableDeactivateField()const{static auto ___return=QByteArrayLiteral(#name).trimmed(); return ___return;}

#define QORM_DESCRIPTOR_ORDERBY(name)\
public:\
Q_INVOKABLE virtual QByteArray descriptorOrderBy()const{static const auto ___return=QByteArrayLiteral(#name);return ___return;}

#define QORM_DECLARE_TABLE_SCHEMA(name)\
public:\
Q_INVOKABLE virtual QByteArray tableSchema()const{static auto __return=QByteArrayLiteral(#name).trimmed(); return __return;}

#define QORM_DECLARE_MODEL_DESCRIPTION(name)\
public:\
Q_INVOKABLE virtual QString modelDescription()const{static auto __return=QString(name).trimmed(); return __return;}

#define QORM_DECLARE_TABLE(prefix, separator, table)\
public:\
Q_INVOKABLE virtual QByteArray tablePrefix()const{static const auto ___return=QByteArrayLiteral(#prefix).trimmed();return ___return;}\
Q_INVOKABLE virtual QByteArray tablePrefixSeparator()const{static const auto ___return=QByteArrayLiteral(#separator).trimmed(); return ___return;}\
Q_INVOKABLE virtual QByteArray tableName()const{static const auto ___return=QByteArrayLiteral(#table).trimmed();return ___return;}

#define QORM_DECLARE_TABLE_SEQUENCE(name)\
public:\
Q_INVOKABLE virtual QVariantHash tableSequence()const{ \
    static QVariantHash vseq_##name; \
    static const auto static_name=QString(#name).trimmed();\
    auto&v=vseq_##name;\
    if(v.isEmpty()){\
        QString schema=this->tableSchema();\
        schema+=schema.isEmpty()?"":QStringLiteral(".");\
        v.insert(QStringLiteral("type"), QOrm::kgcSequence); \
        v.insert(QStringLiteral("name"), schema+static_name); \
    }\
    return v; \
}

#define QORM_DECLARE_SEQUENCE(name)\
public:\
Q_INVOKABLE virtual QVariantHash seq_##name()const{ \
    static QVariantHash vseq_##name; \
    static const auto static_name=QString(#name).trimmed();\
    auto&v=vseq_##name;\
    if(v.isEmpty()){\
        QString schema=this->tableSchema();\
        schema+=schema.isEmpty()?"":QStringLiteral(".");\
        v.insert(QStringLiteral("type"), QStringLiteral("sequence")); \
        v.insert(QStringLiteral("name"), schema+static_name); \
    }\
    return v; \
}

#define QORM_DECLARE_TABLE_PRIMARY_KEY(propertyPk)\
public:\
Q_INVOKABLE virtual QByteArray tablePk()const{const auto ___return=QByteArrayLiteral(#propertyPk); return ___return;}\
Q_INVOKABLE virtual QByteArray tablePk(const QByteArray&alias)const{return (alias.trimmed().isEmpty()?"":alias.toLower().trimmed()+QByteArrayLiteral("."))+QByteArrayLiteral(#propertyPk);}\
Q_INVOKABLE virtual QByteArray tablePrimaryKey()const{static const auto ___return=QByteArrayLiteral(#propertyPk); return ___return;}\
Q_INVOKABLE virtual QByteArray tablePrimaryKey(const QByteArray&alias)const{return (alias.trimmed().isEmpty()?"":alias.toLower().trimmed()+QByteArrayLiteral("."))+QByteArrayLiteral(#propertyPk);}



#define QORM_DECLARE_TABLE_ORDERBY(name)\
public:\
Q_INVOKABLE virtual QByteArray tableOrderBy()const{static const auto ___return=QByteArrayLiteral(#name); return ___return;}

#define QORM_DECLARE_TABLE_PRIMARY_KEY_AUTO(name)\
public:\
Q_INVOKABLE virtual bool tablePkAutoGenerate()const{return true;}

#define QORM_DECLARE_TABLE_PRIMARY_KEY_NON_AUTO(name)\
public:\
Q_INVOKABLE virtual bool tablePkAutoGenerate()const{return false;}

#define QORM_DECLARE_TABLE_FOREIGN_KEY(propertyPk)\
public:\
Q_INVOKABLE virtual QByteArray tableForeignPk()const{static const auto ___return=QByteArrayLiteral(#propertyPk); return ___return;}\

#define QORM_DECLARE_TABLE_FOREIGN_KEY_ON_PRIMARY_KEY(fk_propertyName, pk_modelName, pk_propertyName)\
Q_INVOKABLE virtual QVariantHash tableForeignKey_##fk_propertyName()const{ \
    static const auto __return=QVariantHash({{qsl("fk"),qsl(#fk_propertyName)}, {qsl("pk.model"),qsl(#pk_modelName)}, {qsl("pk"),qsl(#pk_propertyName)}});\
    return __return;\
}

#define QORM_DECLARE_WRAPPER_FIELD(wrapperName, src, to)\
    private:\
        bool ____declare_wrapper_##src__##to##wrapperName = this->addWrapper(QByteArrayLiteral(#wrapperName), QByteArrayLiteral(#src), QByteArrayLiteral(#to));\
    public:

#define QORM_DECLARE_FIELD(propertyName, propertyTitle)\
Q_INVOKABLE QOrm::SqlParserItem&propertyName##_field(QByteArray alias="")const{ \
    alias=alias.toLower().trimmed();\
    alias=(alias=="")?"":(alias+QByteArrayLiteral("."));\
    static auto fieldName = alias+tablePrefix() + tablePrefixSeparator()+QByteArrayLiteral(#propertyName);\
    static auto fieldTitle = QByteArrayLiteral(#propertyTitle);\
    static QOrm::SqlParserItem ____pn(fieldName, fieldTitle, QOrm::koiObject);\
    return ____pn;\
}\
Q_INVOKABLE QByteArray&propertyName##_property()const{ \
    static QByteArray __return = #propertyName;\
    return __return;\
}\
Q_INVOKABLE QByteArray&propertyName##_fieldName(const QByteArray&vAlias="")const{ \
    auto alias=vAlias.toLower().trimmed();\
    alias=(alias=="")?"":(alias+QByteArrayLiteral("."));\
    static QByteArray ___return = alias+tablePrefix() + tablePrefixSeparator()+QByteArrayLiteral(#propertyName);\
    return ___return;\
}

#define QORM_DECLARE_PROPERTY_HEADER(propertyType, propertyName, propertyDefault, propertyTitle)\
public:\
QORM_DECLARE_FIELD(propertyName,)\
Q_PROPERTY(propertyType propertyName READ propertyName WRITE set_##propertyName NOTIFY changeProperty)\
Q_INVOKABLE virtual QVariant propertyName##_keyValue(){\
    static const auto ___name=QByteArrayLiteral(#propertyName);\
    return QVariantHash({{___name, this->property(___name)}});\
}\
Q_INVOKABLE virtual propertyType propertyName()const{\
    return z____##propertyName;\
}\
Q_INVOKABLE virtual bool set_##propertyName(const propertyType&value){\
    if(this->propertyBeforeSet(QByteArrayLiteral(#propertyName), value)){\
        auto oldValue=z____##propertyName;\
        z____##propertyName=value;\
        if(!this->propertyAfterSet(QByteArrayLiteral(#propertyName), value)){\
            oldValue=oldValue;\
        }\
        else{\
            return true;\
        }\
    }\
    return false;\
}\
Q_INVOKABLE virtual bool propertyName##Eq(const propertyType&value)const{\
    if(this->propertyName()==value)\
        return true;\
    else\
        return false;\
}\
private:\
    propertyType z____##propertyName=propertyDefault;\
public:


#define QORM_DECLARE_PROPERTY(propertyType, propertyName, propertyDefault)\
public:\
QORM_DECLARE_PROPERTY_HEADER(propertyType, propertyName, propertyDefault,)

#define QORM_DECLARE_PROPERTY_QString(propertyName, propertyDefault)\
public:\
QORM_DECLARE_FIELD(propertyName)\
Q_PROPERTY(QString propertyName READ propertyName WRITE set_##propertyName NOTIFY changeProperty)\
Q_INVOKABLE virtual QString propertyName(){\
    return z____##propertyName.mid(0, propertySize);\
}\
Q_INVOKABLE virtual void set_##propertyName(const QString&value){\
    z____##propertyName=value.mid(0, propertySize);\
}\
private:\
    QString z____##propertyName=propertyDefault;

#define QORM_DECLARE_PROPERTY_QByteArray(propertyName, propertyDefault)\
public:\
QORM_DECLARE_FIELD(propertyName)\
Q_PROPERTY(QByteArray propertyName READ propertyName WRITE set_##propertyName NOTIFY changeProperty)\
Q_INVOKABLE virtual QByteArray propertyName(){\
    return z____##propertyName.mid(0, propertySize);\
}\
Q_INVOKABLE virtual void set_##propertyName(const QByteArray&value){\
    z____##propertyName=value.mid(0, propertySize);\
}\
private:\
    QByteArray z____##propertyName=propertyDefault;

#define QORM_DECLARE_PROPERTY_QInt(propertyName, propertyDefault)\
    QORM_DECLARE_PROPERTY(int, propertyName, propertyDefault)

#define QORM_DECLARE_PROPERTY_QBool(propertyName, propertyDefault)\
    QORM_DECLARE_PROPERTY(bool, propertyName, propertyDefault)

#define QORM_DECLARE_PROPERTY_QLongLong(propertyName, propertyDefault)\
    QORM_DECLARE_PROPERTY(qlonglong, propertyName, propertyDefault)

#define QORM_DECLARE_PROPERTY_QDateTime(propertyName, propertyDefault)\
    QORM_DECLARE_PROPERTY(QDateTime, propertyName, propertyDefault)

#define QORM_DECLARE_PROPERTY_QUuid(propertyName, propertyDefault)\
    QORM_DECLARE_PROPERTY(QUuid, propertyName, propertyDefault)

#define QORM_DECLARE_PROPERTY_QMd5(propertyName, propertyDefault)\
    QORM_DECLARE_PROPERTY(QByteArray, propertyName, propertyDefault)

#define QORM_DECLARE_PROPERTY_QDate(propertyName, propertyDefault)\
    QORM_DECLARE_PROPERTY(QDate, propertyName, propertyDefault)

#define QORM_DECLARE_PROPERTY_QTime(propertyName, propertyDefault)\
    QORM_DECLARE_PROPERTY(QTime, propertyName, propertyDefault)

#define qmo(object)\
    object##::staticMetaObject

#define QORM_DECLARE_DAO(ModelName)\
    class ModelName##GDao : public QOrm::ModelDao<ModelName>{\
        Q_OBJECT\
    public:\
        Q_INVOKABLE explicit ModelName##GDao(QObject *parent = nullptr) : QOrm::ModelDao<ModelName>(parent){\
            if(this->parent()!=parent)\
                this->setParent(parent);\
        }\
        Q_INVOKABLE ~ModelName##GDao(){\
        }\
    };\
    static const auto&ModelName##R = QOrm::ModelInfo::modelInfoInit(ModelName::staticMetaObject);\
    static const auto&ModelName##M = ModelName::m();

#define QORM_DECLARE_CRUD(ModelName)\
    class ModelName##GDao : public QOrm::ModelDao<ModelName>{\
        Q_OBJECT\
    public:\
        Q_INVOKABLE explicit ModelName##GDao(QObject *parent = nullptr) : QOrm::ModelDao<ModelName>(parent){\
            if(this->parent()!=parent)\
                this->setParent(parent);\
        }\
        Q_INVOKABLE ~ModelName##GDao(){\
        }\
    };\
    static const auto&ModelName##R = QOrm::ModelInfo::modelInfoInit(ModelName::staticMetaObject);\
    static const auto&ModelName##M = ModelName::m();\
    class ModelName##CRUD : public QOrm::CRUD<ModelName>{\
        Q_OBJECT\
    public:\
        Q_INVOKABLE explicit ModelName##CRUD(QObject *parent = nullptr) : QOrm::CRUD<ModelName>(parent){\
            if(this->parent()!=parent)\
                this->setParent(parent);\
        }\
        Q_INVOKABLE explicit ModelName##CRUD(const QVariant&crudBody, QObject *parent = nullptr) : QOrm::CRUD<ModelName>(crudBody, parent){\
            if(this->parent()!=parent)\
                this->setParent(parent);\
        }\
        Q_INVOKABLE ~ModelName##CRUD(){\
        }\
    };\

#define QORM_DECLARE_REPORT(ModelName)\
    class ModelName##GDao : public QOrm::ModelDao<ModelName>{\
        Q_OBJECT\
    public:\
        Q_INVOKABLE explicit ModelName##GDao(QObject *parent = nullptr) : QOrm::ModelDao<ModelName>(parent){\
            if(this->parent()!=parent)\
                this->setParent(parent);\
        }\
        Q_INVOKABLE ~ModelName##GDao(){\
        }\
    };\
    static const auto&ModelName##R = QOrm::ModelInfo::modelInfoInit(ModelName::staticMetaObject);\
    static const auto&ModelName##M = ModelName::m();\
    class ModelName##Report : public QOrm::ModelReport<ModelName>{\
        Q_OBJECT\
    public:\
        Q_INVOKABLE explicit ModelName##Report(QObject *parent = nullptr) : QOrm::ModelReport<ModelName>(parent){\
            if(this->parent()!=parent)\
                this->setParent(parent);\
        }\
        Q_INVOKABLE explicit ModelName##Report(const QVariant&crudBody, QObject *parent = nullptr) : QOrm::ModelReport<ModelName>(crudBody, parent){\
            if(this->parent()!=parent)\
                this->setParent(parent);\
        }\
        Q_INVOKABLE ~ModelName##Report(){\
        }\
    };\

#define QORM_DECLARE_DTO_TYPE(_MACRO_VALUE_)\
    Q_INVOKABLE virtual QVariant defaultType()const{\
        static const QVariant ___return=_MACRO_VALUE_;\
        return ___return;\
    }

#define QORM_DECLARE_DTO_LAYOUT(_MACRO_VALUE_)\
    Q_INVOKABLE virtual QVariant defaultLayout()const{\
        static const QVariant ___return=_MACRO_VALUE_;\
        return ___return;\
    }

#define QORM_DECLARE_CRU_TYPE(_MACRO_VALUE_)\
    QORM_DECLARE_DTO_TYPE(_MACRO_VALUE_)

#define QORM_DECLARE_CRUD_LAYOUT(_MACRO_VALUE_)\
    QORM_DECLARE_DTO_LAYOUT(_MACRO_VALUE_)

#define QORM_DECLARE_KEY_WORD(SSKClass)\
    static SqlSuitableKeyWord*SSKClass##Parser = nullptr;\
    {\
        auto&keywork=SSKClass##Parser;\
        if(keywork==nullptr)\
            keywork=SqlSuitableKeyWord::parserMaker(new SSKClass());\
    }

#define QORM_CHECK_CONNECTION()                                                 \
{                                                                               \
    auto __db=this->connection();                                               \
    if(!__db.isValid()){                                                        \
        auto msg=qsl("Invalid QSqlDatabase");                                   \
        sWarning()<< msg;                                                       \
        this->rq().co().setInternalServerError();                               \
        return QVariant(msg);                                                   \
    }                                                                           \
                                                                                \
    if(!__db.isOpen()){                                                         \
        auto msg=qsl("Connection %1 is not open").arg(__db.connectionName());   \
        sWarning()<< msg;                                                       \
        this->rq().co().setInternalServerError();                               \
        return QVariant(msg);                                                   \
    }                                                                           \
}                                                                               \

#define QORM_CONNECTION_SUPPORT \
private:\
    QByteArray ____connectionId;\
public:\
    Q_INVOKABLE virtual QSqlDatabase connection()const{\
        return QSqlDatabase::database(____connectionId);\
    }\
    Q_INVOKABLE virtual bool setConnection(QSqlDatabase connection){\
        ____connectionId=connection.connectionName().toUtf8();\
        return this->connection().isValid() && this->connection().isOpen();\
    }\
    Q_INVOKABLE virtual bool setConnection(QString connectionId){\
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

#define QORM_SERVER_CONNECTION_MANAGER_SUPPORT(instanceClassName)\
private:\
    QOrm::ConnectionManager*____connectionManager=nullptr;\
public:\
    Q_INVOKABLE virtual QOrm::ConnectionManager&connectionManager(){\
        static QMutex ____instance_mutex;\
        if(____connectionManager==nullptr){\
            QMutexLocker locker(&____instance_mutex);/*garantia de unica instancia*/ \
            if(____connectionManager==nullptr)\
                ____connectionManager = new QOrm::ConnectionManager(this);\
        }\
        if(____connectionManager->isEmpty() || !____connectionManager->isLoaded()){\
            QMutexLocker locker(&____instance_mutex);/*garantia de unica instancia*/ \
            if(____connectionManager->isEmpty() || !____connectionManager->isLoaded())\
                ____connectionManager->load(this);\
        }\
        return*____connectionManager;\
    }\
    Q_INVOKABLE virtual QOrm::ConnectionPool&pool(){\
        return this->connectionManager().pool();\
    }\
    Q_INVOKABLE virtual QOrm::ConnectionPool&pool(const QByteArray &value){\
        return this->connectionManager().pool(value);\
    }

#define Q_V_SET(v)\
    auto v=vOrmMap.value(#v)

#define Q_V_SET_UUID(v)\
    auto v=vu.toUuid(vOrmMap.value(#v))\

#define Q_V_SET_BYTE(v)\
    auto v=vu.toByteArray(vOrmMap.value(#v))\

#define Q_V_SET_STRING(v)\
    auto v=vu.toStr(vOrmMap.value(#v))\

#define Q_V_SET_LONGLONG(v)\
    auto v=vu.toLongLong(vOrmMap.value(#v))\

#define Q_V_SET_INT(v)\
    auto v=vu.toInt(vOrmMap.value(#v))\

#define Q_V_SET_DATE(v)\
    auto v=vu.toDat(vOrmMap.value(#v));

#define Q_V_SET_DATETIME(v)\
    auto v=vu.toDateTime(vOrmMap.value(#v));

#define Q_V_SET_TIME(v)\
    auto v=vu.toTime(vOrmMap.value(#v));

#define Q_V_SET_DOUBLE(v)\
    auto v=vu.toDouble(vOrmMap.value(#v));

#define Q_V_SET_MAP(v)\
    auto v=vu.toMap(vOrmMap.value(#v));

#define Q_V_SET_HASH(v)\
    auto v=vu.toHash(vOrmMap.value(#v));

#define Q_V_SET_LIST(v)\
    auto v=vu.toList(vOrmMap.value(#v));

#define Q_V_SET_STRINGLIST(v)\
    auto v=vu.toStringList(vOrmMap.value(#v));

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

#define QORM_VCHECK_IS_EMPTY(v,m)\
{\
Q_DECLARE_VU;\
if(vu.vIsEmpty(v))\
    return this->lr().setValidation(m);\
}

#define likeL(v)QStringLiteral("%")+v
#define likeR(v)v+QStringLiteral("%")
#define likeLR(v)QStringLiteral("%")+v+QStringLiteral("%")

#define vlikeL(v)likeL(QVariant(v).toString())
#define vlikeR(v)likeR(QVariant(v).toString())
#define vlikeLR(v)likeLR(QVariant(v).toString())
