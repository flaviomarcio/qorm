#include "./qorm_model_field_descriptors.h"
#include "./qorm_model_field_descriptor.h"
//#include "./private/p_qorm_const.h"
#include <QMetaProperty>
#include <QRect>
#include "./qorm_model_consts.h"
#include "../../../qstm/src/qstm_meta_enum.h"

namespace QOrm {

static const auto __30P="30%";

class ModelDescriptorPvt:public QObject
{
public:
    QStm::MetaEnum<QOrm::ModelFieldDescriptors::FormType> type=ModelFieldDescriptors::RegisterForm;
    QString description;
    ModelFieldCollection descriptorsCollection, filtersCollection;
    QVariantHash sort;
    QVariantHash design={{vpWidth,"20%"}, {vpHeight,"20%"}, {vpRows,2}, {vpLayout, vlVertical}};
    Host host;
    EndPoints endPoints;
    EndPoint endPoint;
    bool obrigatory=false;
    bool readOnly=false;
    ModelFieldDescriptors *parent=nullptr;
    explicit ModelDescriptorPvt(ModelFieldDescriptors *parent)
        :
          QObject{parent},
          endPoints{parent},
          endPoint{parent}
    {

    }

    void clear()
    {
        this->type=ModelFieldDescriptors::RegisterForm;
        this->description.clear();
        this->descriptorsCollection.clear();
        this->filtersCollection.clear();
        this->design={{vpWidth,"20%"}, {vpHeight,"20%"}, {vpRows,2}, {vpLayout, vlVertical}};
        this->sort.clear();
        this->host.clear();
        this->endPoint.clear();
        this->endPoints.clear();
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

ModelFieldCollection *ModelFieldDescriptors::descriptors()
{
    return &p->descriptorsCollection;
}

ModelFieldCollection &ModelFieldDescriptors::descriptorsCollection() const
{
    return p->descriptorsCollection;
}

ModelFieldDescriptors &ModelFieldDescriptors::setDescriptors(const ModelFieldCollection *newDescriptors)
{
    p->descriptorsCollection=newDescriptors;
    emit descriptorsChanged();
    return *this;
}

ModelFieldDescriptors &ModelFieldDescriptors::resetDescriptors()
{
    return this->setDescriptors({});
}

ModelFieldCollection *ModelFieldDescriptors::filters()
{
    return &p->filtersCollection;
}

ModelFieldCollection &ModelFieldDescriptors::filtersCollection() const
{
    return p->filtersCollection;
}

ModelFieldDescriptors &ModelFieldDescriptors::setFilters(const ModelFieldCollection *newFilters)
{
    p->filtersCollection=newFilters;
    emit filtersChanged();
    return *this;
}

ModelFieldDescriptors &ModelFieldDescriptors::resetFilters()
{
    return this->setFilters({});
}

void ModelFieldDescriptors::descriptorsInit()
{
    p->clear();
    QORM_MODEL_SET_FORM_TYPE(this->RegisterForm);
    QORM_MODEL_SET_FORM_DESIGN(__30P, __30P, 0, 3, vlVertical);
}

ModelFieldDescriptors &ModelFieldDescriptors::makeDescriptorToFilters()
{
//    QHashIterator <QString, ModelFieldDescriptor*> i(p->descriptorsCollection);
//    qDeleteAll(p->filtersCollection);
//    p->filtersCollection.clear();
//    while(i.hasNext()){
//        i.next();
//        auto field=i.value();
//        if(!field)
//            continue;
//        this->addFilter(field->name(), field->toHash());
//    }
    return *this;
}

QByteArray ModelFieldDescriptors::className() const
{
    return this->metaObject()->className();
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

QOrm::ModelFieldDescriptors::FormType ModelFieldDescriptors::type() const
{
    return p->type.type();
}

ModelFieldDescriptors &ModelFieldDescriptors::type(const QVariant &type)
{
    p->type=type;
    emit typeChanged();
    return *this;
}

ModelFieldDescriptors &ModelFieldDescriptors::setType(const QVariant &type)
{
    p->type=type;
    emit typeChanged();
    return *this;
}

ModelFieldDescriptors &ModelFieldDescriptors::resetType()
{
    return this->setType({});
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

QVariantHash &ModelFieldDescriptors::design()
{
    return p->design;
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

ModelFieldDescriptors &ModelFieldDescriptors::setDesign(const QVariantHash &value)
{
    p->design=value;
    emit designChanged();
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

} // namespace QOrm
