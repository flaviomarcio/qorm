#include "./p_qorm_model_controls.h"
#include "../../qstm/src/qstm_macro.h"
#include <QStm>
#include <QVariantHash>
#include <QCoreApplication>

namespace QOrm {

class ModelDtoControlsPvt:public QObject{
public:
    QUuid uuid;
    QString name;
    QString text;
    ModelDtoControls::FormType type=ModelDtoControls::FormType::CustomForm;
    ModelDtoControls::FormLayout layout=ModelDtoControls::FormLayout::Vertical;
    QVariantHash design;
    QVariantHash sort;
    QString settingName;
    QOrm::DtoOutPutStyle outPutStyle=QOrm::doRowObject;
    ModelDtoHeaders<ModelDtoControls> headers;
    ModelDtoFilters<ModelDtoControls> filters;
    ModelDtoItems<ModelDtoControls> items;
    QStm::ResultInfo resultInfo;
    EndPoints endpoints;
    QVariantMap descriptors;
    ModelDtoControls *dto=nullptr;

    explicit ModelDtoControlsPvt(ModelDtoControls *parent)
        :QObject{parent},
        headers{parent, parent},
        filters{parent, parent},
        items{parent, parent},
        resultInfo{parent},
        endpoints{parent}
    {
        this->dto=parent;
    }

    virtual ~ModelDtoControlsPvt()
    {
    }

    QVariantHash toOutput()
    {
        QVariantHash vHash;
        {//rows
            QVector<QString> cacheHeader;

            const auto &vHeaderList=this->headers.list();
            const auto vFilter=this->filters.toVar().toList();
            const auto vEndPoints=this->endpoints.toList();
            auto vItems=this->items.toVar().toList();

            auto &vList=this->items.list();
            if(vList.isEmpty()){
                QVariantMap vRecord;
                for(const auto &header:vHeaderList){
                    auto headerName=header->value();
                    vRecord[headerName]={};
                }
                vList.append(vRecord);
            }

            for(auto &v:vList){
                const auto vMap=v.toHash();
                for(const auto &header:vHeaderList){
                    auto headerName=header->value();
                    if(vMap.contains(headerName))
                        cacheHeader.append(headerName);
                }
                break;
            }

            if(this->outPutStyle==QOrm::doRowArray){
                QVariantList arItems;
                for(auto &v:this->items.list()){
                    const auto vMap=v.toHash();
                    QVariantList aRow;
                    for(auto &headerName:cacheHeader){
                        auto value=vMap.value(headerName);
                        aRow.append(value);
                    }
                    if(!aRow.isEmpty())
                        arItems.append(aRow);
                }
                vHash.insert(vpItems, vItems );
                vItems=arItems;
            }



            QVariantList vHeader;
            for(const auto &header:vHeaderList){
                auto headerName=header->value();
                auto var=header->toVar();

                if(headerName==vActions)
                    vHeader.append(var);
                else if(cacheHeader.contains(headerName))//se o cachec contiver a header entao lancaremos
                    vHeader.append(var);

            }

            vHash[vpUuid]=this->dto->uuid();
            vHash[vpName]=this->dto->name();
            vHash[vpTitle]=this->text;
            vHash[vpDesign]=this->dto->design();
            vHash[vpType]=QVariant::fromValue<ModelDtoControls::FormType>(this->type);
            vHash[vpLayout]=QVariant::fromValue<ModelDtoControls::FormLayout>(this->layout);
            vHash[vpHeaders]=vHeader;
            vHash[vpFilters]=vFilter;
            vHash[vpItems]=vItems;
            vHash[vpEndPoints]=vEndPoints;
            vHash[vpResultInfo]=this->resultInfo.toHash();

            {
                Q_V_HASH_ITERATOR(this->sort){
                    i.next();
                    vHash.insert(i.key(), i.value());
                }
            }

        }
        return vHash;
    }


    void setSettings(const QVariant &value)
    {
        auto dtoMap=value.toHash();
        QVariantHash v;
        if(!dtoMap.isEmpty()){
            auto id=this->uuid.toString().trimmed();
            if(!id.isEmpty() || dtoMap.contains(id)){
                v=dtoMap.value(id).toHash();
            }
            else if(dtoMap.contains({}) || dtoMap.contains(vpDefault)){
                v=dtoMap.value(vpDefault).toHash();
                if(v.isEmpty())
                    v=dtoMap.value({}).toHash();
            }
        }

        if(!v.isEmpty()){
            this->headers.fromHash(v.value(vpHeaders).toHash());
            this->filters.fromHash(v.value(vpFilters).toHash());
            this->items.fromList(v.value(vpItems));
            this->design=v.value(vpDesign).toHash();
        }
    }

    void clear()
    {
        this->headers.clear();
        this->filters.clear();
        this->items.clear();
        this->design.clear();
        this->resultInfo.clear();
    }
};

ModelDtoControls::ModelDtoControls(QObject *parent) : QStm::Object{parent}
{
    this->p = new ModelDtoControlsPvt{this};
}


QStm::ResultInfo &ModelDtoControls::resultInfo()
{
    return p->resultInfo;
}

ModelDtoControls &ModelDtoControls::setResultInfo(const QStm::ResultInfo &resultInfo)
{
    p->resultInfo.fromHash(resultInfo.toHash());
    return*this;
}

QVariantMap &ModelDtoControls::descriptors()
{
    return p->descriptors;
}

void ModelDtoControls::setDescriptors(const QVariantMap &descriptors)
{
    p->descriptors=descriptors;
    auto vHeaders=descriptors.value(vpHeaders).toList();
    p->design=descriptors.value(vpDesign).toHash();
    p->headers.clear();
    for (auto &v : vHeaders)
        p->headers.value(v.toHash());
}

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

QString ModelDtoControls::name() const
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
    return p->type;
}

ModelDtoControls &ModelDtoControls::type(const FormType &v)
{
    p->type=v;
    return*this;
}

ModelDtoControls &ModelDtoControls::setType(const FormType &v)
{
    p->type=v;
    return*this;
}

ModelDtoControls::FormLayout ModelDtoControls::layout() const
{

    return p->layout;
}

ModelDtoControls &ModelDtoControls::layout(const FormLayout &v)
{
    p->layout=v;
    return*this;
}

ModelDtoControls &ModelDtoControls::setLayout(const FormLayout &v)
{
    p->layout=v;
    return*this;
}

QVariantHash ModelDtoControls::design() const
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

QVariantHash ModelDtoControls::sort() const
{
    return p->sort;
}

ModelDtoControls &ModelDtoControls::sort(const QVariant &v)
{
    p->sort=v.toHash();
    return*this;
}

ModelDtoControls &ModelDtoControls::setSort(const QVariant &v)
{
    p->sort=v.toHash();
    return*this;
}

QString ModelDtoControls::text() const
{
    return p->text;
}

ModelDtoControls &ModelDtoControls::text(const QVariant &v)
{
    p->text=v.toString();
    return*this;
}

QOrm::DtoOutPutStyle&ModelDtoControls::outPutStyle() const
{
    return p->outPutStyle;
}

ModelDtoControls &ModelDtoControls::outPutStyle(const QOrm::DtoOutPutStyle &value)
{
    p->outPutStyle = value;
    return*this;
}

ModelDtoControls &ModelDtoControls::setOutPutStyle(const QOrm::DtoOutPutStyle &value)
{
    p->outPutStyle = value;
    return*this;
}

ModelDtoControls &ModelDtoControls::settings(const QVariant &setting)
{
    p->setSettings(setting);
    return*this;
}

ModelDtoHeaders<ModelDtoControls> &ModelDtoControls::headers()
{
    return p->headers;
}

ModelDtoFilters<ModelDtoControls> &ModelDtoControls::filters()
{
    return p->filters;
}

Host &ModelDtoControls::host()
{
    return p->endpoints.host();
}

EndPoints &ModelDtoControls::endpoints()
{
    return p->endpoints;
}

ModelDtoItems<ModelDtoControls> &ModelDtoControls::items()
{
    return p->items;
}

ModelDtoControls &ModelDtoControls::items(const QVariant &v)
{
    p->items.fromList(v);
    return*this;
}

ModelDtoControls &ModelDtoControls::items(const ResultValue &lr)
{
    p->items.fromList(this->lr(lr).resultVariant());
    return*this;
}

ModelDtoControls &ModelDtoControls::setValue(const QVariant &v)
{
    p->items.fromList(v);
    return*this;
}

ModelDtoControls &ModelDtoControls::setValue(const ResultValue &lr)
{
    p->items.fromList(this->lr(lr).resultVariant());
    return*this;
}

ModelDtoControls &ModelDtoControls::clear()
{
    p->clear();
    return*this;
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
