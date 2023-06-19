#include "./qorm_task_runner.h"
#include "./private/p_qorm_task_pool.h"
#include "./qorm_object_db.h"
#include "./qorm_const.h"
#if Q_ORM_LOG
#include "./qorm_macro.h"
#endif

namespace QOrm {

static const auto __uuid="uuid";
static const auto __order="order";
static const auto __value="value";
static const auto __tskPool="tskPool";
static const auto __tskRunner="tskRunner";

class TaskRunnerPvt : public ObjectDb
{
public:
    TaskPool *pool=nullptr;
    int timeout = 0;
    explicit TaskRunnerPvt(TaskRunner *parent) : ObjectDb{parent}, pool{new TaskPool{parent}}
    {
    }

    virtual ~TaskRunnerPvt()
    {
        pool->quit();
        pool->wait();
        delete pool;
    }

    void clear()
    {
        this->pool->quit();
        this->pool->wait();
        this->pool->clear();
        this->timeout = 0;
    }
};

TaskRunner::TaskRunner(QObject *parent) : QOrm::Object{parent}, p{new TaskRunnerPvt{this}}
{
}

TaskRunner &TaskRunner::print()
{
    oInfo() << "slotCount: " << p->pool->slotCount();
    oInfo() << "taskQueueValue.size: " << p->pool->taskQueueValue().size();
    return *this;
}

TaskRunner &TaskRunner::v(const QVariant &v)
{
    p->pool->addTaskQueueValue(v);
    return *this;
}

TaskRunner &TaskRunner::value(const QVariant &v)
{
    return this->v(v);
}

TaskRunner &TaskRunner::vs(const QVariant &values)
{
    QVariantList vList;
    switch (values.typeId()) {
    case QMetaType::QVariantHash:
    case QMetaType::QVariantMap:
        vList = values.toHash().values();
        break;
    case QMetaType::QVariantList:
    case QMetaType::QStringList:
        vList = values.toList();
        break;
    default:
        vList.append(values);
    }
    for (auto &v : vList)
        p->pool->addTaskQueueValue(v);
    return *this;
}

TaskRunner &TaskRunner::vs(const QVariantList &values)
{
    if (p->pool->taskQueueValue().isEmpty()){
        p->pool->setTaskQueueValue(values);
    }
    else {
        for (auto &v : values)
            p->pool->addTaskQueueValue(v);
    }
    return *this;
}

TaskRunner &TaskRunner::vs(const QVariantHash &values)
{
    return this->vs(values.values());
}

TaskRunner &TaskRunner::values(const QVariantList &values)
{
    return this->vs(values);
}

TaskRunner &TaskRunner::values(const QVariantHash &values)
{
    return this->vs(values.values());
}

TaskRunner &TaskRunner::values(const QVariant &values)
{
    return this->vs(values);
}

TaskRunner &TaskRunner::values(ResultValue &values)
{
    return this->vs(values.resultVariant());
}

TaskRunner &TaskRunner::onExecute(TaskRunnerMethod method)
{
    p->pool->setMethodExecute(method);
    return *this;
}

TaskRunner &TaskRunner::onSuccess(TaskRunnerMethod method)
{
    p->pool->setMethodSuccess(method);
    return *this;
}

TaskRunner &TaskRunner::onFailed(TaskRunnerMethod method)
{
    p->pool->setMethodFailed(method);
    return *this;
}

ResultValue &TaskRunner::start()
{
    if(p->pool->isRunning())
        return this->lr()=false;

    auto name = this->objectName().trimmed();
    if (!name.isEmpty())
        name = QThread::currentThread()->objectName().trimmed();
    this->setObjectName(__tskRunner + name);

    name = this->objectName().trimmed();
    if (!name.isEmpty())
        name = QThread::currentThread()->objectName().trimmed();
    p->pool->setObjectName(__tskPool + name);
    p->pool->start(p->connection());
    p->pool->wait();
    return this->lr().setResult(p->pool->resultList()) = p->pool->resultBool();
}

TaskRunner &TaskRunner::stop()
{
    p->pool->setResultBool(false);
    p->pool->quit();
    return *this;
}

TaskRunner &TaskRunner::clear()
{
    p->clear();
    return *this;
}

QUuid TaskRunner::taskAppend(const QVariant &taskValue)
{
    auto uuid = QUuid::createUuid();
    QVariantHash vTask={
        {__uuid, uuid.toString()},
        {__order, p->pool->taskQueueValue().size() + 1},
        {__value, taskValue},
    };
    p->pool->addTaskQueueValue(vTask);
    return uuid;
}

TaskRunner &TaskRunner::operator<<(const QVariant &taskValue)
{
    this->taskAppend(taskValue);
    return *this;
}

int TaskRunner::slotCount() const
{
    auto __return = p->pool->slotCount();
    return __return;
}

TaskRunner &TaskRunner::slotCount(int value)
{
    return this->setSlotCount(value);
}

TaskRunner &TaskRunner::setSlotCount(int value)
{
    p->pool->setSlotCount(value);
    return *this;
}

int TaskRunner::timeout() const
{
    return p->timeout;
}

TaskRunner &TaskRunner::timeout(int value)
{
    return this->setTimeout(value);
}

TaskRunner &TaskRunner::setTimeout(int value)
{
    p->timeout = value;
    return *this;
}

} // namespace QOrm
