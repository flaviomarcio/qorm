#include "./p_qorm_model_dto_host.h"

namespace QOrm {

Host::Host(QObject *parent) : QStm::ObjectWrapper{parent}
{

}

bool Host::isValid() const
{
    if(!this->protocol().isEmpty())
        return true;

    if(!this->hostName().isEmpty())
        return true;

    if(this->port()>0)
        return true;

    if(!this->basePath().isEmpty())
        return true;

    return {};
}

bool Host::isEmpty() const
{
    if(this->protocol().isEmpty())
        return {};

    if(this->hostName().isEmpty())
        return {};

    if(this->port()<=0)
        return {};

    if(this->basePath().isEmpty())
        return {};

    return true;
}

const QString &Host::protocol() const
{
    return _protocol;
}

void Host::setProtocol(const QString &newProtocol)
{
    if (_protocol == newProtocol)
        return;
    _protocol = newProtocol;
    emit protocolChanged();
}

void Host::resetProtocol()
{
    setProtocol(QStringLiteral("http"));
}

const QString &Host::hostName() const
{
    return _hostName;
}

void Host::setHostName(const QString &newHostName)
{
    if (_hostName == newHostName)
        return;
    _hostName = newHostName;
    emit hostNameChanged();
}

void Host::resetHostName()
{
    setHostName(QStringLiteral("${HOST}"));
}

int Host::port() const
{
    return _port;
}

void Host::setPort(int newPort)
{
    if (_port == newPort)
        return;
    _port = newPort;
    emit portChanged();
}

void Host::resetPort()
{
    setPort(-1);
}

QVariantHash &Host::headers()
{
    return _headers;
}

void Host::setHeaders(const QVariantHash &newHeaders)
{
    if (_headers == newHeaders)
        return;
    _headers = newHeaders;
    emit headersChanged();
}

void Host::resetHeaders()
{
    setHeaders({});
}

const QString &Host::basePath() const
{
    return _basePath;
}

void Host::setBasePath(const QString &newBasePath)
{
    if (_basePath == newBasePath)
        return;
    _basePath = newBasePath;
    emit basePathChanged();
}

void Host::resetBasePath()
{
    setBasePath(QStringLiteral("/"));
}

} // namespace QOrm
