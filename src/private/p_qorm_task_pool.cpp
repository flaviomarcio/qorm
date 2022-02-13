#include "./p_qorm_task_pool.h"

static QVariant __methodExecute(QSqlDatabase&db, const QVariant&task){Q_UNUSED(task) Q_UNUSED(db) return {};};
static QVariant __methodSuccess(QSqlDatabase&db, const QVariant&task){Q_UNUSED(task) Q_UNUSED(db) return task;};
static QVariant __methodFailed(QSqlDatabase&db, const QVariant&task){Q_UNUSED(task) Q_UNUSED(db) return task;};

namespace QOrm {

#define dPvt()\
auto&p = *reinterpret_cast<TaskPoolPvt*>(this->p)

class TaskPoolPvt{
public:
    QMutex running;
    TaskRunner *runner=nullptr;
    int slotCount=QThread::idealThreadCount();
    TaskRunnerMethod methodExecute=nullptr;
    TaskRunnerMethod methodSuccess=nullptr;
    TaskRunnerMethod methodFailed=nullptr;
    QHash<int, TaskSlot*> taskSlotList;
    QMap<QUuid, QVariant> taskQueueResponse;
    QVariantList taskQueueValue;
    QVariantList taskQueueStarted;
    QVariantList resultList;
    bool resultBool=false;
    QVariantHash connectionSetting;
    explicit TaskPoolPvt(QObject*)
    {
    }
    virtual ~TaskPoolPvt()
    {
    }
};

TaskPool::TaskPool(TaskRunner *parent):QThread(nullptr)
{
    this->p=new TaskPoolPvt(this);
    this->moveToThread(this);
    dPvt();
    p.runner=parent;
    QObject::connect(this, &TaskPool::taskProgress, parent, &TaskRunner::taskProgress);
    this->clear();
}

TaskPool::~TaskPool()
{
    this->threadDinit();
    dPvt();
    delete&p;
}

bool TaskPool::start(const QSqlDatabase &connection)
{
    dPvt();
    if(!p.running.tryLock(1))
        return false;

    p.resultBool=true;
    p.taskQueueResponse.clear();
    p.taskQueueStarted.clear();
    p.connectionSetting=QOrm::ConnectionSetting(connection, nullptr).toHash();

    if(this->isRunning()){
        this->threadInit();
        return true;
    }

    QThread::start();
    while(this->eventDispatcher()==nullptr)
        this->msleep(1);
    return true;
}

void TaskPool::run()
{
    dPvt();
    QTimer::singleShot(10, this, [this](){
        this->threadInit();
    });
    this->exec();
    if(p.running.tryLock(100))
        p.running.unlock();
    else
        p.running.unlock();
}

void TaskPool::clear()
{
    dPvt();
    QMutexLocker locker(&p.running);
    p.methodExecute=__methodExecute;
    p.methodSuccess=__methodSuccess;
    p.methodFailed=__methodFailed;
    p.taskQueueResponse.clear();
    p.taskQueueValue.clear();
    p.taskQueueStarted.clear();
    p.resultList.clear();
    p.resultBool=false;
}

void TaskPool::threadInit()
{
    dPvt();
    p.resultBool=true;
    p.taskQueueStarted=p.taskQueueValue;

    auto slotCount=p.slotCount>0?p.slotCount:QThread::idealThreadCount();
    auto idealSlotCount=p.taskQueueStarted.size();
    idealSlotCount=(idealSlotCount>slotCount)?slotCount:idealSlotCount;

    if(p.taskQueueStarted.isEmpty()){
#if Q_ORM_LOG_VERBOSE
        sWarning()<<"no tasks";
#endif
        p.running.unlock();
        return;
    }

    if(slotCount<=0){
#if Q_ORM_LOG
        sWarning()<<"slotCount is zero";
#endif
        p.running.unlock();
        return;
    }

    auto&lst=p.taskSlotList;
    for (int slot = 0; slot < idealSlotCount; ++slot) {
        auto taskSlot=lst.value(slot);
        if(taskSlot==nullptr){
            taskSlot=new TaskSlot(this, p.connectionSetting, p.methodExecute, p.methodSuccess, p.methodFailed);
            lst.insert(slot, taskSlot);
            auto poolName=qsl("Pool%1-%2").arg(slot).arg(QString::number(qlonglong(taskSlot->currentThreadId())));
            taskSlot->setObjectName(poolName);
        }
        taskSlot->init();
    }
}

void TaskPool::threadDinit()
{
    dPvt();
    auto aux=p.taskSlotList;
    p.taskSlotList.clear();
    for (auto&slot:aux){
        slot->quit();
        slot->wait();
    }
    for (auto&slot:aux){
        slot->wait();
        delete slot;
    }
}

void TaskPool::taskRequest(TaskSlot *slot)
{
    dPvt();
    if(!p.taskQueueStarted.isEmpty()){
        auto task=p.taskQueueStarted.takeFirst();
        emit slot->taskSend(task);
        return;
    }

    p.running.unlock();
}

void TaskPool::taskResponse(const QVariantHash &task)
{
    dPvt();
    auto uuid=task[qsl("uuid")].toUuid();
    p.taskQueueResponse.insert(uuid, task);
    double maximum=p.taskQueueValue.size();
    double minimum=0;
    double value=p.taskQueueValue.size()-p.taskQueueStarted.size();
    double progress=(maximum<=0)?0:((value/maximum)*100);
    progress=(progress<0)?0:progress;
    emit taskProgress(maximum, minimum, value, progress);
}

TaskRunner *TaskPool::runner()
{
    dPvt();
    return p.runner;
}

QMutex &TaskPool::running()
{
    dPvt();
    return p.running;
}

void TaskPool::setTaskQueueValue(const QVariantList &newTaskQueueValue)
{
    dPvt();
    p.taskQueueValue = newTaskQueueValue;
}

bool TaskPool::resultBool() const
{
    dPvt();
    return p.resultBool;
}

void TaskPool::setResultBool(bool newResultBool)
{
    dPvt();
    p.resultBool = newResultBool;
}

const QVariantList &TaskPool::resultList() const
{
    dPvt();
    return p.resultList;
}

void TaskPool::setResultList(const QVariantList &newResultList)
{
    dPvt();
    p.resultList = newResultList;
}

const QVariantList &TaskPool::taskQueueStarted() const
{
    dPvt();
    return p.taskQueueStarted;
}

void TaskPool::setTaskQueueStarted(const QVariantList &newTaskQueueStarted)
{
    dPvt();
    p.taskQueueStarted = newTaskQueueStarted;
}

QVariantList &TaskPool::taskQueueValue()
{
    dPvt();
    return p.taskQueueValue;
}

const QHash<int, TaskSlot *> &TaskPool::taskSlotList() const
{
    dPvt();
    return p.taskSlotList;
}

void TaskPool::setTaskSlotList(const QHash<int, TaskSlot *> &newTaskSlotList)
{
    dPvt();
    p.taskSlotList = newTaskSlotList;
}

const TaskRunnerMethod &TaskPool::methodFailed() const
{
    dPvt();
    return p.methodFailed;
}

void TaskPool::setMethodFailed(const TaskRunnerMethod &newMethodFailed)
{
    dPvt();
    p.methodFailed = newMethodFailed;
}

const TaskRunnerMethod &TaskPool::methodSuccess() const
{
    dPvt();
    return p.methodSuccess;
}

void TaskPool::setMethodSuccess(const TaskRunnerMethod &newMethodSuccess)
{
    dPvt();
    p.methodSuccess = newMethodSuccess;
}

const TaskRunnerMethod &TaskPool::methodExecute() const
{
    dPvt();
    return p.methodExecute;
}

void TaskPool::setMethodExecute(const TaskRunnerMethod &newMethodExecute)
{
    dPvt();
    p.methodExecute = newMethodExecute;
}

int TaskPool::slotCount() const
{
    dPvt();
    return p.slotCount>0?p.slotCount:QThread::idealThreadCount();
}

void TaskPool::setSlotCount(int newSlotCount)
{
    dPvt();
    p.slotCount = newSlotCount;
}

}

