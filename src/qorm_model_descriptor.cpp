#include "./qorm_model_descriptor.h"
//#include "./private/p_qorm_const.h"
#include <QMetaProperty>
#include <QRect>
#include "./qorm_model_consts.h"
#include "../../../qstm/src/qstm_meta_enum.h"

namespace QOrm {

typedef QHash<QString, ModelDescriptorField*> DescriptorHash;

class ModelDescriptorPvt:public QObject
{
public:
    QStm::MetaEnum<QOrm::ModelDescriptor::FormType> type=ModelDescriptor::RegisterForm;
    QStringList descriptorsOrder; //é importa ser QMap devido a ordem necessaria para exibicao
    QString description;
    QVariantMap descriptors; //é importa ser QMap devido a ordem necessaria para exibicao
    QVariantHash edit;
    QVariantHash perfumery;
    QVariantHash flags;
    QVariantHash options;
    QVariantHash sort;
    QVariantHash design={{vpWidth,"20%"}, {vpHeight,"20%"}, {vpRows,2}, {vpLayout, vlVertical}};
    DescriptorHash hashDescriptors;
    DescriptorHash hashFilters;
    Host host;
    EndPoints endPoints;
    EndPoint endPoint;
    bool obrigatory=false;
    bool readonly=false;
    ModelDescriptor *parent=nullptr;
    explicit ModelDescriptorPvt(ModelDescriptor *parent)
        :
          QObject{parent},
          endPoints{parent},
          endPoint{parent}
    {

    }

    ModelDescriptorField &addField(DescriptorHash &hash, const QVariant &fieldName)
    {
        auto name=fieldName.toString().trimmed().toLower();
        auto &field=hash[name];
        if(!field){
            field=new ModelDescriptorField{this->parent};
            field->name(name);
        }
        return *field;
    }
};

ModelDescriptor::ModelDescriptor(QObject *parent) : QObject{parent}
{
    this->p = new ModelDescriptorPvt{this};
}

bool ModelDescriptor::isValid() const
{
    const auto m1 = this->metaObject()->className();
    const auto m2 = ModelDescriptor::staticMetaObject.className();
    if (m1 == m2)
        return false;
    return true;
}

QVariantMap ModelDescriptor::descriptors() const
{
    QVariantMap descriptors = p->descriptors;

    {
        auto &mapCheck = p->edit;
        QMapIterator<QString, QVariant> i(descriptors);
        while (i.hasNext()) {
            i.next();
            if (!mapCheck.contains(i.key()))
                continue;
            auto h = descriptors[i.key()].toMap();
            h[vpEdit] = mapCheck[i.key()];
            descriptors[i.key()] = h;
        }
    }

    {
        auto &mapCheck = p->perfumery;
        QMapIterator<QString, QVariant> i(descriptors);
        while (i.hasNext()) {
            i.next();
            if (!mapCheck.contains(i.key()))
                continue;
            auto h = descriptors[i.key()].toMap();
            h[vpPerfumery] = mapCheck[i.key()];
            descriptors[i.key()] = h;
        }
    }

    {
        auto &mapCheck = p->flags;
        QMapIterator<QString, QVariant> i(descriptors);
        while (i.hasNext()) {
            i.next();
            if (!mapCheck.contains(i.key()))
                continue;
            auto h = descriptors[i.key()].toMap();
            h[vpFlags] = mapCheck[i.key()];
            descriptors[i.key()] = h;
        }
    }

    {
        auto &mapCheck = p->options;
        QMapIterator<QString, QVariant> i(descriptors);
        while (i.hasNext()) {
            i.next();
            if (!mapCheck.contains(i.key()))
                continue;
            auto h = descriptors[i.key()].toMap();
            h[vpOptions] = mapCheck[i.key()];
            descriptors[i.key()] = h;
        }
    }

    QVariantList vHeadersList;
    p->descriptors.clear();
    for (auto &name : p->descriptorsOrder)
        vHeadersList.append(descriptors[name]);

    descriptors.clear();
    descriptors[vpType]=p->type.name();
    descriptors[vpHeaders]=vHeadersList;
    descriptors[vpDesign]=p->design;
    descriptors[vpEndPoint]=p->endPoint.toHash();
    return descriptors;
}

void ModelDescriptor::descriptorsInit()
{
    p->descriptorsOrder.clear();
    p->descriptors.clear();
    static auto __30P="30%";
    QORM_MODEL_SET_FORM_TYPE(this->RegisterForm);
    QORM_MODEL_SET_FORM_DESIGN(__30P, __30P, 0, 3, vlVertical);
}

QOrm::ModelDescriptor::FormType ModelDescriptor::type() const
{
    return p->type.type();
}

void ModelDescriptor::type(const QVariant &type) const
{
    p->type=type;
}

void ModelDescriptor::setType(const QVariant &type) const
{
    p->type=type;
}

QVariant ModelDescriptor::descriptor(const QString &name) const
{
    return p->descriptors.value(name);
}

ModelDescriptorField &ModelDescriptor::addDescriptor(const QVariant &fieldName)
{
    return p->addField(p->hashDescriptors,fieldName);
}

ModelDescriptorField &ModelDescriptor::addFilter(const QVariant &fieldName)
{
    return p->addField(p->hashFilters, fieldName);
}

void ModelDescriptor::setFieldDescriptor(const QString &name, const QVariantHash &v)
{
    auto d = v;
    if (!p->descriptorsOrder.contains(name))
        p->descriptorsOrder.append(name);
    d[vpField] = name;
    p->descriptors[name] = d;
}

void ModelDescriptor::addFieldDescriptor(const QString &name, const QVariantHash &v)
{
    if (!p->descriptorsOrder.contains(name))
        p->descriptorsOrder.append(name);
    auto d = p->descriptors.value(name).toHash();
    QHashIterator<QString, QVariant> i(v);
    while (i.hasNext()) {
        i.next();
        d[i.key()] = i.value();
    }
    d[vpField] = name;
    p->descriptors[name] = d;
}

QString ModelDescriptor::description()
{
    return p->description.trimmed();
}

void ModelDescriptor::setDescription(const QString &v)
{
    p->description = v.trimmed();
}

QVariantHash &ModelDescriptor::fieldEdit() const
{
    return p->edit;
}

QVariant ModelDescriptor::fieldEdit(const QString &name) const
{
    return p->edit.value(name);
}

void ModelDescriptor::setFieldEdit(const QString &name, const QVariantHash &v)
{
    p->edit[name] = v;
}

void ModelDescriptor::addFieldEdit(const QString &name, const QVariantHash &v)
{
    auto d = p->edit.value(name).toHash();
    QHashIterator<QString, QVariant> i(v);
    while (i.hasNext()) {
        i.next();
        d[i.key()] = i.value();
    }
    p->edit[name] = d;
}

QVariantHash &ModelDescriptor::fieldPerfumerys() const
{
    return p->perfumery;
}

QVariant ModelDescriptor::fieldPerfumery(const QString &name) const
{
    return p->perfumery.value(name);
}

void ModelDescriptor::setFieldPerfumery(const QString &name, const QVariantHash &v)
{
    p->perfumery[name] = v;
}

void ModelDescriptor::addFieldPerfumery(const QString &name, const QVariantHash &v)
{
    auto d = p->perfumery.value(name).toHash();
    QHashIterator<QString, QVariant> i(v);
    while (i.hasNext()) {
        i.next();
        d[i.key()] = i.value();
    }
    p->perfumery[name] = d;
}

QVariantHash &ModelDescriptor::fieldFlags() const
{
    return p->flags;
}

QVariant ModelDescriptor::fieldFlag(const QString &name) const
{
    return p->perfumery.value(name);
}

void ModelDescriptor::setFieldFlag(const QString &name, const QVariantHash &v)
{
    p->flags[name] = v;
}

void ModelDescriptor::addFieldFlag(const QString &name, const QVariantHash &v)
{
    auto d = p->flags.value(name).toHash();
    QHashIterator<QString, QVariant> i(v);
    while (i.hasNext()) {
        i.next();
        d[i.key()] = i.value();
    }
    p->flags[name] = d;
}

QVariantHash &ModelDescriptor::fieldOptions() const
{
    return p->options;
}

QVariant ModelDescriptor::fieldOption(const QString &name) const
{
    return p->options.value(name);
}

void ModelDescriptor::setFieldOption(const QString &name, const QVariantHash &v)
{
    p->options[name] = v;
}

void ModelDescriptor::addFieldOption(const QString &name, const QVariantHash &v)
{
    auto d = p->options.value(name).toHash();
    QHashIterator<QString, QVariant> i(v);
    while (i.hasNext()) {
        i.next();
        d.insert(i.key(),i.value());
    }
    p->options[name] = d;
}

QVariantHash &ModelDescriptor::sort() const
{
    return p->sort;
}

void ModelDescriptor::setSort(const QVariantHash &value)
{
    p->sort = value;
}

QVariantHash &ModelDescriptor::design()
{
    return p->design;
}

Host &ModelDescriptor::addHost(Host *newHost)
{
    p->host=newHost;
    return p->host;
}

Host &ModelDescriptor::addHost(const QVariant &newHost)
{
    p->host=newHost;
    return p->host;
}

void ModelDescriptor::setDesign(const QVariantHash &value)
{
    p->design=value;
}

EndPoints &ModelDescriptor::endPoints() const
{
    return p->endPoints;
}

void ModelDescriptor::setEndPoints(const EndPoints &newEndPoints)
{
    if (&p->endPoints == &newEndPoints)
        return;
    p->endPoints = &newEndPoints;
    emit endPointsChanged();
}

void ModelDescriptor::resetEndPoints()
{
    p->endPoints.clear();
    emit endPointsChanged();
}

EndPoint &ModelDescriptor::addEndPoint(EndPoint *newEndPoint)
{
    auto v=p->endPoints.value(newEndPoint->uuid());
    if(v){
        if(v==newEndPoint)
            return *newEndPoint;
        delete v;
    }
    newEndPoint->host()->setValues(&p->host);
    p->endPoints.insert(newEndPoint);
    return *newEndPoint;
}

EndPoint &ModelDescriptor::addEndPoint(const QString &name, const QVariant &values)
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
    return this->addEndPoint(endpoint);
}

EndPoint &ModelDescriptor::endPoint() const
{
    return p->endPoint;
}

EndPoint *ModelDescriptor::endPoint(const QUuid &uuid) const
{
    return p->endPoints.value(uuid);
}

ModelDescriptor &ModelDescriptor::setEndPoint(const EndPoint &v)
{
    p->endPoint.setValues(&v);
    return *this;
}

ModelDescriptor &ModelDescriptor::setEndPoint(const QVariant &v)
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

bool ModelDescriptor::obrigatory() const
{
    return p->obrigatory;
}

void ModelDescriptor::setObrigatory(bool newObrigatory)
{
    if (p->obrigatory == newObrigatory)
        return;
    p->obrigatory = newObrigatory;
    emit obrigatoryChanged();
}

void ModelDescriptor::resetObrigatory()
{
    setObrigatory({}); 
}

bool ModelDescriptor::readonly() const
{
    return p->readonly;
}

void ModelDescriptor::setReadonly(bool newReadonly)
{
    if (p->readonly == newReadonly)
        return;
    p->readonly = newReadonly;
    emit readonlyChanged();
}

void ModelDescriptor::resetReadonly()
{
    setReadonly({}); 
}

} // namespace QOrm
