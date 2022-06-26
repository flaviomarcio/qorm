#include "./qmfe_endpoint.h"
#include "./qmfe_host.h"

namespace QMFE {

class EndPointPvt{
public:
    QUuid uuid;
    Host host;
    MetaEnum<Network::Method> method;
    DataBody body;
    QString path;
    QString url;
    explicit EndPointPvt(){

    }
    virtual ~EndPointPvt(){
    }
private:
};

EndPoint::EndPoint(QObject *parent) : ObjectWrapper{parent}
{
    this->p=new EndPointPvt{};
}

bool EndPoint::isValid() const
{
    if(!this->p->host.isValid())
        return false;
    return true;
}

void EndPoint::addContentType(const QString &applicationType) const
{
    Network n;
    auto appType=p->host.headers().value(n.HEADER_CONTENT_TYPE).toString().split(QStringLiteral(","));
    if(appType.size()==1 && appType.first().trimmed().isEmpty())
        appType.clear();
    if(applicationType.trimmed().isEmpty()){
        if(appType.count()==1){
            appType.clear();
            p->host.headers().clear();
        }
        return;
    }
    if(!appType.contains(applicationType))
        appType.append(applicationType);
    p->host.headers().insert(n.HEADER_CONTENT_TYPE, appType.join(','));
}

void EndPoint::addContentTypeJSON() const
{
    Network n;
    return this->addContentType(n.APPLICATION_JSON);
}

void EndPoint::addContentTypeXML() const
{
    Network n;
    return this->addContentType(n.APPLICATION_XML);
}

void EndPoint::addContentTypeTextXML() const
{
    Network n;
    return this->addContentType(n.APPLICATION_TEXT_XML);
}

void EndPoint::addContentTypeX_WWW_FORT_URLENCODED() const
{
    Network n;
    return this->addContentType(n.APPLICATION_X_WWW_FORT_URLENCODED);
}

void EndPoint::addContentTypeDefault() const
{
    Network n;
    return this->addContentType(n.APPLICATION_JSON);
}

QVariantHash EndPoint::contentTypeDefault()
{
    Network n;
    return p->host.headers().value(n.HEADER_CONTENT_TYPE).toHash();
}

QVariantHash &EndPoint::headers()const
{
    return p->host.headers();
}

void EndPoint::addAuthBasic(const QString &userName, const QString &password) const
{
    Network n;
    auto auth=p->host.headers().value(n.HEADER_AUTHORIZATION).toString().split(",");
    if(userName.trimmed().isEmpty() || password.trimmed().isEmpty()){
        if(auth.count()==1)
            p->host.headers().remove(n.HEADER_AUTHORIZATION);
        return;
    }
    auth.append(QStringLiteral("%1 %2").arg(n.AUTHORIZATION_BASIC,userName,password));
    p->host.headers().insert(n.HEADER_AUTHORIZATION, auth.join(','));
}

void EndPoint::addAuthBearer(const QString &credentials) const
{
    Network n;
    auto auth=p->host.headers().value(n.HEADER_AUTHORIZATION).toString().split(",");
    if(credentials.trimmed().isEmpty()){
        if(auth.count()==1)
            p->host.headers().remove(n.HEADER_AUTHORIZATION);
        return;
    }
    auth.append(QStringLiteral("%1 %2").arg(n.AUTHORIZATION_BEARER,credentials));
    p->host.headers().insert(n.HEADER_AUTHORIZATION, auth.join(','));
}

QUuid &EndPoint::uuid() const
{
    return p->uuid;
}

void EndPoint::setUuid(const QUuid &newUuid)
{
    if (p->uuid == newUuid)
        return;
    p->uuid = newUuid;
    emit uuidChanged();
}

void EndPoint::resetUuid()
{
    setUuid({});
}

Host *EndPoint::host()
{
    return &p->host;
}

void EndPoint::setHost(Host *newHost)
{
    this->p->host = newHost;
    emit hostChanged();
}

void EndPoint::resetHost()
{
    setHost({});
}

DataBody *EndPoint::paramerters() const
{
    return &p->body;
}

void EndPoint::setParamerters(const DataBody *newParamerters)
{
    if(&p->body==newParamerters)
        return;
    p->body=newParamerters;
    emit paramertersChanged();
}

void EndPoint::setParamerters(const QVariant &newParamerters)
{
    p->body=newParamerters;
    emit paramertersChanged();
}

void EndPoint::resetParamerters()
{
    setParamerters({});
}

DataBody *EndPoint::body() const
{
    return &p->body;
}

void EndPoint::setBody(const DataBody *newBody)
{
    if(&p->body==newBody)
        return;
    p->body=newBody;
    emit bodyChanged();
}

void EndPoint::setBody(const QVariant &newBody)
{
    auto vh=qvariant_cast<VariantHash*>(newBody);
    p->body=(vh?vh->data():newBody);
    emit bodyChanged();
}

void EndPoint::resetBody()
{
    setParamerters({});
}

const QString &EndPoint::path() const
{
    return p->path;
}

void EndPoint::setPath(const QString &newPath)
{
    if (p->path == newPath)
        return;
    p->path = newPath;
    emit pathChanged();
}

void EndPoint::resetPath()
{
    setPath({});
}

const QString &EndPoint::basePath() const
{
    return this->p->host.basePath();
}

void EndPoint::setBasePath(const QString &newBasePath)
{
    this->p->host.setBasePath(newBasePath);
    emit basePathChanged();
}

void EndPoint::resetBasePath()
{
    setBasePath({});
}

Network::Method EndPoint::method()
{
    return p->method.type();
}

void EndPoint::setMethod(const QVariant &newMethod)
{
    this->p->method=newMethod;
    emit methodChanged();
}

void EndPoint::resetMethod()
{
    setMethod({}); 
}

QString &EndPoint::url()const
{
    if(this->isValid()){
        auto url=QStringLiteral("%1:%2/%3/%4").arg(this->p->host.hostName()).arg(this->p->host.port()).arg(this->basePath(), this->path());
        while(url.contains(QStringLiteral("//")))
            url.replace(QStringLiteral("//"), QStringLiteral("/"));
        p->url=QStringLiteral("%1://%2").arg(this->p->host.protocol(), url);
    }
    else{
        p->url.clear();
    }
    return p->url;
}

} // namespace QMFE
