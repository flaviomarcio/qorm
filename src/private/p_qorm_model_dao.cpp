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
};

ModelDao::ModelDao(QObject *parent) : QOrm::ObjectDb{parent}
{
    this->p = new ModelDaoPvt{this};
}

QVariant ModelDao::variantToParameters(const QOrm::ModelInfo &modelRef, const QVariant &value) const
{
    SearchParameters searchParameters;
    Q_DECLARE_VU;
    if(!value.isValid() || value.isNull())
        return searchParameters.buildVariant();

    if(vu.vIsEmpty(value))
        return searchParameters.buildVariant();

    auto typeIdValue=value.typeId();
    switch (typeIdValue) {
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

QOrm::SqlSuitableValue &ModelDao::suitableValue()
{
    auto &s=p->suitableValue;
    return s.setConnection(this->connection());

}

}

