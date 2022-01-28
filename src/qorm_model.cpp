#include "./qorm_model.h"
#include "./qorm_query.h"
#include "./private/p_qorm_const.h"
#include "./private/p_qorm_query.h"
#include "./private/p_qorm_model_info.h"
#include <QCryptographicHash>
#include <QMetaProperty>
#include <QtSql/QSqlField>
#include <QtSql/QSqlError>
#include <QtSql/QSqlRecord>
#include <QtSql/QSqlQuery>
#include <QJsonDocument>
#include <QJsonObject>
#include <QBitArray>

namespace PrivateOrm {
Q_GLOBAL_STATIC_WITH_ARGS(QByteArray,__static_uuid_base_data,(QUuid::createUuid().toString().toUtf8()))
}

namespace QOrm {

static auto&__static_uuid_base_data =*PrivateOrm::__static_uuid_base_data;

#define dPvt()\
    auto&p = *reinterpret_cast<QOrm::ModelPvt*>(this->p)


typedef QMultiHash<QByteArray,QByteArray> WrapperMap;

class ModelPvt{
public:
    QMultiHash<QByteArray, QMultiHash<QByteArray,QByteArray>> mapWrapper;
    QOrm::Model*model=nullptr;

    explicit ModelPvt(QOrm::Model*model)
    {
        this->model=model;
    }

    virtual ~ModelPvt()
    {
    }

    const ModelInfo&modelInfo()
    {
        const auto&className=this->model->metaObject()->className();
        return ModelInfo::modelInfo(className);
    }

    const QByteArray storedMd5Make()
    {
        const auto list=this->modelInfo().propertyList();
        auto&metaObject = *this->model->metaObject();
        QVariantHash vBody;
        for(int col = 0; col < metaObject.propertyCount(); ++col) {
            auto property = metaObject.property(col);
            if(__propertyIgnoredList.contains(property.name()))
                continue;

            if(list.contains(property.name()))
                vBody[property.name()]=property.read(this->model);
        }
        return this->model->toMd5(vBody);
    }

    void clear()
    {
        auto metaObject=this->model->metaObject();
        for(int col = 0; col < metaObject->propertyCount(); ++col) {
            auto property = metaObject->property(col);
            if(__propertyIgnoredList.contains(property.name()))
                continue;

            switch (qTypeId(property)) {
                case QMetaType_User:
                    this->write(property, 0);
                    continue;
                case QMetaType_QUuid:
                    this->write(property, QUuid());
                    continue;
                case QMetaType_QUrl:
                    this->write(property, QUrl());
                    continue;
                case QMetaType_QDate:
                    this->write(property, QDate());
                    continue;
                case QMetaType_QTime:
                    this->write(property, QTime());
                    continue;
                case QMetaType_QDateTime:
                    this->write(property, QDateTime());
                    continue;
                case QMetaType_Double:
                case QMetaType_Int:
                case QMetaType_UInt:
                case QMetaType_LongLong:
                case QMetaType_ULongLong:
                    this->write(property, 0);
                    continue;
                case QMetaType_QString:
                    this->write(property, qsl_null);
                    continue;
                case QMetaType_QByteArray:
                    this->write(property, qbl_null);
                    continue;
                case QMetaType_QChar:
                    this->write(property, QChar());
                    continue;
                case QMetaType_QBitArray:
                    this->write(property, QBitArray());
                    continue;
                case QMetaType_QVariantMap:
                    this->write(property, QVariantMap());
                    continue;
                case QMetaType_QVariantHash:
                    this->write(property, QVariantHash());
                    continue;
                case QMetaType_QVariantList:
                    this->write(property, QVariantList());
                    continue;
                case QMetaType_QStringList:
                    this->write(property, QStringList());
                    continue;
            default:
                this->write(property, QVariant());
            }
        }
    }

    QMetaObject metaObject()const
    {
        return*model->metaObject();
    }

    bool write(const QMetaProperty&property, const QVariant&value)
    {
        Q_DECLARE_VU;

        QVariant vValue;
        auto typeIdp = qTypeId(property);
        if(typeIdp==QMetaType_QUuid && qTypeId(value)!=typeIdp)
            vValue=vu.toUuid(value);
        else
            vValue=value;

        if(property.write(this->model, vValue))
            return true;

        auto typeIdv=qTypeId(vValue);
        switch (typeIdp) {
        case QMetaType_QString:
        case QMetaType_QByteArray:
        case QMetaType_QChar:
        case QMetaType_QBitArray:
        {
            QVariant v;
            switch (typeIdv) {
            case QMetaType_QVariantMap:
            case QMetaType_QVariantHash:
            case QMetaType_QVariantList:
            case QMetaType_QStringList:
                v=QJsonDocument::fromVariant(vValue).toJson(QJsonDocument::Compact);
                break;
            case QMetaType_QUuid:
                v=vu.toUuid(vValue);
                break;
            case QMetaType_QUrl:
                v=vu.toUrl(vValue);
                break;
            default:
                v=vValue;
                break;
            }

            switch (typeIdp) {
            case QMetaType_QUuid:
            {
                if(property.write(this->model, vValue.toUuid()))
                    return true;
                break;
            }
            case QMetaType_QString:
            {
                if(property.write(this->model, v.toString()))
                    return true;
                break;
            }
            case QMetaType_QByteArray:
            {
                if(property.write(this->model, v.toByteArray()))
                    return true;
                break;
            }
            case QMetaType_QChar:
            {
                if(property.write(this->model, v.toChar()))
                    return true;
                break;
            }
            case QMetaType_QBitArray:
            {
                if(property.write(this->model, v.toBitArray()))
                    return true;
                break;
            }
            default:
                if((typeIdp>=QMetaType_User) && (property.write(this->model, vValue.toInt())))
                    return true;
            }
            return false;
        }
        case QMetaType_LongLong:
        {
            if(property.write(this->model, QLocale::c().toLongLong(vValue.toString())))
                return true;
            break;
        }
        case QMetaType_ULongLong:
        {
            if(property.write(this->model, static_cast<qlonglong>(QLocale::c().toULongLong(vValue.toString()))))
                return true;
            break;
        }
        case QMetaType_Int:
        {
            if(property.write(this->model, QLocale::c().toInt(vValue.toString())))
                return true;
            break;
        }
        case QMetaType_UInt:
        {
            if(property.write(this->model, QLocale::c().toUInt(vValue.toString())))
                return true;
            break;
        }
        case QMetaType_Double:
        {
            if(property.write(this->model, QLocale::c().toDouble(vValue.toString())))
                return true;
            return false;
        }
        case QMetaType_QUrl:
        {
            if(property.write(this->model, vValue.toUrl()))
                return true;
            break;
        }
        case QMetaType_QUuid:
        {
            if(property.write(this->model, vValue.toUuid()))
                return true;
            break;
        }
        case QMetaType_QVariantHash:
        {
            if(property.write(this->model, vValue.toHash()))
                return true;
            break;
        }
        case QMetaType_QVariantMap:
        {
            if(property.write(this->model, vValue.toMap()))
                return true;
            break;
        }
        case QMetaType_QVariantList:
        {
            if(property.write(this->model, vValue.toList()))
                return true;
            break;
        }
        case QMetaType_QStringList:
        {
            if(property.write(this->model, vValue.toStringList()))
                return true;
            break;
        }
        case QMetaType_QDate:
        {
            if(property.write(this->model, vValue.toDate()))
                return true;
            break;
        }
        case QMetaType_QDateTime:
        {
            if(property.write(this->model, vValue.toDateTime()))
                return true;
            break;
        }
        case QMetaType_QTime:
        {
            if(property.write(this->model, vValue.toTime()))
                return true;
            break;
        }
        default:
            if((typeIdp==QMetaType_Bool) && (property.write(this->model, vValue.toBool())))
                return true;

            if(typeIdv==QMetaType_Bool){
                if(property.write(this->model, vValue.toBool()))
                    return true;

                switch (typeIdv) {
                case QMetaType_Int:
                case QMetaType_UInt:
                case QMetaType_ULongLong:
                case QMetaType_LongLong:
                case QMetaType_Double:
                {
                    if(property.write(this->model, (vValue.toInt()==1)))
                        return true;
                    break;
                }
                case QMetaType_QString:
                case QMetaType_QByteArray:
                case QMetaType_QChar:
                {
                    auto vv=vValue.toString().toLower();
                    bool vBool=(vv==qsl("true"));
                    if(property.write(this->model, vBool))
                        return true;
                    break;
                }
                default:
                    break;
                }
            }
            return false;
            break;
        }
        return false;
    }

    auto&read(const QVariant&vRecord)
    {
        this->clear();
        this->model->lr().clear();
        const auto&modelInfo = this->modelInfo();
        auto prefix = modelInfo.tablePrefix();

        QVariant record;
        switch (qTypeId(vRecord)) {
        case QMetaType_QVariantList:
        {
            auto vList=vRecord.toList();
            auto v=vList.isEmpty()?QVariant():vList.first();
            //check object
            switch (qTypeId(v)) {
            case QMetaType_QVariantHash:
            case QMetaType_QVariantMap:
                record=v;
                break;
            default:
                record=QVariant();
            }
            break;
        }
        default:
            record=vRecord;
        }

        if(!record.isValid())
            return this->model->lr();

        switch (qTypeId(record)) {
        case QMetaType_QVariantHash:
        case QMetaType_QVariantMap:
        {
            Q_DECLARE_VU;
            auto recordHash=record.toHash();
            if(recordHash.isEmpty())
                return this->model->lr()=false;

            for(auto&property:modelInfo.property()){
                auto propertyName=QByteArray(property.name()).toLower().trimmed();
                Q_V_HASH_ITERATOR (recordHash){
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
            return this->model->lr();
        }
        default:
            auto tablePk = this->modelInfo().tablePk();
            if(tablePk.size()!=1)
                break;

            auto name=tablePk.first().trimmed();
            if(name.startsWith(prefix))
                name=name.split(prefix).last();

            switch (qTypeId(record)) {
            case QMetaType_QUuid:
            {
                auto uuid=record.toUuid();
                if(!this->model->setProperty(name.toUtf8(), uuid))
                    sWarning()<<qsl("no setProperty('%1', %2)").arg(name, record.toString());
                break;
            }
            default:
                if(!this->model->setProperty(name.toUtf8(), record))
                    sWarning()<<qsl("no setProperty('%1', %2)").arg(name, record.toString());
            }
        }

        return this->model->lr();
    }

    auto&read(const QByteArray&record)
    {
        this->model->lr()=true;
        auto v=QJsonDocument::fromJson(record).object().toVariantHash();
        return this->read(v);
    }

    auto&read(QSqlQuery&sqlQuery)
    {
        QVariantHash record;
        auto sqlRecord=sqlQuery.record();
        for (int col = 0; col < sqlRecord.count(); ++col)
            record.insert(sqlRecord.fieldName(col), sqlRecord.value(col));
        return this->model->lr()=this->read(record);
    }

    ResultValue&merge(QSqlQuery&cursorQuery)
    {
        QVariantHash record;
        auto sqlRecord=cursorQuery.record();
        for (int col = 0; col < sqlRecord.count(); ++col)
            record.insert(sqlRecord.fieldName(col), sqlRecord.value(col));
        return this->merge(record);
    }

    ResultValue&merge(const QVariantHash&record)
    {

        auto metaObject = this->model->metaObject();
        auto prefix=this->modelInfo().tablePrefix();

        for(int col = 0; col < metaObject->propertyCount(); ++col) {
            auto property = metaObject->property(col);
            if(__propertyIgnoredList.contains(property.name()))
                continue;

            //auto value=record.value(property.name());
            auto propertyName=QByteArray(property.name()).toLower().trimmed();
            Q_V_HASH_ITERATOR (record){
                i.next();
                auto valueName=i.key().toLower().trimmed();
                if((valueName==propertyName) || valueName==prefix+propertyName){
                    if(!this->write(property, i.value()))
                        return this->model->lr().setValidation(QObject::tr("Invalid data model"));
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
        auto typeId=qTypeId(v);
        switch (typeId) {
        case QMetaType_QVariantHash:
        case QMetaType_QVariantMap:
            vList<<v.toHash();
            break;
        case QMetaType_QVariantList:
        case QMetaType_QStringList:
            vList=v.toList();
            break;
        default:
            return {};
        }

        QVariantList rList;
        for(auto&v:vList){
            auto record=v.toHash();
            if(record.isEmpty())
                continue;
            
            const auto&modelInfo = this->modelInfo();
            QVariantHash rHash;
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
            QHashIterator<QByteArray, QByteArray> i(mapWrapper);
#else
            QMultiHashIterator<QByteArray, QByteArray> i(mapWrapper);
#endif
            while (i.hasNext()) {
                i.next();
                auto&k=modelInfo.tablePrefix()+i.key();
                auto&s=i.value();
                auto v=record.value(k);
                if(!v.isValid()){
                    auto&k=i.key();
                    auto v=record.value(k);
                    switch (qTypeId(v)){
                    case QMetaType_QUuid:
                        rHash.insert(s,v.toUuid().toString());
                        break;
                    case QMetaType_QUrl:
                        rHash.insert(s,v.toUrl().toString());
                        break;
                    case QMetaType_User:
                        rHash.insert(s,v.toInt());
                        break;
                    case QMetaType_CustomType:
                        rHash.insert(s,v.toInt());
                        break;
                    default:
                        rHash.insert(s,v);
                    }
                    continue;
                }
                switch (qTypeId(v)){
                case QMetaType_QUuid:
                    rHash.insert(s,v.toUuid().toString());
                    break;
                case QMetaType_QUrl:
                    rHash.insert(s,v.toUrl().toString());
                    break;
                case QMetaType_User:
                    rHash.insert(s,v.toInt());
                    break;
                case QMetaType_CustomType:
                    rHash.insert(s,v.toInt());
                    break;
                default:
                    rHash.insert(s,v);
                }
            }
            rList<<rHash;
        }
        return rList;
    }

    QVariantHash wrapperToModelMap(const QString&wrapperName, const QVariant&v)const
    {
        auto vWrapper=v.toHash();
        QVariantHash vHash;
        const auto name=wrapperName.toLower().trimmed().toUtf8();
        const auto &mapWrapper = this->mapWrapper.value(name);
        QHashIterator<QString,QVariant> iO(vWrapper);
        while (iO.hasNext()) {
            iO.next();
            auto atualKey   = iO.key().toLower().toUtf8();
            auto wrapperKey = mapWrapper.key(atualKey);
            auto novaKey    = (!wrapperKey.isEmpty())?wrapperKey:atualKey;
            vHash.insert(novaKey, iO.value());
        }
        return vHash;
    }

    ResultValue&fromWrapper(const QString&wrapperName, const QVariant&vWrapper)const
    {
        auto vMap=this->wrapperToModelMap(wrapperName, vWrapper);
        return this->model->readFrom(vMap);
    }
};

Model::Model(QObject *parent):QStm::Object(parent)
{
    this->p=new ModelPvt(this);
}

Model::~Model()
{
    dPvt();delete&p;
}

QVariant Model::tablePkCompuser() const
{
    return {};
}

Model &Model::clear()
{
    dPvt();
    p.clear();
    return*this;
}

bool Model::makeUuid()
{
    return true;
}

bool Model::autoMakeUuid()
{
    return this->makeUuid();
}

QByteArray Model::storedMd5Make() const
{
    dPvt();
    return p.storedMd5Make();
}

bool Model::isModifier(const QVariantHash &vMap)
{
    auto vHash=this->toHash();
    Q_V_HASH_ITERATOR (vMap){
        i.next();
        auto&k=i.key();
        auto v0=i.value().toString().trimmed();
        auto v1=vHash.value(k).toString().trimmed();
        if(v0==v1)
            continue;
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
        if(!this->readFrom(v))
            continue;
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
    auto vHash=p.modelInfo().toHashModel(this);
    return vHash;
}

QVariantHash Model::toMapPKValues() const
{
    dPvt();
    QVVM qVVM;
    auto&modelInfo=p.modelInfo();
    const auto&propertyShortVsTable=modelInfo.propertyShortVsTable();
    auto pList=modelInfo.propertyPK().values();
    for(auto&property:pList){
        if(!property.isValid())
            return {};

        auto fieldName=propertyShortVsTable[property.name()].trimmed();
        if(fieldName.isEmpty())
            return {};

        auto value=property.read(this);

        switch (qTypeId(property)) {
        case QMetaType_QUuid:{
            auto v=value.toUuid().toString();//no remove the .toString()
            if(v.isNull())
                return {};
            value=v;
            break;
        }
        case QMetaType_QUrl:{
            auto v=value.toUrl().toString();//no remove the .toString()
            if(v.isNull())
                return {};
            value=v;
            break;
        }
        case QMetaType_QVariantHash:
        case QMetaType_QVariantMap:
        {
            auto v=value.toHash();
            if(v.isEmpty())
                return {};
            value=v;
            break;
        }
        case QMetaType_QString:
        case QMetaType_QByteArray:
        case QMetaType_QChar:
        case QMetaType_QBitArray:
        {
            auto v=value.toString().trimmed();
            if(v.isEmpty())
                return {};
            value=v;
            break;
        }
        default:
            break;
        }

        //value=property.read(this);
        const auto k=SqlParserItem::createObject(fieldName);
        const auto v=SqlParserItem::createValue(value);
        qVVM.insert(k,v);

    }
    return QVariant(qVVM).toHash();
}

QVariantHash Model::toMapFKValues() const
{
    dPvt();
    QVVM qvvm;
    auto&modelInfo=p.modelInfo();
    const auto&propertyShortVsTable=modelInfo.propertyShortVsTable();
    auto pList=modelInfo.propertyFK().values();
    for(auto&property:pList){
        if(!property.isValid())
            continue;

        auto fieldName=propertyShortVsTable[property.name()].trimmed();
        if(fieldName.isEmpty())
            continue;

        const auto k=SqlParserItem::createObject(fieldName);
        const auto v=SqlParserItem::createValue(property.read(this));
        qvvm.insert(k,v);
    }
    return QVariant(qvvm).toHash();
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

    auto l=p.toWrapper(wrapperName, map);
    return l.isEmpty()?QVariantHash():l.first().toHash();
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
    Q_V_HASH_ITERATOR (this->toWrapper()){
        i.next();
        wrapper[i.key()]=i.value();
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
    return false;
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
    if(this->isModifier(tMap))
        return this->readFrom(tMap);
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
    return this->readFrom(model.toHash());
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
    return p.merge(resultValue.resultHash());
}

bool Model::setProperty(const char *name, const QVariant &value)
{
    dPvt();
    auto index=this->metaObject()->indexOfProperty(name);
    if(index<0)
        return false;

    auto property=this->metaObject()->property(index);
    if(!property.isValid())
        return false;

    return p.write(property, value);
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
        if(qTypeId(property)!=QMetaType_QUuid)
            continue;

        auto v=property.read(this).toUuid();
        if(!v.isNull())
            continue;

        property.write(this, this->uuidGenerator());
    }
    return this->lr()=true;
}

ResultValue &Model::deactivateSetValues()
{
    dPvt();
    auto propertyDeactivateField=p.modelInfo().propertyDeactivateField();
    if(propertyDeactivateField.isEmpty())
        return this->lr()=false;

    Q_V_HASH_ITERATOR(propertyDeactivateField){
        i.next();
        const auto k=i.key().toUtf8();
        const auto&v=i.value();
        if(this->setProperty(k, v))
            continue;
        return this->lr().setValidation(tr("Invalid data to define in the model as deleted"));
    }
    return this->lr();
}

ResultValue &Model::datetimeSet()
{
    dPvt();
    auto&modelInfo=p.modelInfo();
    Q_V_PROPERTY_ITERATOR(modelInfo.propertyPK()){
        i.next();
        auto&property=i.value();
        auto v=property.read(this);
        switch (qTypeId(property)) {
        case QMetaType_QDateTime:
        {
            auto dt=v.toDateTime();
            if(dt.isNull() || !v.isValid()){
                property.write(this, QDateTime::currentDateTime());
            }
            break;
        }
        case QMetaType_QDate:
        {
            auto dt=v.toDate();
            if(dt.isNull() || !v.isValid()){
                property.write(this, QDate::currentDate());
            }
            break;
        }
        case QMetaType_QTime:
        {
            auto dt=v.toTime();
            if(dt.isNull() || !v.isValid()){
                property.write(this, QTime::currentTime());
            }
            break;
        }
        default:
            break;
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
