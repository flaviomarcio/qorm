#include "qorm_model_action_descriptor.h"
#include "../../qstm/src/qstm_meta_enum.h"

namespace QOrm {

class ModelActionDescriptorPvt:public QObject{
public:
    QString action;
    QString title;
    QStm::MetaEnum<ModelActionDescriptor::Type> actionType=ModelActionDescriptor::None;
    int order=0;
    QVariant width;
    QVariant inputLink;
    explicit ModelActionDescriptorPvt(QObject *parent):QObject{parent}{}
};

ModelActionDescriptor::ModelActionDescriptor(QObject *parent)
    : QStm::ObjectWrapper{parent}
{
    this->p=new ModelActionDescriptorPvt{parent};
}

const QString &ModelActionDescriptor::action() const
{
    return p->action;
}

ModelActionDescriptor &ModelActionDescriptor::action(const QString &newAction)
{
    p->action=newAction;
    emit actionChanged();
    return *this;
}

ModelActionDescriptor &ModelActionDescriptor::resetAction()
{
    return this->action({});
}

const QString &ModelActionDescriptor::title() const
{
    return p->title;
}

const QVariant &ModelActionDescriptor::width() const
{
    return p->width;
}

ModelActionDescriptor &ModelActionDescriptor::title(const QString &newTitle)
{
    if (p->title == newTitle.trimmed())
        return *this;
    p->title = newTitle.trimmed();
    emit titleChanged();
    return *this;
}

ModelActionDescriptor &ModelActionDescriptor::resetTitle()
{
    return this->title({});
}

const QVariant ModelActionDescriptor::actionType() const
{
    return p->actionType.type();
}

ModelActionDescriptor &ModelActionDescriptor::actionType(const QVariant &newActionType)
{
    p->actionType=newActionType;
    emit actionTypeChanged();
    return *this;
}

ModelActionDescriptor &ModelActionDescriptor::resetActionType()
{
    return this->actionType({});
}

int ModelActionDescriptor::order() const
{
    return p->order;
}

ModelActionDescriptor &ModelActionDescriptor::order(int newOrder)
{
    if (p->order == newOrder)
        return *this;
    p->order = newOrder;
    emit orderChanged();
    return *this;
}

ModelActionDescriptor &ModelActionDescriptor::resetOrder()
{
    return this->order({});
}

ModelActionDescriptor &ModelActionDescriptor::width(const QVariant &newWidth)
{
    if (p->width == newWidth)
        return *this;
    p->width = newWidth;
    emit widthChanged();
    return *this;
}

ModelActionDescriptor &ModelActionDescriptor::resetWidth()
{
    return this->width({});
}

const QVariant &ModelActionDescriptor::inputLink() const
{
    return p->inputLink;
}

ModelActionDescriptor &ModelActionDescriptor::inputLink(const QVariant &newInputLink)
{
    if (p->inputLink == newInputLink)
        return *this;
    p->inputLink = newInputLink;
    emit inputLinkChanged();
    return *this;
}

ModelActionDescriptor &ModelActionDescriptor::resetInputLink()
{
    return this->inputLink({});
}

} // namespace QOrm
