#include "./qorm_model_field_descriptors.h"
#include "./qorm_model_field_descriptor.h"
#include <QMetaProperty>
#include <QRect>
#include "../../../qstm/src/qstm_meta_enum.h"

namespace QOrm {

static const auto __30P="30%";
static const auto __40P="40%";

static auto const __remove="remove";
static auto const __removeName="Remover";
static auto const __save="save";
static auto const __saveName="Salvar";
static auto const __print="print";
static auto const __printName="Imprimir";
static auto const __refresh="print";
static auto const __refreshName="Atualizar";
static auto const __finalize="finalize";
static auto const __finalizeName="Finalizar";

class ModelDescriptorPvt:public QObject
{
public:
    QStm::MetaEnum<QOrm::ModelFieldDescriptors::FormType> type = ModelFieldDescriptors::RegisterForm;
    QByteArray className;
    QUuid uuid;
    QString description;
    ModelFieldCollection descriptorsCollection, filtersCollection;
    ModelActionCollection actionsCollection;
    QVariantHash sort;
    Design design;
    Host host;
    EndPoints endPoints;
    EndPoint endPoint;
    bool obrigatory=false;
    bool readOnly=false;
    QStringList fieldsValid;
    ModelFieldDescriptors *parent=nullptr;
    explicit ModelDescriptorPvt(ModelFieldDescriptors *parent)
        :
          QObject{parent},
          descriptorsCollection{parent},
          filtersCollection{parent},
          actionsCollection{parent},
          design{parent},
          host{parent},
          endPoints{parent},
          endPoint{parent}
    {
    }

    void clear()
    {
        this->type=ModelFieldDescriptors::RegisterForm;
        this->className.clear();
        this->uuid={};
        this->description.clear();
        this->descriptorsCollection.clear();
        this->filtersCollection.clear();
        this->actionsCollection.clear();
        this->sort.clear();
        this->design.clear();
        this->design
                .width(__30P)
                .height(__40P)
                .columns(0)
                .rows(3)
                .layout(Design::Vertical)
                .type(Design::RegisterForm);
        this->host.clear();
        this->endPoint.clear();
        this->endPoints.clear();
        this->fieldsValid.clear();
    }

    void actionCRUDMaker()
    {

        this->descriptorsCollection.clear();
        this->actionsCollection.item(__save).title(__saveName).order(0);
        this->actionsCollection.item(__remove).title(__removeName).order(1);
    }

    void actionReportMaker()
    {
        this->descriptorsCollection.clear();
        this->actionsCollection.item(__refresh).title(__refreshName).order(0);
        this->actionsCollection.item(__print).title(__printName).order(1);
    }

    void actionOperationMaker()
    {
        this->descriptorsCollection.clear();
        this->actionsCollection.item(__finalize).title(__finalizeName).order(0);
    }
};

ModelFieldDescriptors::ModelFieldDescriptors(QObject *parent) : QStm::ObjectWrapper{parent}
{
    this->p = new ModelDescriptorPvt{this};
}

bool ModelFieldDescriptors::isValid() const
{
    const auto m1 = this->metaObject()->className();
    const auto m2 = ModelFieldDescriptors::staticMetaObject.className();
    if (m1 == m2)
        return false;
    return true;
}

void ModelFieldDescriptors::descriptorsInit()
{
    p->clear();
}

ModelFieldCollection *ModelFieldDescriptors::descriptors()
{
    return &p->descriptorsCollection;
}

ModelFieldDescriptors &ModelFieldDescriptors::setDescriptors(const ModelFieldCollection *newDescriptors)
{
    p->descriptorsCollection=newDescriptors;
    emit descriptorsChanged();
    return *this;
}

ModelFieldDescriptors &ModelFieldDescriptors::resetDescriptors()
{
    p->descriptorsCollection.clear();
    emit descriptorsChanged();
    return *this;
}

ModelFieldCollection *ModelFieldDescriptors::filters()
{
    return &p->filtersCollection;
}

ModelFieldDescriptors &ModelFieldDescriptors::setFilters(const ModelFieldCollection *newFilters)
{
    p->filtersCollection=newFilters;
    emit filtersChanged();
    return *this;
}

ModelFieldDescriptors &ModelFieldDescriptors::resetFilters()
{
    p->filtersCollection.clear();
    emit filtersChanged();
    return *this;
}

ModelActionCollection *ModelFieldDescriptors::actions() const
{
    return &p->actionsCollection;
}

ModelFieldDescriptors &ModelFieldDescriptors::setActions(const ModelActionCollection *newActions)
{
    p->actionsCollection = newActions;
    emit actionsChanged();
    return *this;
}

ModelFieldDescriptors &ModelFieldDescriptors::resetActions()
{
    p->actionsCollection.clear();
    emit actionsChanged();
    return *this;
}

ModelFieldDescriptors &ModelFieldDescriptors::actionCRUDMaker()
{
    p->actionCRUDMaker();
    return *this;
}

ModelFieldDescriptors &ModelFieldDescriptors::actionReportMaker()
{
    p->actionReportMaker();
    return *this;
}

ModelFieldDescriptors &ModelFieldDescriptors::actionOperationMaker()
{
    p->actionOperationMaker();
    return *this;
}

ModelFieldDescriptors &ModelFieldDescriptors::copyDescriptorToFilters()
{
    p->filtersCollection.clear();
    for(auto&field:p->descriptorsCollection.list()){
        if(!field->filtrable()) continue;
        this->addFilter(field->field(), field->toHash());
    }
    return *this;
}

QByteArray ModelFieldDescriptors::className() const
{
    if(p->className.isEmpty())
        return this->metaObject()->className();
    return p->className;
}

ModelFieldDescriptors &ModelFieldDescriptors::setClassName(const QByteArray &value)
{
    p->className=value;
    return *this;
}

ModelFieldDescriptors &ModelFieldDescriptors::resetClassName()
{
    return this->setClassName({});
}

QUuid &ModelFieldDescriptors::uuid() const
{
    if(p->uuid.isNull()){
        Q_DECLARE_VU;
        p->uuid=vu.toUuid(this->className());
    }
    return p->uuid;
}

ModelFieldDescriptors &ModelFieldDescriptors::setUuid(const QUuid &value)
{
    p->uuid=value;
    return *this;
}

ModelFieldDescriptors &ModelFieldDescriptors::resetUuid()
{
    return this->setUuid({});
}

ModelFieldDescriptor &ModelFieldDescriptors::addDescriptor(const QString &fieldName)
{
    return p->descriptorsCollection.item(fieldName);
}

ModelFieldDescriptors &ModelFieldDescriptors::addDescriptor(const QString &fieldName, const QVariant &values)
{
    p->descriptorsCollection.item(fieldName, values);
    return *this;
}

ModelFieldDescriptor &ModelFieldDescriptors::addFilter(const QString &fieldName)
{
    return p->filtersCollection.item(fieldName);
}

ModelFieldDescriptors &ModelFieldDescriptors::addFilter(const QString &fieldName, const QVariant &values)
{
    p->filtersCollection.item(fieldName, values);
    return *this;
}

ModelFieldDescriptor &ModelFieldDescriptors::addAction(const QString &actionName)
{
    return p->descriptorsCollection.item(actionName);
}

ModelFieldDescriptors &ModelFieldDescriptors::addAction(const QString &actionName, const QVariant &values)
{
    p->filtersCollection.item(actionName, values);
    return *this;
}

QString &ModelFieldDescriptors::description()
{
    return p->description;
}

ModelFieldDescriptors &ModelFieldDescriptors::setDescription(const QString &v)
{
    p->description = v.trimmed();
    emit descriptionChanged();
    return *this;
}

ModelFieldDescriptors &ModelFieldDescriptors::resetDescription()
{
    return this->setDescription({});
}

QVariantHash &ModelFieldDescriptors::sort() const
{
    return p->sort;
}

ModelFieldDescriptors &ModelFieldDescriptors::setSort(const QVariantHash &value)
{
    p->sort = value;
    emit sortChanged();
    return *this;
}

ModelFieldDescriptors &ModelFieldDescriptors::resetSort()
{
    return this->setSort({});
}

Design *ModelFieldDescriptors::design()
{
    return &p->design;
}

ModelFieldDescriptors &ModelFieldDescriptors::setDesign(const Design *value)
{
    p->design=value;
    return *this;
}

ModelFieldDescriptors &ModelFieldDescriptors::setDesign(const QVariantHash &value)
{
    p->design=value;
    emit designChanged();
    return *this;
}

Host *ModelFieldDescriptors::host()
{
    return &p->host;
}

Host &ModelFieldDescriptors::setHost(const Host *newHost)
{
    p->host=newHost;
    return p->host;
}

Host &ModelFieldDescriptors::setHost(const QVariant &newHost)
{
    p->host=newHost;
    emit hostChanged();
    return p->host;
}

ModelFieldDescriptors &ModelFieldDescriptors::resetHost()
{
    p->host.clear();
    emit hostChanged();
    return *this;
}

ModelFieldDescriptors &ModelFieldDescriptors::resetDesign()
{
    return this->setDesign({});
}

EndPoints *ModelFieldDescriptors::endPoints() const
{
    return &p->endPoints;
}

ModelFieldDescriptors &ModelFieldDescriptors::setEndPoints(const EndPoints *newEndPoints)
{
    p->endPoints = newEndPoints;
    emit endPointsChanged();
    return *this;
}

ModelFieldDescriptors &ModelFieldDescriptors::resetEndPoints()
{
    p->endPoints.clear();
    emit endPointsChanged();
    return *this;
}

ModelFieldDescriptors &ModelFieldDescriptors::addEndPoint(EndPoint *newEndPoint)
{
    auto v=p->endPoints.value(newEndPoint->uuid());
    if(v){
        if(v==newEndPoint)
            return *this;
        delete v;
    }
    newEndPoint->host()->setValues(&p->host);
    newEndPoint->setParent(this);
    p->endPoints.insert(newEndPoint);
    emit endPointsChanged();
    return *this;
}

EndPoint &ModelFieldDescriptors::addEndPoint(const QString &name, const QVariant &values)
{
    auto endpoint=new EndPoint{this};
    switch (values.typeId()) {
    case QMetaType::QString:
    case QMetaType::QByteArray:
        endpoint->setPath(values.toByteArray());
        break;
    case QMetaType::QVariantHash:
    case QMetaType::QVariantMap:
        endpoint->setValues(values);
        break;
    default:
        break;
    }
    endpoint->setName(name.toUtf8());
    endpoint->host()->setValues(&p->host);
    p->endPoints.insert(endpoint);
    emit endPointsChanged();
    return *endpoint;
}

EndPoint *ModelFieldDescriptors::endPoint() const
{
    return &p->endPoint;
}

EndPoint *ModelFieldDescriptors::endPoint(const QUuid &uuid) const
{
    return p->endPoints.value(uuid);
}

ModelFieldDescriptors &ModelFieldDescriptors::setEndPoint(const EndPoint *v)
{
    p->endPoint.setValues(v);
    return *this;
}

ModelFieldDescriptors &ModelFieldDescriptors::setEndPoint(const QVariant &v)
{
    switch (v.typeId()) {
    case QMetaType::QString:
    case QMetaType::QByteArray:
    case QMetaType::QUuid:{
        auto obj=p->endPoints.value(v.toUuid());
        if(obj)
            p->endPoint.setValues(obj);
        else
            p->endPoint.clear();
        break;
    }
    case QMetaType::QVariantHash:
    case QMetaType::QVariantMap:
        p->endPoint.setValues(v);
        break;
    default:
        break;
    }

    emit endPointChanged();
    return *this;
}

bool ModelFieldDescriptors::obrigatory() const
{
    return p->obrigatory;
}

ModelFieldDescriptors &ModelFieldDescriptors::setObrigatory(bool newObrigatory)
{
    if (p->obrigatory == newObrigatory)
        return *this;
    p->obrigatory = newObrigatory;
    emit obrigatoryChanged();
    return *this;
}

ModelFieldDescriptors &ModelFieldDescriptors::resetObrigatory()
{
    return setObrigatory({});
}

bool ModelFieldDescriptors::readOnly() const
{
    return p->readOnly;
}

ModelFieldDescriptors &ModelFieldDescriptors::setReadOnly(bool newReadonly)
{
    if (p->readOnly == newReadonly)
        return *this;
    p->readOnly = newReadonly;
    emit readOnlyChanged();
    return *this;
}

ModelFieldDescriptors &ModelFieldDescriptors::resetReadOnly()
{
    return setReadOnly({});
}

const QStringList &ModelFieldDescriptors::fieldsValid() const
{
    return p->fieldsValid;
}

ModelFieldDescriptors &ModelFieldDescriptors::setFieldsValid(const QStringList &newFieldsValid)
{
    if (p->fieldsValid == newFieldsValid)
        return *this;
    p->fieldsValid = newFieldsValid;
    emit fieldsValidChanged();
    return *this;
}

ModelFieldDescriptors &ModelFieldDescriptors::resetFieldsValid()
{
    return setFieldsValid({});
}

} // namespace QOrm
