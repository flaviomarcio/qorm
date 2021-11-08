#include "./p_qorm_model_dao.h"
#include "./p_qorm_sql_suitable_parser_strategy_options.h"
#include "./qstm_util_variant.h"

namespace PrivateQOrm {

#define dPvt()\
    auto&p = *reinterpret_cast<ModelDaoPvt*>(this->p)\

class ModelDaoPvt : public QObject{
public:
    QOrm::SqlSuitableValue suitableValue;
    explicit ModelDaoPvt(QObject*parent):QObject(parent){
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

QVariant ModelDao::variantToParameters(const QOrm::ModelInfo &modelRef, const QVariant &value) const{
    SearchParameters map;
    VariantUtil vu;
    if(value.isValid() && !value.isNull()){
        if(value.typeId()==QMetaType::QVariantMap || value.typeId()==QMetaType::QVariantHash){
            QHashIterator<QString, QVariant> i(value.toHash());
            while (i.hasNext()) {
                i.next();
                auto k=vu.toVariant(i.key());
                auto v=vu.toVariant(i.value());
                auto key=QOrm::SqlParserItem::from(k);
                if(v.typeId()==QMetaType::QUuid)
                    map.insert(key,v.toUuid().toString());
                else if(v.typeId()==QMetaType::QUrl)
                    map.insert(key,v.toUrl().toString());
                else
                    map.insert(key,v);
            }
        }
        else if(!vu.vIsEmpty(value)){
            const auto&propertyByFieldName=modelRef.propertyByFieldName();
            const auto&propertyByPropertyName=modelRef.propertyByPropertyName();
            if(value.typeId()==QMetaType::QVariantList || value.typeId()==QMetaType::QStringList){
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
            }
            else{
                VariantUtil vu;
                for(auto&i_key:modelRef.tablePk()){
                    auto key=QOrm::SqlParserItem::createObject(i_key);
                    QVariant v;
                    auto property = propertyByFieldName.value(i_key);
                    if(!property.isValid())
                        property = propertyByPropertyName.value(i_key);

                    if(property.isValid()){//validation value of property type
                        if(v.typeId()==property.typeId()){
                            v=value;
                        }
                        else if(value.typeId()==QMetaType::QVariantList || value.typeId()==QMetaType::QStringList){
                            v=value;
                        }
                        else if(property.typeId()==QMetaType::QUuid){
                            v=vu.toUuid(value);
                        }
                        else{
                            v=value;
                        }
                    }
                    else{
                        v=value;
                    }

                    map.insert(key,v);
                }
            }
        }

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

