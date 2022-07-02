#include "./p_qorm_model_dto_endpoints.h"
#include <QVariantHash>

namespace QOrm {

class EndPointsPvt:public QObject{
public:
    QVariantList list;
    QVariantHash hash;
    QVector<EndPoint*> objectList;
    QHash<QString, EndPoint*> objectHash;
    explicit EndPointsPvt(QObject *parent):QObject{parent}
    {
    }

    ~EndPointsPvt()
    {
        this->clear();
    }

    void clear()
    {
        this->hash.clear();
        this->list.clear();
        this->objectList.clear();
        auto aux=objectHash.values();
        objectHash.clear();
        qDeleteAll(aux);
    }
private:
};

EndPoints::EndPoints(QObject *parent)
    : QStm::ObjectWrapper{parent}
{
    this->p=new EndPointsPvt{this};
}

bool EndPoints::setValues(const QVariant &v)
{
    for(auto&v:v.toList()){
        auto vHash=v.toHash();
        auto name=vHash.value(QStringLiteral("endpoint")).toString();
        this->insert(name, vHash);
    }
    return QStm::ObjectWrapper::setValues(v);
}

void EndPoints::clear()
{
    QStm::ObjectWrapper::clear();
    p->clear();
}

int EndPoints::count()const
{
    return p->objectHash.count();
}

EndPoint *EndPoints::endpoint()
{
    if(p->objectHash.isEmpty())
        return nullptr;

    return p->objectHash.cbegin().value();
}

EndPoint *EndPoints::value(const QString &name)
{
    auto obj=p->objectHash.value(name.trimmed().toLower());
    if(obj)
        return obj;
    return nullptr;
}

EndPoint *EndPoints::method(const QVariant &method)
{
    QHashIterator<QString, EndPoint*> i(p->objectHash);
    while(i.hasNext()){
        i.next();
        auto value=i.value();
        if(value->method()==method)
            return value;
    }
    return nullptr;
}

void EndPoints::insert(const QString &name, EndPoint *endPoint)
{
    if(!endPoint) return;
    if(endPoint->parent()!=this)
        endPoint->setParent(this);
    p->objectHash.insert(name.trimmed().toLower(), endPoint);
}

void EndPoints::insert(const QString &name, const QVariant &link)
{
    auto _link=link.value<EndPoint*>();
    if(_link==nullptr)
        return;
    p->objectHash.insert(name.trimmed().toLower(), _link);
}

void EndPoints::remove(const QString &name)
{
    p->objectHash.remove(name.trimmed());
}

QVariantList &EndPoints::items() const
{
    p->list.clear();
    if(p->objectHash.isEmpty())
        return p->list;
    auto vList=p->objectHash.values();
    for(auto&v:vList){
        auto e=v->toHash();
        p->list.append(e);
    }
    return p->list;
}

void EndPoints::setItems(const QVariant &newItems)
{
    p->clear();
    for(auto&v:newItems.toList()){
        auto e=EndPoint::from(v, this);
        if(!e) continue;
        this->insert(e->name(), e);
    }
    emit itemsChanged();
}

void EndPoints::resetItems()
{
    setItems({});
}

QVariantList &EndPoints::toList() const
{
    return this->items();
}

QVector<EndPoint *> &EndPoints::toObjectList() const
{
    p->objectList=p->objectHash.values();
    return p->objectList;
}

QHash<QString, EndPoint *> &EndPoints::toObjectHash() const
{
    return p->objectHash;
}


} // namespace QOrm
