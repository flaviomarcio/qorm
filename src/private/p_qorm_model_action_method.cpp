#include "./p_qorm_model_action_method.h"
//#include "../qorm_query.h"
//#include "../qorm_transaction_scope.h"
#include <QJsonDocument>

namespace QOrm {

class ModelActionPvt:public QObject{
public:
    ModelAction*parent=nullptr;
    ObjectDb*parentDb=nullptr;
    ModelActionMethodPointer actionBefore=nullptr;
    ModelActionMethodPointer action=nullptr;
    ModelActionMethodPointer actionAfter=nullptr;
    explicit ModelActionPvt(ModelAction*parent):QObject{parent}
    {
        this->parent=parent;
        if(this->parent)
            this->parentDb = dynamic_cast<ObjectDb*>(this->parent->parent());
    }
};

ModelAction::ModelAction(QObject *parent) : QOrm::ObjectDb{parent}
{
    this->p = new ModelActionPvt{this};
}

ModelAction &ModelAction::onActionBefore(ModelActionMethodPointer action)
{
    p->actionBefore=action;
    return *this;
}

ModelAction &ModelAction::onAction(ModelActionMethodPointer action)
{
    p->action = action;
    return *this;
}

ModelAction &ModelAction::onActionAfter(ModelActionMethodPointer action)
{
    p->actionAfter = action;
    return *this;
}

ResultValue &ModelAction::action(const QVariant &vSource)
{
    QVariantList vList;
    switch (vSource.typeId()) {
    case QMetaType::QVariantList:
    case QMetaType::QStringList:
        vList=vSource.toList();
        break;
    default:
        vList.append(vSource);
        break;
    }
    if(vList.isEmpty())
        vList.append(QVariant{});
    for(auto &vSource:vList){
        if(p->actionBefore!=nullptr){
            auto lr=p->actionBefore(p->parentDb, vSource);
            this->lr(*lr);
        }

        if(p->action!=nullptr){
            auto lr=p->action(p->parentDb, vSource);
            this->lr(*lr);
        }

        if(p->actionAfter!=nullptr){
            auto lr=p->actionAfter(p->parentDb, vSource);
            this->lr(*lr);
        }
    }
    return this->lr();
}

}
