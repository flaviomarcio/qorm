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
//    virtual ModelDtoCrudItem<T>&search(){
//        auto func=__func__;
//        auto object=PrivateQOrm::ModelDtoLinkCollection::find(func);
//        if(object==nullptr){
//            object=new ModelDtoCrudItem<T>(&this->d(), this);
//            PrivateQOrm::ModelDtoLinkCollection::ref(object);
//        }
//        return*object;
//    }
//    virtual ModelDtoCrudItem<T>&searchObject(){
//        auto func=__func__;
//        auto object=PrivateQOrm::ModelDtoLinkCollection::find(func);
//        if(object==nullptr){
//            object=new ModelDtoCrudItem<T>(&this->d(), this);
//            PrivateQOrm::ModelDtoLinkCollection::ref(object);
//        }
//        return*object;
//    }
//    virtual ModelDtoCrudItem<T>&upsert(){
//        auto func=__func__;
//        auto object=PrivateQOrm::ModelDtoLinkCollection::find(func);
//        if(object==nullptr){
//            object=new ModelDtoCrudItem<T>(&this->d(), this);
//            PrivateQOrm::ModelDtoLinkCollection::ref(object);
//        }
//        return*object;
//    }
//    virtual ModelDtoCrudItem<T>&remove(){
//        auto func=__func__;
//        auto object=PrivateQOrm::ModelDtoLinkCollection::find(func);
//        if(object==nullptr){
//            object=new ModelDtoCrudItem<T>(&this->d(), this);
//            PrivateQOrm::ModelDtoLinkCollection::ref(object);
//        }
//        return*object;
//    }
//    virtual ModelDtoCrudItem<T>&disabled(){
//        auto func=__func__;
//        ModelDtoCrudItem<T>*object=PrivateQOrm::ModelDtoLinkCollection::find(func);
//        if(object==nullptr){
//            object=new ModelDtoCrudItem<T>(&this->d(), this);
//            PrivateQOrm::ModelDtoLinkCollection::ref(object);
//        }
//        return*object;
//    }
};


}
