#include "./p_qorm_model_dto_endpoint.h"
#include "./p_qorm_model_dto_host.h"
#include <QJsonDocument>
#include "../../../qstm/src/qstm_network_types.h"
#include "../../../qstm/src/qstm_util_variant.h"

namespace QOrm {

class EndPointPvt:public QObject{
public:
    QUuid uuid;
    Host host;
    QVariant method;
    QByteArray path;
    QByteArray url;
    QByteArray name;
    QVariantHash args;
    explicit EndPointPvt(QObject *parent):QObject{parent}{

    }

private:
};

EndPoint::EndPoint(QObject *parent) : QStm::ObjectWrapper{parent}, p{new EndPointPvt{this}}
{
}

bool EndPoint::isValid() const
{
    if(!this->p->host.isValid())
        return false;
    return true;
}

void EndPoint::addContentType(const QByteArray &applicationType) const
{
    QStm::Network n;
    auto appType=p->host.headers().value(n.HEADER_CONTENT_TYPE).toString().split(QByteArrayLiteral(","));
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

void EndPoint::addAuthBasic(const QByteArray &userName, const QByteArray &password) const
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

void EndPoint::addAuthBearer(const QByteArray &credentials) const
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

const QUuid &EndPoint::uuid()const
{
    Q_DECLARE_VU;
    if(p->uuid.isNull())
        p->uuid=QUuid::createUuidV3(QUuid::createUuid(), QByteArray{__func__});
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

Host *EndPoint::host()const
{
    return &p->host;
}

void EndPoint::setHost(const Host *newHost)
{
    this->p->host = newHost;
    emit hostChanged();
}

void EndPoint::resetHost()
{
    setHost({});
}

const QByteArray &EndPoint::path() const
{
    return p->path;
}

void EndPoint::setPath(const QByteArray &newPath)
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

const QByteArray &EndPoint::basePath() const
{
    return this->p->host.basePath();
}

void EndPoint::setBasePath(const QByteArray &newBasePath)
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

QByteArray &EndPoint::url()const
{
    QStm::Network n;
    if(this->isValid()){
        auto &h=p->host;
        p->url=n.urlParsert(h.protocol(), h.hostName(), h.port(), h.basePath(), this->path());
    }
    else{
        p->url.clear();
    }
    return p->url;
}

const QByteArray &EndPoint::name() const
{
    return p->name;
}

void EndPoint::setName(const QByteArray &newName)
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

QVariantHash &EndPoint::args()
{
    return p->args;
}

void EndPoint::setArgs(const QVariant &newArgs)
{
    if (p->args == newArgs)
        return;
    Q_DECLARE_VU;
    p->args.clear();
    auto v=vu.toHash(newArgs);
    this->args(v);
}

EndPoint &EndPoint::args(const QVariantPair &newArgs)
{
    auto v=QVariantHash{{newArgs.first.toString(), newArgs.second}};
    return this->args(v);
}

EndPoint &EndPoint::args(const QVariantHash &newArgs)
{
    if(newArgs.isEmpty())
        return *this;

    QHashIterator<QString, QVariant> i(newArgs);
    while(i.hasNext()){
        i.next();
        auto value=i.value().toString().trimmed().toLower();
        p->args.insert(i.key().toLower().trimmed(), value);
    }
    emit argsChanged();
    return *this;
}

void EndPoint::resetArgs()
{
    setArgs({});
}

} // namespace QOrm
