#include "./p_qorm_task_pool.h"
#include "./p_qorm_task_slot.h"
#include "../qorm_connection_setting.h"
#include "../qorm_task_runner.h"
#include "./p_qorm_task_slot.h"
//#include "../qorm_const.h"
//#include "../qorm_macro.h"

static QVariant __methodExecute(QSqlDatabase&db, const QVariant &task){Q_UNUSED(task) Q_UNUSED(db) return {};};
static QVariant __methodSuccess(QSqlDatabase&db, const QVariant &task){Q_UNUSED(task) Q_UNUSED(db) return task;};
static QVariant __methodFailed(QSqlDatabase&db, const QVariant &task){Q_UNUSED(task) Q_UNUSED(db) return task;};

namespace QOrm {

static const auto __uuid="uuid";

class TaskPoolPvt:public QObject{
public:
    TaskRunner *runner=nullptr;
    QString name;
    int slotCount=QThread::idealThreadCount();
    TaskRunnerMethod methodExecute=nullptr;
    TaskRunnerMethod methodSuccess=nullptr;
    TaskRunnerMethod methodFailed=nullptr;
    QHash<int, TaskSlot*> taskSlotList;
    QVariantList taskQueueResponse;
    QVariantList taskQueueValue;
    QVariantList taskQueueStarted;
    QVariantList resultList;
    bool resultBool=false;
    QVariantHash connectionSetting;
    explicit TaskPoolPvt(QObject *parent, TaskRunner *runner):QObject{parent}, runner{runner}
    {
    }

    bool threadsStoppeds()
    {
        for(auto &v:taskSlotList){
            if(v->isRunning())
                return false;
        }
        return true;
    }
};

TaskPool::TaskPool(QObject *parent):QThread{nullptr}, p{new TaskPoolPvt{this, dynamic_cast<TaskRunner*>(parent)}}
{
    this->moveToThread(this);
    if(p->runner)
        QObject::connect(this, &TaskPool::taskProgress, p->runner, &TaskRunner::taskProgress);
    this->clear();
}

bool TaskPool::start(const QSqlDatabase &connection)
{
    if(this->isRunning()){
        this->quit();
        this->wait();
        return true;
    }

    p->resultBool=true;
    p->taskQueueResponse.clear();
    p->taskQueueStarted.clear();
    p->connectionSetting=QOrm::ConnectionSetting(connection, nullptr).toHash();

    QThread::start();
    while(this->eventDispatcher()==nullptr)
        this->msleep(1);
    return true;
}

void TaskPool::run()
{
    QTimer::singleShot(10, this, [this](){
        this->threadDinit()
            .threadInit()
            .threadStart();
    });
    this->exec();
    this->threadDinit();
}

TaskPool &TaskPool::clear()
{
    p->methodExecute=__methodExecute;
    p->methodSuccess=__methodSuccess;
    p->methodFailed=__methodFailed;
    p->taskQueueResponse.clear();
    p->taskQueueValue.clear();
    p->taskQueueStarted.clear();
    p->resultList.clear();
    p->resultBool=false;
    return *this;
}

QString &TaskPool::name() const
{
    if(p->name.isEmpty()){
        static int taskPoolCounter=0;
        static const auto __name=QStringLiteral("pool%1");
        p->name=__name.arg(QString::number(++taskPoolCounter).rightJustified(2,'0'));
    }
    return p->name;
}

TaskPool &TaskPool::setName(const QString &newName)
{
    p->name=newName.trimmed();
    this->setObjectName(this->name());
    return *this;
}

TaskPool &TaskPool::threadInit()
{
    p->resultBool=true;
    p->taskQueueStarted=p->taskQueueValue;

    if(p->taskQueueStarted.isEmpty())
        return *this;

    //tasksStarteds
    int slotCount=p->slotCount;

    slotCount=(slotCount>p->taskQueueStarted.count())
                    ?p->taskQueueStarted.count()
                    :slotCount;

    //check zero slots
    slotCount=(slotCount>0)?slotCount:1;
    //check overload slots
    slotCount=(slotCount>QThread::idealThreadCount())
                    ?QThread::idealThreadCount()
                    :slotCount;

    static const auto __formatName=QStringLiteral("%1_%2");
    for (int slotNumber = 0; slotNumber < slotCount; ++slotNumber) {
        auto taskSlot=new TaskSlot(this, slotNumber+1, p->connectionSetting, p->methodExecute, p->methodSuccess, p->methodFailed);
        auto poolName=__formatName.arg(this->name(), QString::number(slotNumber).rightJustified(2,'0'));
        taskSlot->setObjectName(poolName);
        p->taskSlotList.insert(taskSlot->slotNumber(), taskSlot);
    }
    return *this;
}

TaskPool &TaskPool::threadStart()
{
    QHashIterator<int,TaskSlot*> i(p->taskSlotList);
    while(i.hasNext()){
        i.next();
        auto slot=i.value();
        slot->quit();
        slot->wait();
        slot->start();
        this->taskRequest(slot->slotNumber());
    }
    return *this;
}

TaskPool &TaskPool::threadDinit()
{
    if(p->taskSlotList.isEmpty())
        return *this;
    auto aux=p->taskSlotList.values();
    p->taskSlotList={};
    for (auto &slot:aux){
        slot->quit();
        slot->wait();
    }
    for (auto &slot:aux){
        slot->wait();
        delete slot;
    }
    return *this;
}

void TaskPool::taskRequest(int slotNumber)
{
    auto slot=p->taskSlotList.value(slotNumber);
    if(!slot)
        return;

    if(!p->taskQueueStarted.isEmpty()){
        auto task=p->taskQueueStarted.takeFirst();
        emit slot->taskSend(task);
        return;
    }

    slot->quit();
    slot->wait();
    if(p->threadsStoppeds())
        this->quit();
}

void TaskPool::taskResponse(const int slotNumber, const QVariantHash &task)
{
    p->taskQueueResponse.append(task);
    double maximum=p->taskQueueValue.size();
    double minimum=0;
    double value=p->taskQueueValue.size()-p->taskQueueStarted.size();
    double progress=(maximum<=0)?0:((value/maximum)*100);
    progress=(progress<0)?0:progress;
    emit taskProgress(maximum, minimum, value, progress);
    this->taskRequest(slotNumber);
    return;
}

TaskRunner *TaskPool::runner()const
{
    return p->runner;
}

TaskPool &TaskPool::setTaskQueueValue(const QVariantList &newTaskQueueValue)
{
    p->taskQueueValue = newTaskQueueValue;
    return *this;
}

bool TaskPool::resultBool() const
{
    return p->resultBool;
}

TaskPool &TaskPool::setResultBool(bool newResultBool)
{
    p->resultBool = newResultBool;
    return *this;
}

const QVariantList &TaskPool::resultList() const
{
    return p->resultList;
}

TaskPool &TaskPool::setResultList(const QVariantList &newResultList)
{
    p->resultList = newResultList;
    return *this;
}

const QVariantList &TaskPool::taskQueueStarted() const
{
    return p->taskQueueStarted;
}

TaskPool &TaskPool::setTaskQueueStarted(const QVariantList &newTaskQueueStarted)
{
    p->taskQueueStarted = newTaskQueueStarted;
    return *this;
}

const QVariantList &TaskPool::taskQueueValue()const
{
    return p->taskQueueValue;
}

TaskPool &TaskPool::addTaskQueueValue(const QVariant &newTaskQueueValue)
{
    p->taskQueueValue.append(newTaskQueueValue);
    return *this;
}

const TaskRunnerMethod &TaskPool::methodFailed() const
{
    return p->methodFailed;
}

TaskPool &TaskPool::setMethodFailed(const TaskRunnerMethod &newMethodFailed)
{
    p->methodFailed = newMethodFailed;
    return *this;
}

const TaskRunnerMethod &TaskPool::methodSuccess() const
{
    return p->methodSuccess;
}

TaskPool &TaskPool::setMethodSuccess(const TaskRunnerMethod &newMethodSuccess)
{
    p->methodSuccess = newMethodSuccess;
    return *this;
}

const TaskRunnerMethod &TaskPool::methodExecute() const
{
    return p->methodExecute;
}

TaskPool &TaskPool::setMethodExecute(const TaskRunnerMethod &newMethodExecute)
{
    p->methodExecute = newMethodExecute;
    return *this;
}

int TaskPool::slotCount() const
{
    return p->slotCount;
}

TaskPool &TaskPool::setSlotCount(int newSlotCount)
{
    p->slotCount = newSlotCount;
    return *this;
}

}

