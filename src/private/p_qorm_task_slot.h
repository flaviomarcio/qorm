#pragma once

#include "../qorm_connection_pool.h"
#include "../qorm_global.h"
#include <QDateTime>
#include <QObject>
#include <QString>
#include <QThread>
#include <QTimer>
#include <QUrl>
#include <QUuid>
#include <QVariant>
#include <QtSql/QSqlDatabase>

namespace QOrm {
class TaskSlotPvt;
typedef std::function<QVariant(QSqlDatabase &db, const QVariant &task)> TaskRunnerMethod;
class TaskPool;
class TaskRunner;

//!
//! \brief The TaskSlot class
//!
class Q_ORM_EXPORT TaskSlot : public QThread
{
    Q_OBJECT
public:
    //!
    //! \brief TaskSlot
    //! \param pool
    //! \param connectionSetting
    //! \param methodExecute
    //! \param methodSuccess
    //! \param methodFailed
    //!
    explicit TaskSlot(TaskPool *pool,
                      const QVariantHash &connectionSetting,
                      TaskRunnerMethod methodExecute,
                      TaskRunnerMethod methodSuccess,
                      TaskRunnerMethod methodFailed);
    ~TaskSlot();

    //!
    //! \brief start
    //! \return
    //!
    bool start();

    //!
    //! \brief run
    //!
    void run() override;

    //!
    //! \brief init
    //!
    void init();
signals:
    //!
    //! \brief taskSend
    //! \param task
    //!
    void taskSend(const QVariant &task);

    //!
    //! \brief taskRequest
    //! \param slot
    //!
    void taskRequest(QOrm::TaskSlot *slot);

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
    void taskError(const QVariantHash &task);

    //!
    //! \brief taskSuccess
    //! \param task
    //!
    void taskSuccess(const QVariantHash &task);

private:
    TaskSlotPvt *p = nullptr;
};

} // namespace QOrm
