#include "./p_qorm_model_action_method.h"
#include "../qorm_query.h"
#include "../qorm_transaction_scope.h"
#include <QJsonDocument>

namespace QOrm {


#define dPvt()\
    auto&p = *reinterpret_cast<ModelActionPvt*>(this->p)\

class ModelActionPvt{
public:
    ModelAction*parent=nullptr;
    ObjectDb*parentDb=nullptr;
    ModelActionMethod actionBefore=nullptr;
    ModelActionMethod action=nullptr;
    ModelActionMethod actionAfter=nullptr;
    explicit ModelActionPvt(ModelAction*parent){
        this->parent=parent;
        if(this->parent)
            this->parentDb = dynamic_cast<ObjectDb*>(this->parent->parent());
    }
    virtual ~ModelActionPvt(){
    }


    QVariantList makeBodyLoop(const QVariant&v){
        auto vHash=v.toHash();
        QVariantList vList;
        if(vHash.contains(qsl("id")) && vHash.contains(qsl("items")))
            vList=vHash[qsl("items")].toList();
        else
            vList.append(vHash);
        return vList;
    }


};

ModelAction::ModelAction(QObject *parent) : QOrm::ObjectDb(parent)
{
    this->p = new ModelActionPvt(this);
}

ModelAction::~ModelAction(){
    dPvt();
    delete&p;
}

ModelAction &ModelAction::onActionBefore(ModelActionMethod action)
{
    dPvt();
    p.actionBefore = action;
    return*this;
}

ModelAction &ModelAction::onAction(ModelActionMethod action)
{
    dPvt();
    p.action = action;
    return*this;
}

ModelAction &ModelAction::onActionAfter(ModelActionMethod action)
{
    dPvt();
    p.actionAfter = action;
    return*this;
}

ResultValue &ModelAction::action(const QVariant &vSource)
{
    dPvt();
    auto vList=p.makeBodyLoop(vSource);
    for(auto&vSource:vList){
        if(p.actionBefore!=nullptr){
            auto&lr=*p.actionBefore(p.parentDb, vSource);
            if(!lr)
                return this->lr(lr);
            else
                this->lr(lr);
        }

        if(p.action!=nullptr){
            auto&lr=*p.action(p.parentDb, vSource);
            if(!lr)
                return this->lr(lr);
            else
                this->lr(lr);
        }

        if(p.actionAfter!=nullptr){
            auto&lr=*p.actionAfter(p.parentDb, vSource);
            if(!lr)
                return this->lr(lr);
            else
                this->lr(lr);
        }
    }
    return this->lr();
}

}
