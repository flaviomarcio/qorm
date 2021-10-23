#pragma once

#include "./p_qorm_model_dto_types.h"

namespace QOrm {

template <class T>
class ModelDtoItems:public QStm::Object
{
public:
    explicit ModelDtoItems(QObject*parent=nullptr):QStm::Object(parent){
    }

    explicit ModelDtoItems(QObject*dtoParent, QObject*parent=nullptr):QStm::Object(parent){
        this->___d=dtoParent;
    }

    virtual ~ModelDtoItems(){
    }

    template<class TD=T>
    TD&d(){
        return*dynamic_cast<T*>(this->___d);
    }

    virtual ModelDtoItems&clear(){
        this->___objectList.clear();
        return*this;
    }

    virtual ModelDtoItems&fromList(const QVariant&v){
        this->___objectList=v.toList();
        return*this;
    }

    virtual QVariant toVar()const{
        return this->___objectList;
    };

    virtual QVariantList&list(){
        return this->___objectList;
    }

private:
    QObject*___d=nullptr;
    QVariantList ___objectList;
};
}
