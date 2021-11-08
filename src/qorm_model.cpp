#include "./qorm_model.h"
#include "./qorm_query.h"
#include "./private/p_qorm_const.h"
#include "./private/p_qorm_query.h"
#include "./private/p_qorm_model_info.h"
#include <QCryptographicHash>
#include <QMetaProperty>
#include <QSqlField>
#include <QSqlError>
#include <QSqlRecord>
#include <QSqlQuery>
#include <QJsonDocument>
#include <QJsonObject>
#include <QBitArray>

namespace PrivateOrm {
Q_GLOBAL_STATIC_WITH_ARGS(QByteArray,__static_uuid_base_data,(QUuid::createUuid().toString().toUtf8()));
}

namespace QOrm {

static auto&__static_uuid_base_data =*PrivateOrm::__static_uuid_base_data;

#define dPvt()\
    auto&p = *reinterpret_cast<QOrm::ModelPvt*>(this->p)


typedef QMultiHash<QByteArray,QByteArray> WrapperMap;

class ModelPvt{
public:
    QMultiHash<QByteArray, QMultiHash<QByteArray,QByteArray>> mapWrapper;
    int cursorListIndex=-1;
    QVariantList*cursorList=nullptr;
    QHash<int, QMetaProperty> cursorFieldProperty ;
    QSqlRecord cursorRecord;
    QSqlQuery*cursorQuery=nullptr;
    QOrm::Model::Cursor*cursor=nullptr;
    QOrm::Model*model=nullptr;

    explicit ModelPvt(QOrm::Model*model){
        this->model=model;
    }
    virtual ~ModelPvt(){
        if(this->cursor!=nullptr)
            delete this->cursor;
        this->cursor=nullptr;
    }

    const ModelInfo&modelInfo(){
        const auto&className=this->model->metaObject()->className();
        return ModelInfo::modelInfo(className);
    }

    const QByteArray storedMd5Make(){
        const auto list=this->modelInfo().propertyList();
        auto&metaObject = *this->model->metaObject();
        QVariantHash vBody;
        for(int col = 0; col < metaObject.propertyCount(); ++col) {
            auto property = metaObject.property(col);
            if(__propertyIgnoredList.contains(property.name()))
                continue;
            else if(list.contains(property.name()))
                vBody[property.name()]=property.read(this->model);
        }
        return this->model->toMd5(vBody);
    }

    void clear(){
        auto metaObject=this->model->metaObject();
        for(int col = 0; col < metaObject->propertyCount(); ++col) {
            auto property = metaObject->property(col);
            if(__propertyIgnoredList.contains(property.name()))
                continue;

            if(property.typeId()>=QMetaType::User){
                this->write(property, 0);
                continue;
            }

            if(property.typeId()==QMetaType::QUuid){
                this->write(property, QUuid());
                continue;
            }

            if(property.typeId()==QMetaType::QUrl){
                this->write(property, QUrl());
                continue;
            }

            if(property.typeId()==QMetaType::QDate){
                this->write(property, QDate());
                continue;
            }

            if(property.typeId()==QMetaType::QTime){
                this->write(property, QTime());
                continue;
            }

            if(property.typeId()==QMetaType::QDateTime){
                this->write(property, QDateTime());
                continue;
            }

            if(property.typeId()==QMetaType::Double || property.typeId()==QMetaType::Int || property.typeId()==QMetaType::UInt || property.typeId()==QMetaType::LongLong || property.typeId()==QMetaType::ULongLong){
                this->write(property, 0);
                continue;
            }

            if(property.typeId()==QMetaType::QString || property.typeId()==QMetaType::QByteArray || property.typeId()==QMetaType::QChar || property.typeId()==QMetaType::QBitArray){
                this->write(property, qsl_null);
            }

            if( property.typeId()==QMetaType::QVariantMap || property.typeId()==QMetaType::QVariantHash){
                this->write(property, QVariantHash());
                continue;
            }

            if(property.typeId()==QMetaType::QVariantList || property.typeId()==QMetaType::QStringList){
                this->write(property, QVariantList());
                continue;
            }

            this->write(property, QVariant());
        }
    }

    QMetaObject metaObject()const {
        return*model->metaObject();
    }

    void cursorMakeFieldQuery(){
        if(this->cursorRecord.isEmpty()){
            this->cursorRecord=this->cursorQuery->record();
            for (int col1 = 0; col1 < this->cursorRecord.count(); ++col1) {
                auto fieldProperty=this->cursorRecord.field(col1);
                auto fieldName=fieldProperty.name().toLower().trimmed();
                auto&metaObject = *this->model->metaObject();
                for(int col2 = 0; col2 < metaObject.propertyCount(); ++col2) {
                    auto metaProperty = metaObject.property(col2);
                    auto propertyName=QByteArray(metaProperty.name()).toLower().trimmed();
                    if(propertyName==fieldName){
                        cursorFieldProperty.insert(col2, metaProperty);
                        break;
                    }
                }
            }
        }
    }

    bool write(const QMetaProperty&property, const QVariant&value){
        auto type = property.typeId();

        QVariant vValue;

        VariantUtil vu;

        if(type==QMetaType::QUuid && value.typeId()!=type)
            vValue=vu.toUuid(value);
        else
            vValue=value;

        if(property.write(this->model, vValue))
            return true;

        if(QStmTypesListString.contains(type)){
            QVariant v;
            if(QStmTypesListObjects.contains(vValue.typeId()))
                v=QJsonDocument::fromVariant(vValue).toJson(QJsonDocument::Compact);
            else if(type==QMetaType::QUuid)
                v=vu.toUuid(vValue);
            else if(type==QMetaType::QUrl)
                v=vu.toUrl(vValue);
            else
                v=vValue;

            if((property.typeId()>=QMetaType::User) && (property.write(this->model, vValue.toInt())))
                return true;
            if((type==QMetaType::QUuid) && (property.write(this->model, vValue.toUuid())))
                return true;
            if((type==QMetaType::QString) && (property.write(this->model, v.toString())))
                return true;
            if((type==QMetaType::QByteArray) && (property.write(this->model, v.toByteArray())))
                return true;
            if((type==QMetaType::QChar) && (property.write(this->model, v.toChar())))
                return true;
            if((type==QMetaType::QBitArray) && (property.write(this->model, v.toBitArray())))
                return true;
        }
        else if(QStmTypesListNumeric.contains(type)){//ints
            if((type==QMetaType::LongLong) && (property.write(this->model, static_cast<qlonglong>(QLocale::c().toDouble(vValue.toString())))))
                return true;
            if((type==QMetaType::Int) && (property.write(this->model, QLocale::c().toInt(vValue.toString()))))
                return true;
            if((type==QMetaType::UInt) && (property.write(this->model, QLocale::c().toInt(vValue.toString()))))
                return true;
            if((type==QMetaType::UInt) && (property.write(this->model, QLocale::c().toUInt(vValue.toString()))))
                return true;
            if((type==QMetaType::ULongLong || type==QMetaType::LongLong) && (property.write(this->model, QLocale::c().toLongLong(vValue.toString()))))
                return true;
            if((type==QMetaType::Double) && (property.write(this->model, QLocale::c().toDouble(vValue.toString()))))
                return true;
        }
        else if(QStmTypesListClass.contains(type)){
            if((type==QMetaType::QUrl) && (property.write(this->model, vValue.toUrl())))
                return true;
            if((type==QMetaType::QVariantMap) && (property.write(this->model, vValue.toHash())))
                return true;
            if((type==QMetaType::QVariantHash) && (property.write(this->model, vValue.toHash())))
                return true;
            if((type==QMetaType::QVariantList) && (property.write(this->model, vValue.toList())))
                return true;
            if((type==QMetaType::QStringList) && (property.write(this->model, vValue.toStringList())))
                return true;
        }
        else if(QStmTypesListDates.contains(type)){
            if((type==QMetaType::QDate) && (property.write(this->model, vValue.toDate())))
                return true;
            if((type==QMetaType::QDateTime) && (property.write(this->model, vValue.toDateTime())))
                return true;
            if((type==QMetaType::QTime) && (property.write(this->model, vValue.toTime())))
                return true;
        }
        else if(QStmTypesListBool.contains(type) || QStmTypesListBool.contains(vValue.typeId())){
            auto&v=vValue;
            if((type==QMetaType::Bool) && (property.write(this->model, vValue.toBool())))
                return true;

            if(v.typeId()==QMetaType::Bool){
                if(v.typeId()==QMetaType::Bool && property.write(this->model, vValue.toBool()))
                    return true;

                if(v.typeId()==QMetaType::Int || v.typeId()==QMetaType::UInt || v.typeId()==QMetaType::ULongLong || v.typeId()==QMetaType::LongLong || v.typeId()==QMetaType::Double){
                    if(property.write(this->model, (vValue.toInt()==1)))
                        return true;
                }

                if(v.typeId()==QMetaType::QString || v.typeId()==QMetaType::QByteArray || v.typeId()==QMetaType::QChar){
                    auto vv=vValue.toString().toLower();
                    bool vBool=(vv==qsl("true"));
                    if(property.write(this->model, vBool))
                        return true;
                }
            }
        }
        else if(QStmTypesListObjects.contains(type)){
            vValue=vu.toVariantObject(vValue);
            if(property.write(this->model, vValue.toDate()))
                return true;
        }
        return false;
    }

    auto&read(const QVariant&vRecord){
        this->clear();
        this->model->lr().clear();
        const auto&modelInfo = this->modelInfo();
        auto prefix = modelInfo.tablePrefix();

        QVariant record;
        if(vRecord.typeId()!=QMetaType::QVariantList)
            record=vRecord;
        else{
            auto vList=vRecord.toList();
            auto v=vList.isEmpty()?QVariant():vList.first();
            if(record.typeId()==QMetaType::QVariantMap || record.typeId()==QMetaType::QVariantHash)//check object
                record=v;
            else
                record=vRecord;
        }
        if(record.isValid()){
            if(record.typeId()==QMetaType::QVariantMap || record.typeId()==QMetaType::QVariantHash){
                VariantUtil u;
                auto recordMap=record.toHash();
                if(recordMap.isEmpty())
                    return this->model->lr()=false;

                for(auto&property:modelInfo.property()){
                    auto propertyName=QByteArray(property.name()).toLower().trimmed();
                    QHashIterator<QString, QVariant> i(recordMap);
                    while (i.hasNext()) {
                        i.next();
                        auto valueName=i.key().toLower().trimmed();
                        if((valueName==propertyName) || valueName==prefix+propertyName){
                            auto&value=i.value();
                            if(!this->write(property, value))
                                this->model->lr()=false;
                            break;
                        }
                    }
                }
            }
            else {
                auto tablePk = this->modelInfo().tablePk();
                if(tablePk.size()==1){
                    auto name=tablePk.first().trimmed();
                    if(name.startsWith(prefix)){
                        name=name.split(prefix).last();
                    }
                    if(record.typeId()==QMetaType::QUuid){
                        auto uuid=record.toUuid();
                        if(!this->model->setProperty(name.toUtf8(), uuid)){
                            sWarning()<<qsl("no setProperty('%1', %2)").arg(name, record.toString());
                        }
                    }
                    else if(!this->model->setProperty(name.toUtf8(), record)){
                        sWarning()<<qsl("no setProperty('%1', %2)").arg(name, record.toString());
                    }
                }
            }

        }
        return this->model->lr();
    }

    auto&read(const QByteArray&record){
        this->model->lr()=true;
        auto vMap=QJsonDocument::fromJson(record).object().toVariantHash();
        return this->read(vMap);
    }

    auto&read(QSqlQuery&sqlQuery){
        QVariantHash record;
        auto sqlRecord=sqlQuery.record();
        for (int col = 0; col < sqlRecord.count(); ++col)
            record.insert(sqlRecord.fieldName(col), sqlRecord.value(col));
        return this->model->lr()=this->read(record);
    }

    ResultValue&merge(QSqlQuery&cursorQuery){
        QVariantHash record;
        auto sqlRecord=cursorQuery.record();
        for (int col = 0; col < sqlRecord.count(); ++col)
            record.insert(sqlRecord.fieldName(col), sqlRecord.value(col));
        return this->merge(record);
    }

    ResultValue&merge(const QVariantHash&record){

        auto metaObject = this->model->metaObject();
        auto prefix=this->modelInfo().tablePrefix();

        for(int col = 0; col < metaObject->propertyCount(); ++col) {
            auto property = metaObject->property(col);
            if(__propertyIgnoredList.contains(property.name()))
                continue;

            //auto value=record.value(property.name());
            auto propertyName=QByteArray(property.name()).toLower().trimmed();
            QHashIterator<QString, QVariant> i(record);
            while (i.hasNext()) {
                i.next();
                auto valueName=i.key().toLower().trimmed();
                if((valueName==propertyName) || valueName==prefix+propertyName){
                    if(!this->write(property, i.value())){
                        return this->model->lr().setValidation(QObject::tr("Invalid data model"));
                    }
                    break;
                }
            }
        }
        return this->model->lr();
    }

    const QVariantList toWrapper(const QString&wrapperName, const QVariant&v)
    {                
        auto name=wrapperName.toLower().trimmed().toUtf8();
        const auto&mapWrapper=this->mapWrapper.value(name);
        QVariantList vList;
        QVariantList rList;
        if(v.typeId()==QMetaType::QVariantMap || v.typeId()==QMetaType::QVariantHash)
            vList<<v.toHash();
        else if(v.typeId()==QMetaType::QVariantList || v.typeId()==QMetaType::QStringList)
            vList=v.toList();

        for(auto&v:vList){
            auto record=v.toHash();
            if(record.isEmpty())
                continue;
            
            const auto&modelInfo = this->modelInfo();
            QVariantHash rMap;
            QMultiHashIterator<QByteArray, QByteArray> i(mapWrapper);
            while (i.hasNext()) {
                i.next();
                auto&k=modelInfo.tablePrefix()+i.key();
                auto&s=i.value();
                auto v=record.value(k);
                if(v.isValid()){
                    if(v.typeId()==QMetaType::QUuid)
                        rMap.insert(s,v.toString());
                    else if(v.typeId()==QMetaType::QUrl)
                        rMap.insert(s,v.toString());
                    else if(v.typeId()>=QMetaType::User)
                        rMap.insert(s,v.toInt());
                    else
                        rMap.insert(s,v);
                }
                else{
                    auto&k=i.key();
                    auto v=record.value(k);
                    if(v.typeId()==QMetaType::QUuid)
                        rMap.insert(s,v.toString());
                    else if(v.typeId()==QMetaType::QUrl)
                        rMap.insert(s,v.toString());
                    else if(v.typeId()>=QMetaType::User)
                        rMap.insert(s,v.toInt());
                    else
                        rMap.insert(s,v);
                }
            }
            rList<<rMap;
        }
        return rList;
    }

    QVariantHash wrapperToModelMap(const QString&wrapperName, const QVariant&v)const{
        auto vWrapper=v.toHash();
        QVariantHash vMap;
        const auto name=wrapperName.toLower().trimmed().toUtf8();
        const auto &mapWrapper = this->mapWrapper.value(name);
        QHashIterator<QString,QVariant> iO(vWrapper);
        while (iO.hasNext()) {
            iO.next();
            auto atualKey   = iO.key().toLower().toUtf8();
            auto wrapperKey = mapWrapper.key(atualKey);
            auto novaKey    = (!wrapperKey.isEmpty())?wrapperKey:atualKey;
            vMap.insert(novaKey, iO.value());
        }
        return vMap;
    }

    ResultValue&fromWrapper(const QString&wrapperName, const QVariant&vWrapper)const{
        auto vMap=this->wrapperToModelMap(wrapperName, vWrapper);
        return this->model->readFrom(vMap);
    }

    void cursorClear(){
        if(this->cursor!=nullptr){
            delete this->cursor;
            this->cursor=nullptr;
        }
        this->cursorQuery=nullptr;
        this->cursorList=nullptr;
        this->cursorReset();
    }

    void cursorMake(){
        this->cursor = new QOrm::Model::Cursor(this);
        this->cursorReset();
    }

    void cursorReset(){
        if(this->cursorQuery!=nullptr)
            this->cursorQuery->first();
        this->cursorListIndex=-1;
        this->cursorRecord=QSqlRecord();
        this->cursorFieldProperty.clear();
    }

    bool next(){
        if(this->cursorList!=nullptr){
            if(this->cursorListIndex>=0 && (cursorListIndex<this->cursorList->size()) ){
                auto&v=this->cursorList->at(this->cursorListIndex);
                ++this->cursorListIndex;
                return this->read(v);
            }
        }
        else if(this->cursorQuery!=nullptr){
            if(this->cursorQuery->next()){
                this->cursorMakeFieldQuery();
                QHashIterator<int, QMetaProperty> i(this->cursorFieldProperty);
                while (i.hasNext()) {
                    i.next();
                    auto value = this->cursorQuery->value(i.key());
                    if(i.value().write(this->model, value))
                        continue;

                    if(this->write(i.value(), value))
                        continue;

                    return false;
                }
                return true;
            }
        }
        this->cursorListIndex=0;
        this->cursorRecord=QSqlRecord();
        this->cursorFieldProperty.clear();
        return false;
    }
};

Model::Cursor::Cursor(void *p){
    this->p = p;
}

Model::Cursor::~Cursor(){

}

bool Model::Cursor::next()
{
    dPvt();
    return p.next();
}

Model::Model(QObject *parent):QStm::Object(parent)
{
    this->p=new ModelPvt(this);
}

Model::~Model()
{
    dPvt();delete&p;
}

Model &Model::clear()
{
    dPvt();
    p.clear();
    return*this;
}

QByteArray Model::storedMd5Make() const
{
    dPvt();
    return p.storedMd5Make();
}

bool Model::isModifier(const QVariantHash &vMap)
{
    auto tMap=this->toHash();
    QHashIterator<QString, QVariant> i(vMap);
    while (i.hasNext()) {
        i.next();
        auto&k=i.key();
        auto v0=i.value().toString().trimmed();
        auto v1=tMap.value(k).toString().trimmed();
        if(v0!=v1)
            return true;
    }
    return false;
}

QVariantList Model::toList(ResultValue &vList)
{
    return this->toList(vList.resultList());
}

QVariantList Model::toList(const QVariantList &vList)
{
    QVariantList rList;
    for(const auto&v:vList){
        if(this->readFrom(v))
            rList<<this->toHash();
    }
    return rList;
}

QVariantMap Model::toMap()const
{
    dPvt();
    return p.modelInfo().toMap(this);
}

QVariantHash Model::toHash() const
{
    dPvt();
    return p.modelInfo().toHash(this);
}

QVariantHash Model::toHashModel() const
{
    dPvt();
    auto vMap=p.modelInfo().toHashModel(this);
    return vMap;
}

QVariantHash Model::toMapPKValues() const
{
    dPvt();
    QVVM map;
    auto&modelInfo=p.modelInfo();
    const auto&propertyShortVsTable=modelInfo.propertyShortVsTable();
    auto pList=modelInfo.propertyPK().values();
    for(auto&property:pList){
        if(property.isValid()){
            auto fieldName=propertyShortVsTable[property.name()].trimmed();
            if(!fieldName.isEmpty()){
                const auto k=SqlParserItem::createObject(fieldName);
                const auto v=SqlParserItem::createValue(property.read(this));
                map.insert(k,v);
            }
        }
    }
    return QVariant(map).toHash();
}

QVariantHash Model::toMapFKValues() const
{
    dPvt();
    QVVM map;
    auto&modelInfo=p.modelInfo();
    const auto&propertyShortVsTable=modelInfo.propertyShortVsTable();
    auto pList=modelInfo.propertyFK().values();
    for(auto&property:pList){
        if(property.isValid()){
            auto fieldName=propertyShortVsTable[property.name()].trimmed();
            if(!fieldName.isEmpty()){
                const auto k=SqlParserItem::createObject(fieldName);
                const auto v=SqlParserItem::createValue(property.read(this));
                map.insert(k,v);
            }
        }
    }
    return QVariant(map).toHash();
}

QVariantHash Model::toWrapper() const
{
    return this->toWrapper(qsl_null);
}

QVariantHash Model::toWrapper(const QString &wrapperName) const
{
    dPvt();
    auto map=this->toHashModel();
    if(map.isEmpty())
        return map;
    else{
        auto l=p.toWrapper(wrapperName, map);
        return l.isEmpty()?QVariantHash():l.first().toHash();
    }
}

QVariantList Model::toWrapper(const QString &wrapperName, const QVariantList &v) const
{
    dPvt();
    return p.toWrapper(wrapperName, v);
}


QVariantList Model::toWrapper(const QString &wrapperName, const QVariantHash &v) const
{
    dPvt();
    return p.toWrapper(wrapperName, v);
}

QVariantList Model::toWrapper(const QString &wrapperName, ResultValue &v) const
{
    dPvt();
    return p.toWrapper(wrapperName.trimmed(), v.resultVariant());
}

QVariantList Model::toWrapper(const QVariantList &v) const
{
    dPvt();
    return p.toWrapper(qsl_null, v);
}

QVariantList Model::toWrapper(const QVariantHash &v) const
{
    dPvt();
    return p.toWrapper(qsl_null, v);
}

QVariantList Model::toWrapper(ResultValue &v) const
{
    dPvt();
    return p.toWrapper(qsl_null, v.resultVariant());
}

bool Model::addWrapper(const QString &wrapperName, const QString &src, const QString &dst)
{
    dPvt();
    auto name=wrapperName.toLower().trimmed().toUtf8();
    auto mapWrapper=p.mapWrapper.value(name);
    auto v1=src.toUtf8().replace(qbl("\""), qbl_null).trimmed();
    auto v2=dst.toUtf8().replace(qbl("\""), qbl_null).trimmed();
    mapWrapper.insert(v1, v2);
    p.mapWrapper.insert(name,mapWrapper);
    return true;
}

bool Model::writeToWrapper(QVariantHash &wrapper) const
{
    QHashIterator<QString, QVariant> i(this->toWrapper());
    while (i.hasNext()) {
        i.next();
        wrapper.insert(i.key(),i.value());
    }
    return true;
}

bool Model::appendToWrapper(QVariantHash &wrapper)const
{
    auto fromWrapper=QString();
    auto toWrapper=this->modelName();
    return this->appendToWrapper(wrapper, fromWrapper, toWrapper);
}

bool Model::appendToWrapper(QVariantHash &wrapper, const QString &fromWrapper)const
{
    auto toWrapper=this->modelName();
    return this->appendToWrapper(wrapper, fromWrapper, toWrapper);
}

bool Model::appendToWrapper(QVariantHash &wrapper, const QString &fromWrapper, const QString &toWrapper)const
{
    //auto _toWrapper=toWrapper.trimmed().isEmpty()?this->modelName():toWrapper;
    auto v=this->toWrapper(fromWrapper);
    if(!v.isEmpty()){
        wrapper.insert(toWrapper, v);
        return true;
    }
    else{
        return false;
    }
}

bool Model::isWrapperModifier(const QVariantHash &wrapper, const QString &fromWrapper)
{
    dPvt();
    auto tMap=p.wrapperToModelMap(fromWrapper, wrapper);
    return this->isModifier(tMap);
}

bool Model::isWrapperModifierSet(const QVariantHash &wrapper)
{
    return this->isWrapperModifierSet(wrapper, qsl_null);
}

bool Model::isWrapperModifierSet(const QVariantHash &wrapper, const QString &fromWrapper)
{
    dPvt();
    auto tMap=p.wrapperToModelMap(fromWrapper, wrapper);
    if(this->isModifier(tMap)){
        return this->readFrom(tMap);
    }
    return false;
}

bool Model::propertyBeforeSet(const QByteArray &property, const QVariant &value) const
{
    Q_UNUSED(property)
    Q_UNUSED(value)
    return true;
}

bool Model::propertyAfterSet(const QByteArray &property, const QVariant &value) const
{
    Q_UNUSED(property)
    Q_UNUSED(value)
    return true;
}

void Model::setCursor(QVariantList &cursor)
{
    dPvt();
    if(p.cursorList!=&cursor){
        p.cursorClear();
        p.cursorList=&cursor;
        p.cursorMake();
    }
}

void Model::setCursor(QSqlQuery &cursor)
{
    dPvt();
    if(p.cursorQuery!=&cursor){
        p.cursorClear();
        p.cursorQuery=&cursor;
        p.cursorMake();

    }
}

void Model::setCursor(Query &cursor)
{
    auto&p=*static_cast<QueryPvt*>(cursor.p);
    return this->setCursor(p.sqlQuery);
}

Model::Cursor &Model::cursor()
{
    dPvt();
    return*p.cursor;
}

ResultValue &Model::readFrom(const QVariant &record)
{
    dPvt();
    return p.read(record);
}

ResultValue &Model::readFrom(QSqlQuery &record)
{
    dPvt();
    return p.read(record);
}

ResultValue &Model::readFrom(Query &record)
{
    auto&p=*static_cast<QueryPvt*>(record.p);
    return this->readFrom(p.sqlQuery);
}

ResultValue &Model::readFrom(const ResultValue &resultValue)
{
    dPvt();
    return p.read(resultValue.resultVariant());
}

ResultValue &Model::readFrom(const Model &model)
{
    auto map=model.toHash();
    return this->readFrom(map);
}

ResultValue &Model::readWrapper(const QString &wrapper, const QVariant &vMap)
{
    dPvt();
    return p.fromWrapper(wrapper, vMap.toHash());
}

ResultValue &Model::readWrapper(ResultValue &resultValue)
{
    dPvt();
    return p.fromWrapper(qsl_null, resultValue.resultMap());
}

ResultValue &Model::readWrapper(const QVariant &vMap)
{
    dPvt();
    return p.fromWrapper(qsl_null, vMap.toHash());
}

ResultValue &Model::readWrapper(const QString &wrapper, ResultValue &resultValue)
{
    dPvt();
    return p.fromWrapper(wrapper, resultValue.resultMap());
}

ResultValue &Model::mergeFrom(const QByteArray &record)
{
    dPvt();
    return p.merge(QJsonDocument::fromJson(record).toVariant().toHash());
}

ResultValue &Model::mergeFrom(const QVariantHash &record)
{
    dPvt();
    return p.merge(record);
}

ResultValue &Model::mergeFrom(const QVariant &record)
{
    dPvt();
    return p.merge(record.toHash());
}

ResultValue &Model::mergeFrom(QSqlQuery &record)
{
    dPvt();
    return p.merge(record);
}

ResultValue &Model::mergeFrom(ResultValue &resultValue)
{
    dPvt();
    return p.merge(resultValue.resultMap());
}

bool Model::setProperty(const char *name, const QVariant &value)
{
    dPvt();
    auto index=this->metaObject()->indexOfProperty(name);
    if(index>=0){
        auto property=this->metaObject()->property(index);
        if(property.isValid()){
            return p.write(property, value);
        }
    }
    return false;
}

bool Model::setProperty(const QMetaProperty &property, const QVariant &value)
{
    dPvt();
    return p.write(property, value);
}

ResultValue &Model::uuidSet()
{
    dPvt();
    auto&modelInfo=p.modelInfo();
    Q_V_PROPERTY_ITERATOR(modelInfo.propertyPK()){
        i.next();
        auto&property=i.value();
        if(property.typeId()==QMetaType::QUuid){
            auto v=property.read(this);
            if(v.isNull() || !v.isValid()){
                property.write(this, this->uuidGenerator());
            }
        }
    }
    return this->lr()=true;
}

ResultValue &Model::deactivateSetValues()
{
    dPvt();
    auto propertyDeactivateField=p.modelInfo().propertyDeactivateField();
    if(propertyDeactivateField.isEmpty())
        return this->lr()=false;
    else{
        Q_V_HASH_ITERATOR(propertyDeactivateField){
            i.next();
            const auto k=i.key().toUtf8();
            const auto&v=i.value();
            if(!this->setProperty(k, v)){
                return this->lr().setValidation(tr("Invalid data to define in the model as deleted"));
            }
        }
        return this->lr();
    }
}

ResultValue &Model::datetimeSet()
{
    dPvt();
    auto&modelInfo=p.modelInfo();
    Q_V_PROPERTY_ITERATOR(modelInfo.propertyPK()){
        i.next();
        auto&property=i.value();
        auto v=property.read(this);
        if(property.typeId()==QMetaType::QDateTime){
            auto dt=v.toDateTime();
            if(dt.isNull() || !v.isValid()){
                property.write(this, QDateTime::currentDateTime());
            }
        }
        else if(property.typeId()==QMetaType::QDate){
            auto dt=v.toDate();
            if(dt.isNull() || !v.isValid()){
                property.write(this, QDate::currentDate());
            }
        }
        else if(property.typeId()==QMetaType::QTime){
            auto dt=v.toTime();
            if(dt.isNull() || !v.isValid()){
                property.write(this, QTime::currentTime());
            }
        }
    }
    return this->lr()=true;
}

ResultValue &Model::isValid()
{
    auto pk=this->modelInfo().tablePk();
    for(const auto&s:pk){
        auto v=this->property(s.toUtf8());
        if(v.isNull() || !v.isValid())
            return this->lr()=false;
    }
    return this->lr()=true;
}

ResultValue &Model::isActive()
{
    return this->lr();
}

ResultValue &Model::isEmptyPK()
{
    auto pk=this->modelInfo().tablePk();
    for(const auto&s:pk){
        auto v=this->property(s.toUtf8());
        if(v.isNull() || !v.isValid())
            return this->lr()=true;
    }
    return this->lr()=false;
}

bool Model::beforePost()const
{
    return true;
}

bool Model::afterPost()const
{
    return true;
}

Model &Model::operator=(const QVariant &v)
{
    this->readFrom(v);
    return*this;
}

}
