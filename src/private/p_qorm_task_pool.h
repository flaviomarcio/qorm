#pragma once

#include <QObject>
#include <QString>
#include <QVariant>
#include <QDateTime>
#include <QUrl>
#include <QUuid>
#include <QThread>
#include <QSqlDatabase>
#include <QVariantHash>
#include "../qorm_global.h"
#include "./p_qorm_task_types.h"
namespace QOrm {
class TaskRunner;
class TaskPoolPvt;
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
    explicit TaskPool(QObject *parent=nullptr);

    //!
    //! \brief start
    //! \param connection
    //! \return
    //!
    bool start(const QSqlDatabase &connection);

    //!
    //! \brief run
    //!
    void run() override;

    //!
    //! \brief clear
    //!
    TaskPool &clear();

    //!
    //! \brief slotCount
    //! \return
    //!
    QString &name() const;
    TaskPool &setName(const QString &newName);

    //!
    //! \brief slotCount
    //! \return
    //!
    int slotCount() const;
    TaskPool &setSlotCount(int newSlotCount);

    //!
    //! \brief methodExecute
    //! \return
    //!
    const TaskRunnerMethod &methodExecute() const;
    TaskPool &setMethodExecute(const TaskRunnerMethod &newMethodExecute);

    //!
    //! \brief methodSuccess
    //! \return
    //!
    const TaskRunnerMethod &methodSuccess() const;
    TaskPool &setMethodSuccess(const TaskRunnerMethod &newMethodSuccess);

    //!
    //! \brief methodFailed
    //! \return
    //!
    const TaskRunnerMethod &methodFailed() const;
    TaskPool &setMethodFailed(const TaskRunnerMethod &newMethodFailed);

    //!
    //! \brief taskQueueValue
    //! \return
    //!
    const QVariantList &taskQueueValue() const;
    TaskPool &addTaskQueueValue(const QVariant &newTaskQueueValue);
    TaskPool &setTaskQueueValue(const QVariantList &newTaskQueueValue);

    //!
    //! \brief taskQueueStarted
    //! \return
    //!
    const QVariantList &taskQueueStarted() const;
    TaskPool &setTaskQueueStarted(const QVariantList &newTaskQueueStarted);

    //!
    //! \brief resultList
    //! \return
    //!
    const QVariantList &resultList() const;
    TaskPool &setResultList(const QVariantList &newResultList);

    //!
    //! \brief resultBool
    //! \return
    //!
    bool resultBool() const;
    TaskPool &setResultBool(bool newResultBool);

    //!
    //! \brief runner
    //! \return
    //!
    TaskRunner *runner() const;

signals:
    //!
    //! \brief taskProgress
    //! \param maximum
    //! \param minimum
    //! \param value
    //! \param progress
    //!
    void taskProgress(qlonglong maximum, qlonglong minimum, qlonglong value, qlonglong progress);
private:
    //!
    //! \brief threadInit
    //!
    TaskPool &threadInit();

    //!
    //! \brief threadStart
    //!
    TaskPool &threadStart();

    //!
    //! \brief threadDinit
    //!
    TaskPool &threadDinit();
public slots:

    //!
    //! \brief taskRequest
    //! \param slot
    //!
    void taskRequest(int slotNumber);

    //!
    //! \brief taskResponse
    //! \param task
    //! \return
    //!
    void taskResponse(const int slotNumber, const QVariantHash &task);
private:
    TaskPoolPvt *p=nullptr;
};

}
