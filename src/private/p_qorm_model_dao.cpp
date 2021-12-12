#include "./p_qorm_model_dao.h"
#include "./p_qorm_sql_suitable_parser_strategy_options.h"
#include "./qstm_util_variant.h"

namespace PrivateQOrm {

#define dPvt()\
    auto&p = *reinterpret_cast<ModelDaoPvt*>(this->p)\

class ModelDaoPvt : public QObject{
public:
    QOrm::SqlSuitableValue suitableValue;
    explicit ModelDaoPvt(QObject*parent):QObject(parent)
    {
    }
    ~ModelDaoPvt(){
    }
};

ModelDao::ModelDao(QObject *parent) : QOrm::ObjectDb(parent)
{
    this->p = new ModelDaoPvt(this);
}

ModelDao::~ModelDao()
{
    dPvt();
    p.deleteLater();
}

QVariant ModelDao::variantToParameters(const QOrm::ModelInfo &modelRef, const QVariant &value) const
{
    SearchParameters map;
    Q_DECLARE_VU;
    if(!value.isValid() || value.isNull())
        return map.buildVariant();

    if(vu.vIsEmpty(value))
        return map.buildVariant();

    auto typeIdValue=qTypeId(value);
    if(QStmTypesVariantDictionary.contains(typeIdValue)){
        Q_V_HASH_ITERATOR (value.toHash()){
            i.next();
            auto k=vu.toVariant(i.key());
            auto v=vu.toVariant(i.value());
            auto key=QOrm::SqlParserItem::from(k);
            switch (qTypeId(v)) {
            case QMetaType_QUuid:
                map.insert(key,v.toUuid().toString());
                break;
            case QMetaType_QUrl:
                map.insert(key,v.toUrl().toString());
                break;
            default:
                map.insert(key,v);
                break;
            }
        }
        return map.buildVariant();
    }

    const auto&propertyByFieldName=modelRef.propertyByFieldName();
    const auto&propertyByPropertyName=modelRef.propertyByPropertyName();
    if(QStmTypesVariantList.contains(typeIdValue)){
        if(!map.canRead(value)){
            for(auto&i_key:modelRef.tablePk()){
                auto key=QOrm::SqlParserItem::createObject(i_key);
                auto v=QOrm::SqlParserItem::createValue(value);
                map.insert(key,v);
            }
        }
        else{
            map+=value;
        }
        return map.buildVariant();
    }

    for(auto&i_key:modelRef.tablePk()){
        auto key=QOrm::SqlParserItem::createObject(i_key);

        auto property = propertyByFieldName.value(i_key);
        if(!property.isValid())
            property = propertyByPropertyName.value(i_key);

        QVariant v=vu.convertTo(value, qTypeId(property));
        map.insert(key,v);
    }

    return map.buildVariant();
}

QOrm::SqlSuitableValue &ModelDao::suitableValue()
{
    dPvt();
    auto&s=p.suitableValue;
    return s.setConnection(this->connection());

}

}

