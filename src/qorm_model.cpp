#include "./qorm_model.h"
#include "./qorm_query.h"
#include "./qorm_macro.h"
#include "./private/p_qorm_sql_suitable_parser_item.h"
#include "./private/p_qorm_const.h"
#include "./private/p_qorm_model_info.h"
#include "../../qstm/src/qstm_util_variant.h"
#include "../../qstm/src/qstm_util_hash.h"
#include "../../qstm/src/qstm_macro.h"
#include <QCryptographicHash>
#include <QMetaProperty>
#include <QSqlField>
#include <QSqlError>
#include <QSqlRecord>
#include <QSqlQuery>
#include <QJsonDocument>
#include <QJsonObject>
#include <QBitArray>
#include <QSqlQuery>

namespace QOrm {

Q_ORM_DECLARE_PROPERTY_IGNORE_LIST

typedef QMultiHash<QByteArray,QByteArray> WrapperMap;

class ModelPvt:public QObject{
public:
    QOrm::Model*model=nullptr;
    QMultiHash<QByteArray, QMultiHash<QByteArray,QByteArray>> mapWrapper;
    explicit ModelPvt(QOrm::Model*model):model{model}
    {
    }

    const ModelInfo &modelInfo()
    {
        const auto &className=this->model->metaObject()->className();
        return ModelInfo::from(className);
    }

    const QByteArray storedMd5Make()
    {
        const auto list=this->modelInfo().propertyList();
        auto &metaObject = *this->model->metaObject();
        QVariantHash vBody;
        for(int col = 0; col < metaObject.propertyCount(); ++col) {
            auto property = metaObject.property(col);
            if(staticPropertyIgnoredList->contains(property.name()))
                continue;

            if(list.contains(property.name()))
                vBody.insert(property.name(), property.read(this->model));
        }
        Q_DECLARE_HU;
        return hu.toMd5(vBody);
    }

    void clear()
    {
        auto metaObject=this->model->metaObject();
        for(int col = 0; col < metaObject->propertyCount(); ++col) {
            auto property = metaObject->property(col);
            if(!property.isWritable() || !property.isResettable())
                continue;

            if(staticPropertyIgnoredList->contains(property.name()))
                continue;

            property.reset(this->model);
        }
    }


    bool write(const QMetaProperty &property, const QVariant &value)
    {
        Q_DECLARE_VU;

        QVariant vValue;
        auto typeIdp = property.typeId();
        if(typeIdp==QMetaType::QUuid && value.typeId()!=typeIdp)
            vValue=vu.toUuid(value);
        else
            vValue=value;

        if(property.write(this->model, vValue))
            return true;

        auto typeIdv=vValue.typeId();
        switch (typeIdp) {
        case QMetaType::QString:
        case QMetaType::QByteArray:
        case QMetaType::QChar:
        case QMetaType::QBitArray:
        {
            QVariant v;
            switch (typeIdv) {
            case QMetaType::QVariantMap:
            case QMetaType::QVariantHash:
            case QMetaType::QVariantList:
            case QMetaType::QStringList:
                v=QJsonDocument::fromVariant(vValue).toJson(QJsonDocument::Compact);
                break;
            case QMetaType::QUuid:
                v=vu.toUuid(vValue);
                break;
            case QMetaType::QUrl:
                v=vu.toUrl(vValue);
                break;
            default:
                v=vValue;
                break;
            }

            switch (typeIdp) {
            case QMetaType::QUuid:
            {
                if(property.write(this->model, vValue.toUuid()))
                    return true;
                break;
            }
            case QMetaType::QString:
            {
                if(property.write(this->model, v.toString()))
                    return true;
                break;
            }
            case QMetaType::QByteArray:
            {
                if(property.write(this->model, v.toByteArray()))
                    return true;
                break;
            }
            case QMetaType::QChar:
            {
                if(property.write(this->model, v.toChar()))
                    return true;
                break;
            }
            case QMetaType::QBitArray:
            {
                if(property.write(this->model, v.toBitArray()))
                    return true;
                break;
            }
            default:
                if((typeIdp>=QMetaType::User) && (property.write(this->model, vValue.toInt())))
                    return true;
            }
            return false;
        }
        case QMetaType::LongLong:
        {
            if(property.write(this->model, QLocale::c().toLongLong(vValue.toString())))
                return true;
            break;
        }
        case QMetaType::ULongLong:
        {
            if(property.write(this->model, static_cast<qlonglong>(QLocale::c().toULongLong(vValue.toString()))))
                return true;
            break;
        }
        case QMetaType::Int:
        {
            if(property.write(this->model, QLocale::c().toInt(vValue.toString())))
                return true;
            break;
        }
        case QMetaType::UInt:
        {
            if(property.write(this->model, QLocale::c().toUInt(vValue.toString())))
                return true;
            break;
        }
        case QMetaType::Double:
        {
            if(property.write(this->model, QLocale::c().toDouble(vValue.toString())))
                return true;
            return false;
        }
        case QMetaType::QUrl:
        {
            if(property.write(this->model, vValue.toUrl()))
                return true;
            break;
        }
        case QMetaType::QUuid:
        {
            if(property.write(this->model, vValue.toUuid()))
                return true;
            break;
        }
        case QMetaType::QVariantHash:
        {
            if(property.write(this->model, vValue.toHash()))
                return true;
            break;
        }
        case QMetaType::QVariantMap:
        {
            if(property.write(this->model, vValue.toMap()))
                return true;
            break;
        }
        case QMetaType::QVariantList:
        {
            if(property.write(this->model, vValue.toList()))
                return true;
            break;
        }
        case QMetaType::QStringList:
        {
            if(property.write(this->model, vValue.toStringList()))
                return true;
            break;
        }
        case QMetaType::QDate:
        {
            if(property.write(this->model, vValue.toDate()))
                return true;
            break;
        }
        case QMetaType::QDateTime:
        {
            if(property.write(this->model, vValue.toDateTime()))
                return true;
            break;
        }
        case QMetaType::QTime:
        {
            if(property.write(this->model, vValue.toTime()))
                return true;
            break;
        }
        default:
            if((typeIdp==QMetaType::Bool) && (property.write(this->model, vValue.toBool())))
                return true;

            if(typeIdv==QMetaType::Bool){
                if(property.write(this->model, vValue.toBool()))
                    return true;

                switch (typeIdv) {
                case QMetaType::Int:
                case QMetaType::UInt:
                case QMetaType::ULongLong:
                case QMetaType::LongLong:
                case QMetaType::Double:
                {
                    if(property.write(this->model, (vValue.toInt()==1)))
                        return true;
                    break;
                }
                case QMetaType::QString:
                case QMetaType::QByteArray:
                case QMetaType::QChar:
                {
                    auto vv=vValue.toString().toLower();
                    bool vBool=(vv==QStringLiteral("true"));
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

    auto &read(const QVariant &vRecord)
    {
        this->clear();
        this->model->lr().clear();
        const auto &modelInfo = this->modelInfo();
        auto prefix = modelInfo.tablePrefix();

        QVariant record;
        switch (vRecord.typeId()) {
        case QMetaType::QVariantList:
        {
            auto vList=vRecord.toList();
            auto v=vList.isEmpty()?QVariant{}:vList.first();
            //check object
            switch (v.typeId()) {
            case QMetaType::QVariantHash:
            case QMetaType::QVariantMap:
                record=v;
                break;
            default:
                record={};
            }
            break;
        }
        default:
            record=vRecord;
        }

        if(!record.isValid())
            return this->model->lr();

        switch (record.typeId()) {
        case QMetaType::QVariantHash:
        case QMetaType::QVariantMap:
        {
            Q_DECLARE_VU;
            auto recordHash=record.toHash();
            if(recordHash.isEmpty())
                return this->model->lr()=false;

            for(auto &property:modelInfo.property()){
                auto propertyName=QByteArray{property.name()}.toLower().trimmed();
                Q_V_HASH_ITERATOR (recordHash){
                    i.next();
                    auto valueName=i.key().toLower().trimmed();
                    if((valueName==propertyName) || valueName==prefix+propertyName){
                        auto &value=i.value();
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

            switch (record.typeId()) {
            case QMetaType::QUuid:
            {
                auto uuid=record.toUuid();
                if(!this->model->setProperty(name.toUtf8(), uuid))
                    oWarning()<<QStringLiteral("no setProperty('%1', %2)").arg(name, record.toString());
                break;
            }
            default:
                if(!this->model->setProperty(name.toUtf8(), record))
                    oWarning()<<QStringLiteral("no setProperty('%1', %2)").arg(name, record.toString());
            }
        }

        return this->model->lr();
    }

    auto &read(const QByteArray &record)
    {
        this->model->lr()=true;
        auto v=QJsonDocument::fromJson(record).object().toVariantHash();
        return this->read(v);
    }

    auto &read(const QSqlQuery *sqlQuery)
    {
        QVariantHash record;
        auto sqlRecord=sqlQuery->record();
        for (int col = 0; col < sqlRecord.count(); ++col)
            record.insert(sqlRecord.fieldName(col), sqlRecord.value(col));
        return this->model->lr()=this->read(record);
    }

    ResultValue &merge(const QSqlQuery *query)
    {
        QVariantHash record;
        auto sqlRecord=query->record();
        for (int col = 0; col < sqlRecord.count(); ++col)
            record.insert(sqlRecord.fieldName(col), sqlRecord.value(col));
        return this->merge(record);
    }

    ResultValue &merge(const QVariant &values)
    {
        QVariantList vList;
        switch (values.typeId()) {
        case QMetaType::QVariantList:
        case QMetaType::QStringList:
        {
            auto list=values.toList();
            for(auto &v : list){
                auto vHash=v.toHash();
                if(!vHash.isEmpty())
                    vList.append(v);
            }
            break;
        }
        default:
            vList.append(values);
        }

        auto metaObject = this->model->metaObject();
        auto prefix=this->modelInfo().tablePrefix();

        for(auto &v:vList){
            auto record=v.toHash();
            for(int col = 0; col < metaObject->propertyCount(); ++col) {
                auto property = metaObject->property(col);
                if(staticPropertyIgnoredList->contains(property.name()))
                    continue;

                //auto value=record.value(property.name());
                auto propertyName=QByteArray{property.name()}.toLower().trimmed();
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
        }
        return this->model->lr();
    }

    const QVariantList toWrapper(const QString &wrapperName, const QVariant &v)
    {                
        auto name=wrapperName.toLower().trimmed().toUtf8();
        const auto &mapWrapper=this->mapWrapper.value(name);
        QVariantList vList;
        switch (v.typeId()) {
        case QMetaType::QVariantHash:
        case QMetaType::QVariantMap:
            vList.append(v.toHash());
            break;
        case QMetaType::QVariantList:
        case QMetaType::QStringList:
            vList=v.toList();
            break;
        default:
            return {};
        }

        QVariantList rList;
        for(auto &v:vList){
            auto record=v.toHash();
            if(record.isEmpty())
                continue;
            
            const auto &modelInfo = this->modelInfo();
            QVariantHash rHash;
            QMultiHashIterator<QByteArray, QByteArray> i(mapWrapper);
            while (i.hasNext()) {
                i.next();
                QString k=i.key();
                if(!record.contains(i.key()))
                    k=modelInfo.tablePrefix()+i.key();
                auto &s=i.value();
                auto v=record.value(k);
                switch (v.typeId()){
                case QMetaType::QUuid:
                    rHash.insert(s,v.toUuid().toString());
                    break;
                case QMetaType::QUrl:
                    rHash.insert(s,v.toUrl().toString());
                    break;
                case QMetaType::User:
                case 65538/*CustomType*/:
                    rHash.insert(s,v.toInt());
                    break;
                default:
                    rHash.insert(s,v);
                }
            }
            rList.append(rHash);
        }
        return rList;
    }

    QVariantHash wrapperToModelHash(const QString &wrapperName, const QVariant &v)const
    {
        QVariantHash vHash;
        auto vWrapper=v.toHash();
        const auto name=wrapperName.toLower().trimmed().toUtf8();
        const auto &mapWrapper = this->mapWrapper.value(name);
        QHashIterator<QString,QVariant> iO(vWrapper);
        while (iO.hasNext()) {
            iO.next();
            auto atualKey = iO.key().toLower().toUtf8();
            auto wrapperKey = mapWrapper.key(atualKey);
            auto novaKey = (!wrapperKey.isEmpty())?wrapperKey:atualKey;
            vHash.insert(novaKey, iO.value());
        }
        return vHash;
    }

    ResultValue &mergeWrapper(const QString &wrapperName, const QVariant &vWrapper)const
    {
        auto vHash = this->wrapperToModelHash(wrapperName, vWrapper);
        return this->model->mergeFrom(vHash);
    }

    ResultValue &fromWrapper(const QString &wrapperName, const QVariant &vWrapper)const
    {
        auto vHash=this->wrapperToModelHash(wrapperName, vWrapper);
        return this->model->readFrom(vHash);
    }

    QStringList autoSetFields()
    {
        auto __return=modelInfo().propertyPK().keys();
        __return<<modelInfo().tableAutoSetFields();
        return __return;
    }

    bool isEmptyPK()
    {
        const auto &modelInfo=this->modelInfo();
        auto pks=modelInfo.propertyPK();
        if(pks.isEmpty())
            return true;
        QHashIterator<QString,QMetaProperty> i(pks);
        while(i.hasNext()){
            i.next();
            auto &property=i.value();
            auto v=property.read(this->model);
            switch (property.typeId()) {
            case QMetaType::QUuid:{
                if(v.toUuid().isNull())
                    return true;
                break;
            }
            case QMetaType::QUrl:{
                if(v.toUrl().isEmpty())
                    return true;
                break;
            }
            case QMetaType::QVariantHash:
            case QMetaType::QVariantMap:
            case QMetaType::QVariantPair:
            {
                if(v.toHash().isEmpty())
                    return true;
                break;
            }
            case QMetaType::QVariantList:
            case QMetaType::QStringList:
            {
                if(v.toHash().isEmpty())
                    return true;
                break;
            }
            default:
                if(v.isNull() || !v.isValid())
                    return true;
            }
        }
        return {};
    }

};

Model::Model(QObject *parent):QOrm::Object{parent}, p{new ModelPvt{this}}
{
}


QVariant Model::tablePkCompuser() const
{
    return {};
}

Model &Model::clear()
{
    p->clear();
    return *this;
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
    return p->storedMd5Make();
}

bool Model::isModifier(const QVariantHash &vMap)
{
    auto vHash=this->toHash();
    Q_V_HASH_ITERATOR (vMap){
        i.next();
        auto &k=i.key();
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
    for(const auto &v:vList){
        if(!this->readFrom(v))
            continue;
        rList.append(this->toHash());
    }
    return rList;
}

QVariantHash Model::toHash() const
{
    return p->modelInfo().toHash(this, true);
}

QVariantHash Model::toHashModel(bool nullValuesAdd) const
{
    return p->modelInfo().toHashModel(this, nullValuesAdd);
}

QVariantHash Model::toPKValues() const
{
    QVVM qvvm;
    auto &modelInfo=p->modelInfo();
    const auto &propertyShortVsTable=modelInfo.propertyShortVsTable();
    auto pList=modelInfo.propertyPK().values();
    for(auto &property:pList){
        if(!property.isValid())
            return {};

        auto fieldName=propertyShortVsTable.value(property.name()).trimmed();
        if(fieldName.isEmpty())
            return {};

        auto value=property.read(this);

        switch (property.typeId()) {
        case QMetaType::QUuid:{
            auto uuid=value.toUuid();
            auto v=uuid.toString();
            value=v;
            break;
        }
        case QMetaType::QUrl:{
            auto v=value.toUrl().toString();//no remove the .toString()
            value=v;
            break;
        }
        case QMetaType::QVariantHash:
        case QMetaType::QVariantMap:
        {
            auto v=value.toHash();
            value=v;
            break;
        }
        case QMetaType::QString:
        case QMetaType::QByteArray:
        case QMetaType::QChar:
        case QMetaType::QBitArray:
        {
            auto v=value.toString().trimmed();
//            if(v.isEmpty())
//                return {};
            value=v;
            break;
        }
        default:
            break;
        }

        //value=property.read(this);
        const auto k=SqlParserItem::createObject(fieldName);
        const auto v=SqlParserItem::createValue(value);
        qvvm.insert(k,v);

    }
    return QVariant{qvvm}.toHash();
}

QVariantHash Model::toPKValuesValid() const
{
    QVVM qvvm;
    auto &modelInfo=p->modelInfo();
    const auto &propertyShortVsTable=modelInfo.propertyShortVsTable();
    auto pList=modelInfo.propertyPK().values();
    for(auto &property:pList){
        if(!property.isValid())
            return {};

        auto fieldName=propertyShortVsTable.value(property.name()).trimmed();
        if(fieldName.isEmpty())
            return {};

        auto value=property.read(this);

        switch (property.typeId()) {
        case QMetaType::QUuid:{
            auto uuid=value.toUuid();
            auto v=uuid.toString();
            if(uuid.isNull())
                return {};
            value=v;
            break;
        }
        case QMetaType::QUrl:{
            auto v=value.toUrl().toString();//no remove the .toString()
            if(v.isNull())
                return {};
            value=v;
            break;
        }
        case QMetaType::QVariantHash:
        case QMetaType::QVariantMap:
        {
            auto v=value.toHash();
            if(v.isEmpty())
                return {};
            value=v;
            break;
        }
        case QMetaType::QString:
        case QMetaType::QByteArray:
        case QMetaType::QChar:
        case QMetaType::QBitArray:
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
        qvvm.insert(k,v);

    }
    return QVariant{qvvm}.toHash();
}


QVariantHash Model::toFKValues() const
{
    QVVM qvvm;
    auto &modelInfo=p->modelInfo();
    const auto &propertyShortVsTable=modelInfo.propertyShortVsTable();
    auto pList=modelInfo.propertyForeignKeys().values();
    for(auto &property:pList){
        if(!property.isValid())
            continue;

        auto fieldName=propertyShortVsTable.value(property.name()).trimmed();
        if(fieldName.isEmpty())
            continue;

        const auto k=SqlParserItem::createObject(fieldName);
        const auto v=SqlParserItem::createValue(property.read(this));
        qvvm.insert(k,v);
    }
    return QVariant{qvvm}.toHash();
}

QVariantHash Model::toWrapper() const
{
    return this->toWrapper("");
}

QVariantHash Model::toWrapper(const QString &wrapperName) const
{
    auto map=this->toHashModel();
    if(map.isEmpty())
        return map;

    auto l=p->toWrapper(wrapperName, map);
    return l.isEmpty()?QVariantHash{}:l.first().toHash();
}

QVariantList Model::toWrapper(const QString &wrapperName, const QVariantList &v) const
{
    return p->toWrapper(wrapperName, v);
}


QVariantList Model::toWrapper(const QString &wrapperName, const QVariantHash &v) const
{
    return p->toWrapper(wrapperName, v);
}

QVariantList Model::toWrapper(const QString &wrapperName, ResultValue &v) const
{
    return p->toWrapper(wrapperName.trimmed(), v.resultVariant());
}

QVariantList Model::toWrapper(const QVariantList &v) const
{
    return p->toWrapper({}, v);
}

QVariantList Model::toWrapper(const QVariantHash &v) const
{
    return p->toWrapper({}, v);
}

QVariantList Model::toWrapper(ResultValue &v) const
{
    return p->toWrapper({}, v.resultVariant());
}

bool Model::addWrapper(const QString &wrapperName, const QString &src, const QString &dst)
{
    auto name=wrapperName.toLower().trimmed().toUtf8();
    auto mapWrapper=p->mapWrapper.value(name);
    auto v1=src.toUtf8().replace(QByteArrayLiteral("\""), "").trimmed();
    auto v2=dst.toUtf8().replace(QByteArrayLiteral("\""), "").trimmed();
    mapWrapper.insert(v1, v2);
    p->mapWrapper.insert(name,mapWrapper);
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
    QString fromWrapper;
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
    auto tMap=p->wrapperToModelHash(fromWrapper, wrapper);
    return this->isModifier(tMap);
}

bool Model::isWrapperModifierSet(const QVariantHash &wrapper)
{
    return this->isWrapperModifierSet(wrapper, {});
}

bool Model::isWrapperModifierSet(const QVariantHash &wrapper, const QString &fromWrapper)
{
    auto tMap=p->wrapperToModelHash(fromWrapper, wrapper);
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

ResultValue &Model::readFrom(const QVariant &values)
{
    return p->read(values);
}

ResultValue &Model::readFrom(const QSqlQuery *values)
{
    return p->read(values);
}

ResultValue &Model::readFrom(const Query *values)
{
    return this->readFrom(&values->sqlQuery());
}

ResultValue &Model::readFrom(const ResultValue &resultValue)
{
    return p->read(resultValue.resultVariant());
}

ResultValue &Model::readFrom(const Model &model)
{
    return this->readFrom(model.toHash());
}

ResultValue &Model::readWrapper(const QString &wrapper, const QVariant &values)
{
    return p->fromWrapper(wrapper, values.toHash());
}

ResultValue &Model::readWrapper(ResultValue &resultValue)
{
    return p->fromWrapper({}, resultValue.resultHash());
}

ResultValue &Model::mergeWrapper(const QVariant &values)
{
    return p->mergeWrapper({}, values);
}

ResultValue &Model::readWrapper(const QVariant &values)
{
    return p->fromWrapper({}, values.toHash());
}

ResultValue &Model::readWrapper(const QString &wrapper, ResultValue &resultValue)
{
    return p->fromWrapper(wrapper, resultValue.resultHash());
}

ResultValue &Model::mergeFrom(const QByteArray &values)
{
    return p->merge(QJsonDocument::fromJson(values).toVariant().toHash());
}

ResultValue &Model::mergeFrom(const QVariantHash &values)
{
    return p->merge(values);
}

ResultValue &Model::mergeFrom(const QVariant &values)
{
    return p->merge(values);
}

ResultValue &Model::mergeFrom(const QSqlQuery *values)
{
    return p->merge(values);
}

ResultValue &Model::mergeFrom(ResultValue &resultValue)
{
    return p->merge(resultValue.resultHash());
}

bool Model::setProperty(const char *name, const QVariant &value)
{
    auto index=this->metaObject()->indexOfProperty(name);
    if(index<0)
        return false;

    auto property=this->metaObject()->property(index);
    if(!property.isValid())
        return false;

    return p->write(property, value);
}

bool Model::setProperty(const QMetaProperty &property, const QVariant &value)
{
    return p->write(property, value);
}

ResultValue &Model::autoSet()
{
    auto vList=p->autoSetFields();
    auto &modelInfo=p->modelInfo();
    auto &propertyInfo=modelInfo.propertyByPropertyName();
    for(auto &v: vList){
        auto property=propertyInfo.value(v);
        if(!property.isReadable() || !property.isWritable())
            continue;
        auto value=property.read(this);

        switch (property.typeId()) {
        case QMetaType::QUuid:
            if(value.toUuid().isNull())
                property.write(this, this->uuidGenerator());
            break;
        case QMetaType::QDate:{
            auto val=value.toDateTime();
            if(val.isNull() || !val.isValid())
                property.write(this, QDate::currentDate());
            break;
        }
        case QMetaType::QTime:{
            auto val=value.toDateTime();
            if(val.isNull() || !val.isValid())
                property.write(this, QTime::currentTime());
            break;
        }
        case QMetaType::QDateTime:{
            auto val=value.toDateTime();
            if(val.isNull() || val.isValid())
                property.write(this, QDateTime::currentDateTime());
            break;
        }
        default:
            break;
        }
    }
    return this->lr()=true;
}

ResultValue &Model::uuidSet()
{
    auto vList=p->autoSetFields();
    auto &modelInfo=p->modelInfo();
    auto &propertyInfo=modelInfo.propertyByPropertyName();
    for(auto &v: vList){
        auto property=propertyInfo.value(v);
        if(!property.isReadable() || !property.isWritable())
            continue;
        auto value=property.read(this);
        switch (property.typeId()) {
        case QMetaType::QUuid:
            if(value.toUuid().isNull())
                property.write(this, this->uuidGenerator());
            break;
        default:
            break;
        }
    }
    return this->lr()=true;
}

ResultValue &Model::datetimeSet()
{
    auto vList=p->autoSetFields();
    auto &modelInfo=p->modelInfo();
    auto &propertyInfo=modelInfo.propertyByPropertyName();
    for(auto &v: vList){
        auto property=propertyInfo.value(v);
        if(!property.isReadable() || !property.isWritable())
            continue;
        auto value=property.read(this);

        switch (property.typeId()) {
        case QMetaType::QDate:{
            auto val=value.toDateTime();
            if(val.isNull() || !val.isValid())
                property.write(this, QDate::currentDate());
            break;
        }
        case QMetaType::QTime:{
            auto val=value.toDateTime();
            if(val.isNull() || !val.isValid())
                property.write(this, QTime::currentTime());
            break;
        }
        case QMetaType::QDateTime:{
            auto val=value.toDateTime();
            if(val.isNull() || val.isValid())
                property.write(this, QDateTime::currentDateTime());
            break;
        }
        default:
            break;
        }
    }
    return this->lr()=true;
}

ResultValue &Model::activateSetValues()
{
    this->mergeFrom(p->modelInfo().propertyActivateField());
    return this->lr();
}

ResultValue &Model::deactivateSetValues()
{
    this->mergeFrom(p->modelInfo().propertyDeactivateField());
    return this->lr();
}

ResultValue &Model::isValid()
{
    return this->lr()=!p->isEmptyPK();
}

ResultValue &Model::isActive()
{
    return this->lr();
}

ResultValue &Model::isEmptyPK()
{
    return this->lr()=p->isEmptyPK();
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
    return *this;
}

}
