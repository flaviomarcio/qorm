#include "./p_qorm_sql_suitable_parser_item.h"

namespace QOrm{

SqlParserItem::SqlParserItem():SqlParserCommand()
{
    auto map=this->toHash();
    map.insert(qsl("info"),KeywordObjectInfo::koiObject);
    this->init(map);
}

SqlParserItem::SqlParserItem(const QVariant &value):SqlParserCommand()
{
    auto map=this->toHash();
    map.insert(qsl("info"),KeywordObjectInfo::koiObject);
    map.insert(qsl("value"),value);
    this->init(map);
}

SqlParserItem::SqlParserItem(const QVariant &value, const QVariant &title, const KeywordObjectInfo &info):SqlParserCommand()
{
    auto map=this->toHash();
    map.insert(qsl("info"),info);
    map.insert(qsl("value"),value);
    if(title.isValid() && !title.isNull())
        map.insert(qsl("title"),title);
    this->init(map);
}

void SqlParserItem::init(QVariantHash &map)
{
    this->_____zzzzz_uuid=this->sequence_zzzz();//QUuid::createUuidV5(QUuid::createUuid(),QString::number(++staticInit).toUtf8());
    map.insert(qsl("uuid"), this->_____zzzzz_uuid);
    this->setValue(map);
}

SqlParserItem SqlParserItem::setDefaultValue(const QVariant &defaultValue) const
{//nao deve passar referencia[&] ou vai alterar objeto em memoria
    auto map=this->toMap();
    map.insert(qsl("defaultValue"), defaultValue);
    auto v=QVariant(map);
    auto item=SqlParserItem::from(v);
    return item;
}

SqlParserItem SqlParserItem::defVal(const QVariant &defaultValue)
{//nao deve passar referencia[&] ou vai alterar objeto em memoria
    return this->setDefaultValue(defaultValue);
}

QVariantMap SqlParserItem::toMap() const
{
    auto map=QVariant::toMap();
    if(this->_____zzzzz_uuid>0)
        map.insert(qsl("uuid"),QString::number(this->_____zzzzz_uuid).rightJustified(11,'0'));
    return map;
}

QString SqlParserItem::toFormatParameter(SqlSuitableKeyWord &parser) const
{
    auto map=this->toMap();
    auto v=this->value();
    auto defValue=map.value(qsl("defaultValue"));
    if(this->info()==KeywordObjectInfo::koiValue){
        if(defValue.isValid() && !defValue.isNull() && v.isNull())
            return parser.formatValue(defValue);
        return parser.formatValue(v);
    }

    auto name=v.toString();
    if(defValue.isValid() && !defValue.isNull()){
        auto command =parser.parserCommand(KeywordGenericCommand::kgcIsNullCheckValue);
        if(command.contains(qsl("%1")) && command.contains(qsl("%2"))){
            auto defValueFormated=parser.formatValue(defValue);
            name=command.arg(name, defValueFormated);
        }
    }
    return name;
}

QString SqlParserItem::toFormat(SqlSuitableKeyWord &parser) const
{
    auto v=this->value();
    if(this->info()==KeywordObjectInfo::koiValue)
        return parser.formatValue(v);
    return v.toString();
}

const SqlParserItem SqlParserItem::from(const QVariant &v)
{
    SqlParserItem r;
    Q_DECLARE_VU;
    auto vValue=vu.toVariant(v);
    auto rMap=r.toMap();
    auto typeId=qTypeId(vValue);
    switch (typeId) {
    case QMetaType_QVariantHash:
    case QMetaType_QVariantMap:
    {
        auto vMap=vValue.toMap();
        typeId=vMap.value(qsl("typeId")).toInt();
        auto&value=vMap[qsl("value")];
        vMap.insert(qsl("uuid"), rMap.value(qsl("uuid")));
        value=vu.convertTo(value, typeId);
        r.setValue(vMap);
        return r;
    }
    default:
        vValue=vu.convertTo(vValue, typeId);
        QVariantHash vHash;
        vHash.insert(qsl("uuid"), rMap.value(qsl("uuid")));
        vHash.insert(qsl("info"), KeywordObjectInfo::koiValue);
        vHash.insert(qsl("value"), vValue);
        vHash.insert(qsl("typeId"), typeId);
        r.setValue(vHash);
        return r;
    }
}

SqlParserItem SqlParserItem::createObject(const QVariant &v)
{
    SqlParserItem r;
    Q_DECLARE_VU;
    auto vValue=vu.toVariant(v);
    auto rMap=r.toHash();

    auto typeId=qTypeId(vValue);
    switch (typeId) {
    case QMetaType_QVariantHash:
    case QMetaType_QVariantMap:
    {
        auto vHash=vValue.toHash();
        if(!vHash.contains(qsl("uuid"))){
            vHash.insert(qsl("uuid"),rMap.value(qsl("uuid")));
        }
        vHash.insert(qsl("info"),KeywordObjectInfo::koiObject);
        return r;
    }
    default:
        QVariantHash vHash;
        vHash.insert(qsl("uuid"), rMap.value(qsl("uuid")));
        vHash.insert(qsl("info"), KeywordObjectInfo::koiObject);
        vHash.insert(qsl("value"), vValue);
        vHash.insert(qsl("typeId"), qTypeId(vValue));
        r.setValue(vHash);
        return r;
    }
}

SqlParserItem SqlParserItem::createValue(const QVariant &v)
{
    SqlParserItem r;
    Q_DECLARE_VU;
    auto vValue=vu.toVariant(v);
    auto rMap=r.toHash();
    QVariantHash vHash;
    vHash.insert(qsl("uuid"),rMap.value(qsl("uuid")));
    vHash.insert(qsl("info"),KeywordObjectInfo::koiValue);
    vHash.insert(qsl("value"),vValue);
    vHash.insert(qsl("typeId"),qTypeId(vValue));
    r.setValue(vHash);
    return r;
}

void SqlParserItem::operator=(const QVariant &value)
{
    QVariant::setValue(value);
}

KeywordObjectInfo SqlParserItem::info() const{
    auto v=this->toHash().value(qsl("info"));
    return KeywordObjectInfo(v.toInt());
}

QVariant SqlParserItem::value() const
{
    auto vHash=this->toHash();
    auto v=vHash.value(qsl("value"));
    auto d=vHash.value(qsl("defaultValue"));
    v=v.isValid() && !v.isNull()?v:d;
    return v;
}

QVariant SqlParserItem::valueTypeId() const
{
    auto vHash=this->toHash();
    auto v=vHash.value(qsl("value"));
    auto d=vHash.value(qsl("defaultValue"));
    v=v.isValid() && !v.isNull()?v:d;
    return qTypeId(v);
}

bool SqlParserItem::isList() const
{
    switch (qTypeId(this->value())){
    case QMetaType_QVariantList:
    case QMetaType_QStringList:
        return true;
    default:
        return false;
    }
}

bool SqlParserItem::isMap() const
{
    switch (qTypeId(this->value())){
    case QMetaType_QVariantMap:
    case QMetaType_QVariantHash:
        return true;
    default:
        return false;
    }
}

QVariant SqlParserItem::name() const
{
    if(this->info()==KeywordObjectInfo::koiObject){
        auto v=this->toHash().value(qsl("value"));
        return v;
    }
    return {};
}

bool SqlParserItem::isObject() const
{
    return this->info()==KeywordObjectInfo::koiObject;
}

bool SqlParserItem::isValue() const
{
    return this->info()==KeywordObjectInfo::koiValue;
}

QVariant SqlParserItem::title() const
{
    if(this->info()==KeywordObjectInfo::koiObject){
        auto v=this->toHash().value(qsl("title"));
        return v;
    }
    return {};
}

}
