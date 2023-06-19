#include "./p_qorm_task_slot.h"
#include "./p_qorm_task_pool.h"
#include "../qorm_task_runner.h"
#include "../qorm_connection_pool.h"
#include <QSqlError>

namespace QOrm {

static const auto __request="request";
static const auto __error="error";
static const auto __response="response";

class TaskSlotPvt : public QObject
{
public:
    int slotNumber=-1;
    QVariantList taskQueueValue;
    QVariantList resultList;
    TaskRunnerMethod methodExecute;
    TaskRunnerMethod methodSuccess;
    TaskRunnerMethod methodFailed;
    QVariantHash connectionSetting;
    bool resultBool;
    TaskSlot *parent = nullptr;
    TaskPool *pool = nullptr;
    const TaskRunner *runner = nullptr;

    explicit TaskSlotPvt(TaskSlot *parent, TaskPool *pool)
        :
        QObject{parent},
        parent{parent},
        pool{pool},
        runner{pool->runner()}
    {
        this->signalConnect();
    }

    ~TaskSlotPvt()
    {
        this->signalDisconnect();
    }

    void signalConnect()
    {
        QObject::connect(this->parent, &TaskSlot::taskSend, this, &TaskSlotPvt::onTaskSend);
        QObject::connect(this->parent, &TaskSlot::taskSuccess, this->pool, &TaskPool::taskResponse);
        QObject::connect(this->parent, &TaskSlot::taskError, this->pool, &TaskPool::taskResponse);
        QObject::connect(this->parent, &TaskSlot::taskStart, this->runner, &TaskRunner::taskStart);
        QObject::connect(this->parent, &TaskSlot::taskState, this->runner, &TaskRunner::taskState);
        QObject::connect(this->parent, &TaskSlot::taskSuccess, this->runner, &TaskRunner::taskSuccess);
        QObject::connect(this->parent, &TaskSlot::taskError, this->runner, &TaskRunner::taskError);
    }

    void signalDisconnect()
    {
        QObject::disconnect(this->parent, &TaskSlot::taskSend, this, &TaskSlotPvt::onTaskSend);
        QObject::disconnect(this->parent, &TaskSlot::taskSuccess, this->pool, &TaskPool::taskResponse);
        QObject::disconnect(this->parent, &TaskSlot::taskError, this->pool, &TaskPool::taskResponse);
        QObject::disconnect(this->parent, &TaskSlot::taskStart, this->runner, &TaskRunner::taskStart);
        QObject::disconnect(this->parent, &TaskSlot::taskState, this->runner, &TaskRunner::taskState);
        QObject::disconnect(this->parent, &TaskSlot::taskSuccess, this->runner, &TaskRunner::taskSuccess);
        QObject::disconnect(this->parent, &TaskSlot::taskError, this->runner, &TaskRunner::taskError);
    }

private slots:

    void onTaskSend(const QVariant &task)
    {

        QOrm::ConnectionPool cnnPool(this->connectionSetting, this);
        QSqlDatabase connection;

        auto connectionCheck=[&cnnPool, &connection]()
        {
            if (!cnnPool.isValid())
                return cnnPool.finish(connection);

            if (!cnnPool.get(connection))
                return !cnnPool.finish(connection);

            return connection.isOpen();
        };

        QVariantHash vTask={{__request, task}};
        emit this->parent->taskStart(vTask);
        if (!connectionCheck()) {
            static const auto __msg=QStringLiteral("Invalid connection on Slot[%1]");
            vTask.insert(__error, __msg.arg(this->slotNumber));
            this->methodFailed(connection, task);
            emit this->parent->taskError(this->slotNumber, vTask);
            return;
        }

        auto response = this->methodExecute(connection, task);
        vTask.insert(__response, response);
        auto r = this->methodSuccess(connection, response);
        vTask.insert(__response, r);

        emit this->parent->taskSuccess(this->slotNumber, vTask);
        cnnPool.finish(connection);
    }
};

TaskSlot::TaskSlot(QObject *parent): QThread{nullptr}, p{new TaskSlotPvt{this, dynamic_cast<TaskPool*>(parent)}}
{

}

TaskSlot::TaskSlot(TaskPool *pool, int slotNumber,
                   const QVariantHash &connectionSetting,
                   TaskRunnerMethod methodExecute,
                   TaskRunnerMethod methodSuccess,
                   TaskRunnerMethod methodFailed)
    : QThread{nullptr}, p{new TaskSlotPvt{this, pool}}
{
    this->moveToThread(this);

    p->slotNumber=slotNumber;
    p->connectionSetting = connectionSetting;
    p->methodExecute = methodExecute;
    p->methodSuccess = methodSuccess;
    p->methodFailed = methodFailed;
}

int TaskSlot::slotNumber()
{
    return p->slotNumber;
}

bool TaskSlot::start()
{
    if (this->isRunning())
        return true;
    QThread::start();
    while (this->eventDispatcher() == nullptr)
        this->msleep(1);

    return true;
}

void TaskSlot::run()
{
    this->exec();
}

} // namespace QOrm
