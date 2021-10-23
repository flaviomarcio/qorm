#pragma once

#include "./qorm_object.h"

namespace QOrm {

typedef std::function<QVariant(QSqlDatabase&db, const QVariant&task)> TaskRunnerMethod;
class Q_ORM_EXPORT TaskRunner : public QStm::Object
{
    Q_OBJECT
public:
    /**
     * @brief TaskRunner
     * @param parent
     */
    Q_INVOKABLE explicit TaskRunner(QObject *parent = nullptr);

    /**
     * @brief ~TaskRunner
     */
    Q_INVOKABLE ~TaskRunner();

    /**
     * @brief print
     * @return
     */
    virtual TaskRunner &print();

    /**
     * @brief values
     * @param values
     * @return
     */
    virtual TaskRunner &v(const QVariant &v);
    virtual TaskRunner &value(const QVariant &v);

    virtual TaskRunner &vs(const QVariantList &values);
    virtual TaskRunner &vs(const QVariantHash &values);
    virtual TaskRunner &vs(const QVariant &values);
    virtual TaskRunner &values(const QVariantList &values);
    virtual TaskRunner &values(const QVariantHash &values);
    virtual TaskRunner &values(const QVariant &values);
    virtual TaskRunner &values(ResultValue&values);

    virtual TaskRunner &onExecute(TaskRunnerMethod method);
    virtual TaskRunner &onSuccess(TaskRunnerMethod method);
    virtual TaskRunner &onFailed(TaskRunnerMethod method);

    /**
     * @brief exec
     * @return
     */
    virtual ResultValue&start();

    /**
     * @brief stop
     */
    virtual void stop();

    /**
     * @brief clear
     */
    virtual void clear();

    /**
     * @brief taskAppend
     * @param taskValue
     * @return
     */
    virtual QUuid taskAppend(const QVariant&taskValue);

    /**
     * @brief operator <<
     * @param taskValue
     * @return
     */
    TaskRunner &operator <<(const QVariant&taskValue);

    /**
     * @brief slotCount
     * @return
     */
    virtual int slotCount() const;
    virtual TaskRunner &slotCount(int slotCount);
    virtual TaskRunner &setSlotCount(int slotCount);
    virtual int idealThreadCount() const;

    virtual int timeout() const;
    virtual TaskRunner &timeout(int value);
    virtual TaskRunner &setTimeout(int value);

signals:
    void taskStart(const QVariantHash&task);
    void taskState(const QVariantHash&task);
    void taskError(const QVariantHash&task);
    void taskSuccess(const QVariantHash&task);
    void taskProgress(qlonglong maximum, qlonglong minimum, qlonglong value, qlonglong progress);
private:
    void*p=nullptr;
};

} // namespace QOrm

