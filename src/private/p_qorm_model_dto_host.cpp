#include "./p_qorm_model_dto_host.h"

namespace QOrm {

class HostPvt{
public:
    QStm::Network n;
    Host *parent=nullptr;
    QString protocol=n.HTTP_PROTOCOL;
    QString hostName=n.LOCALHOST;
    int port=-1;
    QVariantHash headers;
    QString basePath;
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

const QString &Host::protocol() const
{

    if(p->protocol.trimmed().isEmpty())
        return p->n.HTTP_PROTOCOL;
    return p->protocol;
}

void Host::setProtocol(const QString &newProtocol)
{
    if (p->protocol == newProtocol)
        return;
    p->protocol = newProtocol;
    emit protocolChanged();
}

void Host::resetProtocol()
{
    setProtocol(QStringLiteral("http"));
}

const QString &Host::hostName() const
{
    if(p->hostName.isEmpty())
        p->hostName=p->n.LOCALHOST;
    return p->hostName;
}

void Host::setHostName(const QString &newHostName)
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

const QString &Host::basePath() const
{
    if(p->basePath.trimmed().isEmpty())
        p->basePath=QStringLiteral("/");
    return p->basePath;
}

void Host::setBasePath(const QString &newBasePath)
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

} // namespace QOrm
