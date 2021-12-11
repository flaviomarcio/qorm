#pragma once

#include <QTimer>
#include <QObject>
#include <QString>
#include <QVariant>
#include <QDateTime>
#include <QUrl>
#include <QUuid>
#include <QThread>
#include <QSqlDatabase>
#include "./qorm_global.h"
#include "./qorm_connection_pool.h"

namespace QOrm {

typedef std::function<QVariant(QSqlDatabase&db, const QVariant&task)> TaskRunnerMethod;
class TaskPool;
class TaskRunner;
class Q_ORM_EXPORT TaskSlot : public QThread
{
    Q_OBJECT
public:
    QVariantList taskQueueValue;
    QVariantList resultList;
    TaskRunnerMethod methodExecute;
    TaskRunnerMethod methodSuccess;
    TaskRunnerMethod methodFailed;
    QVariantHash connectionSetting;
    bool resultBool;
    TaskPool*pool=nullptr;
    TaskRunner*runner=nullptr;
    QSqlDatabase connection;
    QOrm::ConnectionPool cnnPool;
    explicit TaskSlot(TaskPool*pool, const QVariantHash&connectionSetting, TaskRunnerMethod methodExecute, TaskRunnerMethod methodSuccess, TaskRunnerMethod methodFailed);
    Q_INVOKABLE ~TaskSlot();

    bool start();

    void run() override;
    void init();
signals:
    void taskSend(const QVariant&task);
    void taskRequest(QOrm::TaskSlot*slot);

    void taskStart(const QVariantHash&task);
    void taskState(const QVariantHash&task);
    void taskError(const QVariantHash&task);
    void taskSuccess(const QVariantHash&task);
private slots:
    void on_taskRequest();
    void on_taskSend(const QVariant&task);
private:
    void*p=nullptr;
    bool connectionCheck();
};

}
