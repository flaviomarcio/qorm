#pragma once

#include <QObject>
#include <QString>
#include <QVariant>
#include <QDateTime>
#include <QUrl>
#include <QUuid>
#include <QMutex>
#include "./qorm_task_runner.h"
#include "./qorm_connection_pool.h"
#include "./qorm_connection_setting.h"
#include "./p_qorm_task_slot.h"

namespace QOrm {

static TaskRunnerMethod __methodExecute=[](QSqlDatabase&db, const QVariant&task){Q_UNUSED(task) Q_UNUSED(db) return QVariant();};
static TaskRunnerMethod __methodSuccess=[](QSqlDatabase&db, const QVariant&task){Q_UNUSED(task) Q_UNUSED(db) return task;};
static TaskRunnerMethod __methodFailed=[](QSqlDatabase&db, const QVariant&task){Q_UNUSED(task) Q_UNUSED(db) return task;};

class Q_ORM_EXPORT TaskPool : public QThread
{
    Q_OBJECT
public:
    QMutex running;
    int slotCount=QThread::idealThreadCount();
    TaskRunnerMethod methodExecute=__methodExecute;
    TaskRunnerMethod methodSuccess=__methodSuccess;
    TaskRunnerMethod methodFailed=__methodFailed;
    QHash<int, TaskSlot*> taskSlotList;
    QMap<QUuid, QVariant> taskQueueResponse;
    QVariantList taskQueueValue;
    QVariantList taskQueueStarted;
    QVariantList resultList;
    bool resultBool=false;
    TaskRunner *runner=nullptr;
    QVariantHash connectionSetting;
    explicit TaskPool(TaskRunner *parent):QThread(nullptr){
        this->moveToThread(this);
        this->runner=parent;
        QObject::connect(this, &TaskPool::taskProgress, parent, &TaskRunner::taskProgress);
    }
    ~TaskPool(){
        this->threadDinit();
    }

    bool start(const QSqlDatabase&connection){
        if(!this->running.tryLock(1))
            return false;

        this->resultBool=true;
        this->taskQueueResponse.clear();
        this->taskQueueStarted.clear();
        this->connectionSetting=QOrm::ConnectionSetting(connection, nullptr).toHash();
        if(this->isRunning())
            this->threadInit();
        else{
            QThread::start();
            while(this->eventDispatcher()==nullptr)
                this->msleep(1);
        }
        return true;
    }

    void run() override{
        QTimer::singleShot(10, this, [this](){this->threadInit();});
        this->exec();
        if(running.tryLock(100))
            running.unlock();
        else
            running.unlock();
    }

    void clear(){
        QMutexLocker locker(&this->running);
        this->methodExecute=__methodExecute;
        this->methodSuccess=__methodSuccess;
        this->methodFailed=__methodFailed;
        this->taskQueueResponse.clear();
        this->taskQueueValue.clear();
        this->taskQueueStarted.clear();
        this->resultList.clear();
        this->resultBool=false;
    }

signals:
    void taskProgress(qlonglong maximum, qlonglong minimum, qlonglong value, qlonglong progress);
public slots:

    void threadInit(){
        this->resultBool=true;
        this->taskQueueStarted=this->taskQueueValue;

        auto slotCount=this->slotCount>0?this->slotCount:QThread::idealThreadCount();
        auto idealSlotCount=this->taskQueueStarted.size();
        idealSlotCount=(idealSlotCount>slotCount)?slotCount:idealSlotCount;

        if(this->taskQueueStarted.isEmpty()){
#if Q_ORM_LOG_VERBOSE
            sWarning()<<"no tasks";
#endif
            this->running.unlock();
        }
        else if(slotCount<=0){
#if Q_ORM_LOG
            sWarning()<<"slotCount is zero";
#endif
            this->running.unlock();
        }
        else {
            auto&lst=this->taskSlotList;
            for (int slot = 0; slot < idealSlotCount; ++slot) {
                auto taskSlot=lst.value(slot);
                if(taskSlot==nullptr){
                    taskSlot=new TaskSlot(this, this->connectionSetting, methodExecute, methodSuccess, methodFailed);
                    lst.insert(slot, taskSlot);
                    auto poolName=qsl("Pool%1-%2").arg(slot).arg(QString::number(qlonglong(taskSlot->currentThreadId())));
                    taskSlot->setObjectName(poolName);
                }
                taskSlot->init();
            }
        }
    }

    void threadDinit(){
        auto aux=this->taskSlotList;
        this->taskSlotList.clear();
        for (auto&slot:aux){
            slot->quit();
            slot->wait();
        }
        for (auto&slot:aux){
            slot->wait();
            delete slot;
        }
    }

    void taskRequest(QOrm::TaskSlot*slot){
        if(!this->taskQueueStarted.isEmpty()){
            auto task=this->taskQueueStarted.takeFirst();
            emit slot->taskSend(task);
        }
        else{
            this->running.unlock();
        }
    }

    void taskResponse(const QVariantHash&task){
        auto uuid=task[qsl("uuid")].toUuid();
        this->taskQueueResponse.insert(uuid, task);
        double maximum=this->taskQueueValue.size();
        double minimum=0;
        double value=this->taskQueueValue.size()-this->taskQueueStarted.size();
        double progress=(maximum<=0)?0:((value/maximum)*100);
        progress=(progress<0)?0:progress;
        emit taskProgress(maximum, minimum, value, progress);
    }
private:
    void*p=nullptr;
};

}
