#include "./p_qorm_model_dto_host.h"

namespace QOrm {

class HostPvt{
public:
    QStm::Network n;
    Host *parent=nullptr;
    QUuid uuid;
    QByteArray protocol=n.HTTP_PROTOCOL;
    QByteArray hostName=n.LOCALHOST;
    int port=-1;
    QVariantHash headers;
    QByteArray basePath;
    explicit HostPvt(Host *parent)
    {
        this->parent=parent;
    }

    virtual ~HostPvt()
    {

    }
};

Host::Host(QObject *parent) : ObjectWrapper{parent}
{
    this->p=new HostPvt{this};
}

Host::~Host()
{
    delete p;
}

bool Host::isValid() const
{
    if(!this->protocol().isEmpty())
        return true;

    if(!this->hostName().isEmpty())
        return true;

    if(!this->basePath().isEmpty())
        return true;

    if(!this->headers().isEmpty())
        return true;

    return {};
}

bool Host::isEmpty() const
{
    if(!this->protocol().isEmpty())
        return true;

    if(!this->hostName().isEmpty())
        return true;

    if(this->port()>0)
        return true;

    if(!this->basePath().isEmpty())
        return true;

    return true;
}

const QByteArray &Host::protocol() const
{

    if(p->protocol.trimmed().isEmpty())
        return p->n.HTTP_PROTOCOL;
    return p->protocol;
}

void Host::setProtocol(const QByteArray &newProtocol)
{
    if (p->protocol == newProtocol)
        return;
    p->protocol = newProtocol;
    emit protocolChanged();
}

void Host::resetProtocol()
{
    setProtocol(QByteArrayLiteral("http"));
}

const QByteArray &Host::hostName() const
{
    if(p->hostName.isEmpty())
        p->hostName=p->n.LOCALHOST;
    return p->hostName;
}

void Host::setHostName(const QByteArray &newHostName)
{
    if (p->hostName == newHostName)
        return;
    p->hostName = newHostName;
    emit hostNameChanged();
}

void Host::resetHostName()
{
    setHostName({});
}

int Host::port() const
{
    if(p->port<=0)
        p->port=p->n.HTTP_PORT_DEFAULT;
    return p->port;
}

void Host::setPort(int newPort)
{
    if (p->port == newPort)
        return;
    p->port = newPort;
    emit portChanged();
}

void Host::resetPort()
{
    setPort(-1);
}

QVariantHash &Host::headers() const
{
    return p->headers;
}

void Host::setHeaders(const QVariantHash &newHeaders)
{
    if (p->headers == newHeaders)
        return;
    p->headers = newHeaders;
    emit headersChanged();
}

void Host::addHeaders(const QVariantHash &newHeaders)
{
    QHashIterator <QString, QVariant> i(newHeaders);
    while(i.hasNext()){
        i.next();
        p->headers.insert(i.key(), i.value());
    }
    emit headersChanged();
}

void Host::resetHeaders()
{
    setHeaders({});
}

const QByteArray &Host::basePath() const
{
    if(p->basePath.trimmed().isEmpty())
        p->basePath=QByteArrayLiteral("/");
    return p->basePath;
}

void Host::setBasePath(const QByteArray &newBasePath)
{
    if (p->basePath == newBasePath)
        return;
    p->basePath = newBasePath;
    emit basePathChanged();
}

void Host::resetBasePath()
{
    setBasePath({});
}

const QUuid &Host::uuid() const
{
    return p->uuid;
}

void Host::setUuid(const QUuid &newUuid)
{
    if (p->uuid == newUuid)
        return;
    p->uuid = newUuid;
    emit uuidChanged();
}

void Host::resetUuid()
{
    setUuid({}); 
}

} // namespace QOrm
