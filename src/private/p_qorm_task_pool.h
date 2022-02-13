#pragma once

#include <QObject>
#include <QString>
#include <QVariant>
#include <QDateTime>
#include <QUrl>
#include <QUuid>
#include <QMutex>
#include "../qorm_task_runner.h"
#include "../qorm_connection_pool.h"
#include "../qorm_connection_setting.h"
#include "./p_qorm_task_slot.h"

namespace QOrm {

//!
//! \brief The TaskPool class
//!
class Q_ORM_EXPORT TaskPool : public QThread
{
    Q_OBJECT
public:

    //!
    //! \brief TaskPool
    //! \param parent
    //!
    explicit TaskPool(TaskRunner *parent);

    //!
    //!
    ~TaskPool();

    //!
    //! \brief start
    //! \param connection
    //! \return
    //!
    bool start(const QSqlDatabase&connection);

    //!
    //! \brief run
    //!
    void run() override;

    //!
    //! \brief clear
    //!
    void clear();

    //!
    //! \brief slotCount
    //! \return
    //!
    int slotCount() const;
    void setSlotCount(int newSlotCount);

    //!
    //! \brief methodExecute
    //! \return
    //!
    const TaskRunnerMethod &methodExecute() const;
    void setMethodExecute(const TaskRunnerMethod &newMethodExecute);

    //!
    //! \brief methodSuccess
    //! \return
    //!
    const TaskRunnerMethod &methodSuccess() const;
    void setMethodSuccess(const TaskRunnerMethod &newMethodSuccess);

    //!
    //! \brief methodFailed
    //! \return
    //!
    const TaskRunnerMethod &methodFailed() const;
    void setMethodFailed(const TaskRunnerMethod &newMethodFailed);

    //!
    //! \brief taskSlotList
    //! \return
    //!
    const QHash<int, TaskSlot *> &taskSlotList() const;
    void setTaskSlotList(const QHash<int, TaskSlot *> &newTaskSlotList);

    //!
    //! \brief taskQueueValue
    //! \return
    //!
    QVariantList &taskQueueValue();
    void setTaskQueueValue(const QVariantList &newTaskQueueValue);

    //!
    //! \brief taskQueueStarted
    //! \return
    //!
    const QVariantList &taskQueueStarted() const;
    void setTaskQueueStarted(const QVariantList &newTaskQueueStarted);

    //!
    //! \brief resultList
    //! \return
    //!
    const QVariantList &resultList() const;
    void setResultList(const QVariantList &newResultList);

    //!
    //! \brief resultBool
    //! \return
    //!
    bool resultBool() const;
    void setResultBool(bool newResultBool);

    //!
    //! \brief running
    //! \return
    //!
    QMutex &running();

    //!
    //! \brief runner
    //! \return
    //!
    TaskRunner *runner();

signals:
    //!
    //! \brief taskProgress
    //! \param maximum
    //! \param minimum
    //! \param value
    //! \param progress
    //!
    void taskProgress(qlonglong maximum, qlonglong minimum, qlonglong value, qlonglong progress);
public slots:
    //!
    //! \brief threadInit
    //!
    void threadInit();

    //!
    //! \brief threadDinit
    //!
    void threadDinit();

    //!
    //! \brief taskRequest
    //! \param slot
    //!
    void taskRequest(QOrm::TaskSlot*slot);

    //!
    void taskResponse(const QVariantHash&task);
private:
    void*p=nullptr;
};

}
