#include "./p_qorm_task_slot.h"
#include "./p_qorm_task_pool.h"

namespace QOrm {

TaskSlot::TaskSlot(TaskPool *pool, const QVariantHash &connectionSetting, TaskRunnerMethod methodExecute,TaskRunnerMethod methodSuccess,TaskRunnerMethod methodFailed):QThread(nullptr), cnnPool(connectionSetting){
    this->moveToThread(this);
    this->pool=pool;
    this->runner=pool->runner;
    this->connectionSetting=connectionSetting;
    this->methodExecute=methodExecute;
    this->methodSuccess=methodSuccess;
    this->methodFailed=methodFailed;

    QObject::connect(this, &TaskSlot::taskSend      , this          , &TaskSlot::on_taskSend    );
    QObject::connect(this, &TaskSlot::taskRequest   , this->pool    , &TaskPool::taskRequest    );
    QObject::connect(this, &TaskSlot::taskSuccess   , this->pool    , &TaskPool::taskResponse   );
    QObject::connect(this, &TaskSlot::taskError     , this->pool    , &TaskPool::taskResponse   );
    QObject::connect(this, &TaskSlot::taskStart     , this->runner  , &TaskRunner::taskStart    );
    QObject::connect(this, &TaskSlot::taskState     , this->runner  , &TaskRunner::taskState    );
    QObject::connect(this, &TaskSlot::taskSuccess   , this->runner  , &TaskRunner::taskSuccess  );
    QObject::connect(this, &TaskSlot::taskError     , this->runner  , &TaskRunner::taskError    );
}

TaskSlot::~TaskSlot(){
    QObject::disconnect(this, &TaskSlot::taskSend      , this          , &TaskSlot::on_taskSend    );
    QObject::disconnect(this, &TaskSlot::taskRequest   , this->pool    , &TaskPool::taskRequest    );
    QObject::disconnect(this, &TaskSlot::taskSuccess   , this->pool    , &TaskPool::taskResponse   );
    QObject::disconnect(this, &TaskSlot::taskError     , this->pool    , &TaskPool::taskResponse   );
    QObject::disconnect(this, &TaskSlot::taskStart     , this->runner  , &TaskRunner::taskStart    );
    QObject::disconnect(this, &TaskSlot::taskState     , this->runner  , &TaskRunner::taskState    );
    QObject::disconnect(this, &TaskSlot::taskSuccess   , this->runner  , &TaskRunner::taskSuccess  );
    QObject::disconnect(this, &TaskSlot::taskError     , this->runner  , &TaskRunner::taskError    );
}

bool TaskSlot::start(){
    QThread::start();
    while(this->eventDispatcher()==nullptr)
        this->msleep(1);
    return true;
}

void TaskSlot::run(){
    if(!this->connectionCheck())
        this->quit();
    else{
        connection.close();
        this->exec();
        cnnPool.finish(this->connection);
    }
}

void TaskSlot::init()
{
    if(!this->isRunning())
        this->start();
    emit this->taskRequest(this);
}

void TaskSlot::on_taskRequest(){
    emit taskRequest(this);
}

void TaskSlot::on_taskSend(const QVariant &task){
    QVariantHash vTask;
    vTask.insert(qsl("request"), task);
    emit taskStart(vTask);
    if(!this->connectionCheck()){
        vTask.insert(qsl("error"), qsl("Invalid connection on Slot"));
        this->methodFailed(this->connection, task);
        emit taskError(vTask);
    }
    else{
        auto response=this->methodExecute(this->connection, task);
        vTask.insert(qsl("response"), response);
        this->connection.close();
        if(!this->connection.open()){
            vTask.insert(qsl("error"),connection.lastError().text());
            this->methodFailed(this->connection, vTask);
        }
        else{
            auto r=this->methodSuccess(this->connection, response);
            vTask.insert(qsl("response"), r);
        }
        emit taskSuccess(vTask);
    }
    emit taskRequest(this);
    this->connection.close();
}

bool TaskSlot::connectionCheck()
{
    if(this->connection.isValid()){
        if(!this->connection.isOpen())
            this->connection.open();
        return this->connection.isOpen();
    }

    if(!cnnPool.isValid())
        return this->cnnPool.finish(this->connection);

    if(!cnnPool.get(connection))
        return !this->cnnPool.finish(this->connection);

    return this->connection.isOpen();

}

}
