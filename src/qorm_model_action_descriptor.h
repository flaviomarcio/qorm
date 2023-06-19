#pragma once

#include "./qorm_global.h"
#include "../../qstm/src/qstm_object_wrapper.h"

namespace QOrm {
class ModelActionDescriptorPvt;
class Q_ORM_EXPORT ModelActionDescriptor : public QStm::ObjectWrapper
{
    Q_OBJECT
    Q_STM_OBJECT_WRAPPER(ModelActionDescriptor)
    Q_PROPERTY(QString action READ action WRITE action RESET resetAction NOTIFY actionChanged)
    Q_PROPERTY(QString title READ title WRITE title RESET resetTitle NOTIFY titleChanged)
    Q_PROPERTY(QVariant actionType READ actionType WRITE actionType RESET resetActionType NOTIFY actionTypeChanged)
    Q_PROPERTY(int order READ order WRITE order RESET resetOrder NOTIFY orderChanged)
    Q_PROPERTY(QVariant width READ width WRITE width RESET resetWidth NOTIFY widthChanged)
    Q_PROPERTY(QVariant inputLink READ inputLink WRITE inputLink RESET resetInputLink NOTIFY inputLinkChanged)
public:
    enum Type{Search,Create,Upsert,Remove,Deactivate,Apply,Execute,Finalize,Print};
    Q_ENUM(Type)

    //!
    //! \brief ModelActionDescriptor
    //! \param parent
    //!
    explicit ModelActionDescriptor(QObject *parent = nullptr);

    //!
    //! \brief action
    //! \return
    //!
    const QString &action() const;
    ModelActionDescriptor &action(const QString &newAction);
    ModelActionDescriptor &resetAction();

    //!
    //! \brief title
    //! \return
    //!
    const QString &title() const;
    ModelActionDescriptor &title(const QString &newTitle);
    ModelActionDescriptor &resetTitle();

    //!
    //! \brief actionType
    //! \return
    //!
    const QVariant actionType() const;
    ModelActionDescriptor &actionType(const QVariant &newActionType);
    ModelActionDescriptor &resetActionType();

    //!
    //! \brief order
    //! \return
    //!
    int order() const;
    ModelActionDescriptor &order(int newOrder);
    ModelActionDescriptor &resetOrder();

    //!
    //! \brief width
    //! \return
    //!
    const QVariant &width() const;
    ModelActionDescriptor &width(const QVariant &newWidth);
    ModelActionDescriptor &resetWidth();

    //!
    //! \brief inputLink
    //! \return
    //!
    const QVariant &inputLink() const;
    ModelActionDescriptor &inputLink(const QVariant &newInputLink);
    ModelActionDescriptor &resetInputLink();

signals:
    void actionChanged();

    void titleChanged();

    void actionTypeChanged();

    void orderChanged();

    void widthChanged();

    void inputLinkChanged();

private:
    ModelActionDescriptorPvt *p=nullptr;
};

} // namespace QOrm

