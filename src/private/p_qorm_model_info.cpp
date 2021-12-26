#include "./p_qorm_model_info.h"
#include "./qorm_model.h"
#include "./p_qorm_const.h"
#include <QMutex>
#include <QCoreApplication>

namespace PrivateQOrm{
    typedef QHash<QByteArray, QOrm::ModelInfo*> ByteModelInfo;
    Q_GLOBAL_STATIC(ByteModelInfo, __static_model_info);
}

namespace QOrm {

static auto&__static_model_info=*PrivateQOrm::__static_model_info;

#define dPvt()\
    auto&p = *reinterpret_cast<ModelInfoPvt*>(this->p)

class ModelInfoPvt{
public:
    QMetaObject staticMetaObject;
    QHash<QString, QMetaMethod> methods;
    QList<QMetaProperty> property;
    QHash<QString, QMetaProperty> propertyHash;
    QString modelName;
    QString modelDescription;
    QHash<QString, QMetaProperty> propertyByFieldName;
    QHash<QString, QMetaProperty> propertyByPropertyName;
    QHash<QString, QMetaProperty> propertyFiltrable;
    QHash<QString, QMetaProperty> propertyInfo;
    QHash<QString, QMetaProperty> propertyPK;
    QHash<QString, QMetaProperty> propertyFK;
    QHash<QString, QMetaProperty> propertyFKonPK;

    QHash<QString,QString> propertyTableVsShort;
    QHash<QString,QString> propertyShortVsTable;
    QHash<QString,QString> propertyShortFKVsShortPK;
    QVariantList propertyDescriptors;
    QVariantHash propertySort;
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
    QVVM tableDeactivateField;
    QString tablePkSingle;
    QVariantHash tableSequence;
    QVariant tablePkAutoGenerate;
    QVariantHash tableForeignKey;
    QVariantList tableFiltrableField;
    QStringList tableForeignPk;
    QVariantList tableForeignPkField;
    ModelInfo*modelInfo=nullptr;

    explicit ModelInfoPvt(ModelInfo*parent){
        this->modelInfo=parent;
    }

    virtual ~ModelInfoPvt(){

    }

    static bool invoke(QObject*objectCheck, const QString&methodName)
    {
        auto __methodName=methodName.trimmed().replace(qbl("\""), qbl_null).toLower();
        auto metaObject=objectCheck->metaObject();
        for(int methodIndex = 0; methodIndex < metaObject->methodCount(); ++methodIndex){

            auto method = metaObject->method(methodIndex);

            if(method.returnType()!=QMetaType_Void)
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

    static QVariant invokeVar(QObject*objectCheck, const QString&methodName)
    {
        auto __methodName=methodName.toUtf8().trimmed().replace(qbl("\""), qbl_null).toLower();
        auto metaObject=objectCheck->metaObject();
        for(int methodIndex = 0; methodIndex < metaObject->methodCount(); ++methodIndex) {
            auto method = metaObject->method(methodIndex);

            if(!(method.returnType()==QMetaType_QVariantList || method.returnType()==QMetaType_QStringList))
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
            case QMetaType_QVariantList:
                invokeReturn=Q_RETURN_ARG(QVariantList, _vList);
                break;
            case QMetaType_QStringList:
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

    static QVariantList invokeList(QObject*objectCheck, const QString&methodName)
    {
        auto __methodName=methodName.toUtf8().trimmed().replace(qbl("\""), qbl_null).toLower();
        auto metaObject=objectCheck->metaObject();
        for(int methodIndex = 0; methodIndex < metaObject->methodCount(); ++methodIndex) {
            auto method = metaObject->method(methodIndex);

            if(!(method.returnType()==QMetaType_QVariantList || method.returnType()==QMetaType_QStringList))
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
            case QMetaType_QVariantList:
                invokeReturn=Q_RETURN_ARG(QVariantList, _vList);
                break;
            case QMetaType_QStringList:
                invokeReturn=Q_RETURN_ARG(QStringList, _sList);
                break;
            default:
                invokeReturn=Q_RETURN_ARG(QVariant, _v);
                break;
            }

            if(!method.invoke(objectCheck, Qt::DirectConnection, invokeReturn))
                continue;

            switch (method.returnType()) {
            case QMetaType_QVariantList:
                return _vList;
            case QMetaType_QStringList:
                return QVariant(_sList).toList();
            default:
                return _v.toList();
            }

        }
        return {};
    };

    static QVariantHash invokeMap(QObject*objectCheck, const QString&methodName){
        //QSqlDatabase RETURN;

        auto __methodName=methodName.toUtf8().trimmed().replace(qbl("\""), qbl_null).toLower();
        auto metaObject=objectCheck->metaObject();
        for(int methodIndex = 0; methodIndex < metaObject->methodCount(); ++methodIndex) {
            auto method = metaObject->method(methodIndex);

            if(!(method.returnType()==QMetaType_QVariantMap || method.returnType()==QMetaType_QVariantHash))
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
            case QMetaType_QVariantHash:
                invokeReturn=Q_RETURN_ARG(QVariantHash, _vHash);
                break;
            case QMetaType_QVariantMap:
                invokeReturn=Q_RETURN_ARG(QVariantMap, _vMap);
                break;
            default:
                invokeReturn=Q_RETURN_ARG(QVariant, _v);
                break;
            }

            if(!method.invoke(objectCheck, Qt::DirectConnection, invokeReturn))
                continue;

            switch (method.returnType()) {
            case QMetaType_QVariantHash:
                return _vHash;
            case QMetaType_QVariantMap:
                return QVariant(_vMap).toHash();
            default:
                return _v.toHash();
            }

        }
        return {};
    };

    static QString invokeText(QObject*objectCheck, const QString&methodName)
    {
        QSqlDatabase RETURN;

        auto __methodName=methodName.trimmed().replace(qbl("\""), qbl_null).toLower();
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
            case QMetaType_QByteArray:
                invokeReturn=Q_RETURN_ARG(QByteArray, _textBytes);
                break;
            case QMetaType_QString:
                invokeReturn=Q_RETURN_ARG(QString, _textString);
                break;
            default:
                invokeReturn=Q_RETURN_ARG(QVariant, _v);
                break;
            }

            if(!method.invoke(objectCheck, Qt::DirectConnection, invokeReturn))
                continue;

            switch (method.returnType()) {
            case QMetaType_QByteArray:
                return _textBytes;
            case QMetaType_QString:
                return _textString;
            default:
                return _v.toString();
            }

            return (method.returnType()==QMetaType_QString)?_textString.toUtf8():_textBytes;
        }
        return qsl_null;
    };

    static bool invokeBool(QObject*objectCheck, const QString&methodName){
        QSqlDatabase RETURN;

        auto __methodName=methodName.trimmed().replace(qbl("\""), qbl_null).toLower();
        auto metaObject=objectCheck->metaObject();
        for(int methodIndex = 0; methodIndex < metaObject->methodCount(); ++methodIndex) {
            auto method = metaObject->method(methodIndex);

            if(method.parameterCount()>0)
                continue;

            if(method.name().toLower() != __methodName)
                continue;

            if(method.returnType()==QMetaType_Bool)
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

    static ModelInfo&static_initMetaObject(const QMetaObject&staticMetaObject){
        const auto&className=staticMetaObject.className();
        auto&modelInfo=__static_model_info[className];
        if(modelInfo==nullptr){
            modelInfo=new ModelInfo();
            modelInfo->setValue(QVariantHash{{qsl("classname"), className}});
            auto&pvt=*static_cast<ModelInfoPvt*>(modelInfo->p);
            static_init_make(pvt, staticMetaObject);
        }
        return*modelInfo;
    }

    void read(void*pp){
        auto&p=*static_cast<ModelInfoPvt*>(pp);

#define ____copy(var)\
        this->var=p.var;
        ____copy(tablePkAutoGenerate    );
        ____copy(property               );
        ____copy(propertyHash           );
        ____copy(propertyByFieldName    );
        ____copy(propertyByPropertyName );
        ____copy(propertyFiltrable      );
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
        ____copy(propertyFK             );
        ____copy(propertyFKonPK         );
        ____copy(tablePkSingle          );
        ____copy(tableSequence          );
        ____copy(tableOrderBy           );
        ____copy(tableOrderByField      );
        ____copy(tableDeactivateField   );
        ____copy(tableForeignKey        );
        ____copy(tableFiltrableField    );
        ____copy(tableForeignPk         );
        ____copy(tableForeignPkField    );
    }

    void clear(){
#define ____clear(var)\
        this->var.clear()

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
        ____clear(propertyFK             );
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
        ____clear(tableDeactivateField   );
        ____clear(tableForeignKey        );
        ____clear(tableFiltrableField    );
        ____clear(tableForeignPk         );
        ____clear(tableForeignPkField    );
    }

    static void static_init_make(ModelInfoPvt&pvt, const QMetaObject&staticMetaObject)
    {
        if(!staticMetaObject.inherits(&QOrm::Model::staticMetaObject))
            return;

        pvt.clear();

        pvt.staticMetaObject=staticMetaObject;
        //auto object=staticMetaObject.newInstance();
        QScopedPointer<QObject> scopePointer(staticMetaObject.newInstance(Q_ARG(QObject*, nullptr )));
        auto object=scopePointer.data();

        if(object==nullptr)
            return;

        for(int i = 0; i < staticMetaObject.propertyCount(); ++i) {
            auto property=staticMetaObject.property(i);
            const QByteArray propertyName(property.name());

            if(!property.isValid())
                continue;

            if(__propertyIgnoredList.contains(propertyName.toLower()))
                continue;

            pvt.propertyHash[property.name()]=property;
            pvt.property.append(property);
        }

        for(int i = 0; i < staticMetaObject.methodCount(); ++i) {
            auto method=staticMetaObject.method(i);
            if(!method.isValid())
                continue;

            pvt.methods[method.name()]=method;
        }

        auto tablePkAutoGenerate = pvt.invokeBool(object, qbl("tablePkAutoGenerate"));
        auto modelName = pvt.invokeText(object, qbl("modelName")).trimmed();
        auto modelDescription = pvt.invokeText(object, qbl("modelDescription")).trimmed();
        auto tableSchema = pvt.invokeText(object, qbl("tableSchema")).trimmed();
        auto tablePrefix = pvt.invokeText(object, qbl("tablePrefix")).trimmed();
        auto tablePrefixSeparator = pvt.invokeText(object, qbl("tablePrefixSeparator")).trimmed();
        auto tableName = pvt.invokeText(object, qbl("tableName")).trimmed();
        auto tablePk = pvt.invokeText(object, qbl("tablePk")).trimmed();
        auto tableOrderBy = pvt.invokeText(object, qbl("tableOrderBy")).trimmed();
        auto tableForeignPk = pvt.invokeText(object, qbl("tableForeignPk")).trimmed();
        auto tableDeactivateField = pvt.invokeText(object, qbl("tableDeactivateField")).trimmed();
        auto tableSequence = pvt.invokeMap(object, qbl("tableSequence"));
        auto tableFiltrableField = pvt.invokeList(object, qbl("tableFiltrableField"));
        auto tablePkCompuser = pvt.invokeVar(object, qbl("tablePkCompuser"));

        QVector<QString> tableFiltrableField_;
        if(!tableFiltrableField.isEmpty()){
            for(auto&v : tableFiltrableField){
                auto s = v.toString().trimmed();
                if(tableFiltrableField_.contains(s))
                    continue;
                tableFiltrableField_<<s;
            }
        }

        pvt.tablePkAutoGenerate=tablePkAutoGenerate;
        pvt.modelName=modelName;
        pvt.modelDescription=modelDescription;
        pvt.tableSchema=tableSchema;
        pvt.tablePrefix=tablePrefix+tablePrefixSeparator;
        pvt.tablePrefixSeparator=tablePrefixSeparator;
        pvt.tableName=tablePrefix+tablePrefixSeparator+tableName.trimmed();
        pvt.tableSequence=tableSequence;
        pvt.tableFiltrableField=tableFiltrableField;

        {//make tableFull
            auto makeTableSchema = (tableSchema.isEmpty())?qbl_null:qsl("%1.").arg(tableSchema);
            pvt.tableNameFull = makeTableSchema+pvt.tablePrefix+tableName;
        }

        while(tablePk.contains(qbl("  ")))
            tablePk = tablePk.replace(qbl("  "), qbl_space).trimmed();

        while(tableOrderBy.contains(qbl("  ")))
            tableOrderBy = tableOrderBy.replace(qbl("  "), qbl_space).trimmed();

        while(tableForeignPk.contains(qbl("  ")))
            tableForeignPk = tableForeignPk.replace(qbl("  "), qbl_space).trimmed();

        while(tableDeactivateField.contains(qbl("  ")))
            tableDeactivateField = tableDeactivateField.replace(qbl("  "), qbl_space).trimmed();

        while(tableDeactivateField.contains(qbl("==")))
            tableDeactivateField = tableDeactivateField.replace(qbl("=="), qbl("=")).trimmed();

        for(auto&propertyName:tablePk.split(qbl_space)){
            if(propertyName.isEmpty())
                continue;
            auto field=tablePrefix+tablePrefixSeparator+propertyName;
            auto property=pvt.propertyHash.value(propertyName);
            if(!property.isValid())
                continue;

            pvt.tablePk<<field;
            pvt.tablePkField<<SqlParserItem::createObject(field);
            pvt.propertyPK[propertyName]=pvt.propertyHash[propertyName];
        }

        pvt.tablePkSingle=pvt.tablePk.isEmpty()?qsl_null:pvt.tablePk.join(qbl_space);

        for(auto&propertyName:tableForeignPk.split(qbl_space)){
            if(propertyName.isEmpty())
                continue;
            auto property=pvt.propertyHash.value(propertyName);
            if(!property.isValid())
                continue;
            auto field=tablePrefix+tablePrefixSeparator+propertyName;
            pvt.tableForeignPk<<field;
            pvt.tableForeignPkField<<SqlParserItem::createObject(field);
            pvt.propertyFK.insert(property.name(), property);
        }

        {
            Q_V_PROPERTY_ITERATOR(pvt.propertyHash){
                i.next();
                auto&propertyName=i.key();
                auto&property=i.value();
                const auto propertyA=propertyName;
                const auto propertyB=tablePrefix+qbl("_")+propertyName;
                pvt.propertyList<<propertyName;
                pvt.propertyTableList<<propertyB;
                pvt.propertyShortVsTable[propertyA]=propertyB;
                pvt.propertyTableVsShort[propertyB]=propertyA;
                pvt.propertyByPropertyName[propertyA]=property;
                pvt.propertyByFieldName[propertyB]=property;

                if(tableFiltrableField_.contains(propertyName))
                    pvt.propertyFiltrable[propertyName]=property;

                if(!QStmTypesVariantDictionary.contains(qTypeId(property)))//property info
                    continue;
                auto split=propertyName.split(qsl("qorm__info__"));
                if(split.size()!=2)
                    continue;
                auto name=split.last().trimmed();
                pvt.propertyInfo.insert(name, property);
            }

            {
                Q_V_METHOD_ITERATOR(pvt.methods){
                    i.next();
                    auto methodName=i.key();
                    if(!methodName.startsWith(qbl("tableForeignKey_")))
                        continue;
                    auto vHash = pvt.invokeMap(object, methodName);
                    auto fieldName=vHash[qsl("fk")].toString().trimmed();
                    if(fieldName.isEmpty())
                        continue;
                    pvt.tableForeignKey.insert(fieldName, vHash);
                    auto property=pvt.propertyHash.value(fieldName);
                    if(property.isValid())//if exists
                        pvt.propertyFK[property.name()]=property;
                }
            }

            for(auto&propertyName:tableOrderBy.split(qbl_space)){
                if(!propertyName.isEmpty()){
                    auto property=pvt.propertyHash.value(propertyName);
                    if(!property.isValid())
                        continue;
                    auto field=tablePrefix+tablePrefixSeparator+propertyName;
                    if(!pvt.propertyTableList.contains(field))
                        continue;
                    pvt.tableOrderBy<<field;
                    pvt.tableOrderByField<<SqlParserItem::createObject(field);
                }
            }

            for(auto&row:tableDeactivateField.split(qbl_space)){
                if(row.isEmpty())
                    continue;
                auto rowSplited=row.split(qsl("="));
                if(rowSplited.size()<=1){
                    sWarning()<<qsl("invalid deactivate fields on ")+pvt.tableNameFull;
                    continue;
                }

                auto propertyName=rowSplited.first();
                auto property=pvt.propertyHash.value(propertyName);
                if(!property.isValid())
                    continue;
                //sWarning()<<qsl("deactivate field(%1) not exists in model(%2) properties").arg(propertyName, staticMetaObject.className());
                if(!pvt.propertyByPropertyName.contains(propertyName))
                    continue;

                property=pvt.propertyByPropertyName.value(propertyName);
                auto value=QVariant(rowSplited.last());
                if(!value.isValid() || value.isNull())
                    value=QVariant();
                else
                    value=VariantUtil(value).toType(qTypeId(property));

                auto field=tablePrefix+tablePrefixSeparator+propertyName;
                auto vField=SqlParserItem::createObject(field);
                auto vValue=SqlParserItem::createValue(value);
                pvt.propertyDeactivateField.insert(propertyName,vValue);
                pvt.tableDeactivateField.insert(vField, vValue);
            }

            //TODO FLAVIO REMOVER
            auto model=dynamic_cast<QOrm::Model*>(object);
            if(model!=nullptr){
                auto metaObject=model->descriptor();
                if(metaObject.inherits(&ModelDescriptor::staticMetaObject)){//SE HERDAR de QOrm::ModelDescriptor
                    QScopedPointer<QObject> scopePointer(metaObject.newInstance(Q_ARG(QObject*, nullptr )));
                    if(scopePointer.data()!=nullptr){
                        auto objectDescriptor=dynamic_cast<ModelDescriptor*>(scopePointer.data());
                        if(objectDescriptor==nullptr){
                            pvt.propertyDescriptors.clear();
                        }
                        else{
                            objectDescriptor->descriptorsInit();
                            if(!objectDescriptor->description().isEmpty())
                                pvt.modelDescription=objectDescriptor->description();
                            pvt.propertyDescriptors=objectDescriptor->descriptors();
                            pvt.propertySort=objectDescriptor->sort();
                        }
                    }
                }
            }

            if(tablePkCompuser.isValid() && !tablePkCompuser.isNull()){
                auto typeId=qTypeId(tablePkCompuser);

                if(QStmTypesVariantList.contains(typeId)){
                    for(auto&property:pvt.propertyPK){
                        pvt.tablePkCompuser.insert(QString(property.name()), tablePkCompuser);
                    }
                }
                if(QStmTypesVariantDictionary.contains(typeId)){
                    auto vHash=tablePkCompuser.toHash();
                    QVariantHash vHashOut;
                    for(auto&property:pvt.propertyPK){
                        auto v=vHash.value(property.name());
                        if(v.isNull() || !v.isValid()){
                            vHashOut.clear();
                            break;
                        }
                        vHashOut.insert(QString(property.name()), tablePkCompuser);
                    }
                    pvt.tablePkCompuser=vHashOut;
                }
            }
        }
    }
};

ModelInfo::ModelInfo():QVariant()
{
    this->p=new ModelInfoPvt(this);
}

ModelInfo::~ModelInfo()
{
    dPvt();
    this->p=nullptr;
    delete&p;
}

QString &ModelInfo::modelName() const
{
    dPvt();
    return p.modelName;
}

void ModelInfo::setModelName(const QString &value)
{
    dPvt();
    p.modelName = value;
}

QString &ModelInfo::modelDescription() const
{
    dPvt();
    return p.modelDescription;
}

void ModelInfo::setModelDescription(const QString &value)
{
    dPvt();
    p.modelDescription = value;
}

const ModelInfo&ModelInfo::modelInfo(const QVariant &v)
{
    QByteArray className;

    switch (qTypeId(v)) {
    case QMetaType_QVariantHash:
        className=v.toHash().value(qsl("classname")).toByteArray();
        break;
    case QMetaType_QVariantMap:
        className=v.toHash().value(qsl("classname")).toByteArray();
        break;
    default:
        className=v.toByteArray();
    }

    if(__static_model_info.contains(className)){
        const auto&info=*__static_model_info.value(className);
        return info;
    }

    static ModelInfo info;
    return info;
}

const ModelInfo&ModelInfo::modelInfo(const QMetaObject &metaObject)
{
    return ModelInfo::modelInfo(metaObject.className());
}

ModelInfo &ModelInfo::modelInfoInit(const QMetaObject &staticMetaObject)
{
    return ModelInfoPvt::static_initMetaObject(staticMetaObject);
}

QByteArray ModelInfo::className() const
{
    dPvt();
    return p.staticMetaObject.className();
}

QMetaObject&ModelInfo::staticMetaObject() const
{
    dPvt();
    return p.staticMetaObject;
}

const QStringList &ModelInfo::propertyIgnoredList()
{
    return __propertyIgnoredList;
}

QList<QMetaProperty> &ModelInfo::property() const
{
    dPvt();
    return p.property;
}

QHash<QString, QMetaProperty> &ModelInfo::propertyByFieldName() const
{
    dPvt();
    return p.propertyByFieldName;
}

QMetaProperty ModelInfo::propertyByFieldName(const QString &fieldName) const
{
    dPvt();
    auto _fieldName=fieldName.trimmed().toLower();
    Q_V_PROPERTY_ITERATOR(p.propertyByFieldName){
        i.next();
        if(i.key().toLower()==_fieldName)
            return i.value();
    }
    const static QMetaProperty metaProperty;
    return metaProperty;
}

QHash<QString, QMetaProperty> &ModelInfo::propertyFiltrable() const
{
    dPvt();
    return p.propertyFiltrable;
}

QHash<QString, QMetaProperty> &ModelInfo::propertyByPropertyName() const
{
    dPvt();
    return p.propertyByPropertyName;
}

QMetaProperty ModelInfo::propertyByPropertyName(const QString &propertyName) const
{
    dPvt();
    auto property=p.propertyByPropertyName.value(propertyName);
    if(property.isValid())
        return property;

    const auto pName=propertyName.trimmed().toLower();
    Q_V_PROPERTY_ITERATOR(p.propertyByPropertyName){
        i.next();
        if(i.key().toLower()==pName)
            return i.value();
    }
    const static QMetaProperty metaProperty;
    return metaProperty;
}

QStringList &ModelInfo::propertyList() const
{
    dPvt();
    return p.propertyList;
}

QStringList &ModelInfo::propertyTableList() const
{
    dPvt();
    return p.propertyTableList;
}

QHash<QString, QString> &ModelInfo::propertyTableVsShort() const
{
    dPvt();
    return p.propertyTableVsShort;
}

QHash<QString, QString> &ModelInfo::propertyShortVsTable() const
{
    dPvt();
    return p.propertyShortVsTable;
}

QHash<QString, QMetaProperty> &ModelInfo::propertyInfo() const
{
    dPvt();
    return p.propertyInfo;
}

QHash<QString, QMetaProperty> &ModelInfo::propertyPK() const
{
    dPvt();
    return p.propertyPK;
}

QHash<QString, QMetaProperty> &ModelInfo::propertyFK() const
{
    dPvt();
    return p.propertyFK;
}

QVariantList ModelInfo::propertyDescriptors() const
{
    dPvt();
    return p.propertyDescriptors;
}

QVariantHash ModelInfo::propertySort() const
{
    dPvt();
    return p.propertySort;
}

QVariantHash ModelInfo::propertyParserToTable(const QVariant&v) const
{
    dPvt();
    QVVM vvm;
    auto&propertyList=p.propertyShortVsTable;
    auto vHash=v.toHash();
    Q_V_HASH_ITERATOR(vHash){
        i.next();
        auto k=i.key().trimmed();
        SqlParserItem ik;
        SqlParserItem iv;
        if(propertyList.contains(k))
            ik=SqlParserItem::createObject(propertyList[k]);
        else
            ik=SqlParserItem::createValue(k);

        if(QStmTypesVariantDictionary.contains(qTypeId(i.value())))
            iv=i.value();
        else if(propertyList.contains(v.toString()))
            iv=SqlParserItem::createObject(i.value());
        else
            iv=SqlParserItem::createValue(i.value());

        vvm.insert(ik, iv);
    }
    return QVariantHash(vvm);
}

QVariantHash ModelInfo::propertyParserToProperty(const QVariant&v) const
{
    dPvt();
    QVVM vvm;
    auto&propertyList=p.propertyTableVsShort;
    auto vHash=v.toHash();
    Q_V_HASH_ITERATOR(vHash){
        i.next();
        auto k=i.key().trimmed();
        SqlParserItem ik;
        SqlParserItem iv;
        if(propertyList.contains(k))
            ik=SqlParserItem::createObject(propertyList[k]);
        else
            ik=SqlParserItem::createValue(k);

        switch (qTypeId(i.value())) {
        case QMetaType_QVariantHash:
            iv=i.value();
            break;
        case QMetaType_QVariantMap:
            iv=i.value();
            break;
        default:
            if(propertyList.contains(v.toString()))
                iv=SqlParserItem::createObject(i.value());
            else
                iv=SqlParserItem::createValue(i.value());
            break;
        }
        vvm.insert(ik, iv);
    }
    return QVariantHash(vvm);
}

bool ModelInfo::propertyNameConvertMap(QVariantHash &propertyMap)
{
    dPvt();
    auto vHash=propertyMap;
    propertyMap.clear();
    Q_V_HASH_ITERATOR(vHash){
        i.next();
        const auto key=p.propertyShortVsTable.value(i.key()).toLower();
        if(key.isEmpty()){
            propertyMap.clear();
            return false;
        }

        auto value=vHash.value(i.key());
        propertyMap.insert(key, value);
    }
    return !propertyMap.isEmpty();
}

QVariantHash ModelInfo::propertyDeactivateField() const
{
    dPvt();
    return p.propertyDeactivateField;
}

QString &ModelInfo::tableSchema()const
{
    dPvt();
    return p.tableSchema;
}

QString &ModelInfo::tablePrefix()const
{
    dPvt();
    return p.tablePrefix;
}

QString ModelInfo::tablePrefixSeparator() const
{
    dPvt();
    return p.tablePrefixSeparator;
}

QString ModelInfo::tableName()const
{
    dPvt();
    return p.tableName;
}

QString ModelInfo::tableNameFull()const
{
    dPvt();
    return p.tableNameFull;
}

QVariantHash ModelInfo::tableSequence()const
{
    dPvt();
    return p.tableSequence;
}

QStringList ModelInfo::tablePk()const
{
    dPvt();
    return p.tablePk;
}

QVariantList ModelInfo::tablePkField() const
{
    dPvt();
    return p.tablePkField;
}

QVariantHash ModelInfo::tableForeignKey() const
{
    dPvt();
    return p.tableForeignKey;
}

QVariantList ModelInfo::tableFiltrableField() const
{
    dPvt();
    return p.tableFiltrableField;
}

QStringList ModelInfo::tableOrderBy() const
{
    dPvt();
    return p.tableOrderBy;
}

QVariantList ModelInfo::tableOrderByField() const
{
    dPvt();
    return p.tableOrderByField;
}

QVariantHash ModelInfo::tableDeactivateField() const
{
    dPvt();
    return p.tableDeactivateField;
}

bool ModelInfo::tablePkAutoGenerate()const
{
    dPvt();
    return p.tablePkAutoGenerate.toBool();
}

QString ModelInfo::tablePkSingle()const
{
    dPvt();
    return p.tablePkSingle;
}

QVariantMap ModelInfo::toMap(const QObject *object)const
{
    dPvt();
    QVariantMap __return;
    for(auto&property:p.property){
        if(!property.isValid())
            continue;
        auto propertyName = QString(property.name()).toLower();
        QVariant value;
        switch (qTypeId(property)){
        case QMetaType_User:
            value=property.read(object).toInt();
            break;
        case QMetaType_CustomType:
            value=property.read(object).toInt();
            break;
        default:
            value=value=property.read(object);
        }
        __return[propertyName]=value;
    }
    return __return;
}

QVariantHash ModelInfo::toHash(const QObject *object) const
{
    dPvt();
    QVariantHash __return;
    for(auto&property:p.property){
        if(!property.isValid())
            continue;
        auto propertyName = QString(property.name()).toLower();
        QVariant value;
        switch (qTypeId(property)){
        case QMetaType_User:
            value=property.read(object).toInt();
            break;
        case QMetaType_CustomType:
            value=property.read(object).toInt();
            break;
        default:
            value=value=property.read(object);
        }
        __return[propertyName]=value;
    }
    return __return;
}

QVariantMap ModelInfo::toMapModel(const QObject *object) const
{
    auto v=QVariant(this->toHashModel(object)).toMap();
    return v;
}

QVariantHash ModelInfo::toHashModel(const QObject *object)const
{
    dPvt();
    QVariantHash __return;
    for(auto&property:p.property){
        if(!property.isValid())
            continue;

        auto propertyName = QString(property.name()).toLower();
        propertyName=p.propertyShortVsTable.value(propertyName);
        __return.insert(propertyName, property.read(object));
    }
    return __return;
}

QVariantHash ModelInfo::toForeign(const QVariant &vModel,const QVariant &v) const
{
    dPvt();
    QVariantHash __return=vModel.toHash();
    auto vRecord=v;
    auto typeId=qTypeId(vRecord);
    if(QStmTypesVariantList.contains(typeId)){
        auto vList=vRecord.toList();
        vRecord = vList.isEmpty()?v:vList;
    }

    if(QStmTypesVariantDictionary.contains(typeId)){
        auto vRecordHash=vRecord.toHash();
        Q_V_HASH_ITERATOR(p.tableForeignKey){
            i.next();
            auto vMap=i.value().toHash();
            auto fkName=vMap[qsl("fk")].toString();
            auto pkName=vMap[qsl("pk")].toString();
            auto pkValue=vRecordHash.value(pkName);
            __return[fkName]=pkValue;
        }
        return __return;
    }

    Q_V_HASH_ITERATOR(p.tableForeignKey){
        i.next();
        auto vMap=i.value().toHash();
        auto fkName=vMap[qsl("fk")].toString();
        __return[fkName]=vRecord;
    }
    return __return;
}

}
