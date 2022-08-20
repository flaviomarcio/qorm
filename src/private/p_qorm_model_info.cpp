#include <QMutex>
#include <QCoreApplication>
#include <QCryptographicHash>
#include "./p_qorm_model_info.h"
#include "./p_qorm_const.h"
#include "../qorm_model.h"
#include "../qorm_model_field_descriptors.h"
#include "../qorm_macro.h"
#include "../qorm_model_macro.h"
#include "../qorm_startup.h"
#include "./p_qorm_sql_suitable_parser_item.h"

namespace QOrm {

typedef QHash<QByteArray, QOrm::ModelInfo*> HashModelInfo;
Q_GLOBAL_STATIC(HashModelInfo, __static_model_info)

class ModelInfoPvt{
public:
    QUuid uuid;
    ModelFieldDescriptors *descriptor=nullptr;
    QMetaObject staticMetaObjectModel;
    QMetaObject staticMetaObjectDescriptor;
    QHash<QString, QMetaMethod> methods;
    QList<QMetaProperty> property;
    QHash<QString, QMetaProperty> propertyHash;
    QString name;
    QString description;
    QHash<QString, QMetaProperty> propertyByFieldName;
    QHash<QString, QMetaProperty> propertyByPropertyName;
    QHash<QString, QMetaProperty> propertyFiltrable;
    QHash<QString, QMetaProperty> propertyFiltrableByLike;
    QHash<QString, QMetaProperty> propertyInfo;
    QHash<QString, QMetaProperty> propertyPK;
    QHash<QString, QMetaProperty> propertyForeignKeysPK;
    QHash<QString, QMetaProperty> propertyForeignKeys;
    QHash<QString, QMetaProperty> propertyFKonPK;

    QHash<QString,QString> propertyTableVsShort;
    QHash<QString,QString> propertyShortVsTable;
    QHash<QString,QString> propertyShortFKVsShortPK;
    QVariantHash propertyDescriptors;
    QStringList propertyList;
    QStringList propertyTableList;
    QVariantHash propertyDeactivateField;
    QString tableSchema;
    QString tablePrefix;
    QString tablePrefixSeparator;
    QString tableName;
    QString tableNameFull;
    QStringList tablePk;
    QVariantList tablePkField;
    QVariantHash tablePkCompuser;
    QStringList tableOrderBy;
    QVariantList tableOrderByField;
    QStringList tableAutoSetFields;
    QVVM tableDeactivateField;
    QString tablePkSingle;
    QVariantHash tableSequence;
    QVariant tablePkAutoGenerate;
    QStringList tableForeignKeysPK;
    QVariantHash tableForeignKeys;
    QVariantList tableFiltrableField;
    QVariantList tableForeignPKField;
    ModelInfo*modelInfo=nullptr;

    explicit ModelInfoPvt(ModelInfo*parent)
    {
        this->modelInfo=parent;
    }

    virtual ~ModelInfoPvt()
    {

    }

    static bool invoke(QObject*objectCheck, const QString &methodName)
    {
        auto __methodName=methodName.trimmed().replace(QByteArrayLiteral("\""), "").toLower();
        auto metaObject=objectCheck->metaObject();
        for(int methodIndex = 0; methodIndex < metaObject->methodCount(); ++methodIndex){

            auto method = metaObject->method(methodIndex);

            if(method.returnType()!=QMetaType::Void)
                continue;

            if(method.parameterCount()>0)
                continue;

            if(method.name().toLower() != __methodName)
                continue;

            if(!method.invoke(objectCheck, Qt::DirectConnection))
                return false;

            return true;

        }
        return false;
    };

    static QVariant invokeVar(QObject*objectCheck, const QString &methodName)
    {
        auto __methodName=methodName.toUtf8().trimmed().replace(QByteArrayLiteral("\""), "").toLower();
        auto metaObject=objectCheck->metaObject();
        for(int methodIndex = 0; methodIndex < metaObject->methodCount(); ++methodIndex) {
            auto method = metaObject->method(methodIndex);

            if(!(method.returnType()==QMetaType::QVariantList || method.returnType()==QMetaType::QStringList))
                continue;

            if(method.parameterCount()>0)
                continue;

            if(method.name().toLower() != __methodName)
                continue;

            QVariant _v;
            QVariantList _vList;
            QStringList _sList;
            QGenericReturnArgument invokeReturn;

            switch (method.returnType()) {
            case QMetaType::QVariantList:
                invokeReturn=Q_RETURN_ARG(QVariantList, _vList);
                break;
            case QMetaType::QStringList:
                invokeReturn=Q_RETURN_ARG(QStringList, _sList);
                break;
            default:
                invokeReturn=Q_RETURN_ARG(QVariant, _v);
                break;
            }

            if(!method.invoke(objectCheck, Qt::DirectConnection, invokeReturn))
                continue;

            return _v;

        }
        return {};
    }

    static QVariantList invokeList(QObject*objectCheck, const QString &methodName)
    {
        auto __methodName=methodName.toUtf8().trimmed().replace(QByteArrayLiteral("\""), "").toLower();
        auto metaObject=objectCheck->metaObject();
        for(int methodIndex = 0; methodIndex < metaObject->methodCount(); ++methodIndex) {
            auto method = metaObject->method(methodIndex);

            if(!(method.returnType()==QMetaType::QVariantList || method.returnType()==QMetaType::QStringList))
                continue;

            if(method.parameterCount()>0)
                continue;

            if(method.name().toLower() != __methodName)
                continue;

            QVariant _v;
            QVariantList _vList;
            QStringList _sList;
            QGenericReturnArgument invokeReturn;

            switch (method.returnType()) {
            case QMetaType::QVariantList:
                invokeReturn=Q_RETURN_ARG(QVariantList, _vList);
                break;
            case QMetaType::QStringList:
                invokeReturn=Q_RETURN_ARG(QStringList, _sList);
                break;
            default:
                invokeReturn=Q_RETURN_ARG(QVariant, _v);
                break;
            }

            if(!method.invoke(objectCheck, Qt::DirectConnection, invokeReturn))
                continue;

            switch (method.returnType()) {
            case QMetaType::QVariantList:
                return _vList;
            case QMetaType::QStringList:
                return QVariant(_sList).toList();
            default:
                return _v.toList();
            }

        }
        return {};
    };

    static QVariantHash invokeMap(QObject*objectCheck, const QString &methodName)
    {
        auto __methodName=methodName.toUtf8().trimmed().replace(QByteArrayLiteral("\""), "").toLower();
        auto metaObject=objectCheck->metaObject();
        for(int methodIndex = 0; methodIndex < metaObject->methodCount(); ++methodIndex) {
            auto method = metaObject->method(methodIndex);

            if(!(method.returnType()==QMetaType::QVariantMap || method.returnType()==QMetaType::QVariantHash))
                continue;

            if(method.parameterCount()>0)
                continue;

            if(method.name().toLower() != __methodName)
                continue;

            QVariant _v;
            QVariantMap _vMap;
            QVariantHash _vHash;
            QGenericReturnArgument invokeReturn;

            switch (method.returnType()) {
            case QMetaType::QVariantHash:
                invokeReturn=Q_RETURN_ARG(QVariantHash, _vHash);
                break;
            case QMetaType::QVariantMap:
                invokeReturn=Q_RETURN_ARG(QVariantMap, _vMap);
                break;
            default:
                invokeReturn=Q_RETURN_ARG(QVariant, _v);
                break;
            }

            if(!method.invoke(objectCheck, Qt::DirectConnection, invokeReturn))
                continue;

            switch (method.returnType()) {
            case QMetaType::QVariantHash:
                return _vHash;
            case QMetaType::QVariantMap:
                return QVariant(_vMap).toHash();
            default:
                return _v.toHash();
            }

        }
        return {};
    };

    static QString invokeText(QObject*objectCheck, const QString &methodName)
    {
        QSqlDatabase RETURN;

        auto __methodName=methodName.trimmed().replace(QByteArrayLiteral("\""), "").toLower();
        auto metaObject=objectCheck->metaObject();
        for(int methodIndex = 0; methodIndex < metaObject->methodCount(); ++methodIndex) {
            auto method = metaObject->method(methodIndex);

            if(method.parameterCount()>0)
                continue;

            if(method.name().toLower() != __methodName)
                continue;

            QVariant _v;
            QByteArray _textBytes;
            QString _textString;
            QGenericReturnArgument invokeReturn;


            switch (method.returnType()) {
            case QMetaType::QByteArray:
                invokeReturn=Q_RETURN_ARG(QByteArray, _textBytes);
                break;
            case QMetaType::QString:
                invokeReturn=Q_RETURN_ARG(QString, _textString);
                break;
            default:
                invokeReturn=Q_RETURN_ARG(QVariant, _v);
                break;
            }

            if(!method.invoke(objectCheck, Qt::DirectConnection, invokeReturn))
                continue;

            switch (method.returnType()) {
            case QMetaType::QByteArray:
                return _textBytes;
            case QMetaType::QString:
                return _textString;
            default:
                return _v.toString();
            }
        }
        return {};
    };

    static bool invokeBool(QObject*objectCheck, const QString &methodName)
    {
        QSqlDatabase RETURN;

        auto __methodName=methodName.trimmed().replace(QByteArrayLiteral("\""), "").toLower();
        auto metaObject=objectCheck->metaObject();
        for(int methodIndex = 0; methodIndex < metaObject->methodCount(); ++methodIndex) {
            auto method = metaObject->method(methodIndex);

            if(method.parameterCount()>0)
                continue;

            if(method.name().toLower() != __methodName)
                continue;

            if(method.returnType()==QMetaType::Bool)
                continue;

            bool __return=false;
            QGenericReturnArgument invokeReturn;

            invokeReturn=Q_RETURN_ARG(bool, __return);

            if(!method.invoke(objectCheck, Qt::DirectConnection, invokeReturn))
                continue;

            return __return;
        }
        return false;
    };

    static ModelInfo &static_initMetaObject(const QMetaObject &staticMetaObject)
    {
        const auto &className=staticMetaObject.className();
        auto &modelInfo=(*__static_model_info)[className];
        if(modelInfo==nullptr){
            modelInfo=new ModelInfo{};
            modelInfo->setName(className);
            static_init_make(modelInfo->p, staticMetaObject);
        }
        return*modelInfo;
    }

    void read(ModelInfoPvt *p)
    {
#define ____copy(var)\
        this->var=p->var;
        ____copy(descriptor             );
        ____copy(tablePkAutoGenerate    );
        ____copy(property               );
        ____copy(propertyHash           );
        ____copy(propertyByFieldName    );
        ____copy(propertyByPropertyName );
        ____copy(propertyFiltrable      );
        ____copy(propertyFiltrableByLike);
        ____copy(propertyInfo           );
        ____copy(propertyTableVsShort   );
        ____copy(propertyShortVsTable   );
        ____copy(propertyList           );
        ____copy(propertyTableList      );
        ____copy(propertyDeactivateField);
        ____copy(propertyDescriptors    );
        ____copy(tableSchema            );
        ____copy(tablePrefix            );
        ____copy(tablePrefixSeparator   );
        ____copy(tableName              );
        ____copy(tablePk                );
        ____copy(tablePkField           );
        ____copy(tablePkCompuser        );
        ____copy(propertyPK             );
        ____copy(propertyForeignKeysPK  );
        ____copy(propertyForeignKeys    );
        ____copy(propertyFKonPK         );
        ____copy(tablePkSingle          );
        ____copy(tableSequence          );
        ____copy(tableOrderBy           );
        ____copy(tableOrderByField      );
        ____copy(tableAutoSetFields     );
        ____copy(tableDeactivateField   );
        ____copy(tableForeignKeysPK     );
        ____copy(tableForeignKeys       );
        ____copy(tableFiltrableField    );
        ____copy(tableForeignPKField    );
    }

    void clear()
    {
#define ____clear(var)this->var.clear()

        this->descriptor=nullptr;
        ____clear(tablePkAutoGenerate    );
        ____clear(property               );
        ____clear(propertyHash           );
        ____clear(propertyByFieldName    );
        ____clear(propertyByPropertyName );
        ____clear(propertyInfo           );
        ____clear(propertyTableVsShort   );
        ____clear(propertyShortVsTable   );
        ____clear(propertyList           );
        ____clear(propertyTableList      );
        ____clear(propertyDeactivateField);
        ____clear(propertyDescriptors    );
        ____clear(propertyPK             );
        ____clear(propertyForeignKeysPK  );
        ____clear(propertyForeignKeys    );
        ____clear(propertyFKonPK         );
        ____clear(tableSchema            );
        ____clear(tablePrefix            );
        ____clear(tablePrefixSeparator   );
        ____clear(tableName              );
        ____clear(tablePk                );
        ____clear(tablePkField           );
        ____clear(tablePkSingle          );
        ____clear(tableSequence          );
        ____clear(tableOrderBy           );
        ____clear(tableOrderByField      );
        ____clear(tableAutoSetFields     );
        ____clear(tableDeactivateField   );
        ____clear(tableForeignKeys       );
        ____clear(tableFiltrableField    );
        ____clear(tableForeignKeysPK     );
        ____clear(tableForeignPKField    );
    }

    static void static_init_make(ModelInfoPvt *pvt, const QMetaObject &staticMetaObject)
    {
        if(!staticMetaObject.inherits(&QOrm::Model::staticMetaObject))
            return;

        pvt->clear();
        QScopedPointer<QObject> scopePointer(staticMetaObject.newInstance(Q_ARG(QObject*, nullptr )));
        auto object=scopePointer.data();

        if(object==nullptr)
            return;

        auto modelObject=dynamic_cast<Model*>(object);

        if(modelObject==nullptr){
            delete object;
            return;
        }

        pvt->staticMetaObjectModel=staticMetaObject;
        pvt->staticMetaObjectDescriptor=modelObject->descriptor();

        for(int i = 0; i < staticMetaObject.propertyCount(); ++i) {
            auto property=staticMetaObject.property(i);
            const QByteArray propertyName(property.name());

            if(!property.isValid())
                continue;

            if(__propertyIgnoredList->contains(propertyName.toLower()))
                continue;

            pvt->propertyHash.insert(property.name(), property);
            pvt->property.append(property);
        }

        for(int i = 0; i < staticMetaObject.methodCount(); ++i) {
            auto method=staticMetaObject.method(i);
            if(!method.isValid())
                continue;

            pvt->methods[method.name()]=method;
        }

        auto modelName = pvt->invokeText(modelObject, QByteArrayLiteral("modelName")).trimmed();
        auto modelDescription = pvt->invokeText(modelObject, QByteArrayLiteral("modelDescription")).trimmed();
        auto tablePkAutoGenerate = pvt->invokeBool(modelObject, QByteArrayLiteral("tablePkAutoGenerate"));
        auto tableSchema = pvt->invokeText(modelObject, QByteArrayLiteral("tableSchema")).trimmed();
        auto tablePrefix = pvt->invokeText(modelObject, QByteArrayLiteral("tablePrefix")).trimmed();
        auto tablePrefixSeparator = pvt->invokeText(modelObject, QByteArrayLiteral("tablePrefixSeparator")).trimmed();
        auto tableName = pvt->invokeText(modelObject, QByteArrayLiteral("tableName")).trimmed();
        auto tablePk = pvt->invokeText(modelObject, QByteArrayLiteral("tablePk")).trimmed();
        auto tableOrderBy = pvt->invokeText(modelObject, QByteArrayLiteral("tableOrderBy")).trimmed();
        auto tableAutoSetFields = pvt->invokeText(modelObject, QByteArrayLiteral("tableAutoSetFields")).trimmed();
        auto tableForeignPK = pvt->invokeText(modelObject, QByteArrayLiteral("tableForeignPK")).trimmed();
        auto tableDeactivateField = pvt->invokeText(modelObject, QByteArrayLiteral("tableDeactivateField")).trimmed();
        auto tableSequence = pvt->invokeMap(modelObject, QByteArrayLiteral("tableSequence"));
        auto tableFiltrableField = pvt->invokeList(modelObject, QByteArrayLiteral("tableFiltrableField"));
        auto tablePkCompuser = pvt->invokeVar(modelObject, QByteArrayLiteral("tablePkCompuser"));

        QVector<QString> tableFiltrableField_;
        if(!tableFiltrableField.isEmpty()){
            for(auto &v : tableFiltrableField){
                auto s = v.toString().trimmed();
                if(tableFiltrableField_.contains(s))
                    continue;
                tableFiltrableField_.append(s);
            }
        }

        pvt->tablePkAutoGenerate=tablePkAutoGenerate;
        pvt->name=modelName;
        pvt->description=modelDescription;
        pvt->tableSchema=tableSchema;
        pvt->tablePrefix=tablePrefix+tablePrefixSeparator;
        pvt->tablePrefixSeparator=tablePrefixSeparator;
        pvt->tableName=tablePrefix+tablePrefixSeparator+tableName.trimmed();
        pvt->tableSequence=tableSequence;
        pvt->tableFiltrableField=tableFiltrableField;

        {//make tableFull
            auto makeTableSchema = (tableSchema.isEmpty())?"":QStringLiteral("%1.").arg(tableSchema);
            pvt->tableNameFull = makeTableSchema+pvt->tablePrefix+tableName;
        }

        while(tablePk.contains(QByteArrayLiteral("  ")))
            tablePk = tablePk.replace(QByteArrayLiteral("  "), QByteArrayLiteral(" ")).trimmed();

        while(tableOrderBy.contains(QByteArrayLiteral("  ")))
            tableOrderBy = tableOrderBy.replace(QByteArrayLiteral("  "), QByteArrayLiteral(" ")).trimmed();

        while(tableAutoSetFields.contains(QByteArrayLiteral("  ")))
            tableAutoSetFields = tableAutoSetFields.replace(QByteArrayLiteral("  "), QByteArrayLiteral(" ")).trimmed();

        while(tableForeignPK.contains(QByteArrayLiteral("  ")))
            tableForeignPK = tableForeignPK.replace(QByteArrayLiteral("  "), QByteArrayLiteral(" ")).trimmed();

        while(tableDeactivateField.contains(QByteArrayLiteral("  ")))
            tableDeactivateField = tableDeactivateField.replace(QByteArrayLiteral("  "), QByteArrayLiteral(" ")).trimmed();

        while(tableDeactivateField.contains(QByteArrayLiteral("==")))
            tableDeactivateField = tableDeactivateField.replace(QByteArrayLiteral("=="), QByteArrayLiteral("=")).trimmed();

        for(auto &propertyName:tablePk.split(QByteArrayLiteral(" "))){
            if(propertyName.isEmpty())
                continue;
            auto field=tablePrefix+tablePrefixSeparator+propertyName;
            auto property=pvt->propertyHash.value(propertyName);
            if(!property.isValid())
                continue;

            pvt->tablePk.append(field);
            pvt->tablePkField.append(SqlParserItem::createObject(field));
            pvt->propertyPK.insert(propertyName, pvt->propertyHash[propertyName]);
        }

        pvt->tablePkSingle=pvt->tablePk.isEmpty()?"":pvt->tablePk.join(QByteArrayLiteral(" "));

        for(auto &propertyName:tableForeignPK.split(QByteArrayLiteral(" "))){
            if(propertyName.isEmpty())
                continue;

            auto property=pvt->propertyHash.value(propertyName);
            if(!property.isValid())
                continue;

            auto field=tablePrefix+tablePrefixSeparator+propertyName;
            pvt->tableForeignKeysPK.append(field);
            pvt->tableForeignPKField.append(SqlParserItem::createObject(field));
            pvt->propertyForeignKeys.insert(property.name(), property);
            pvt->propertyForeignKeysPK.insert(property.name(), property);
        }

        {
            Q_V_PROPERTY_ITERATOR(pvt->propertyHash){
                i.next();
                auto &propertyName=i.key();
                auto &property=i.value();
                const auto propertyA=propertyName;
                const auto propertyB=tablePrefix+QByteArrayLiteral("_")+propertyName;
                pvt->propertyList.append(propertyName);
                pvt->propertyTableList.append(propertyB);
                pvt->propertyShortVsTable.insert(propertyA, propertyB);
                pvt->propertyTableVsShort.insert(propertyB, propertyA);
                pvt->propertyByPropertyName.insert(propertyA, property);
                pvt->propertyByFieldName.insert(propertyB, property);

                if(tableFiltrableField_.contains(propertyName))
                    pvt->propertyFiltrable.insert(propertyName, property);

                if(!QMetaTypeUtilVariantDictionary.contains(property.typeId()))//property info
                    continue;

                auto split=propertyName.split(QStringLiteral("qorm__info__"));
                if(split.size()!=2)
                    continue;

                auto name=split.last().trimmed();
                pvt->propertyInfo.insert(name, property);
            }

            {
                Q_V_METHOD_ITERATOR(pvt->methods){
                    i.next();
                    auto methodName=i.key();
                    if(!methodName.startsWith(QByteArrayLiteral("tableForeignKeys_")))
                        continue;

                    auto vHash = pvt->invokeMap(modelObject, methodName);
                    auto fieldName=vHash[QStringLiteral("fk")].toString().trimmed();
                    if(fieldName.isEmpty())
                        continue;

                    pvt->tableForeignKeys.insert(fieldName, vHash);
                    auto property=pvt->propertyHash.value(fieldName);
                    if(property.isValid())//if exists
                        pvt->propertyForeignKeys.insert(property.name(), property);
                }
            }

            for(auto &propertyName:tableOrderBy.split(QByteArrayLiteral(" "))){
                if(propertyName.isEmpty())
                    continue;

                auto property=pvt->propertyHash.value(propertyName);
                if(!property.isValid())
                    continue;
                auto field=tablePrefix+tablePrefixSeparator+propertyName;
                if(!pvt->propertyTableList.contains(field))
                    continue;
                pvt->tableOrderBy.append(field);
                pvt->tableOrderByField.append(SqlParserItem::createObject(field));
            }

            for(auto &propertyName:tableAutoSetFields.split(QByteArrayLiteral(" "))){
                if(propertyName.isEmpty())
                    continue;

                auto property=pvt->propertyHash.value(propertyName);
                if(!property.isValid())
                    continue;

                pvt->tableAutoSetFields.append(propertyName);
            }

            for(auto &row:tableDeactivateField.split(QByteArrayLiteral(" "))){
                if(row.isEmpty())
                    continue;

                auto rowSplited=row.split(QStringLiteral("="));
                if(rowSplited.size()<=1){
                    oWarning()<<QStringLiteral("invalid deactivate fields on ")+pvt->tableNameFull;
                    continue;
                }

                auto propertyName=rowSplited.first();
                auto property=pvt->propertyHash.value(propertyName);
                if(!property.isValid())
                    continue;

                if(!pvt->propertyByPropertyName.contains(propertyName))
                    continue;

                property=pvt->propertyByPropertyName.value(propertyName);
                auto value=QVariant{rowSplited.last()};
                if(!value.isValid() || value.isNull())
                    value={};
                else
                    value=VariantUtil(value).toType(property.typeId());

                auto field=tablePrefix+tablePrefixSeparator+propertyName;
                auto vField=SqlParserItem::createObject(field);
                auto vValue=SqlParserItem::createValue(value);
                pvt->propertyDeactivateField.insert(propertyName,vValue);
                pvt->tableDeactivateField.insert(vField, vValue);
            }

            auto makeTablePkCompuser=[&tablePkCompuser, &pvt](){
                if(!tablePkCompuser.isValid() || tablePkCompuser.isNull())
                    return;
                switch (tablePkCompuser.typeId()) {
                case QMetaType::QVariantList:
                case QMetaType::QStringList:
                {
                    for(auto &property:pvt->propertyPK)
                        pvt->tablePkCompuser.insert(property.name(), tablePkCompuser);
                    break;
                }
                case QMetaType::QVariantHash:
                case QMetaType::QVariantMap:
                {
                    auto vHash=tablePkCompuser.toHash();
                    QVariantHash vHashOut;
                    for(auto &property:pvt->propertyPK){
                        auto v=vHash.value(property.name());
                        if(v.isNull() || !v.isValid()){
                            vHashOut.clear();
                            break;
                        }
                        vHashOut.insert(property.name(), tablePkCompuser);
                    }
                    pvt->tablePkCompuser=vHashOut;
                    break;
                }
                default:
                    break;
                }
            };
            makeTablePkCompuser();
        }
    }

    static void initDescriptores()
    {
        auto makeDescritor=[](ModelInfo* modelInfo){

            auto pvt=modelInfo->p;

            auto metaObject=pvt->staticMetaObjectDescriptor;
            if(!metaObject.inherits(&ModelFieldDescriptors::staticMetaObject))//SE HERDAR de QOrm::ModelFieldDescriptors
                return;

            auto obj=metaObject.newInstance(Q_ARG(QObject*, nullptr));
            if(obj==nullptr)
                return;

            pvt->descriptor=dynamic_cast<ModelFieldDescriptors*>(obj);

            if(pvt->descriptor==nullptr){
                pvt->propertyDescriptors.clear();
                delete obj;
                return;
            }

            pvt->descriptor->descriptorsInit();
            if(pvt->description.isEmpty())
                pvt->description=pvt->descriptor->description();
            pvt->propertyDescriptors=pvt->descriptor->toHash();
        };

        QHashIterator <QByteArray, QOrm::ModelInfo*> i(*__static_model_info);
        while(i.hasNext()){
            i.next();
            makeDescritor(i.value());
        }
    }
};


static void init()
{
    ModelInfoPvt::initDescriptores();
}

Q_ORM_STARTUP_FUNCTION(init);

ModelInfo::ModelInfo()
{
    this->p=new ModelInfoPvt{this};
}

ModelInfo::ModelInfo(const QByteArray &className):QVariant{}
{
    this->p=new ModelInfoPvt{this};
    if(__static_model_info->contains(className)){
        const auto info=*__static_model_info->value(className);
        p->read(info.p);
    }
}

ModelInfo::ModelInfo(const QMetaObject &metaObject)
{
    this->p=new ModelInfoPvt{this};
    if(__static_model_info->contains(metaObject.className())){
        const auto info=*__static_model_info->value(metaObject.className());
        p->read(info.p);
    }
}

ModelInfo::~ModelInfo()
{
    delete p;
}

QUuid &ModelInfo::uuid() const
{
    if(p->uuid.isNull()){
        Q_DECLARE_VU;
        p->uuid=vu.toMd5Uuid(QVariant::toByteArray());
    }
    return p->uuid;
}

bool ModelInfo::isValid() const
{
    if(this->name().isEmpty())
        return {};
    return true;
}

QString &ModelInfo::name()const
{
    if(p->name.isEmpty())
        p->name=QVariant::toString();
    return p->name;
}

ModelInfo &ModelInfo::name(const QString &value)
{
    p->name=value;
    QVariant::setValue(value);
    return *this;
}

ModelInfo &ModelInfo::setName(const QString &value)
{
    p->name=value;
    QVariant::setValue(value);
    return *this;
}

QString &ModelInfo::description() const
{
    return p->description;
}

ModelInfo &ModelInfo::description(const QString &value)
{
    p->description = value.trimmed();
    return *this;
}

ModelInfo &ModelInfo::setDescription(const QString &value)
{
    p->description = value.trimmed();
    return *this;
}

const ModelInfo &ModelInfo::from(const QVariant &v)
{
    QByteArray className;

    switch (v.typeId()) {
    case QMetaType::QVariantHash:
    case QMetaType::QVariantMap:
        className=v.toHash().value(QStringLiteral("classname")).toByteArray();
        break;
    default:
        className=v.toByteArray();
    }

    if(__static_model_info->contains(className)){
        const auto &info=*__static_model_info->value(className);
        return info;
    }

    static ModelInfo info;
    return info;
}

const ModelInfo &ModelInfo::from(const QMetaObject &metaObject)
{
    return ModelInfo::from(metaObject.className());
}

ModelInfo &ModelInfo::modelInfoInit(const QMetaObject &staticMetaObject)
{
    return ModelInfoPvt::static_initMetaObject(staticMetaObject);
}

const ModelFieldDescriptors *ModelInfo::descritor() const
{
    return p->descriptor;
}

QMetaObject &ModelInfo::staticMetaObject() const
{
    return p->staticMetaObjectModel;
}

const QStringList &ModelInfo::propertyIgnoredList()
{
    return *__propertyIgnoredList;
}

QList<QMetaProperty> &ModelInfo::property() const
{
    return p->property;
}

QHash<QString, QMetaProperty> &ModelInfo::propertyByFieldName() const
{
    return p->propertyByFieldName;
}

QMetaProperty ModelInfo::propertyByFieldName(const QString &fieldName) const
{
    auto _fieldName=fieldName.trimmed().toLower();
    Q_V_PROPERTY_ITERATOR(p->propertyByFieldName){
        i.next();
        if(i.key().toLower()==_fieldName)
            return i.value();
    }
    const static QMetaProperty metaProperty;
    return metaProperty;
}

QHash<QString, QMetaProperty> &ModelInfo::propertyFiltrable() const
{
    return p->propertyFiltrable;
}

QHash<QString, QMetaProperty> &ModelInfo::propertyFiltrableByLike() const
{
    return p->propertyFiltrableByLike;
}

QHash<QString, QMetaProperty> &ModelInfo::propertyByPropertyName() const
{
    return p->propertyByPropertyName;
}

QMetaProperty ModelInfo::propertyByPropertyName(const QString &propertyName) const
{

    auto property=p->propertyByPropertyName.value(propertyName);
    if(property.isValid())
        return property;

    const auto pName=propertyName.trimmed().toLower();
    Q_V_PROPERTY_ITERATOR(p->propertyByPropertyName){
        i.next();
        if(i.key().toLower()==pName)
            return i.value();
    }
    const static QMetaProperty metaProperty;
    return metaProperty;
}

QStringList &ModelInfo::propertyList() const
{
    return p->propertyList;
}

QStringList &ModelInfo::propertyTableList() const
{
    return p->propertyTableList;
}

QHash<QString, QString> &ModelInfo::propertyTableVsShort() const
{
    return p->propertyTableVsShort;
}

QHash<QString, QString> &ModelInfo::propertyShortVsTable() const
{
    return p->propertyShortVsTable;
}

QHash<QString, QMetaProperty> &ModelInfo::propertyInfo() const
{
    return p->propertyInfo;
}

QHash<QString, QMetaProperty> &ModelInfo::propertyPK() const
{
    return p->propertyPK;
}

QHash<QString, QMetaProperty> &ModelInfo::propertyForeignKeysPK() const
{
    return p->propertyForeignKeysPK;
}

QHash<QString, QMetaProperty> &ModelInfo::propertyForeignKeys() const
{
    return p->propertyForeignKeys;
}

QVariantHash ModelInfo::propertyDescriptors() const
{
    return p->propertyDescriptors;
}

QVariantHash ModelInfo::propertyParserToTable(const QVariant &v) const
{
    QVVM vvm;
    const auto &propertyList=p->propertyShortVsTable;
    auto vHash=v.toHash();
    Q_V_HASH_ITERATOR(vHash){
        i.next();
        auto k=i.key().trimmed();
        SqlParserItem ik;
        SqlParserItem iv;
        if(propertyList.contains(k))
            ik=SqlParserItem::createObject(propertyList.value(k));
        else
            ik=SqlParserItem::createValue(k);

        switch (i.value().typeId()) {
        case QMetaType::QVariantHash:
        case QMetaType::QVariantMap:
            iv=i.value();
            break;
        default:
            if(propertyList.contains(v.toString())){
                iv=SqlParserItem::createObject(i.value());
                break;
            }
            iv=SqlParserItem::createValue(i.value());
        }

        vvm.insert(ik, iv);
    }
    return QVariantHash{vvm};
}

QVariantHash ModelInfo::propertyParserToProperty(const QVariant &v) const
{

    QVVM vvm;
    const auto &propertyList=p->propertyTableVsShort;
    auto vHash=v.toHash();
    Q_V_HASH_ITERATOR(vHash){
        i.next();
        auto k=i.key().trimmed();
        SqlParserItem ik;
        SqlParserItem iv;
        if(propertyList.contains(k))
            ik=SqlParserItem::createObject(propertyList.value(k));
        else
            ik=SqlParserItem::createValue(k);

        switch (i.value().typeId()) {
        case QMetaType::QVariantHash:
        case QMetaType::QVariantMap:
            iv=i.value();
            break;
        default:
            if(propertyList.contains(v.toString())){
                iv=SqlParserItem::createObject(i.value());
                break;
            }
            iv=SqlParserItem::createValue(i.value());
        }
        vvm.insert(ik, iv);
    }
    return QVariantHash{vvm};
}

bool ModelInfo::propertyNameConvertMap(QVariantHash &propertyHash)
{

    auto vHash=propertyHash;
    propertyHash.clear();
    Q_V_HASH_ITERATOR(vHash){
        i.next();
        const auto key=p->propertyShortVsTable.value(i.key()).toLower();
        if(key.isEmpty()){
            propertyHash.clear();
            return false;
        }

        auto value=vHash.value(i.key());
        propertyHash.insert(key, value);
    }
    return !propertyHash.isEmpty();
}

QVariantHash ModelInfo::propertyDeactivateField() const
{
    return p->propertyDeactivateField;
}

QString &ModelInfo::tableSchema()const
{
    return p->tableSchema;
}

QString &ModelInfo::tablePrefix()const
{
    return p->tablePrefix;
}

QString ModelInfo::tablePrefixSeparator() const
{
    return p->tablePrefixSeparator;
}

QString ModelInfo::tableName()const
{
    return p->tableName;
}

QString ModelInfo::tableNameFull()const
{
    return p->tableNameFull;
}

QVariantHash ModelInfo::tableSequence()const
{
    return p->tableSequence;
}

QStringList ModelInfo::tablePk()const
{
    return p->tablePk;
}

QVariantList ModelInfo::tablePkField() const
{
    return p->tablePkField;
}

QStringList ModelInfo::tableForeignKeysPK()const
{
    return p->tableForeignKeysPK;
}

QVariantHash ModelInfo::tableForeignKeys() const
{
    return p->tableForeignKeys;
}

QVariantList ModelInfo::tableFiltrableField() const
{
    return p->tableFiltrableField;
}

QStringList ModelInfo::tableOrderBy() const
{
    return p->tableOrderBy;
}

QVariantList ModelInfo::tableOrderByField() const
{
    return p->tableOrderByField;
}

QStringList ModelInfo::tableAutoSetFields() const
{
    return p->tableAutoSetFields;
}

QVariantHash ModelInfo::tableDeactivateField() const
{
    return p->tableDeactivateField;
}

bool ModelInfo::tablePkAutoGenerate()const
{
    return p->tablePkAutoGenerate.toBool();
}

QString ModelInfo::tablePkSingle()const
{
    return p->tablePkSingle;
}

QVariantMap ModelInfo::toMap(const QObject *object)const
{
    QVariantMap __return;
    for(auto &property:p->property){
        if(!property.isValid())
            continue;
        auto propertyName = QString{property.name()}.toLower();
        QVariant value;
        switch (property.typeId()){
        case QMetaType::User:
        case 65538/*CustomType*/:
            value=property.read(object).toInt();
            break;
        default:
            value=value=property.read(object);
        }
        __return.insert(propertyName, value);
    }
    return __return;
}

QVariantHash ModelInfo::toHash(const QObject *object) const
{
    QVariantHash __return;
    for(auto &property:p->property){
        if(!property.isValid())
            continue;
        auto propertyName =QString{property.name()}.toLower();
        QVariant value;
        switch (property.typeId()){
        case QMetaType::User:
        case 65538/*CustomType*/:
            value=property.read(object).toInt();
            break;
        default:
            value=value=property.read(object);
        }
        __return.insert(propertyName, value);
    }
    return __return;
}

QVariantMap ModelInfo::toMapModel(const QObject *object) const
{
    return QVariant{this->toHashModel(object)}.toMap();
}

QVariantHash ModelInfo::toHashModel(const QObject *object)const
{
    QVariantHash __return;
    for(auto &property:p->property){
        if(!property.isValid())
            continue;

        auto propertyName = QString{property.name()}.toLower();
        propertyName=p->propertyShortVsTable.value(propertyName);
        __return.insert(propertyName, property.read(object));
    }
    return __return;
}

QVVM ModelInfo::parserVVM(const QVariantHash &vvm) const
{
    if(vvm.isEmpty())
        return QVVM{};

    QVVM __return;

    //auto&propertyByFieldName=p->propertyByFieldName;

    Q_DECLARE_VU;
    const auto &propertyByFieldName=p->propertyByFieldName;
    const auto &propertyShortVsTable=p->propertyShortVsTable;
    const auto &propertyTableVsShort=p->propertyTableVsShort;

    QHashIterator<QString, QVariant> i(vvm);
    while(i.hasNext()){
        i.next();
        auto v1=vu.toVariant(i.key());
        auto v2=vu.toVariant(i.value());

        switch (v1.typeId()) {
        case QMetaType::QString:
        case QMetaType::QByteArray:
        {
            auto s=v1.toString().trimmed().toLower();
            if(propertyByFieldName.contains(s))
                v1=SqlParserItem::createObject(s);
            else if(propertyShortVsTable.contains(s))
                v1=SqlParserItem::createObject(propertyShortVsTable.value(s));
            else if(propertyTableVsShort.contains(s))
                v1=SqlParserItem::createObject(propertyTableVsShort.value(s));
            break;
        }
        default:
            break;
        }

        switch (v2.typeId()) {
        case QMetaType::QString:
        case QMetaType::QByteArray:
        {
            auto s=v2.toString().trimmed().toLower();
            if(propertyByFieldName.contains(s))
                v2=SqlParserItem::createObject(s);
            else if(propertyShortVsTable.contains(s))
                v2=SqlParserItem::createObject(propertyShortVsTable.value(s));
            else if(propertyTableVsShort.contains(s))
                v2=SqlParserItem::createObject(propertyTableVsShort.value(s));
            break;
        }
        default:
            break;
        }
        __return.insert(v1,v2);
    }

    return __return;
}

}
