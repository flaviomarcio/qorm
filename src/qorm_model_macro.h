#pragma once

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
    virtual const QByteArray &modelName() const{\
        static const QByteArray name=QString{this->metaObject()->className()}.split(QStringLiteral("::")).last().toLower().toUtf8();\
        return name;\
    }\
public:\
    static modelClass &m(){ modelClass*__m=nullptr; if(__m==nullptr) __m = new modelClass(); return*__m;}\
    auto &modelInfo()const{return QOrm::ModelInfo::from(modelClass::staticMetaObject);}\
    Q_PROPERTY(QByteArray tablePrefix READ tablePrefix)\
    Q_PROPERTY(QByteArray tablePrefixSeparator READ tablePrefixSeparator)\
    Q_PROPERTY(QByteArray tableName READ tableName)\
    Q_PROPERTY(QByteArray tablePk READ tablePk)\
    Q_PROPERTY(QByteArray tablePrimaryKey READ tablePrimaryKey)\
    Q_PROPERTY(QVariantHash tableSequence READ tableSequence)\

#define QORM_DECLARE_DESCRIPTOR(name)public:Q_INVOKABLE virtual const QMetaObject &descriptor(){ return name::staticMetaObject;}

#define QORM_DECLARE_TABLE_PK_COMPUSER(v1)\
public:\
    Q_INVOKABLE virtual QVariant tablePkCompuser()const{\
    return QVariant{v1};\
}

#define QORM_DECLARE_TABLE_DEACTIVATE_FIELD(name) public: Q_INVOKABLE virtual QByteArray tableDeactivateField()const{static auto ___return=QByteArrayLiteral(#name).trimmed(); return ___return;}

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
    auto &v=vseq_##name;\
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
Q_INVOKABLE virtual QByteArray tablePk(const QByteArray &alias)const{return (alias.trimmed().isEmpty()?"":alias.toLower().trimmed()+QByteArrayLiteral("."))+QByteArrayLiteral(#propertyPk);}\
Q_INVOKABLE virtual QByteArray tablePrimaryKey()const{static const auto ___return=QByteArrayLiteral(#propertyPk); return ___return;}\
Q_INVOKABLE virtual QByteArray tablePrimaryKey(const QByteArray &alias)const{return (alias.trimmed().isEmpty()?"":alias.toLower().trimmed()+QByteArrayLiteral("."))+QByteArrayLiteral(#propertyPk);}

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

#define QORM_DECLARE_TABLE_FOREIGN_KEY(propertyPk)\
public:\
Q_INVOKABLE virtual QByteArray tableForeignPk()const{static const auto ___return=QByteArrayLiteral(#propertyPk); return ___return;}

#define QORM_DECLARE_TABLE_FOREIGN_KEY_ON_PRIMARY_KEY(fk_propertyName, pk_modelName, pk_propertyName)\
Q_INVOKABLE virtual QVariantHash tableForeignKey_##fk_propertyName()const{ \
    static const auto __return=QVariantHash{{QStringLiteral("fk"),QStringLiteral(#fk_propertyName)}, {QStringLiteral("pk.model"),QStringLiteral(#pk_modelName)}, {QStringLiteral("pk"),QStringLiteral(#pk_propertyName)}};\
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
Q_INVOKABLE QByteArray &propertyName##_property()const{ \
    static QByteArray __return = #propertyName;\
    return __return;\
}\
Q_INVOKABLE QByteArray &propertyName##_fieldName(const QByteArray &vAlias="")const{ \
    auto alias=vAlias.toLower().trimmed();\
    alias=(alias=="")?"":(alias+QByteArrayLiteral("."));\
    static QByteArray ___return = alias+tablePrefix() + tablePrefixSeparator()+QByteArrayLiteral(#propertyName);\
    return ___return;\
}

#define QORM_DECLARE_PROPERTY_HEADER(propertyType, propertyName, propertyDefault, propertyTitle)\
public:\
QORM_DECLARE_FIELD(propertyName,)\
Q_PROPERTY(propertyType propertyName READ propertyName WRITE set_##propertyName RESET reset_##propertyName NOTIFY changeProperty)\
virtual QVariant propertyName##_keyValue(){\
    static const auto ___name=QByteArrayLiteral(#propertyName);\
    return QVariantHash{{___name, this->property(___name)}};\
}\
virtual propertyType propertyName()const{\
    return z____##propertyName;\
}\
virtual bool set_##propertyName(const propertyType&value){\
    if(this->propertyBeforeSet(QByteArrayLiteral(#propertyName), value)){\
        auto oldValue=z____##propertyName;\
        z____##propertyName=value;\
        if(this->propertyAfterSet(QByteArrayLiteral(#propertyName), value))\
            return true;\
        oldValue=oldValue;\
    }\
    return false;\
}\
void reset_##propertyName(){\
    set_##propertyName({});\
}\
virtual bool propertyName##Eq(const propertyType&value)const{\
    if(this->propertyName()==value)\
        return true;\
    return false;\
}\
private:\
    propertyType z____##propertyName=propertyDefault;\
public:


#define QORM_DECLARE_FILTRABLE_FIELD(vList) public:Q_INVOKABLE virtual QVariantList tableFiltrableField()const{ return vList; }

#define QORM_DECLARE_PROPERTY(propertyType, propertyName, propertyDefault)public:QORM_DECLARE_PROPERTY_HEADER(propertyType, propertyName, propertyDefault,)

#define QORM_DECLARE_DAO(ModelName)\
class ModelName##GDao : public QOrm::ModelDao<ModelName>{\
public:\
    Q_INVOKABLE explicit ModelName##GDao(QObject *parent = nullptr) : QOrm::ModelDao<ModelName>(parent){\
        if(this->parent()!=parent)\
            this->setParent(parent);\
    }\
};\
static const auto &ModelName##R = QOrm::ModelInfo::modelInfoInit(ModelName::staticMetaObject);\
static const auto &ModelName##M = ModelName::m();


#define QORM_DECLARE_CRUD(ModelName)\
class ModelName##GDao : public QOrm::ModelDao<ModelName>{\
public:\
    Q_INVOKABLE explicit ModelName##GDao(QObject *parent = nullptr) : QOrm::ModelDao<ModelName>{parent}{\
        if(this->parent()!=parent)\
            this->setParent(parent);\
    }\
};\
static const auto &ModelName##R = QOrm::ModelInfo::modelInfoInit(ModelName::staticMetaObject);\
static const auto &ModelName##M = ModelName::m();\
class ModelName##CRUD : public QOrm::CRUD<ModelName>{\
public:\
    Q_INVOKABLE explicit ModelName##CRUD(QObject *parent = nullptr) : QOrm::CRUD<ModelName>{parent}{\
        if(this->parent()!=parent)\
            this->setParent(parent);\
    }\
    Q_INVOKABLE explicit ModelName##CRUD(const QVariant &crudBody, QObject *parent = nullptr) : QOrm::CRUD<ModelName>{crudBody, parent}{\
        if(this->parent()!=parent)\
            this->setParent(parent);\
    }\
};

#define QORM_DECLARE_REPORT(ModelName)\
    class ModelName##GDao : public QOrm::ModelDao<ModelName>{\
        Q_OBJECT\
    public:\
        Q_INVOKABLE explicit ModelName##GDao(QObject *parent = nullptr) : QOrm::ModelDao<ModelName>{parent}{\
            if(this->parent()!=parent)\
                this->setParent(parent);\
        }\
    };\
    static const auto &ModelName##R = QOrm::ModelInfo::modelInfoInit(ModelName::staticMetaObject);\
    static const auto &ModelName##M = ModelName::m();\
    class ModelName##Report : public QOrm::ModelReport<ModelName>{\
        Q_OBJECT\
    public:\
        Q_INVOKABLE explicit ModelName##Report(QObject *parent = nullptr) : QOrm::ModelReport<ModelName>{parent}{\
            if(this->parent()!=parent)\
                this->setParent(parent);\
        }\
        Q_INVOKABLE explicit ModelName##Report(const QVariant &crudBody, QObject *parent = nullptr) : QOrm::ModelReport<ModelName>(crudBody, parent){\
            if(this->parent()!=parent)\
                this->setParent(parent);\
        }\
    };
