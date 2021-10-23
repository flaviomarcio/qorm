#pragma once

#include "./qorm_object_db.h"

namespace QOrm {

class Q_ORM_EXPORT ConnectionNotify : public QObject
{
    Q_OBJECT
public:
    Q_INVOKABLE explicit ConnectionNotify(QObject *parent = nullptr);
    Q_INVOKABLE ~ConnectionNotify();
public:
    Q_INVOKABLE virtual bool notify_send(const QVariant &payload);
    Q_INVOKABLE virtual bool notify_send(const QString &channel, const QVariant &payload);

    Q_INVOKABLE virtual QStringList &subscribeToNotification();

    Q_INVOKABLE virtual bool start();
    Q_INVOKABLE virtual bool stop();
    Q_INVOKABLE virtual bool started();

    Q_INVOKABLE virtual bool setConnection(const QSqlDatabase &connection);
signals:
    void notification(const QString &channel, const QVariant &payload);
private:
    void*p=nullptr;
};

} // namespace QOrm
