#pragma once

#include "./p_qorm_model_dto_types.h"

namespace QOrm {

template <class T>
class ModelDtoFilter : public QStm::Object{
public:

    Q_PROPERTY(QVariant     type            READ type           WRITE setType            )
    Q_PROPERTY(QVariant     value           READ value          WRITE setValue           )
    Q_PROPERTY(QVariant     defaultValue    READ defaultValue   WRITE setDefaultValue    )
    Q_PROPERTY(QVariant     comboValue      READ comboValue     WRITE setComboValue      )
    Q_PROPERTY(QVariant     text            READ text           WRITE setText            )
    Q_PROPERTY(Alignment    align           READ align          WRITE setAlign           )
    Q_PROPERTY(QVariant     width           READ width          WRITE setWidth           )
    Q_PROPERTY(bool         sortable        READ sortable       WRITE setSortable        )
    Q_PROPERTY(bool         filtrable       READ filtrable      WRITE setFiltrable       )
    Q_PROPERTY(QVariant     filterStyle     READ filterStyle    WRITE setFilterStyle     )

    Q_INVOKABLE explicit ModelDtoFilter(T*dto, QObject*parent=nullptr):QStm::Object(parent){
        this->___d=dto;
    }

    Q_INVOKABLE virtual ~ModelDtoFilter(){

    }

    virtual T&d(){
        return*this->___d;
    }

    virtual QVariant type() const
    {
        return this->v.value(qsl("type"));
    }

    virtual ModelDtoFilter&type(const QVariant &type)
    {
        return this->setType(type);
    }

    virtual ModelDtoFilter&setType(const QVariant &type)
    {
        this->v.insert(qsl("type"),type);
        return*this;
    }

    virtual QByteArray value() const
    {
        return this->v.value(qsl("value")).toByteArray();
    }

    virtual ModelDtoFilter&value(const QVariant &value)
    {
        return this->setValue(value);
    }

    virtual ModelDtoFilter&setValue(const QVariant &value)
    {
        this->v.insert(qsl("value"), value);
        return*this;
    }

    virtual QString defaultValue() const
    {
        return this->v.value(qsl("defaultValue")).toByteArray();
    }

    virtual ModelDtoFilter&defaultValue(const QVariant &value)
    {
        return this->setDefaultValue(value);
    }

    virtual ModelDtoFilter&setDefaultValue(const QVariant &value)
    {
        this->v.insert(qsl("defaultValue"), value.toByteArray());
        return*this;
    }

    virtual QString comboValue() const
    {
        return this->v.value(qsl("comboValue")).toByteArray();
    }

    virtual ModelDtoFilter&comboValue(const QVariant &value)
    {
        return this->setComboValue(value);
    }

    virtual ModelDtoFilter&setComboValue(const QVariant &value)
    {
        this->v.insert(qsl("comboValue"), value.toByteArray());
        return*this;
    }

    virtual QString text() const
    {
        return this->v.value(qsl("text")).toByteArray();
    }

    virtual ModelDtoFilter&text(const QVariant &value)
    {
        return this->setText(value);
    }

    virtual ModelDtoFilter&setText(const QVariant &value)
    {
        this->v.insert(qsl("text"), value.toByteArray());
        return*this;
    }

    virtual QVariant align() const
    {
        return this->v.value(qsl("align"));
    }

    virtual ModelDtoFilter&align(const QVariant& value)
    {
        return this->setAlign(value);
    }

    virtual ModelDtoFilter&setAlign(const QVariant& value)
    {
        this->v.insert(qsl("align"), value);
        return*this;
    }

    virtual QVariant width() const
    {
        return this->v.value(qsl("width")).toBool();
    }

    virtual ModelDtoFilter&width(const QVariant& value)
    {
        return this->setWidth(value);
    }

    virtual ModelDtoFilter&setWidth(const QVariant& value)
    {
        this->v.insert(qsl("width"), value);
        return*this;
    }

    virtual bool sortable() const
    {
        return this->v.value(qsl("sortable")).toBool();
    }

    virtual ModelDtoFilter&sortable(const QVariant &value)
    {
        return this->setSortable(value);
    }

    virtual ModelDtoFilter&setSortable(const QVariant &value)
    {
        this->v.insert(qsl("sortable"), value);
        return*this;
    }

    virtual bool filtrable() const
    {
        return this->v.value(qsl("filtrable")).toBool();
    }

    virtual ModelDtoFilter&filtrable(const QVariant &value)
    {
        return this->setFiltrable(value);
    }

    virtual ModelDtoFilter&setFiltrable(const QVariant &value)
    {
        this->v.insert(qsl("filtrable"), value);
        return*this;
    }

    virtual DtoFilterStyle filterStyle() const
    {
        return DtoFilterStyle(this->v.value(qsl("filterStyle")).toInt());
    }

    virtual ModelDtoFilter&filterStyle(const DtoFilterStyle &value)
    {
        return this->setFilterStyle(value);
    }

    virtual ModelDtoFilter&setFilterStyle(const QVariant &value)
    {
        this->v.insert(qsl("filterStyle"), value);
        return*this;
    }

    virtual QVariant toVariant()const{
        return this->v;
    };

private:
    QVariantHash v;
    T*___d=nullptr;
};

template <class T>
class ModelDtoFilters:public QStm::Object
{
public:

    explicit ModelDtoFilters(T*dto, QObject*parent=nullptr):QStm::Object(parent){
        this->___d=dto;
    }

    virtual ~ModelDtoFilters(){
    }

    virtual QVariant toVar()const{
        QVariantList vList;
        for(const auto&v:this->___objectList)
            vList<<v->toVariant();
        return vList;
    };

    virtual T&d(){
        return*this->___d;
    }

    virtual ModelDtoFilter<T>&value(const QString&v){
        return this->value(QVariantHash{{vpType,v}});
    }

    virtual ModelDtoFilter<T>&value(const QVariantHash&v){
        auto object=new ModelDtoFilter<T>(this->___d, this);
        object->setType(v.value(vpType));
        object->setValue(v.value(vpValue));
        object->setDefaultValue(v.value(vpDefaultValue));
        object->setText(v.value(vpText));
        object->setAlign(v.value(vpAlign));
        object->setWidth(v.value(vpWidth));
        object->setSortable(v.value(vpSortable));
        object->setFiltrable(v.value(vpFiltrable));
        object->setFilterStyle(v.value(vpFilterStyle));
        this->___objectList<<object;
        return*object;
    }

    virtual ModelDtoFilters<T>&clear(){
        this->___objectList.clear();
        return*this;
    }

    virtual QList<ModelDtoFilter<T>*>&list(){
        return this->___objectList;
    }

private:
    T*___d=nullptr;
    QList<ModelDtoFilter<T>*> ___objectList;
};


}
