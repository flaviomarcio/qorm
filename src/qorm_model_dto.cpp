#include "./qorm_model_dto.h"
#include "./qorm_model.h"
#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>

namespace PrivateOrm {
    Q_GLOBAL_STATIC(QVariantHash, dtoSettings);
}

namespace QOrm {

#define dPvt()\
    auto&p = *reinterpret_cast<ModelDtoPvt*>(this->p)

static auto&dtoSettings=*PrivateOrm::dtoSettings;

static void initDtoSettingsCache(){
    QVariantHash __dtoSettings;
    QDir dir(qsl(":"));
    dir.setNameFilters(QStringList()<<"settings.qorm.dto.json");
    for(auto&info:dir.entryInfoList()){
        QFile fileSrc(info.filePath());
        if(!fileSrc.open(fileSrc.ReadOnly))
#if Q_ORM_LOG
            sWarning()<<"No open file:"<<fileSrc.fileName()<<", error: "<<fileSrc.errorString();
#endif
        else{
            auto bytes=fileSrc.readAll();
            fileSrc.close();
            QVariantList vList;
            auto vDoc=QJsonDocument::fromJson(bytes).toVariant();
            if(vDoc.type()==vDoc.Map || vDoc.type()==vDoc.Hash){
                vList<<vDoc;
            }
            else if(vDoc.type()==vDoc.List || vDoc.type()==vDoc.StringList){
                vList=vDoc.toList();
            }
            for(auto&v:vList){
                auto vDoc=v.toHash();
                QHashIterator<QString, QVariant> i(vDoc);
                while (i.hasNext()) {
                    i.next();
                    __dtoSettings.insert(i.key(), i.value());
                }
            }
        }
    }
    dtoSettings=__dtoSettings;
}

Q_COREAPP_STARTUP_FUNCTION(initDtoSettingsCache)


class ModelDtoPvt{
public:
    ModelDtoControls dtoControls;

    QObject*dto=nullptr;

    explicit ModelDtoPvt(ModelDto*parent){
        this->dto=parent;
        auto pParent=parent->parent();
        if(pParent!=nullptr){
            this->initDescriptors(pParent);
        }
    }

    virtual ~ModelDtoPvt(){
    }

    void initObjects(){
        const auto className=QString::fromUtf8(this->dto->parent()->metaObject()->className()).toLower().trimmed();
        auto settings=dtoSettings.value(className).toHash();
        this->dtoControls.settings(settings);
    }

    void clear(){
        this->dtoControls.clear();
    }

    void initDescriptors(QObject*object){
        auto model=dynamic_cast<QOrm::Model*>(object);
        if(model!=nullptr){
            const auto&modelInfo=ModelInfo::modelInfo(model->metaObject()->className());
            const auto&descriptors=modelInfo.propertyDescriptors();
            this->dtoControls.headers().clear().makeDefault();
            for(auto&v:descriptors){
                this->dtoControls.headers().value(v.toHash());
            }
        }
    }
};

ModelDto::ModelDto(QObject *parent) : QStm::Object(parent)
{
    this->p = new ModelDtoPvt(this);
}

ModelDto::ModelDto(const ResultValue &rows, QObject *parent) : QStm::Object(parent)
{
    this->p = new ModelDtoPvt(this);
    dPvt();
    p.dtoControls.setValue(rows.resultVariant());
}

ModelDto::~ModelDto()
{
    dPvt();
    delete&p;
}

QString ModelDto::id() const
{
    dPvt();
    return p.dtoControls.id();
}

ModelDto &ModelDto::id(const QVariant &v)
{
    dPvt();
    p.dtoControls.id(v);
    return*this;
}

ModelDto &ModelDto::setId(const QVariant &value)
{
    dPvt();
    p.dtoControls.id(value);
    return*this;
}

QVariant ModelDto::type() const
{
    dPvt();
    auto value=p.dtoControls.type();
    if(value.isNull() && !value.isValid()){
        value=this->defaultType();
    }
    return value;
}

ModelDto &ModelDto::type(const QVariant &v)
{
    dPvt();
    p.dtoControls.type(v);
    return*this;
}

ModelDto &ModelDto::setType(const QVariant &v)
{
    dPvt();
    p.dtoControls.type(v);
    return*this;
}

QVariant ModelDto::layout() const
{
    dPvt();
    auto value=p.dtoControls.layout();
    if(value.isNull() && !value.isValid()){
        value=this->defaultLayout();
    }
    return value;
}

ModelDto &ModelDto::layout(const QVariant &v)
{
    dPvt();
    p.dtoControls.layout(v);
    return*this;
}

ModelDto &ModelDto::setLayout(const QVariant &v)
{
    dPvt();
    p.dtoControls.layout(v);
    return*this;
}

ModelDto &ModelDto::settings(const QVariant &setting)
{
    dPvt();
    p.dtoControls.settings(setting.toHash());
    return*this;
}

ModelDtoControls &ModelDto::controls()
{
    dPvt();
    return p.dtoControls;
}

ModelDtoHeaders<ModelDtoControls> &ModelDto::headers()
{
    dPvt();
    return p.dtoControls.headers();
}

ModelDtoFilters<ModelDtoControls> &ModelDto::filters()
{
    dPvt();
    return p.dtoControls.filters();
}

ModelDtoLinks<ModelDtoControls> &ModelDto::links()
{
    dPvt();
    return p.dtoControls.links();
}

ModelDtoItems<ModelDtoControls> &ModelDto::items()
{
    dPvt();
    return p.dtoControls.items();
}

ModelDtoControls &ModelDto::items(const QVariant &v)
{
    dPvt();
    return p.dtoControls.items(v);
}

ModelDtoControls &ModelDto::items(const ResultValue &lr)
{
    dPvt();
    return p.dtoControls.items(lr.resultVariant());
}

ModelDto &ModelDto::setValue(const QVariant &v)
{
    dPvt();
    p.dtoControls.items(v);
    return*this;
}

QString ModelDto::text() const
{
    dPvt();
    return p.dtoControls.text();
}

ModelDto &ModelDto::text(const QVariant &v)
{
    dPvt();
    p.dtoControls.text(v);
    return*this;
}

ModelDto &ModelDto::setText(const QVariant &v)
{
    dPvt();
    p.dtoControls.text(v);
    return*this;
}

QVariantHash ModelDto::sort() const
{
    dPvt();
    return p.dtoControls.sort();
}

ModelDto &ModelDto::sort(const QVariant &v)
{
    dPvt();
    p.dtoControls.sort(v);
    return*this;
}

ModelDto &ModelDto::setSort(const QVariant &v)
{
    dPvt();
    p.dtoControls.setSort(v);
    return*this;
}

ModelDto &ModelDto::setValue(const ResultValue &lr)
{
    dPvt();
    p.dtoControls.setValue(lr.resultVariant());
    return*this;
}

ModelDto &ModelDto::clear()
{
    dPvt();
    p.clear();
    return*this;
}

ResultValue &ModelDto::o()
{
    dPvt();
    return p.dtoControls.o();
}

ResultValue &ModelDto::toOutput()
{
    dPvt();
    return p.dtoControls.o();
}

ModelDto &ModelDto::initDescriptors(QObject *object)
{
    dPvt();
    p.initDescriptors(object);
    return*this;
}

}
