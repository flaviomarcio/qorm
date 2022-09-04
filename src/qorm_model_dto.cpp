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
        fields.setHost(&dtoControls.host());
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

QVariant &ModelDto::type() const
{
    return p->dtoControls.fields().design()->type();
}

QUuid &ModelDto::uuid() const
{
    return p->dtoControls.uuid();
}

ModelDto &ModelDto::uuid(const QUuid &uuid)
{
    p->dtoControls.uuid(uuid);
    return *this;
}

QString &ModelDto::description() const
{
    return p->dtoControls.description();
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

const Host &ModelDto::host() const
{
    return p->dtoControls.host();
}

ModelDto &ModelDto::host(const Host &host)
{
    p->dtoControls.setHost(host);
    return *this;
}

ModelDto &ModelDto::host(const QVariant &host)
{
    p->dtoControls.setHost(host);
    return *this;
}

EndPoint &ModelDto::endPoint() const
{
    return p->dtoControls.endPoint();
}

EndPoints &ModelDto::endPoints() const
{
    return p->dtoControls.endPoints();
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

QVariantHash &ModelDto::sort() const
{
    return p->dtoControls.sort();
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
