#include "./p_qorm_model_dto_endpoints.h"
#include <QVariantHash>

namespace QOrm {

class EndPointsPvt:public QObject{
public:
    QVariantList list;
    QVariantHash hash;
    QVector<EndPoint*> objectList;
    QHash<QString, EndPoint*> objectHash;
    Host host;
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

    void setHost()
    {
        QHashIterator<QString, EndPoint*> i(objectHash);
        while (i.hasNext()){
            i.next();
            i.value()->host()->setValues(&host);
        }
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
    for(auto&v:v.toList())
        this->insert(v);
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

Host &EndPoints::host()
{
    return p->host;
}

void EndPoints::host(const Host &newHost)
{
    p->host = &newHost;
    emit hostChanged();
}

void EndPoints::host(const QVariant &newHost)
{
    p->host = newHost;
    emit hostChanged();
}

void EndPoints::setHost(const Host &newHost)
{
    p->host = &newHost;
    emit hostChanged();
}

void EndPoints::resetHost()
{
    p->host.clear();
    emit hostChanged();
}

EndPoint *EndPoints::endpoint()
{
    if(p->objectHash.isEmpty())
        return nullptr;

    return p->objectHash.cbegin().value();
}

EndPoint *EndPoints::value(const QUuid &uuid)
{
    auto obj=p->objectHash.value(uuid.toString());
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

void EndPoints::insert(EndPoint *endPoint)
{
    if(!endPoint) return;
    if(endPoint->parent()!=this)
        endPoint->setParent(this);
    p->objectHash.insert(endPoint->uuid().toString(), endPoint);
}

void EndPoints::insert(const QVariant &endPoint)
{
    auto endpoint=endPoint.value<EndPoint*>();
    if(endpoint==nullptr)
        endpoint=EndPoint::from(endPoint, this);
    if(!endpoint)return;
    p->objectHash.insert(endpoint->uuid().toString(), endpoint);
}

void EndPoints::remove(const QUuid &uuid)
{
    p->objectHash.remove(uuid.toString());
}

QVariantList &EndPoints::items() const
{
    p->list.clear();
    p->setHost();
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
        this->insert(e);
    }
    emit itemsChanged();
}

void EndPoints::resetItems()
{
    setItems({});
}

const QVariantList EndPoints::toList()
{
    return this->items();
}

const QVariantHash EndPoints::toHash()
{
    p->setHost();
    QVariantHash hash;
    QHashIterator<QString, EndPoint*> i(p->objectHash);
    while (i.hasNext()){
        i.next();
        hash.insert(i.value()->uuid().toString(), i.value()->toHash());
    }
    return hash;
}

QVector<EndPoint *> &EndPoints::toObjectList() const
{
    p->setHost();
    p->objectList=p->objectHash.values();
    return p->objectList;
}

QHash<QString, EndPoint *> &EndPoints::toObjectHash() const
{
    p->setHost();
    return p->objectHash;
}


} // namespace QOrm
