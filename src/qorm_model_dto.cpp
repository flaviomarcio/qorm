#include "./qorm_model_dto.h"
#include "./qorm_model.h"
#include "./qorm_startup.h"

#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>

namespace PrivateOrm {
}

namespace QOrm {

Q_GLOBAL_STATIC(QVariantHash, dtoSettings)

static void initDtoSettingsCache()
{
    QVariantHash __dtoSettings;
    QDir dir(qsl(":"));
    dir.setNameFilters(QStringList{qsl("settings.qorm.dto.json")});
    for (auto &info : dir.entryInfoList()) {
        QFile fileSrc(info.filePath());
        if (!fileSrc.open(fileSrc.ReadOnly))
#if Q_ORM_LOG
            sWarning() << qsl("No open file:") << fileSrc.fileName() << qsl(", error: ")
                       << fileSrc.errorString();
#endif
        continue;

        auto bytes = fileSrc.readAll();
        fileSrc.close();
        QVariantList vList;
        auto vDoc = QJsonDocument::fromJson(bytes).toVariant();
        switch (qTypeId(vDoc)) {
        case QMetaType_QVariantHash:
        case QMetaType_QVariantMap:
        case QMetaType_QVariantList:
        case QMetaType_QStringList:
            vList << vDoc;
            break;
        default:
            break;
        }
        for (auto &v : vList) {
            auto vDoc = v.toHash();
            QHashIterator<QString, QVariant> i(vDoc);
            while (i.hasNext()) {
                i.next();
                __dtoSettings.insert(i.key(), i.value());
            }
        }
    }
    *dtoSettings = __dtoSettings;
}

Q_ORM_STARTUP_FUNCTION(initDtoSettingsCache);

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

    void initObjects()
    {
        const auto className
            = QString::fromUtf8(this->parent->parent()->metaObject()->className()).toLower().trimmed();
        auto settings = dtoSettings->value(className).toHash();
        this->dtoControls.settings(settings);
    }

    void clear() { this->dtoControls.clear(); }

    void initDescriptors(QObject *object)
    {
        auto model = dynamic_cast<QOrm::Model *>(object);
        if (model == nullptr)
            return;
        const auto &modelInfo = ModelInfo::from(model->metaObject()->className());
        auto descriptors = modelInfo.propertyDescriptors();
        dtoControls.setDescriptors(descriptors);
        dtoControls.endpoints().setItems(modelInfo.propertyEndPoints());
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

ModelDto::~ModelDto()
{

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

QString ModelDto::name() const
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

ModelDto::FormType ModelDto::type() const
{

    return FormType(p->dtoControls.type());
}

ModelDto &ModelDto::type(const FormType &v)
{

    p->dtoControls.type(ModelDtoControls::FormType(v));
    return *this;
}

ModelDto &ModelDto::setType(const FormType &v)
{
    p->dtoControls.type(ModelDtoControls::FormType(v));
    return *this;
}

ModelDto::FormLayout ModelDto::layout() const
{
    return FormLayout(p->dtoControls.layout());
}

ModelDto &ModelDto::layout(const FormLayout &v)
{
    p->dtoControls.layout(ModelDtoControls::FormLayout(v));
    return *this;
}

ModelDto &ModelDto::setLayout(const FormLayout &v)
{
    p->dtoControls.layout(ModelDtoControls::FormLayout(v));
    return *this;
}

ModelDto &ModelDto::settings(const QVariant &setting)
{
    p->dtoControls.settings(setting.toHash());
    return *this;
}

ModelDtoControls &ModelDto::controls()
{
    return p->dtoControls;
}

ModelDtoHeaders<ModelDtoControls> &ModelDto::headers()
{

    return p->dtoControls.headers();
}

ModelDtoFilters<ModelDtoControls> &ModelDto::filters()
{

    return p->dtoControls.filters();
}

EndPoints &ModelDto::links()
{

    return p->dtoControls.endpoints();
}

ModelDtoItems<ModelDtoControls> &ModelDto::items()
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

QString ModelDto::text() const
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

QVariantHash ModelDto::sort() const
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
