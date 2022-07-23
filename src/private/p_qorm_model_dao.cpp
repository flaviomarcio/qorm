#include "./p_qorm_model_dao.h"
#include "./p_qorm_sql_suitable_parser_item.h"
#include "./p_qorm_sql_suitable_parameter.h"

#include "../../../qstm/src/qstm_util_variant.h"
#include "../../../qstm/src/qstm_macro.h"

namespace PrivateQOrm {

class ModelDaoPvt : public QObject{
public:
    QOrm::SqlSuitableValue suitableValue;
    explicit ModelDaoPvt(QObject*parent):QObject{parent}
    {
    }

    static bool valuesIsValid(const QVariant &values)
    {
        switch (values.typeId()) {
        case QMetaType::QString:
        case QMetaType::QByteArray:
        case QMetaType::QChar:
        case QMetaType::QBitArray:{
            auto v=values.toString().trimmed();
            if(v.isEmpty())
                return false;
            break;
        }
        case QMetaType::QVariantList:
        case QMetaType::QStringList:
            return !values.toList().isEmpty();
        case QMetaType::QVariantHash:
        case QMetaType::QVariantMap:
            return !values.toHash().isEmpty();
        default:
            break;
        }
        return true;
    }

    static QVariantList cleanList(const QVariant &vValues)
    {

        if(!vValues.isValid())
            return {};

        QVariantList vList;
        switch (vValues.typeId()) {
        case QMetaType::QVariantList:
        {
            vList=vValues.toList();
            break;
        }
        default:
            vList.append(vValues);
        }


        if(vList.isEmpty())
            return {};

        QVariantList __return;

        for(auto &v : vList){

            switch (v.typeId()) {
            case QMetaType::QVariantList:
            case QMetaType::QStringList:{
                auto vList=v.toList();
                if(!vList.isEmpty())
                    __return.append(v);
                break;
            }
            case QMetaType::QVariantHash:
            case QMetaType::QVariantMap:{
                auto vList=v.toHash();
                if(!vList.isEmpty())
                    __return.append(v);
                break;
            }
            default:
                if(v.isValid())
                    __return.append(v);
                break;
            }
        }
        return __return;
    }

};

ModelDao::ModelDao(QObject *parent) : QOrm::ObjectDb{parent}
{
    this->p = new ModelDaoPvt{this};
}

QOrm::SqlSuitableValue &ModelDao::suitableValue()
{
    auto &s=p->suitableValue;
    return s.setConnection(this->connection());
}

QVariant ModelDao::variantToParameters(const QOrm::ModelInfo &modelRef, const QVariant &value) const
{
    SearchParameters searchParameters;
    Q_DECLARE_VU;
    if(!value.isValid() || value.isNull())
        return searchParameters.buildVariant();

    if(vu.vIsEmpty(value))
        return searchParameters.buildVariant();

    switch (value.typeId()) {
    case QMetaType::QVariantHash:
    case QMetaType::QVariantMap:
    {
        Q_V_HASH_ITERATOR (value.toHash()){
            i.next();
            auto k=vu.toVariant(i.key());
            auto v=vu.toVariant(i.value());
            auto key=QOrm::SqlParserItem::from(k);
            switch (v.typeId()) {
            case QMetaType::QUuid:
                searchParameters.insert(key,v.toUuid().toString());
                break;
            case QMetaType::QUrl:
                searchParameters.insert(key,v.toUrl().toString());
                break;
            default:
                searchParameters.insert(key,v);
                break;
            }
        }
        return searchParameters.buildVariant();
    }
    case QMetaType::QVariantList:
    case QMetaType::QStringList:
    {
        if(!searchParameters.canRead(value)){
            for(auto &i_key:modelRef.tablePk()){
                auto key=QOrm::SqlParserItem::createObject(i_key);
                auto v=QOrm::SqlParserItem::createValue(value);
                searchParameters.insert(key,v);
            }
        }
        else{
            searchParameters+=value;
        }
        return searchParameters.buildVariant();
    }
    default:
        const auto &propertyByFieldName=modelRef.propertyByFieldName();
        const auto &propertyByPropertyName=modelRef.propertyByPropertyName();

        for(auto &i_key:modelRef.tablePk()){
            auto key=QOrm::SqlParserItem::createObject(i_key);

            auto property = propertyByFieldName.value(i_key);
            if(!property.isValid())
                property = propertyByPropertyName.value(i_key);

            QVariant v=vu.convertTo(value, property.typeId());
            searchParameters.insert(key,v);
        }
        return searchParameters.buildVariant();
    }
}

QVariantHash ModelDao::toPreparePrimaryKey(const QOrm::ModelInfo &modelRef, const QVariant &vModel) const
{
    Q_UNUSED(modelRef)
    auto vModelList=p->cleanList(vModel);
    if(vModelList.isEmpty())
        return {};

    QVariantHash __return;
    QHashIterator<QString, QMetaProperty> i(modelRef.propertyPK());
    while(i.hasNext()){
        i.next();
        auto property=i.value();
        auto vFilter=__return.value(property.name());
        QVariantList vFilterList=p->cleanList(vFilter);
        for(auto &v : vModelList){
            if(!v.isValid())continue;
            switch (v.typeId()) {
            case QMetaType::QVariantHash:
            case QMetaType::QVariantMap:
            {
                auto itemHash=v.toHash();
                auto val=itemHash.value(property.name());
                if(val.isValid())
                    vFilterList.append(val);
                break;
            }
            case QMetaType::QVariantList:
            case QMetaType::QStringList:
            {
                auto itemList=p->cleanList(v.toList());
                for(auto&v:itemList)
                    vFilterList.append(v);
                break;
            }


            default:
                vFilterList.append(v);
            }
        }
        if(!vFilterList.isEmpty())
            __return.insert(property.name(), vFilterList.size()==1?vFilterList.first():vFilterList);
    }

    return __return;
}

QVariantList ModelDao::toPrepareForeignWrapper(const QOrm::ModelInfo &modelRef, const QVariant &vModelFK, const QVariant &vModelPK) const
{
    auto vRecord=vModelPK;
    {//tratamento do registro que fornecera a PK para FK
        switch (vRecord.typeId()) {
        case QMetaType::QVariantList:
        case QMetaType::QStringList:
        {
            auto vList=vRecord.toList();
            vRecord = vList.isEmpty()?vModelPK:vList.first();
            break;
        }
        default:
            break;
        }

        switch (vRecord.typeId()) {
        case QMetaType::QVariantMap:
        case QMetaType::QVariantHash:
        {
            auto vHash=vRecord.toHash();
            if(vHash.contains(QStringLiteral("source")))
                vRecord=vHash.value(QStringLiteral("source"));
            break;
        }
        default:
            break;
        }

        //validando ultimo valor do source
        switch (vRecord.typeId()) {
        case QMetaType::QVariantList:
        case QMetaType::QStringList:
        {
            auto vList=vRecord.toList();
            vRecord = vList.isEmpty()?vModelPK:vList.first().toHash();
            break;
        }
        default:
            break;
        }
    }

    QVariantList vModelList;
    {
        //tratamentos dos itens a que receberam o valor para FK da PK
        switch (vModelFK.typeId()) {
        case QMetaType::QVariantList:
        case QMetaType::QStringList:
            vModelList=vModelFK.toList();
            break;
        case QMetaType::QVariantHash:
        case QMetaType::QVariantMap:
            vModelList.append(vModelFK);
            break;
        default:
            break;
        }
    }

    auto vRecordHash=vRecord.toHash();
    vRecord.clear();
    if(vModelList.isEmpty())
        vModelList.append(QVariant{});
    for(auto &vItem:vModelList){
        auto vItemHash=vItem.toHash();
        Q_V_HASH_ITERATOR(modelRef.tableForeignKey()){
            i.next();
            auto vHash=i.value().toHash();
            auto fkName=vHash[QStringLiteral("fk")].toString();
            auto pkName=vHash[QStringLiteral("pk")].toString();
            auto pkValue=vRecordHash.value(pkName);
            vItemHash.insert(fkName, pkValue);
        }
        vItem=vItemHash;
    }
    return vModelList;
}

QVariantHash ModelDao::toPrepareForeign(const QOrm::ModelInfo &modelRef, const QVariant &vModel) const
{
    Q_UNUSED(modelRef)
    QVariantHash __return;
    auto vModelList=p->cleanList(vModel);
    if(vModelList.isEmpty())
        return {};

    auto vList=modelRef.tableForeignKey().values();
    for(auto & v:vList){
        auto vPkFk=v.toHash();
        auto fieldName=vPkFk.value(QStringLiteral("fk")).toString();
        auto vFilter=__return.value(fieldName);
        QVariantList vFilterList;
        switch (vFilter.typeId()) {
        case QMetaType::QVariantList:
            vFilterList=vFilter.toList();
            break;
        default:
            if(vFilter.isValid())
                vFilterList.append(vFilter);
            break;
        }
        for(auto &v : vModelList){
            auto itemHash=v.toHash();
            auto val=itemHash.value(fieldName);
            if(val.isValid())
                vFilterList.append(val);
        }
        if(!vFilterList.isEmpty())
            __return.insert(fieldName, vFilterList.size()==1?vFilterList.first():vFilterList);
    }
    return __return;


}

QVariantHash ModelDao::toPrepareSearch(const QOrm::ModelInfo &modelRef, const QVariant &vModel) const
{
    if(!p->valuesIsValid(vModel))
        return {};

    QVariantHash __return=this->toPreparePrimaryKey(modelRef, vModel);
    if(!__return.isEmpty())
        return __return;

    __return=this->toPrepareForeign(modelRef, vModel);
    if(!__return.isEmpty())
        return __return;

    Q_UNUSED(modelRef)
    auto vModelList=p->cleanList(vModel);
    if(vModelList.isEmpty())
        return {};

    for(auto &fieldName:modelRef.propertyTableList()){
        auto vFilter=__return.value(fieldName);
        QVariantList vFilterList;
        switch (vFilter.typeId()) {
        case QMetaType::QVariantList:
            vFilterList=vFilter.toList();
            break;
        default:
            if(vFilter.isValid())
                vFilterList.append(vFilter);
            break;
        }

        for(auto &v : vModelList){
            auto vHash=v.toHash();
            auto formatField=[](const QVariant &v){
                switch (v.typeId()) {
                case QMetaType::QString:
                case QMetaType::QByteArray:
                    return QVariant{v.toString()+QStringLiteral("%")};
                default:
                    return v;
                }
            };
            auto val=vHash.value(fieldName);
            if(!val.isValid())
                continue;
            switch (val.typeId()) {
            case QMetaType::QVariantList:
            {
                auto vList=val.toList();
                for(auto&v : vList)
                    vFilterList.append(formatField(v));
                break;
            }
            default:
                vFilterList.append(formatField(val));
                break;
            }
        }
        if(!vFilterList.isEmpty())
            __return.insert(fieldName, vFilterList.size()==1?vFilterList.first():vFilterList);
    }
    return __return;
}


}

