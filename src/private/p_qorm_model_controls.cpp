#include "./p_qorm_model_controls.h"
#include "../../qstm/src/qstm_macro.h"
#include "../../../qstm/src/qstm_meta_enum.h"
#include <QStm>
#include <QVariantHash>
#include <QCoreApplication>

namespace QOrm {

class ModelDtoControlsPvt:public QObject{
public:
    QUuid uuid;
    QString name;
    QString text;
    QStm::MetaEnum<ModelDtoControls::FormType> type=ModelDtoControls::FormType::RegisterForm;
    QStm::MetaEnum<ModelDtoControls::FormLayout> layout=ModelDtoControls::FormLayout::Vertical;
    QVariantHash design;
    QVariantHash sort;
    QString settingName;
    QOrm::DtoOutPutStyle outPutStyle=QOrm::doRowObject;
    ModelFieldDescriptors fieldDescriptors;
    QVariantList items;
    QStm::ResultInfo resultInfo;
    QVariantHash descriptors;
    ModelDtoControls *dto=nullptr;

    explicit ModelDtoControlsPvt(ModelDtoControls *parent)
        :QObject{parent},
          fieldDescriptors{parent},
          resultInfo{parent}
    {
        this->dto=parent;
    }

    QVariantHash toOutput()
    {
        QVariantHash vHash;

        const auto &vHeaderList=fieldDescriptors.descriptors()->list();
        const auto &vFilterList=fieldDescriptors.filters()->list();
        const auto vEndPoints=fieldDescriptors.endPoints()->toList();
        const auto vEndPoint=fieldDescriptors.endPoint()->toHash();

        if(this->items.isEmpty()){
            QVariantMap vRecord;
            for(const auto &field:vHeaderList){
                auto name=field->field();
                vRecord.insert(name, {});
            }
            this->items.append(vRecord);
        }

        QVector<QString> cacheHeader;
        for(auto &v:this->items){
            const auto vHash=v.toHash();
            for(auto &field:vHeaderList){
                auto name=field->field();
                if(vHash.contains(name))
                    cacheHeader.append(name);
            }
            break;
        }

        QVariantList vFilters;
        for(const auto &field:vFilterList)
            vFilters.append(field->toHash());

        QVariantList vHeaders;
        for(const auto &field:vHeaderList){
            auto name=field->field();
            if(cacheHeader.contains(name))//se o cachec contiver a header entao lancaremos
                vHeaders.append(field->toHash());
        }

        vHash[vpUuid]=this->dto->uuid();
        vHash[vpName]=this->dto->name();
        vHash[vpTitle]=this->text;
        vHash[vpDesign]=this->dto->design();
        vHash[vpType]=this->type.name();
        vHash[vpLayout]=this->layout.name();
        vHash[vpHeaders]=vHeaders;
        vHash[vpFilters]=vFilters;
        vHash[vpItems]=this->items;
        vHash[vpEndPoints]=vEndPoints;
        vHash[vpEndPoint]=vEndPoint;
        vHash[vpResultInfo]=this->resultInfo.toHash();

        {
            Q_V_HASH_ITERATOR(this->sort){
                i.next();
                vHash.insert(i.key(), i.value());
            }
        }
        return vHash;
    }


//    void setSettings(const QVariant &value)
//    {
//        auto dtoMap=value.toHash();
//        QVariantHash v;
//        if(!dtoMap.isEmpty()){
//            auto id=this->uuid.toString().trimmed();
//            if(!id.isEmpty() || dtoMap.contains(id)){
//                v=dtoMap.value(id).toHash();
//            }
//            else if(dtoMap.contains({}) || dtoMap.contains(vpDefault)){
//                v=dtoMap.value(vpDefault).toHash();
//                if(v.isEmpty())
//                    v=dtoMap.value({}).toHash();
//            }
//        }

//        if(!v.isEmpty()){
//            this->headers.fromHash(v.value(vpHeaders).toHash());
//            this->filters.fromHash(v.value(vpFilters).toHash());
//            this->items.fromList(v.value(vpItems));
//            this->design=v.value(vpDesign).toHash();
//        }
//    }

    void clear()
    {
        this->fieldDescriptors.clear();
        this->items.clear();
        this->design.clear();
        this->resultInfo.clear();
    }
};

ModelDtoControls::ModelDtoControls(QObject *parent) : QStm::Object{parent}
{
    this->p = new ModelDtoControlsPvt{this};
}

ModelFieldDescriptors &ModelDtoControls::fields()
{
    return p->fieldDescriptors;
}

QStm::ResultInfo &ModelDtoControls::resultInfo()
{
    return p->resultInfo;
}

ModelDtoControls &ModelDtoControls::setResultInfo(const QStm::ResultInfo &resultInfo)
{
    p->resultInfo.fromHash(resultInfo.toHash());
    return *this;
}

//QVariantHash &ModelDtoControls::descriptors()
//{
//    return p->descriptors;
//}

//ModelDtoControls &ModelDtoControls::setDescriptors(const QVariantHash &values)
//{
//    auto descriptors=values.value(__descriptors).toHash();
//    auto filters=values.value(__filters).toHash();
//    p->descriptors=descriptors;
//    auto vHeaders=descriptors.value(vpHeaders).toList();
//    auto vFilters=descriptors.value(vpFilters).toList();
//    p->type=descriptors.value(vpType);
//    p->layout=descriptors.value(vpLayout);
//    p->design=descriptors.value(vpDesign).toHash();
//    p->headers.clear();
//    for (auto &v : vHeaders)
//        p->headers.value(v.toHash());
//    for (auto &v : vFilters)
//        p->filters.value(v.toHash());
//    return *this;
//}

QUuid &ModelDtoControls::uuid() const
{
    Q_DECLARE_VU;
    if(p->uuid.isNull() && !p->name.isEmpty())
        p->uuid=vu.toMd5Uuid(p->name);
    return p->uuid;
}

ModelDtoControls &ModelDtoControls::uuid(const QUuid &v)
{
    p->uuid=v;
    return *this;
}

ModelDtoControls &ModelDtoControls::setUuid(const QUuid &v)
{
    p->uuid=v;
    return *this;
}

QString &ModelDtoControls::name() const
{
    return p->name;
}

ModelDtoControls &ModelDtoControls::name(const QVariant &value)
{
    p->name=value.toString().trimmed();
    return *this;
}

ModelDtoControls &ModelDtoControls::setName(const QVariant &v)
{
    p->name=v.toString().trimmed();
    return *this;
}

ModelDtoControls::FormType ModelDtoControls::type() const
{
    return p->type.type();
}

ModelDtoControls &ModelDtoControls::type(const FormType &v)
{
    p->type=v;
    return *this;
}

ModelDtoControls &ModelDtoControls::setType(const FormType &v)
{
    p->type=v;
    return *this;
}

ModelDtoControls::FormLayout ModelDtoControls::layout() const
{
    return p->layout.type();
}

ModelDtoControls &ModelDtoControls::layout(const FormLayout &v)
{
    p->layout=v;
    return *this;
}

ModelDtoControls &ModelDtoControls::setLayout(const FormLayout &v)
{
    p->layout=v;
    return *this;
}

QVariantHash &ModelDtoControls::design() const
{
    return p->design;
}

ModelDtoControls &ModelDtoControls::design(const QVariant &v)
{
    p->design=v.toHash();
    return *this;
}

ModelDtoControls &ModelDtoControls::setDesign(const QVariant &v)
{
    p->design=v.toHash();
    return *this;
}

QVariantHash &ModelDtoControls::sort() const
{
    return p->sort;
}

ModelDtoControls &ModelDtoControls::sort(const QVariant &v)
{
    p->sort=v.toHash();
    return *this;
}

ModelDtoControls &ModelDtoControls::setSort(const QVariant &v)
{
    p->sort=v.toHash();
    return *this;
}

QString &ModelDtoControls::text() const
{
    return p->text;
}

ModelDtoControls &ModelDtoControls::text(const QVariant &v)
{
    p->text=v.toString();
    return *this;
}

QOrm::DtoOutPutStyle &ModelDtoControls::outPutStyle() const
{
    return p->outPutStyle;
}

ModelDtoControls &ModelDtoControls::outPutStyle(const QOrm::DtoOutPutStyle &value)
{
    p->outPutStyle = value;
    return *this;
}

ModelDtoControls &ModelDtoControls::setOutPutStyle(const QOrm::DtoOutPutStyle &value)
{
    p->outPutStyle = value;
    return *this;
}

ModelFieldCollection &ModelDtoControls::headers()
{
    return *p->fieldDescriptors.descriptors();
}

ModelFieldCollection &ModelDtoControls::filters()
{
    return *p->fieldDescriptors.filters();
}

Host &ModelDtoControls::host()
{
    return *p->fieldDescriptors.endPoints()->host();
}

EndPoints &ModelDtoControls::endpoints()
{
    return *p->fieldDescriptors.endPoints();
}

EndPoint &ModelDtoControls::endPoint()
{
    return *p->fieldDescriptors.endPoint();
}

const QVariantList &ModelDtoControls::items()const
{
    return p->items;
}

ModelDtoControls &ModelDtoControls::items(const QVariant &v)
{
    p->items=v.toList();
    return *this;
}

ModelDtoControls &ModelDtoControls::items(const ResultValue &lr)
{
    p->items=lr.resultList();
    return *this;
}

ModelDtoControls &ModelDtoControls::setValue(const QVariant &v)
{
    p->items=v.toList();
    return *this;
}

ModelDtoControls &ModelDtoControls::setValue(const ResultValue &lr)
{
    p->items=lr.resultList();
    return *this;
}

ModelDtoControls &ModelDtoControls::clear()
{
    p->clear();
    return *this;
}

ResultValue &ModelDtoControls::o()
{
    return this->lr(p->toOutput());
}

ResultValue &ModelDtoControls::toOutput()
{
    return this->lr(p->toOutput());
}

}
