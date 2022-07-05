#include "./p_qorm_model_dto_endpoint.h"
#include "./p_qorm_model_dto_host.h"
#include <QJsonDocument>

namespace QOrm {

class EndPointPvt{
public:
    QUuid uuid;
    Host host;
    QVariant method;
    QString path;
    QString url;
    QString name;
    explicit EndPointPvt(){

    }
    virtual ~EndPointPvt(){
    }
private:
};

EndPoint::EndPoint(QObject *parent) : QStm::ObjectWrapper{parent}
{
    this->p=new EndPointPvt{};
}

EndPoint::~EndPoint()
{
    delete p;
}

bool EndPoint::isValid() const
{
    if(!this->p->host.isValid())
        return false;
    return true;
}

void EndPoint::addContentType(const QString &applicationType) const
{
    QStm::Network n;
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
    QStm::Network n;
    return this->addContentType(n.APPLICATION_JSON);
}

void EndPoint::addContentTypeXML() const
{
    QStm::Network n;
    return this->addContentType(n.APPLICATION_XML);
}

void EndPoint::addContentTypeTextXML() const
{
    QStm::Network n;
    return this->addContentType(n.APPLICATION_TEXT_XML);
}

void EndPoint::addContentTypeX_WWW_FORT_URLENCODED() const
{
    QStm::Network n;
    return this->addContentType(n.APPLICATION_X_WWW_FORT_URLENCODED);
}

void EndPoint::addContentTypeDefault() const
{
    QStm::Network n;
    return this->addContentType(n.APPLICATION_JSON);
}

QVariantHash EndPoint::contentTypeDefault()
{
    QStm::Network n;
    return p->host.headers().value(n.HEADER_CONTENT_TYPE).toHash();
}

QVariantHash &EndPoint::headers()const
{
    return p->host.headers();
}

void EndPoint::addAuthBasic(const QString &userName, const QString &password) const
{
    QStm::Network n;
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
    QStm::Network n;
    auto auth=p->host.headers().value(n.HEADER_AUTHORIZATION).toString().split(",");
    if(credentials.trimmed().isEmpty()){
        if(auth.count()==1)
            p->host.headers().remove(n.HEADER_AUTHORIZATION);
        return;
    }
    auth.append(QStringLiteral("%1 %2").arg(n.AUTHORIZATION_BEARER,credentials));
    p->host.headers().insert(n.HEADER_AUTHORIZATION, auth.join(','));
}

const QUuid &EndPoint::uuid()
{
    if(p->uuid.isNull()){
        Q_DECLARE_VU;
        auto vHash=this->extractHash(this, QStringList{{__func__}});
        p->uuid=vu.toUuid(vHash);
    }
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

QVariant &EndPoint::method()const
{
    return p->method;
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
        auto port=this->p->host.port()<=0?"":QStringLiteral(":%1").arg(this->p->host.port());
        auto url=QStringLiteral("%1%2/%3/%4").arg(this->p->host.hostName()).arg(port).arg(this->basePath(), this->path());
        while(url.contains(QStringLiteral("//")))
            url.replace(QStringLiteral("//"), QStringLiteral("/"));
        p->url=QStringLiteral("%1://%2").arg(this->p->host.protocol(), url);
    }
    else{
        p->url.clear();
    }
    return p->url;
}

const QString &EndPoint::name() const
{
    return p->name;
}

void EndPoint::setName(const QString &newName)
{
    if (p->name == newName.trimmed())
        return;
    p->name = newName.trimmed();
    emit nameChanged();
}

void EndPoint::resetName()
{
    setName({}); 
}

} // namespace QOrm
