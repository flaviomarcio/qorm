#include "qorm_model_action_collection.h"

#include <qstring.h>

namespace QOrm {

class ModelActionCollectionPvt:public QObject{
public:
    QStringList order;
    QVariantList vList;
    ModelActionCollection *parent=nullptr;
    QList<ModelActionDescriptor *> list;
    QHash<QString, ModelActionDescriptor*> collection;

    explicit ModelActionCollectionPvt(ModelActionCollection *parent):QObject{parent}
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

    ModelActionDescriptor &add(const QString &actionName)
    {
        auto name=actionName.trimmed().toLower();
        auto &item=collection[name];
        if(!item){
            item=new ModelActionDescriptor{this->parent};
            item->action(name);
        }
        return *item;
    }
};

ModelActionCollection::ModelActionCollection(QObject *parent)
    : QStm::ObjectWrapper{parent}
{
    this->p=new ModelActionCollectionPvt{this};
}

ModelActionDescriptor &ModelActionCollection::item(const QString &actionName)
{
    return p->add(actionName);
}

const ModelActionCollection &ModelActionCollection::item(const QString &actionName, const QVariant &values) const
{
    if(actionName.trimmed().isEmpty())
        return *this;
    auto action=&p->add(actionName);
    if(action){
        auto name=action->action().toLower();
        if (!p->order.contains(name))
            p->order.append(name);
        action->mergeFrom(values);
    }
    return *this;
}

const QList<ModelActionDescriptor *> &ModelActionCollection::list() const
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
    return p->list;
}

const QVariantList &ModelActionCollection::toList()const
{
    p->vList.clear();
    auto&list=this->list();
    for(auto &field : list){
        if(field->action().isEmpty())
            continue;
        p->vList.append(field->toHash());
    }
    return p->vList;
}

const QVariantList &ModelActionCollection::items() const
{
    return this->toList();
}

ModelActionCollection &ModelActionCollection::setItems(const QVariant &newItems)
{
    p->clear();
    Q_DECLARE_VU;
    auto vList=vu.toList(newItems);
    for(auto &v : vList){
        auto item=ModelActionDescriptor::from(v, this);
        if(!item || item->action().isEmpty())continue;
        p->collection.insert(item->action(), item);
        if(!p->order.contains(item->action()))
            p->order.append(item->action());
    }
    emit itemsChanged();
    return *this;
}

ModelActionCollection &ModelActionCollection::resetItems()
{
    return setItems({});
}

} // namespace QOrm
