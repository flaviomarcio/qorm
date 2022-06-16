#include "./p_qorm_task_slot.h"
#include "./p_qorm_task_pool.h"

namespace QOrm {

class TaskSlotPvt : public QObject
{
public:
    QVariantList taskQueueValue;
    QVariantList resultList;
    TaskRunnerMethod methodExecute;
    TaskRunnerMethod methodSuccess;
    TaskRunnerMethod methodFailed;
    QVariantHash connectionSetting;
    bool resultBool;
    TaskSlot *parent = nullptr;
    TaskPool *pool = nullptr;
    TaskRunner *runner = nullptr;
    QSqlDatabase connection;
    QOrm::ConnectionPool cnnPool;

    explicit TaskSlotPvt(TaskSlot *parent) : QObject{parent} { this->parent = parent; }

    virtual ~TaskSlotPvt() { this->signalDisconnect(); }

    void signalConnect()
    {
        QObject::connect(this->parent, &TaskSlot::taskSend, this, &TaskSlotPvt::onTaskSend);
        QObject::connect(this->parent, &TaskSlot::taskRequest, this->pool, &TaskPool::taskRequest);
        QObject::connect(this->parent, &TaskSlot::taskSuccess, this->pool, &TaskPool::taskResponse);
        QObject::connect(this->parent, &TaskSlot::taskError, this->pool, &TaskPool::taskResponse);
        QObject::connect(this->parent, &TaskSlot::taskStart, this->runner, &TaskRunner::taskStart);
        QObject::connect(this->parent, &TaskSlot::taskState, this->runner, &TaskRunner::taskState);
        QObject::connect(this->parent,
                         &TaskSlot::taskSuccess,
                         this->runner,
                         &TaskRunner::taskSuccess);
        QObject::connect(this->parent, &TaskSlot::taskError, this->runner, &TaskRunner::taskError);
    }

    void signalDisconnect()
    {
        QObject::disconnect(this->parent, &TaskSlot::taskSend, this, &TaskSlotPvt::onTaskSend);
        QObject::disconnect(this->parent,
                            &TaskSlot::taskRequest,
                            this->pool,
                            &TaskPool::taskRequest);
        QObject::disconnect(this->parent,
                            &TaskSlot::taskSuccess,
                            this->pool,
                            &TaskPool::taskResponse);
        QObject::disconnect(this->parent, &TaskSlot::taskError, this->pool, &TaskPool::taskResponse);
        QObject::disconnect(this->parent,
                            &TaskSlot::taskStart,
                            this->runner,
                            &TaskRunner::taskStart);
        QObject::disconnect(this->parent,
                            &TaskSlot::taskState,
                            this->runner,
                            &TaskRunner::taskState);
        QObject::disconnect(this->parent,
                            &TaskSlot::taskSuccess,
                            this->runner,
                            &TaskRunner::taskSuccess);
        QObject::disconnect(this->parent,
                            &TaskSlot::taskError,
                            this->runner,
                            &TaskRunner::taskError);
    }

    bool connectionCheck()
    {
        if (this->connection.isValid()) {
            if (!this->connection.isOpen())
                this->connection.open();
            return this->connection.isOpen();
        }

        if (!cnnPool.isValid())
            return this->cnnPool.finish(this->connection);

        if (!cnnPool.get(connection))
            return !this->cnnPool.finish(this->connection);

        return this->connection.isOpen();
    }

private slots:
    void onTaskRequest() { emit this->parent->taskRequest(this->parent); }

    void onTaskSend(const QVariant &task)
    {
        QVariantHash vTask;
        vTask[qsl("request")] = task;
        emit this->parent->taskStart(vTask);
        if (!this->connectionCheck()) {
            vTask[qsl("error")] = qsl("Invalid connection on Slot");
            this->methodFailed(this->connection, task);
            emit this->parent->taskError(vTask);
            emit this->parent->taskRequest(this->parent);
            return;
        }

        auto response = this->methodExecute(this->connection, task);
        vTask[qsl("response")] = response;
        this->connection.close();
        if (!this->connection.open()) {
            vTask[qsl("error")] = connection.lastError().text();
            this->methodFailed(this->connection, vTask);
        } else {
            auto r = this->methodSuccess(this->connection, response);
            vTask[qsl("response")] = r;
        }
        emit this->parent->taskSuccess(vTask);
        emit this->parent->taskRequest(this->parent);
        this->connection.close();
    }
};

TaskSlot::TaskSlot(TaskPool *pool,
                   const QVariantHash &connectionSetting,
                   TaskRunnerMethod methodExecute,
                   TaskRunnerMethod methodSuccess,
                   TaskRunnerMethod methodFailed)
    : QThread(nullptr)
{
    this->p = new TaskSlotPvt{this};
    this->moveToThread(this);

    p->pool = pool;
    p->runner = pool->runner();
    p->connectionSetting = connectionSetting;
    p->methodExecute = methodExecute;
    p->methodSuccess = methodSuccess;
    p->methodFailed = methodFailed;
    p->cnnPool.setting() = p->connectionSetting;
    p->signalConnect();
}

TaskSlot::~TaskSlot()
{
}

bool TaskSlot::start()
{
    QThread::start();
    while (this->eventDispatcher() == nullptr)
        this->msleep(1);
    return true;
}

void TaskSlot::run()
{

    if (!p->connectionCheck()) {
        this->quit();
        return;
    }

    p->connection.close();
    this->exec();
    p->cnnPool.finish(p->connection);
}

void TaskSlot::init()
{
    if (!this->isRunning())
        this->start();
    emit this->taskRequest(this);
}

} // namespace QOrm
