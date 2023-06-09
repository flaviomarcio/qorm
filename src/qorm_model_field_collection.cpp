#include "./qorm_model_field_collection.h"
#include "./qorm_model_field_descriptor.h"

#include <qstring.h>

namespace QOrm {

//static const auto __P="%";

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
        auto &item=collection[name];
        if(!item){
            item=new ModelFieldDescriptor{this->parent};
            item->field(name);
            if (!this->order.contains(name))
                this->order.append(name);
        }
        return *item;
    }

    void remove(const QString &fieldName)
    {
        auto name=fieldName.trimmed().toLower();
        this->order.removeAll(name);
        auto item=collection[name];
        if(item){
            collection.remove(name);
            delete item;
        }

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
    if(fieldName.trimmed().isEmpty())
        return *this;
    auto field=&p->add(fieldName);
    if(field)
        field->mergeFrom(values);
    return *this;
}

void ModelFieldCollection::remove(const QString &fieldName)
{
    p->remove(fieldName);
}

const QList<ModelFieldDescriptor *> &ModelFieldCollection::list() const
{
    p->list.clear();
    //int order=0;
    for(auto &name : p->order){
        auto field=p->collection.value(name);
        if(!field)
            continue;
        //field->order(order++);
        p->list.append(field);
    }
    if(p->list.isEmpty()){
        auto vList=p->collection.values();
        for(auto &field:vList)
            p->list.append(field);
    }
    return p->list;
}

const QVariantList &ModelFieldCollection::toList()const
{
    p->vList.clear();
    auto &list=this->list();
    for(auto &field : list){
        if(field->field().isEmpty())
            continue;
        p->vList.append(field->toHash());
    }
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
        if(!item || item->field().isEmpty())continue;
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
