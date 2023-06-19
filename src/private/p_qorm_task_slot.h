#pragma once

#include <QDateTime>
#include <QObject>
#include <QString>
#include <QThread>
#include <QTimer>
#include <QUrl>
#include <QUuid>
#include "../qorm_global.h"
#include "./p_qorm_task_types.h"

namespace QOrm {
class TaskSlotPvt;
class TaskPool;
class TaskRunner;

//!
//! \brief The TaskSlot class
//!
class Q_ORM_EXPORT TaskSlot : public QThread
{
    Q_OBJECT
public:
    explicit TaskSlot(QObject *parent=nullptr);

    //!
    //! \brief TaskSlot
    //! \param pool
    //! \param connectionSetting
    //! \param methodExecute
    //! \param methodSuccess
    //! \param methodFailed
    //!
    explicit TaskSlot(TaskPool *pool,
                      int slotNumber,
                      const QVariantHash &connectionSetting,
                      TaskRunnerMethod methodExecute,
                      TaskRunnerMethod methodSuccess,
                      TaskRunnerMethod methodFailed);

    int slotNumber();

    //!
    //! \brief start
    //! \return
    //!
    bool start();

    //!
    //! \brief run
    //!
    void run() override;
signals:
    //!
    //! \brief taskSend
    //! \param task
    //!
    void taskSend(const QVariant &task);

    //!
    //! \brief taskStart
    //! \param task
    //!
    void taskStart(const QVariantHash &task);

    //!
    //! \brief taskState
    //! \param task
    //!
    void taskState(const QVariantHash &task);

    //!
    //! \brief taskError
    //! \param task
    //!
    void taskError(const int slotNumber, const QVariantHash &task);

    //!
    //! \brief taskSuccess
    //! \param task
    //!
    void taskSuccess(const int slotNumber, const QVariantHash &task);

private:
    TaskSlotPvt *p = nullptr;
};

} // namespace QOrm
