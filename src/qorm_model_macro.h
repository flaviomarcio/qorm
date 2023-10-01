#pragma once

#define Q_ORM_REPORTFORM "ReportForm"
#define Q_ORM_REGISTERFORM "RegisterForm"
#define Q_ORM_EDITFIELDS "EditFields"
#define Q_ORM_EDITADDRESS "EditAddress"
#define Q_ORM_EDITDUALLIST "EditDualList"
#define Q_ORM_REPORTGRIDLIST "ReportGridList"
#define Q_ORM_REPORTGRIDCARD "ReportGridCard"

static const auto __q_orm_private_point=".";
static const auto __q_orm_private_point_dual="::";
static const auto __q_orm_private_crud="CRUD";
static const auto __q_orm_private_fk="fk";
static const auto __q_orm_private_pk_model="pk.model";
static const auto __q_orm_private_type="type";
static const auto __q_orm_private_name="name";
static const auto __q_orm_private_sequence="sequence";
static const auto __q_orm_private_string_null="";

#define QORM_CRUD_PROPERTY_LIST QVector<QString>({"type","id","resultInfo","layout","links","title","items","filters","headers"})

#define QORM_MODEL_CONSTRUCTOR(ModelName, Model)\
    Q_INVOKABLE explicit ModelName(QObject *parent = nullptr):Model{parent}{};\
    Q_INVOKABLE explicit ModelName(const QVariant &record):Model(nullptr){this->readFrom(record);};\
    Q_INVOKABLE explicit ModelName(QObject *parent, const QByteArray &record):Model{parent}{this->readFrom(record);};\
    Q_INVOKABLE explicit ModelName(QObject *parent, const QVariantHash &record):Model{parent}{this->readFrom(record);};\
    Q_INVOKABLE explicit ModelName(QObject *parent, const QVariant &record):Model{parent}{this->readFrom(record);};\
    Q_INVOKABLE explicit ModelName(QObject *parent, const QUuid &record):Model{parent}{this->readFrom(record);};\
    Q_INVOKABLE explicit ModelName(QObject *parent, const QSqlQuery *record):Model{parent}{this->readFrom(record);};\
    Q_INVOKABLE explicit ModelName(QObject *parent, const QOrm::Query *record):Model{parent}{this->readFrom(record);};\
    Q_INVOKABLE explicit ModelName(QObject *parent, const ResultValue &record):Model{parent}{this->readFrom(record);};

#define QORM_MODEL(modelClass)\
public:\
    virtual const QByteArray modelName() const{\
        static const QByteArray name=QString{this->metaObject()->className()}.split(__q_orm_private_point_dual).last().toLower().toUtf8();\
        return name;\
    }\
public:\
    virtual const QOrm::ModelInfo &modelInfo()const{\
        static const auto __modelInfo=QOrm::ModelInfo::from(modelClass::staticMetaObject);\
        return __modelInfo;\
    }\
    Q_PROPERTY(QByteArray tablePrefix READ tablePrefix)\
    Q_PROPERTY(QByteArray tablePrefixSeparator READ tablePrefixSeparator)\
    Q_PROPERTY(QByteArray tableName READ tableName)\
    Q_PROPERTY(QByteArray tablePk READ tablePk)\
    Q_PROPERTY(QVariantHash tableSequence READ tableSequence)\

#define QORM_DECLARE_DESCRIPTOR(name)public:Q_INVOKABLE virtual const QMetaObject &descriptor(){ return name::staticMetaObject;}

#define QORM_DECLARE_TABLE_PK_COMPUSER(v1)\
public:\
    Q_INVOKABLE virtual QVariant tablePkCompuser()const{\
    return QVariant{v1};\
}

#define QORM_DECLARE_FIELD_DEACTIVATE(VALUE) public: Q_INVOKABLE virtual QVariantHash propertyDeactivateField()const{static QVariantHash ___return=VALUE; return ___return;}

#define QORM_DECLARE_FIELD_ACTIVATE(VALUE) public: Q_INVOKABLE virtual QVariantHash propertyActivateField()const{static QVariantHash ___return=VALUE; return ___return;}

#define QORM_DESCRIPTOR_ORDERBY(name)public: Q_INVOKABLE virtual QByteArray descriptorOrderBy()const{static const auto ___return=QByteArrayLiteral(#name);return ___return;}

#define QORM_DECLARE_TABLE_SCHEMA(name)public:Q_INVOKABLE virtual QByteArray tableSchema()const{static auto __return=QByteArrayLiteral(#name).trimmed(); return __return;}

#define QORM_DECLARE_MODEL_DESCRIPTION(name)public: Q_INVOKABLE virtual QString modelDescription()const{static auto __return=QString(name).trimmed(); return __return;}

#define QORM_DECLARE_TABLE(prefix, separator, table)public:\
Q_INVOKABLE virtual QByteArray tablePrefix()const{static const auto ___return=QByteArrayLiteral(#prefix).trimmed();return ___return;}\
Q_INVOKABLE virtual QByteArray tablePrefixSeparator()const{static const auto ___return=QByteArrayLiteral(#separator).trimmed(); return ___return;}\
Q_INVOKABLE virtual QByteArray tableName()const{static const auto ___return=QByteArrayLiteral(#table).trimmed();return ___return;}

#define QORM_DECLARE_TABLE_SEQUENCE(name)\
public:\
Q_INVOKABLE virtual QVariantHash tableSequence()const{ \
    static QVariantHash vseq_##name; \
    static const auto static_name=QString(#name).trimmed();\
    auto &v=vseq_##name;\
    if(v.isEmpty()){\
        QString schema=this->tableSchema();\
        schema+=schema.isEmpty()?__q_orm_private_string_null:__q_orm_private_point;\
        v.insert(__q_orm_private_type, QOrm::kgcSequence); \
        v.insert(__q_orm_private_name, schema+static_name); \
    }\
    return v; \
}

#define QORM_DECLARE_SEQUENCE(name)\
public:\
Q_INVOKABLE virtual QVariantHash seq_##name()const{ \
    static QVariantHash vseq_##name; \
    static const auto static_name=QString(#name).trimmed();\
    auto &v=vseq_##name;\
    if(v.isEmpty()){\
        QString schema=this->tableSchema();\
        schema+=schema.isEmpty()?__q_orm_private_string_null:__q_orm_private_point;\
        v.insert(__q_orm_private_type, __q_orm_private_sequence); \
        v.insert(__q_orm_private_name, schema+static_name); \
    }\
    return v; \
}

#define QORM_DECLARE_TABLE_PRIMARY_KEY(propertyPk)\
public:\
Q_INVOKABLE virtual QByteArray tablePk(const QByteArray &alias={})const{\
    static const auto __propertyPk=QByteArrayLiteral(#propertyPk);\
    auto __alias=alias.toLower().trimmed();\
    if(__alias.isEmpty())\
        return __propertyPk;\
    return __alias+__q_orm_private_point+__propertyPk;\
}

#define QORM_DECLARE_TABLE_ORDERBY(name)\
public:\
Q_INVOKABLE virtual QByteArray tableOrderBy()const{static const auto ___return=QByteArrayLiteral(#name); return ___return;}

#define QORM_DECLARE_TABLE_AUTO_SET(name)\
public:\
Q_INVOKABLE virtual QByteArray tableAutoSetFields()const{static const auto ___return=QByteArrayLiteral(#name); return ___return;}

#define QORM_DECLARE_TABLE_PRIMARY_KEY_AUTO(name)\
public:\
Q_INVOKABLE virtual bool tablePkAutoGenerate()const{return true;}

#define QORM_DECLARE_TABLE_PRIMARY_KEY_NON_AUTO(name)\
public:\
Q_INVOKABLE virtual bool tablePkAutoGenerate()const{return false;}

#define QORM_DECLARE_FOREIGN_KEY(fk_propertyName, pk_modelName, pk_propertyName)\
Q_INVOKABLE virtual QVariantHash __qorm_declare_fk_##fk_propertyName()const{ \
    static const auto __return=QVariantHash{{__q_orm_private_fk,QStringLiteral(#fk_propertyName)}, {__q_orm_private_pk_model,QStringLiteral(#pk_modelName)}, {QStringLiteral("pk"),QStringLiteral(#pk_propertyName)}};\
    return __return;\
}

#define QORM_DECLARE_WRAPPER_FIELD(wrapperName, src, to)\
private:\
    bool ____declare_wrapper_##src__##to##wrapperName = this->addWrapper(QByteArrayLiteral(#wrapperName), QByteArrayLiteral(#src), QByteArrayLiteral(#to));\
public:

#define QORM_DECLARE_FIELD(propertyName, propertyTitle)\
Q_INVOKABLE QOrm::SqlParserItem propertyName##_field(const QByteArray &aliasName={})const{ \
    static const auto __propertyName=QByteArrayLiteral(#propertyName);\
    QByteArray alias=aliasName.toLower().trimmed();\
    static auto fieldName = alias+tablePrefix() + tablePrefixSeparator()+__propertyName;\
    static auto fieldTitle = QByteArrayLiteral(#propertyTitle);\
    if(alias.isEmpty()){\
        static const QOrm::SqlParserItem ____pn(fieldName, fieldTitle, QOrm::koiObject);\
        return ____pn;\
    }\
    return QOrm::SqlParserItem(alias+__q_orm_private_point+fieldName, fieldTitle, QOrm::koiObject);\
}\
Q_INVOKABLE QByteArray &propertyName##_property()const{\
    static QByteArray __return = #propertyName;\
    return __return;\
}\
Q_INVOKABLE QByteArray propertyName##_fieldName(const QByteArray &aliasName={})const{ \
    static const auto __propertyName=QByteArrayLiteral(#propertyName);\
    QByteArray alias=aliasName.toLower().trimmed();\
    if(alias.isEmpty()){\
        static const QByteArray ___return = tablePrefix()+tablePrefixSeparator()+__propertyName;\
        return ___return;\
    }\
    return alias+__q_orm_private_point+tablePrefix()+tablePrefixSeparator()+__propertyName;\
}

#define QORM_DECLARE_PROPERTY_HEADER(propertyType, propertyName, propertyDefault, propertyTitle)\
public:\
QORM_DECLARE_FIELD(propertyName,)\
Q_PROPERTY(propertyType propertyName READ propertyName WRITE set_##propertyName RESET reset_##propertyName NOTIFY changeProperty)\
virtual QVariant propertyName##_keyValue(){\
    static const auto __propertyName=QByteArrayLiteral(#propertyName);\
    return QVariantHash{{__propertyName, this->property(__propertyName)}};\
}\
virtual propertyType propertyName()const{return z____##propertyName;}\
virtual bool set_##propertyName(const propertyType&value){\
    static const auto __propertyName=QByteArrayLiteral(#propertyName);\
    if(this->propertyBeforeSet(__propertyName, value)){\
        auto oldValue=z____##propertyName;\
        z____##propertyName=value;\
        if(this->propertyAfterSet(__propertyName, value))\
            return true;\
        z____##propertyName=oldValue;\
    }\
    return false;\
}\
void reset_##propertyName(){ set_##propertyName({});}\
virtual bool propertyName##Eq(const propertyType&value)const{return (this->propertyName()==value);}\
auto &propertyName(const propertyType &value){ this->set_##propertyName(value); return *this;}\
private:\
    propertyType z____##propertyName=propertyDefault;\
public:

#define QORM_DECLARE_FILTRABLE_FIELD(vList) public:Q_INVOKABLE virtual QVariantList tableFiltrableField()const{ return vList; }

#define QORM_DECLARE_PROPERTY(propertyType, propertyName, propertyDefault)public:QORM_DECLARE_PROPERTY_HEADER(propertyType, propertyName, propertyDefault,)

#define QORM_DECLARE_MODEL_REFERENCE(ModelName)\
static const auto &ModelName##R = QOrm::ModelInfo::init(ModelName::staticMetaObject);\
static const auto &ModelName##M = QOrm::ModelInfo::instance<ModelName>(ModelName::staticMetaObject);\

#define QORM_DECLARE_DAO(ModelName)\
class ModelName##GDao : public QOrm::ModelDao<ModelName>{\
public:\
    Q_INVOKABLE explicit ModelName##GDao(QObject *parent = nullptr) : QOrm::ModelDao<ModelName>(parent){}\
};\
QORM_DECLARE_MODEL_REFERENCE(ModelName)


#define QORM_DECLARE_CRUD(ModelName)\
class ModelName##GDao : public QOrm::ModelDao<ModelName>{\
public:\
    Q_INVOKABLE explicit ModelName##GDao(QObject *parent = nullptr) : QOrm::ModelDao<ModelName>{parent}{}\
};\
QORM_DECLARE_MODEL_REFERENCE(ModelName)\
class ModelName##CRUD : public QOrm::CRUD<ModelName>{\
public:\
    Q_INVOKABLE explicit ModelName##CRUD(QObject *parent = nullptr) : QOrm::CRUD<ModelName>{parent}{}\
    explicit ModelName##CRUD(const QVariant &crudBody, QObject *parent = nullptr) : QOrm::CRUD<ModelName>{crudBody, parent}{}\
public:\
    virtual const QByteArray &name(){ static const QByteArray __name##ModelName=QByteArray(#ModelName)+__q_orm_private_crud; return __name##ModelName;}\
};

#define QORM_DECLARE_REPORT(ModelName)\
class ModelName##GDao : public QOrm::ModelDao<ModelName>{\
public:\
    Q_INVOKABLE explicit ModelName##GDao(QObject *parent = nullptr) : QOrm::ModelDao<ModelName>{parent}{}\
};\
QORM_DECLARE_MODEL_REFERENCE(ModelName)\
class ModelName##Report : public QOrm::ModelReport<ModelName>{\
public:\
    Q_INVOKABLE explicit ModelName##Report(QObject *parent = nullptr) : QOrm::ModelReport<ModelName>{parent}{}\
    explicit ModelName##Report(const QVariant &crudBody, QObject *parent = nullptr) : QOrm::ModelReport<ModelName>(crudBody, parent){}\
};
