#include "./p_qorm_model_controls.h"
#include "../../qstm/src/qstm_macro.h"
#include "../qorm_model_consts.h"
#include <QStm>
#include <QVariantHash>
#include <QCoreApplication>

namespace QOrm {

class ModelDtoControlsPvt:public QObject{
public:
    QUuid uuid;
    QString settingName;
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

        const auto &vActions=fieldDescriptors.actions()->toList();
        const auto &vHeaderList=fieldDescriptors.descriptors()->list();
        const auto &vFilterList=fieldDescriptors.filters()->list();
        const auto &vEndPoints=fieldDescriptors.endPoints()->toList();
        const auto vEndPoint=fieldDescriptors.endPoint()->toHash();

//        if(this->items.isEmpty()){
//            QVariantMap vRecord;
//            for(const auto &field:vHeaderList){
//                auto name=field->field();
//                vRecord.insert(name, {});
//            }
//            this->items.append(vRecord);
//        }

        auto cacheHeader=fieldDescriptors.fieldsValid();
        if(!this->items.isEmpty()){//caso exista itens e existirem novos campos estes serão adicionados e retornados com a header predefinida
            const auto vHash=this->items.first().toHash();
            for(auto &field:vHeaderList){
                auto name=field->field();
                if(vHash.contains(name) && !cacheHeader.contains(name))
                    cacheHeader.append(name);
            }
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

        vHash.insert(vpUuid, this->dto->uuid());
        vHash.insert(vpName, this->dto->description());
        vHash.insert(vpTitle, this->dto->description());
        vHash.insert(vpDesign, this->dto->fields().design()->toHash());
        vHash.insert(vpType, this->dto->fields().design()->type());
        vHash.insert(vpActions, vActions);
        vHash.insert(vpHeaders, vHeaders);
        vHash.insert(vpFilters, vFilters);
        vHash.insert(vpItems, this->items);
        vHash.insert(vpEndPoints, vEndPoints);
        vHash.insert(vpEndPoint, vEndPoint);
        vHash.insert(vpResultInfo, this->resultInfo.toHash());

        {
            Q_V_HASH_ITERATOR(this->dto->sort()){
                i.next();
                vHash.insert(i.key(), i.value());
            }
        }
        return vHash;
    }

    void clear()
    {
        this->fieldDescriptors.clear();
        this->items.clear();
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

QUuid &ModelDtoControls::uuid() const
{
    return p->fieldDescriptors.uuid();
}

ModelDtoControls &ModelDtoControls::uuid(const QUuid &uuid)
{
    p->fieldDescriptors.setUuid(uuid);
    return *this;
}

QString &ModelDtoControls::description() const
{
    return p->fieldDescriptors.description();
}

QVariantHash &ModelDtoControls::sort() const
{
    return p->fieldDescriptors.sort();
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
