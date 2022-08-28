#include "./p_qorm_sql_suitable_parser_item.h"
#include "../../qstm/src/qstm_util_variant.h"

namespace QOrm{

static const auto __value="value";
static const auto __info="info";
static const auto __title="title";
static const auto __uuid="uuid";
static const auto __defaultValue="defaultValue";
static const auto __typeId="typeId";


SqlParserItem::SqlParserItem():SqlParserCommand{}
{
    auto map=this->toHash();
    map.insert(__info,KeywordObjectInfo::koiObject);
    this->init(map);
}

SqlParserItem::SqlParserItem(const QVariant &value):SqlParserCommand{}
{
    auto map=this->toHash();
    map.insert(__info,KeywordObjectInfo::koiObject);
    map.insert(__value,value);
    this->init(map);
}

SqlParserItem::SqlParserItem(const QVariant &value, const QVariant &title, const KeywordObjectInfo &info):SqlParserCommand{}
{
    auto map=this->toHash();
    map.insert(__info,info);
    map.insert(__value,value);
    if(title.isValid() && !title.isNull())
        map.insert(__title,title);
    this->init(map);
}

void SqlParserItem::init(QVariantHash &map)
{
    this->_____zzzzz_uuid=this->sequence_zzzz();//QUuid::createUuidV5(QUuid::createUuid(),QString::number(++staticInit).toUtf8());
    map.insert(__uuid, this->_____zzzzz_uuid);
    this->setValue(map);
}

SqlParserItem SqlParserItem::setDefaultValue(const QVariant &defaultValue) const
{//nao deve passar referencia[&] ou vai alterar objeto em memoria
    auto map=this->toMap();
    map.insert(__defaultValue, defaultValue);
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
        map.insert(__uuid,QString::number(this->_____zzzzz_uuid).rightJustified(11,'0'));
    return map;
}

QString SqlParserItem::toFormatParameter(SqlSuitableKeyWord &parser) const
{
    auto map=this->toMap();
    auto v=this->value();
    auto defValue=map.value(__defaultValue);
    if(this->info()==KeywordObjectInfo::koiValue){
        if(defValue.isValid() && !defValue.isNull() && v.isNull())
            return parser.formatValue(defValue);
        return parser.formatValue(v);
    }

    auto name=v.toString();
    if(defValue.isValid() && !defValue.isNull()){
        static const auto __arg1=QStringLiteral("%1");
        static const auto __arg2=QStringLiteral("%2");
        auto command =parser.parserCommand(KeywordGenericCommand::kgcIsNullCheckValue);
        if(command.contains(__arg1) && command.contains(__arg2)){
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
    switch (vValue.typeId()) {
    case QMetaType::QVariantHash:
    case QMetaType::QVariantMap:
    {
        auto vMap=vValue.toMap();
        auto typeId=vMap.value(__typeId).toInt();
        auto &value=vMap[__value];
        vMap.insert(__uuid, rMap.value(__uuid));
        value=vu.convertTo(value, typeId);
        r.setValue(vMap);
        return r;
    }
    default:
        vValue=vu.convertTo(vValue, vValue.typeId());
        QVariantHash vHash;
        vHash.insert(__uuid, rMap.value(__uuid));
        vHash.insert(__info, KeywordObjectInfo::koiValue);
        vHash.insert(__value, vValue);
        vHash.insert(__typeId, vValue.typeId());
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

    auto typeId=vValue.typeId();
    switch (typeId) {
    case QMetaType::QVariantHash:
    case QMetaType::QVariantMap:
    {
        auto vHash=vValue.toHash();
        if(!vHash.contains(__uuid)){
            vHash.insert(__uuid,rMap.value(__uuid));
        }
        vHash.insert(__info,KeywordObjectInfo::koiObject);
        return r;
    }
    default:
        QVariantHash vHash;
        vHash.insert(__uuid, rMap.value(__uuid));
        vHash.insert(__info, KeywordObjectInfo::koiObject);
        vHash.insert(__value, vValue);
        vHash.insert(__typeId, vValue.typeId());
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
    vHash.insert(__uuid,rMap.value(__uuid));
    vHash.insert(__info,KeywordObjectInfo::koiValue);
    vHash.insert(__value,vValue);
    vHash.insert(__typeId,vValue.typeId());
    r.setValue(vHash);
    return r;
}

void SqlParserItem::operator=(const QVariant &value)
{
    QVariant::setValue(value);
}

KeywordObjectInfo SqlParserItem::info() const{
    auto v=this->toHash().value(__info);
    return KeywordObjectInfo(v.toInt());
}

QVariant SqlParserItem::value() const
{
    auto vHash=this->toHash();
    auto v=vHash.value(__value);
    auto d=vHash.value(__defaultValue);
    v=v.isValid() && !v.isNull()?v:d;
    return v;
}

QVariant SqlParserItem::valueTypeId() const
{
    auto vHash=this->toHash();
    auto v=vHash.value(__value);
    auto d=vHash.value(__defaultValue);
    v=v.isValid() && !v.isNull()?v:d;
    return v.typeId();
}

bool SqlParserItem::isList() const
{
    switch (this->value().typeId()){
    case QMetaType::QVariantList:
    case QMetaType::QStringList:
        return true;
    default:
        return false;
    }
}

bool SqlParserItem::isMap() const
{
    switch (this->value().typeId()){
    case QMetaType::QVariantMap:
    case QMetaType::QVariantHash:
        return true;
    default:
        return false;
    }
}

QVariant SqlParserItem::name() const
{
    if(this->info()==KeywordObjectInfo::koiObject){
        auto v=this->toHash().value(__value);
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
        auto v=this->toHash().value(__title);
        return v;
    }
    return {};
}

}
