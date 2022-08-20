#include "qorm_model_field_collection.h"

#include <qstring.h>

namespace QOrm {

class ModelFieldCollectionPvt:public QObject{
public:
    QStringList order;
    QVariantList vList;
    ModelFieldCollection *parent=nullptr;
    QList<ModelFieldDescriptor *> list;
    QHash<QString, ModelFieldDescriptor*> collection;

    explicit ModelFieldCollectionPvt(ModelFieldCollection *parent):QObject{parent}
    {
        this->parent=parent;
    }

    void clear()
    {
        order.clear();
        vList.clear();
        list.clear();
        auto aux=collection.values();
        collection.clear();
        qDeleteAll(aux);
    }

    ModelFieldDescriptor &add(const QString &fieldName)
    {
        auto name=fieldName.trimmed().toLower();
        auto &field=collection[name];
        if(!field){
            field=new ModelFieldDescriptor{this->parent};
            field->field(name);
        }
        return *field;
    }
};

ModelFieldCollection::ModelFieldCollection(QObject *parent)
    : QStm::ObjectWrapper{parent}
{
    this->p=new ModelFieldCollectionPvt{this};
}

ModelFieldDescriptor &ModelFieldCollection::item(const QString &fieldName)
{
    return p->add(fieldName);
}

const ModelFieldCollection &ModelFieldCollection::item(const QString &fieldName, const QVariant &values) const
{
    auto field=&p->add(fieldName);
    if(field){
        auto name=field->field().toLower();
        if (!p->order.contains(name))
            p->order.append(name);
        field->mergeFrom(values);
    }
    return *this;
}

const QList<ModelFieldDescriptor *> &ModelFieldCollection::list() const
{
    p->list.clear();
    int order=0;
    for(auto &name : p->order){
        auto field=p->collection.value(name);
        if(!field)
            continue;
        field->order(order++);
        p->list.append(field);
    }
    return p->list;
}

const QVariantList &ModelFieldCollection::toList()const
{
    p->vList.clear();
    auto&list=this->list();
    for(auto &field : list)
        p->vList.append(field->toHash());
    return p->vList;
}

const QVariantList &ModelFieldCollection::items() const
{
    return this->toList();
}

ModelFieldCollection &ModelFieldCollection::setItems(const QVariant &newItems)
{
    p->clear();
    Q_DECLARE_VU;
    auto vList=vu.toList(newItems);
    for(auto &v : vList){
        auto item=ModelFieldDescriptor::from(v, this);
        if(!item)continue;
        p->collection.insert(item->field(), item);
        if(!p->order.contains(item->field()))
            p->order.append(item->field());
    }
    emit itemsChanged();
    return *this;
}

ModelFieldCollection &ModelFieldCollection::resetItems()
{
    return setItems({});
}

} // namespace QOrm
