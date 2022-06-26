#include "./qmfe_endpoints.h"
#include <QVariantHash>

namespace QMFE {

class EndPointsPvt:public QObject{
public:
    QHash<QString, EndPoint*> hash;
    explicit EndPointsPvt(QObject *parent):QObject{parent}
    {
    }
private:
};

EndPoints::EndPoints(QObject *parent)
    : ObjectWrapper{parent}
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
    return ObjectWrapper::setValues(v);
}

void EndPoints::clear()
{
    ObjectWrapper::clear();
    p->hash.clear();
}

int EndPoints::count()const
{
    return p->hash.count();
}

EndPoint *EndPoints::endpoint()
{
    if(p->hash.isEmpty())
        return nullptr;

    return p->hash.cbegin().value();
}

EndPoint *EndPoints::value(const QString &name)
{
    auto obj=p->hash.value(name.trimmed().toLower());
    if(obj)
        return obj;
    return nullptr;
}

EndPoint *EndPoints::method(const Network::Method &method)
{
    QHashIterator<QString, EndPoint*> i(p->hash);
    while(i.hasNext()){
        i.next();
        auto value=i.value();
        if(value->method()==method)
            return value;
    }
    return nullptr;
}

void EndPoints::insert(const QString &name, EndPoint *link)
{
    p->hash.insert(name.trimmed().toLower(), link);
}

void EndPoints::insert(const QString &name, const QVariant &link)
{
    auto _link=link.value<EndPoint*>();
    if(_link==nullptr)
        return;
    p->hash.insert(name.trimmed().toLower(), _link);
}

void EndPoints::remove(const QString &name)
{
    p->hash.remove(name.trimmed());
}


} // namespace QMFE
