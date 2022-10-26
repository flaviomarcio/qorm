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
    Host host;
    ModelFieldDescriptors fieldDescriptors;
    QVariantList items;
    QStm::ResultInfo resultInfo;
    QVariantHash descriptors;
    ModelDtoControls *parent=nullptr;

    explicit ModelDtoControlsPvt(ModelDtoControls *parent)
        :
          QObject{parent},
          host{this},
          fieldDescriptors{parent},
          resultInfo{parent}
    {
        this->parent=parent;
    }

    QVariantHash toOutput()
    {
        QVariantHash vHash;
        fieldDescriptors.setHost(&this->host);

        const auto &vActions=fieldDescriptors.actions()->toHash();
        const auto &vHeaderList=fieldDescriptors.descriptors()->list();
        const auto &vFilterList=fieldDescriptors.filters()->list();
        const auto &vEndPoints=fieldDescriptors.endPoints()->toList();
        const auto vEndPoint=fieldDescriptors.endPoint()->toHash();

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
            static const auto __inputActions="InputActions";
            if(field->inputType() == __inputActions)
                vHeaders.append(field->toHash());
            else if(cacheHeader.contains(name))//se o cachec contiver a header entao lancaremos
                vHeaders.append(field->toHash());
        }

        vHash.insert(vpUuid, this->parent->uuid());
        vHash.insert(vpName, this->parent->description());
        vHash.insert(vpTitle, this->parent->description());
        vHash.insert(vpDesign, this->parent->fields().design()->toHash());
        vHash.insert(vpType, this->parent->fields().design()->type());
        vHash.insert(vpActionStart, this->parent->fields().actionStart());
        vHash.insert(vpActions, vActions);
        vHash.insert(vpHeaders, vHeaders);
        vHash.insert(vpFilters, vFilters);
        vHash.insert(vpItems, this->items);
        vHash.insert(vpEndPoints, vEndPoints);
        vHash.insert(vpEndPoint, vEndPoint);
        vHash.insert(vpResultInfo, this->resultInfo.toHash());

        {
            Q_V_HASH_ITERATOR(this->parent->sort()){
                i.next();
                vHash.insert(i.key(), i.value());
            }
        }
        return vHash;
    }

    void clear()
    {
        this->host.clear();
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

const Host &ModelDtoControls::host()const
{
    return p->host;
}

const ModelDtoControls &ModelDtoControls::host(const Host &value)
{
    return this->setHost(value);
}

const ModelDtoControls &ModelDtoControls::host(const QVariant &value)
{
    return this->setHost(value);
}

const ModelDtoControls &ModelDtoControls::setHost(const Host &value)
{
    p->host.setValues(&value);
    return *this;
}

const ModelDtoControls &ModelDtoControls::setHost(const QVariant &value)
{
    p->host.setValues(value);
    return *this;
}

EndPoints &ModelDtoControls::endPoints()
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
    Q_DECLARE_VU;
    p->items=vu.toList(v);
    return *this;
}

ModelDtoControls &ModelDtoControls::items(const ResultValue &lr)
{
    p->items=lr.resultToList();
    return *this;
}

ModelDtoControls &ModelDtoControls::setValue(const QVariant &v)
{
    Q_DECLARE_VU;
    p->items=vu.toList(v);
    return *this;
}

ModelDtoControls &ModelDtoControls::setValue(const ResultValue &lr)
{
    p->items=lr.resultToList();
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
