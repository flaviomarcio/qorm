#include "./private/p_qorm_task_pool.h"
#include "./qorm_object_db.h"

namespace QOrm {

class TaskRunnerPvt : public ObjectDb
{
public:
    TaskPool pool;
    int timeout = 0;
    explicit TaskRunnerPvt(TaskRunner *parent) : ObjectDb{parent}, pool{parent} {}
    virtual ~TaskRunnerPvt()
    {
        pool.quit();
        pool.wait(1000);
    }

    void clear()
    {
        this->pool.clear();
        this->timeout = 0;
    }
};

TaskRunner::TaskRunner(QObject *parent) : QStm::Object{parent}
{
    this->p = new TaskRunnerPvt{this};
}

TaskRunner::~TaskRunner()
{

}

TaskRunner &TaskRunner::print()
{

    sInfo() << "slotCount: " << p->pool.slotCount();
    sInfo() << "taskQueueValue.size: " << p->pool.taskQueueValue().size();
    return *this;
}

TaskRunner &TaskRunner::v(const QVariant &v)
{

    p->pool.taskQueueValue() << v;
    return *this;
}

TaskRunner &TaskRunner::value(const QVariant &v)
{
    return this->v(v);
}

TaskRunner &TaskRunner::vs(const QVariant &values)
{

    QVariantList vList;
    switch (qTypeId(values)) {
    case QMetaType_QVariantHash:
    case QMetaType_QVariantMap:
        vList = values.toHash().values();
        break;
    case QMetaType_QVariantList:
    case QMetaType_QStringList:
        vList = values.toList();
        break;
    default:
        vList << values;
    }
    for (auto &v : vList)
        p->pool.taskQueueValue() << v;
    return *this;
}

TaskRunner &TaskRunner::vs(const QVariantList &values)
{

    if (p->pool.taskQueueValue().isEmpty())
        p->pool.taskQueueValue() = values;
    else {
        for (auto &v : values)
            p->pool.taskQueueValue() << v;
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

    p->pool.setMethodExecute(method);
    return *this;
}

TaskRunner &TaskRunner::onSuccess(TaskRunnerMethod method)
{

    p->pool.setMethodSuccess(method);
    return *this;
}

TaskRunner &TaskRunner::onFailed(TaskRunnerMethod method)
{

    p->pool.setMethodFailed(method);
    return *this;
}

ResultValue &TaskRunner::start()
{

    auto name = this->objectName().trimmed();
    if (!name.isEmpty())
        name = QThread::currentThread()->objectName().trimmed();
    this->setObjectName(qsl("TaskRunner") + name);

    name = this->objectName().trimmed();
    if (!name.isEmpty())
        name = QThread::currentThread()->objectName().trimmed();
    p->pool.setObjectName(qsl("TaskPool") + name);
    p->pool.start(p->connection());
    QMutexLOCKER locker(&p->pool.running());
    return this->lr().setResult(p->pool.resultList()) = p->pool.resultBool();
}

void TaskRunner::stop()
{

    p->pool.setResultBool(false);
    p->pool.quit();
}

void TaskRunner::clear()
{

    p->clear();
}

QUuid TaskRunner::taskAppend(const QVariant &taskValue)
{

    auto uuid = QUuid::createUuid();
    QVariantHash vTask;
    vTask.insert(qsl("uuid"), uuid.toString());
    vTask.insert(qsl("order"), p->pool.taskQueueValue().size() + 1);
    vTask.insert(qsl("value"), taskValue);
    p->pool.taskQueueValue() << vTask;
    return uuid;
}

TaskRunner &TaskRunner::operator<<(const QVariant &taskValue)
{
    this->taskAppend(taskValue);
    return *this;
}

int TaskRunner::slotCount() const
{

    auto __return = p->pool.slotCount();
    return __return;
}

TaskRunner &TaskRunner::slotCount(int value)
{
    return this->setSlotCount(value);
}

TaskRunner &TaskRunner::setSlotCount(int value)
{

    p->pool.setSlotCount(value);
    return *this;
}

int TaskRunner::idealThreadCount() const
{
    return QThread::idealThreadCount();
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
