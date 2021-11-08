#pragma once

#include "./p_qorm_model_dto_link_base.h"

namespace QOrm {

template <class T>
class ModelDtoCrudItem : public QStm::Object{
public:
    Q_INVOKABLE explicit ModelDtoCrudItem(QObject*dtoParent, QObject*parent=nullptr):QStm::Object(parent), item(dtoParent, parent){
    }
    Q_INVOKABLE virtual ~ModelDtoCrudItem(){
    }
    virtual T&d(){
        auto object=&this->item.d();
        return*dynamic_cast<T*>(object);
    }
    PrivateQOrm::ModelDtoLinkItem item;
};

template <class T>
class ModelDtoCrud:public PrivateQOrm::ModelDtoLinkCollection
{
public:
    explicit ModelDtoCrud(QObject*dtoParent, QObject*parent=nullptr):PrivateQOrm::ModelDtoLinkCollection(dtoParent, parent){
    }
    virtual T&d(){
        auto object=&PrivateQOrm::ModelDtoLinkCollection::d();
        return*dynamic_cast<T*>(object);
    }
};


}
