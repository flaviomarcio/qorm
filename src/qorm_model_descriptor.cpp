#include "./qorm_model_descriptor.h"
//#include "./private/p_qorm_const.h"
#include <QMetaProperty>
#include <QRect>

namespace QOrm {

class ModelDescriptorPvt:public QObject
{
public:
    QStringList descriptorsOrder; //é importa ser QMap devido a ordem necessaria para exibicao
    QString description;
    QVariantMap descriptors; //é importa ser QMap devido a ordem necessaria para exibicao
    QVariantHash edit;
    QVariantHash perfumery;
    QVariantHash flags;
    QVariantHash options;
    QVariantHash sort;
    QVariantHash design={{vpWidth,"20%"}, {vpHeight,"20%"}, {vpRows,2}, {vpLayout, vlVertical}};
    Host host;
    EndPoints endPoints;
    ModelDescriptor *parent=nullptr;
    explicit ModelDescriptorPvt(ModelDescriptor *parent)
        :
          QObject{parent},
          endPoints{parent}
    {

    }
    virtual ~ModelDescriptorPvt() {}
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

    QVariantList vList;
    p->descriptors.clear();
    for (auto &name : p->descriptorsOrder) {
        vList.append(descriptors[name]);
    }    
    descriptors.clear();
    descriptors[vpHeaders]=vList;
    descriptors[vpDesign]=p->design;
    return descriptors;
}

void ModelDescriptor::descriptorsInit()
{
    p->descriptorsOrder.clear();
    p->descriptors.clear();
    Q_ORM_MODEL_SET_DESIGN("30%", "30%", 0, 3, vlVertical)
}

QVariant ModelDescriptor::descriptor(const QString &name) const
{

    return p->descriptors.value(name);
}

void ModelDescriptor::setDescriptor(const QString &name, const QVariantHash &v)
{

    auto d = v;
    if (!p->descriptorsOrder.contains(name))
        p->descriptorsOrder.append(name);
    d[vpField] = name;
    p->descriptors[name] = d;
}

void ModelDescriptor::addDescriptor(const QString &name, const QVariantHash &v)
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

QVariantHash &ModelDescriptor::edit() const
{

    return p->edit;
}

QVariant ModelDescriptor::edit(const QString &name) const
{

    return p->edit.value(name);
}

void ModelDescriptor::setEdit(const QString &name, const QVariantHash &v)
{

    p->edit[name] = v;
}

void ModelDescriptor::addEdit(const QString &name, const QVariantHash &v)
{

    auto d = p->edit.value(name).toHash();
    QHashIterator<QString, QVariant> i(v);
    while (i.hasNext()) {
        i.next();
        d[i.key()] = i.value();
    }
    p->edit[name] = d;
}

QVariantHash &ModelDescriptor::perfumerys() const
{

    return p->perfumery;
}

QVariant ModelDescriptor::perfumery(const QString &name) const
{

    return p->perfumery.value(name);
}

void ModelDescriptor::setPerfumery(const QString &name, const QVariantHash &v)
{

    p->perfumery[name] = v;
}

void ModelDescriptor::addPerfumery(const QString &name, const QVariantHash &v)
{

    auto d = p->perfumery.value(name).toHash();
    QHashIterator<QString, QVariant> i(v);
    while (i.hasNext()) {
        i.next();
        d[i.key()] = i.value();
    }
    p->perfumery[name] = d;
}

QVariantHash &ModelDescriptor::flags() const
{

    return p->flags;
}

QVariant ModelDescriptor::flag(const QString &name) const
{

    return p->perfumery.value(name);
}

void ModelDescriptor::setFlag(const QString &name, const QVariantHash &v)
{

    p->flags[name] = v;
}

void ModelDescriptor::addFlag(const QString &name, const QVariantHash &v)
{

    auto d = p->flags.value(name).toHash();
    QHashIterator<QString, QVariant> i(v);
    while (i.hasNext()) {
        i.next();
        d[i.key()] = i.value();
    }
    p->flags[name] = d;
}

QVariantHash &ModelDescriptor::options() const
{

    return p->options;
}

QVariant ModelDescriptor::option(const QString &name) const
{

    return p->options.value(name);
}

void ModelDescriptor::setOption(const QString &name, const QVariantHash &v)
{

    p->options[name] = v;
}

void ModelDescriptor::addOption(const QString &name, const QVariantHash &v)
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
    endpoint->setValues(values);
    endpoint->setName(name.toUtf8());
    endpoint->host()->setValues(&p->host);
    return this->addEndPoint(endpoint);
}

} // namespace QOrm
