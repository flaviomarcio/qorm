#include "./qorm_model_dto.h"
#include "./qorm_model.h"
//#include "./qorm_startup.h"
//#include "./qorm_const.h"
//#include "./qorm_macro.h"
#include "./qorm_model_field_descriptors.h"
#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>

namespace QOrm {

class ModelDtoPvt:public QObject
{
public:
    ModelDtoControls dtoControls;
    QObject *parent = nullptr;
    explicit ModelDtoPvt(ModelDto *parent):QObject{parent}, dtoControls{parent}
    {
        this->parent = parent;
        auto pParent = parent->parent();
        if (pParent != nullptr)
            this->initDescriptors(pParent);
    }

    void clear() { this->dtoControls.clear(); }

    void initDescriptors(QObject *object)
    {
        auto model = dynamic_cast<QOrm::Model *>(object);
        if (model == nullptr)
            return;
        const auto &modelInfo = ModelInfo::from(model->metaObject()->className());
        initDescriptors(modelInfo);
    }

    void initDescriptors(const ModelInfo &modelInfo)
    {
        if(!modelInfo.isValid())
            return;
        auto &fields=dtoControls.fields();
        auto vDescriptores=modelInfo.propertyDescriptors();
        fields.setValues(vDescriptores);
    }
};

ModelDto::ModelDto(QObject *parent) : QStm::Object{parent}
{
    this->p = new ModelDtoPvt{this};
}

ModelDto::ModelDto(const ResultValue &rows, QObject *parent) : QStm::Object{parent}
{
    this->p = new ModelDtoPvt{this};
    p->dtoControls.setValue(rows.resultVariant());
}

ModelDto::ModelDto(const ModelInfo *modelInfo, QObject *parent) : QStm::Object{parent}
{
    this->p = new ModelDtoPvt{this};
    p->initDescriptors(*modelInfo);
}

QVariant ModelDto::type() const
{
    return p->dtoControls.fields().design()->type();
}

ModelDto &ModelDto::setType(const QVariant &newType)
{
    p->dtoControls.fields().design()->setType(newType);
    emit typeChanged();
    return *this;
}

ModelDto &ModelDto::resetType()
{
    return setType({});
}

QUuid &ModelDto::uuid() const
{
    return p->dtoControls.uuid();
}

ModelDto &ModelDto::uuid(const QUuid &value)
{
    p->dtoControls.uuid(value);
    return *this;
}

ModelDto &ModelDto::setUuid(const QUuid &value)
{
    p->dtoControls.uuid(value);
    return *this;
}

QString &ModelDto::name() const
{
    return p->dtoControls.name();
}

ModelDto &ModelDto::name(const QVariant &value)
{
    p->dtoControls.name(value);
    return *this;
}

ModelDto &ModelDto::setName(const QVariant &value)
{
    p->dtoControls.setName(value);
    return *this;
}

ModelDtoControls &ModelDto::controls()
{
    return p->dtoControls;
}

ModelFieldCollection &ModelDto::headers()
{
    return p->dtoControls.headers();
}

ModelFieldCollection &ModelDto::filters()
{
    return p->dtoControls.filters();
}

Host &ModelDto::host() const
{
    return *p->dtoControls.endpoints().host();
}

ModelDto &ModelDto::host(const Host &host)
{
    p->dtoControls.endpoints().host(&host);
    return *this;
}

ModelDto &ModelDto::host(const QVariant &host)
{
    p->dtoControls.endpoints().host(host);
    return *this;
}

EndPoints &ModelDto::endPoints() const
{
    return p->dtoControls.endpoints();
}

const QVariantList &ModelDto::items()const
{
    return p->dtoControls.items();
}

ModelDtoControls &ModelDto::items(const QVariant &v)
{
    return p->dtoControls.items(v);
}

ModelDtoControls &ModelDto::items(const ResultValue &lr)
{
    return p->dtoControls.items(lr.resultVariant());
}

QStm::ResultInfo &ModelDto::resultInfo()
{
    return p->dtoControls.resultInfo();
}

ModelDtoControls &ModelDto::resultInfo(const QVariant &v)
{
    p->dtoControls.resultInfo().fromVar(v);
    return p->dtoControls;
}

ModelDtoControls &ModelDto::setResultInfo(const QStm::ResultInfo &resultInfo)
{
    p->dtoControls.resultInfo().fromHash(resultInfo.toHash());
    return p->dtoControls;
}

ModelDto &ModelDto::setValue(const QVariant &v)
{
    p->dtoControls.items(v);
    return *this;
}

QString &ModelDto::text() const
{
    return p->dtoControls.text();
}

ModelDto &ModelDto::text(const QVariant &v)
{
    p->dtoControls.text(v);
    return *this;
}

ModelDto &ModelDto::setText(const QVariant &v)
{
    p->dtoControls.text(v);
    return *this;
}

QVariantHash &ModelDto::sort() const
{
    return p->dtoControls.sort();
}

ModelDto &ModelDto::sort(const QVariant &v)
{
    p->dtoControls.sort(v);
    return *this;
}

ModelDto &ModelDto::setSort(const QVariant &v)
{
    p->dtoControls.setSort(v);
    return *this;
}

ModelDto &ModelDto::setValue(const ResultValue &lr)
{
    p->dtoControls.setValue(lr.resultVariant());
    return *this;
}

ModelDto &ModelDto::clear()
{
    p->clear();
    return *this;
}

ResultValue &ModelDto::o()
{
    return p->dtoControls.o();
}

ResultValue &ModelDto::toOutput()
{
    return p->dtoControls.o();
}

ModelDto &ModelDto::initDescriptors(QObject *object)
{
    p->initDescriptors(object);
    return *this;
}

} // namespace QOrm
