#include "./qorm_connection_notify.h"
#include "./private/p_qorm_connection_notify.h"
#include "./private/p_qorm_connection_notify_subscribe.h"

namespace QOrm {

ConnectionNotify::ConnectionNotify(QObject *parent) : QObject{parent}
{
    this->p = new ConnectionNotifyPvt{this};
}

ConnectionNotify::~ConnectionNotify()
{

}

bool ConnectionNotify::notify_send(const QVariant &payload)
{

    return p->queueSend(QString(), payload);
}

bool ConnectionNotify::notify_send(const QString &channel, const QVariant &payload)
{

    return p->queueSend(channel, payload);
}

QStringList &ConnectionNotify::subscribeToNotification()
{

    return p->subscribeToNotification;
}

bool ConnectionNotify::start()
{

    return p->queueStart();
}

bool ConnectionNotify::stop()
{

    p->queueStop();
    return true;
}

bool ConnectionNotify::started()
{

    return p->queueStarted();
}

bool ConnectionNotify::setConnection(const QSqlDatabase &connection)
{
    Q_UNUSED(connection)
    qFatal("falta remover");
    return false;
}

} // namespace QOrm
