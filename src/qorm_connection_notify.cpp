#include "./qorm_connection_notify.h"
#include "./private/p_qorm_connection_notify.h"
#include "./private/p_qorm_connection_notify_subscribe.h"


namespace QOrm {

ConnectionNotify::ConnectionNotify(QObject *parent) : QObject(parent)
{
    this->p = new ConnectionNotifyPvt(this);
}

ConnectionNotify::~ConnectionNotify()
{
    dPvt();
    p.deleteLater();
}

bool ConnectionNotify::notify_send(const QVariant &payload)
{
    dPvt();
    return p.queueSend(QString(),payload);
}

bool ConnectionNotify::notify_send(const QString &channel, const QVariant &payload)
{
    dPvt();
    return p.queueSend(channel,payload);
}


QStringList &ConnectionNotify::subscribeToNotification()
{
    dPvt();
    return p.subscribeToNotification;
}

bool ConnectionNotify::start()
{
    dPvt();
    return p.queueStart();
}

bool ConnectionNotify::stop()
{
    dPvt();
    p.queueStop();
    return true;
}

bool ConnectionNotify::started()
{
    dPvt();
    return p.queueStarted();
}

bool ConnectionNotify::setConnection(const QSqlDatabase&connection)
{
    Q_UNUSED(connection)
    qFatal("falta remover");
    return false;
}

}
