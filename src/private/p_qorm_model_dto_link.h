#pragma once

#include "./p_qorm_model_dto_link_base.h"

namespace QOrm {

template <class T>
class ModelDtoLinkItem : public PrivateQOrm::ModelDtoLinkItem{
public:
    Q_INVOKABLE explicit ModelDtoLinkItem(QObject*dtoParent, QObject*parent=nullptr):PrivateQOrm::ModelDtoLinkItem(dtoParent, parent){
    }
    Q_INVOKABLE virtual ~ModelDtoLinkItem(){
    }
    virtual T&d(){
        auto object=&PrivateQOrm::ModelDtoLinkItem::d();
        return*dynamic_cast<T*>(object);
    }

    QVariantHash&header(){
        return PrivateQOrm::ModelDtoLinkItem::header();
    }

    auto&header(const QVariant&value){
        PrivateQOrm::ModelDtoLinkItem::setHeader(value);
        return*this;
    }

    QVariant&method(){
        return PrivateQOrm::ModelDtoLinkItem::method();
    }

    auto&method(const QVariant&value){
        PrivateQOrm::ModelDtoLinkItem::setMethod(value);
        return*this;
    }

    auto&setMethod(const QVariant&value){
        PrivateQOrm::ModelDtoLinkItem::setMethod(value);
        return*this;
    }

    QVariant&ref(){
        return PrivateQOrm::ModelDtoLinkItem::ref();
    }

    auto&ref(const QVariant&value){
        PrivateQOrm::ModelDtoLinkItem::setRef(value);
        return*this;
    }

    auto&setRef(const QVariant&value){
        PrivateQOrm::ModelDtoLinkItem::setRef(value);
        return*this;
    }

    QVariant&hRef(){
        return PrivateQOrm::ModelDtoLinkItem::hRef();
    }

    auto&hRef(const QVariant&value){
        PrivateQOrm::ModelDtoLinkItem::setHRef(value);
        return*this;
    }

    auto&setHRef(const QVariant&value){
        PrivateQOrm::ModelDtoLinkItem::setHRef(value);
        return*this;
    }

    QVariant&parameters(){
        return PrivateQOrm::ModelDtoLinkItem::parameters();
    }

    auto&parameters(const QVariant&value){
        PrivateQOrm::ModelDtoLinkItem::setParameters(value);
        return*this;
    }

    auto&setParameters(const QVariant&value){
        PrivateQOrm::ModelDtoLinkItem::setParameters(value);
        return*this;
    }
};

template <class T>
class ModelDtoLinks:public PrivateQOrm::ModelDtoLinkCollection
{
public:
    explicit ModelDtoLinks(QObject*dtoParent, QObject*parent=nullptr):PrivateQOrm::ModelDtoLinkCollection(dtoParent, parent){
    }
    virtual T&d(){
        auto object=&PrivateQOrm::ModelDtoLinkCollection::d();
        return*dynamic_cast<T*>(object);
    }
    virtual ModelDtoLinkItem<T>&ref(const QVariant&ref){

        auto object=PrivateQOrm::ModelDtoLinkCollection::find(ref);
        if(object==nullptr){
            object=new ModelDtoLinkItem<T>(&this->d(), this);
            object->setRef(ref);
            PrivateQOrm::ModelDtoLinkCollection::ref(object);
        }
        return*dynamic_cast<ModelDtoLinkItem<T>*>(object) ;
    }
};


}
