#pragma once

#include "./qorm_object.h"

namespace QOrm {

typedef std::function<QVariant(QSqlDatabase&db, const QVariant&task)> TaskRunnerMethod;

//!
//! \brief The TaskRunner class
//!
class Q_ORM_EXPORT TaskRunner : public QStm::Object
{
    Q_OBJECT
public:
    //!
    //! \brief TaskRunner
    //! \param parent
    //!
    Q_INVOKABLE explicit TaskRunner(QObject *parent = nullptr);

    //!
    //! \brief ~TaskRunner
    //!
    Q_INVOKABLE ~TaskRunner();

    //!
    //! \brief print
    //! \return
    //!
    virtual TaskRunner &print();

    //!
    //! \brief v
    //! \param v
    //! \return
    //!
    virtual TaskRunner &v(const QVariant &v);

    //!
    //! \brief value
    //! \param v
    //! \return
    //!
    virtual TaskRunner &value(const QVariant &v);

    //!
    //! \brief vs
    //! \param values
    //! \return
    //!
    virtual TaskRunner &vs(const QVariantList &values);

    //!
    //! \brief vs
    //! \param values
    //! \return
    //!
    virtual TaskRunner &vs(const QVariantHash &values);

    //!
    //! \brief vs
    //! \param values
    //! \return
    //!
    virtual TaskRunner &vs(const QVariant &values);

    //!
    //! \brief values
    //! \param values
    //! \return
    //!
    virtual TaskRunner &values(const QVariantList &values);

    //!
    //! \brief values
    //! \param values
    //! \return
    //!
    virtual TaskRunner &values(const QVariantHash &values);

    //!
    //! \brief values
    //! \param values
    //! \return
    //!
    virtual TaskRunner &values(const QVariant &values);

    //!
    //! \brief values
    //! \param values
    //! \return
    //!
    virtual TaskRunner &values(ResultValue&values);

    //!
    //! \brief onExecute
    //! \param method
    //! \return
    //!
    virtual TaskRunner &onExecute(TaskRunnerMethod method);

    //!
    //! \brief onSuccess
    //! \param method
    //! \return
    //!
    virtual TaskRunner &onSuccess(TaskRunnerMethod method);

    //!
    //! \brief onFailed
    //! \param method
    //! \return
    //!
    virtual TaskRunner &onFailed(TaskRunnerMethod method);

    //!
    //! \brief start
    //! \return
    //!
    virtual ResultValue&start();

    /**
     * @brief stop
     */
    virtual void stop();

    //!
    //! \brief clear
    //!
    virtual void clear();

    //!
    //! \brief taskAppend
    //! \param taskValue
    //! \return
    //!
    virtual QUuid taskAppend(const QVariant&taskValue);

    //!
    //! \brief operator <<
    //! \param taskValue
    //! \return
    //!
    TaskRunner &operator <<(const QVariant&taskValue);

    //!
    //! \brief slotCount
    //! \return
    //!
    virtual int slotCount() const;

    //!
    //! \brief slotCount
    //! \param slotCount
    //! \return
    //!
    virtual TaskRunner &slotCount(int slotCount);

    //!
    //! \brief setSlotCount
    //! \param slotCount
    //! \return
    //!
    virtual TaskRunner &setSlotCount(int slotCount);

    //!
    //! \brief idealThreadCount
    //! \return
    //!
    virtual int idealThreadCount() const;

    //!
    //! \brief timeout
    //! \return
    //!
    virtual int timeout() const;

    //!
    //! \brief timeout
    //! \param value
    //! \return
    //!
    virtual TaskRunner &timeout(int value);

    //!
    //! \brief setTimeout
    //! \param value
    //! \return
    //!
    virtual TaskRunner &setTimeout(int value);

signals:
    //!
    //! \brief taskStart
    //! \param task
    //!
    void taskStart(const QVariantHash&task);

    //!
    //! \brief taskState
    //! \param task
    //!
    void taskState(const QVariantHash&task);

    //!
    //! \brief taskError
    //! \param task
    //!
    void taskError(const QVariantHash&task);

    //!
    //! \brief taskSuccess
    //! \param task
    //!
    void taskSuccess(const QVariantHash&task);

    //!
    //! \brief taskProgress
    //! \param maximum
    //! \param minimum
    //! \param value
    //! \param progress
    //!
    void taskProgress(qlonglong maximum, qlonglong minimum, qlonglong value, qlonglong progress);
private:
    void*p=nullptr;
};

}

