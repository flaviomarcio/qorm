#pragma once

#include "./p_qorm_model_dto_types.h"

namespace QOrm {

class ModelDto;

template <class T>
class ModelDtoHeader : public QStm::Object{

public:
    enum FilterStyle{
        dsAutoDectect=DtoFilterStyle::dsAutoDectect
    };

    enum Alignment{
        Start=QOrm::daStart, Center=QOrm::daCenter, End=QOrm::daEnd
    };

    Q_ENUM(FilterStyle)

    Q_PROPERTY(QVariant     order               READ order              WRITE setOrder              )
    Q_PROPERTY(QVariant     type                READ type               WRITE setType               )
    Q_PROPERTY(QVariant     value               READ value              WRITE setValue              )
    Q_PROPERTY(QVariant     defaultValue        READ defaultValue       WRITE setDefaultValue       )
    Q_PROPERTY(QVariant     text                READ text               WRITE setText               )
    Q_PROPERTY(QVariant     length              READ length             WRITE setLength             )
    Q_PROPERTY(Alignment    align               READ align              WRITE setAlign              )
    Q_PROPERTY(QVariant     width               READ width              WRITE setWidth              )
    Q_PROPERTY(bool         visible             READ visible            WRITE setVisible            )
    Q_PROPERTY(bool         editable            READ editable           WRITE setEditable           )
    Q_PROPERTY(bool         sortable            READ sortable           WRITE setSortable           )
    Q_PROPERTY(bool         filtrable           READ filtrable          WRITE setFiltrable          )
    Q_PROPERTY(QVariant     filterStyle         READ filterStyle        WRITE setFilterStyle        )
    Q_PROPERTY(QVariant     filtrableStrategy   READ filtrableStrategy  WRITE setFiltrableStrategy  )
    Q_PROPERTY(QVariant     readOnly            READ readOnly           WRITE setReadOnly           )
    Q_PROPERTY(QVariant     defaultValue        READ defaultValue       WRITE setDefaultValue       )
    Q_PROPERTY(QVariant     defaultSelect       READ defaultSelect      WRITE setDefaultSelect      )
    Q_PROPERTY(QVariant     inputType           READ inputType          WRITE setInputType          )
    Q_PROPERTY(QVariant     inputMask           READ inputMask          WRITE setInputMask          )
    Q_PROPERTY(QVariant     inputLinks          READ inputLinks         WRITE setInputLinks         )

    Q_INVOKABLE explicit ModelDtoHeader(T*dto, QObject*parent=nullptr):QStm::Object(parent){
        this->___d=dto;
    }

    Q_INVOKABLE virtual ~ModelDtoHeader(){

    }

    T&d(){
        return*this->___d;
    }

    virtual QVariant order() const
    {
        return QVariant(this->v.value(vpOrder).toInt());
    }

    virtual ModelDtoHeader&order(const QVariant &order)
    {
        return this->setOrder(order);
    }

    virtual ModelDtoHeader&setOrder(const QVariant &order)
    {
        this->v.insert(vpOrder,order.toInt());
        return*this;
    }

    virtual ModelDtoHeader&type(const QVariant &type)
    {
        return this->setType(type);
    }

    virtual ModelDtoHeader&setType(const QVariant &type)
    {
        this->v.insert(vpType,type);
        return*this;
    }

    virtual QString value() const
    {
        return this->v.value(vpValue).toByteArray();
    }

    virtual ModelDtoHeader&value(const QVariant &value)
    {
        return this->setValue(value);
    }

    virtual ModelDtoHeader&setValue(const QVariant &value)
    {
        this->v.insert(vpValue, value.toByteArray());
        return*this;
    }

    virtual QString defaultValue() const
    {
        return this->v.value(vpDefaultValue).toByteArray();
    }

    virtual ModelDtoHeader&defaultValue(const QVariant &value)
    {
        return this->setDefaultValue(value);
    }

    virtual ModelDtoHeader&setDefaultValue(const QVariant &value)
    {
        this->v.insert(vpDefaultValue, value.toByteArray());
        return*this;
    }

    virtual QVariant defaultSelect() const
    {
        return this->v.value(vpDefaultSelect);
    }

    virtual ModelDtoHeader&defaultSelect(const QVariant &value)
    {
        this->v.insert(vpDefaultSelect, value);
        return*this;
    }

    virtual QString text() const
    {
        return this->v.value(vpText).toByteArray();
    }

    virtual ModelDtoHeader&text(const QVariant &value)
    {
        return this->setText(value);
    }

    virtual ModelDtoHeader&setText(const QVariant &value)
    {
        this->v.insert(vpText, value.toByteArray());
        return*this;
    }

    virtual QVariant align() const
    {
        return this->v.value(vpAlign);
    }

    virtual ModelDtoHeader&align(const QVariant& value)
    {
        return this->setAlign(value);
    }

    virtual ModelDtoHeader&setAlign(const QVariant& value)
    {
        this->v.insert(vpAlign, value);
        return*this;
    }

    virtual QVariant width() const
    {
        return this->v.value(vpWidth).toBool();
    }

    virtual ModelDtoHeader&width(const QVariant& value)
    {
        return this->setWidth(value);
    }

    virtual ModelDtoHeader&setWidth(const QVariant& value)
    {
        this->v.insert(vpWidth, value);
        return*this;
    }

    virtual QVariant inputType() const
    {
        return this->v.value(vpInputType);
    }

    virtual ModelDtoHeader&inputType(const QVariant &value)
    {
        this->v.insert(vpInputType, value);
        return*this;
    }

    virtual QVariant inputMask() const
    {
        return this->v.value(vpInputType);
    }

    virtual ModelDtoHeader&inputMask(const QVariant &value)
    {
        this->v.insert(vpInputMask, value);
        return*this;
    }

    virtual QVariant inputLinks() const
    {
        return this->v.value(vtInputLinks);
    }

    virtual ModelDtoHeader&inputLinks(const QVariant &value)
    {
        this->v.insert(vtInputLinks, value);
        return*this;
    }

    virtual QVariant length() const
    {
        return this->v.value(vpLength);
    }

    virtual ModelDtoHeader&length(const QVariant &value)
    {
        this->v.insert(vpLength, value.toInt());
        return*this;
    }

    virtual bool visible() const
    {
        return this->v.value(vpVisible).toBool();
    }

    virtual ModelDtoHeader&visible(const QVariant&value)
    {
        return this->setVisible(value);
    }

    virtual ModelDtoHeader&setVisible(const QVariant&value)
    {
        auto v=(value.isNull() || !value.isValid())?true:value.toBool();
        this->v.insert(vpVisible, v);
        return*this;
    }

    virtual bool readOnly() const
    {
        return this->v.value(vpReadOnly).toBool();
    }

    virtual ModelDtoHeader&readOnly(const QVariant&value)
    {
        auto v=(value.isNull() || !value.isValid())?true:value.toBool();
        this->v.insert(vpReadOnly, v);
        return*this;
    }

    virtual bool editable() const
    {
        return this->v.value(vpEditable).toBool();
    }

    virtual ModelDtoHeader&editable(const QVariant&value)
    {
        return this->setEditable(value);
    }

    virtual ModelDtoHeader&setEditable(const QVariant&value)
    {
        this->v.insert(vpEditable, value.toBool());
        return*this;
    }

    virtual bool sortable() const
    {
        return this->v.value(vpSortable).toBool();
    }

    virtual ModelDtoHeader&sortable(const QVariant&value)
    {
        return this->setSortable(value);
    }

    virtual ModelDtoHeader&setSortable(const QVariant&value)
    {
        this->v.insert(vpSortable, value.toBool());
        return*this;
    }

    virtual bool filtrable() const
    {
        return this->v.value(vpFiltrable).toBool();
    }

    virtual ModelDtoHeader&filtrable(const QVariant&value)
    {
        return this->setFiltrable(value);
    }

    virtual ModelDtoHeader&setFiltrable(const QVariant&value)
    {
        this->v.insert(vpFiltrable, value.toBool());
        return*this;
    }

    virtual QVariantHash filtrableStrategy() const
    {
        return this->v.value(vpFiltrableStrategy).toHash();
    }

    virtual ModelDtoHeader&filtrableStrategy(const QVariant&value)
    {
        return this->setFiltrableStrategy(value);
    }

    virtual ModelDtoHeader&setFiltrableStrategy(const QVariant&value)
    {
        this->v.insert(vpFiltrableStrategy, value);
        return*this;
    }

    virtual DtoFilterStyle filterStyle() const
    {
        return DtoFilterStyle(this->v.value(vpFilterStyle).toInt());
    }

    virtual ModelDtoHeader&filterStyle(const QVariant &value)
    {
        return this->setFilterStyle(value);
    }

    virtual ModelDtoHeader&setFilterStyle(const QVariant &value)
    {
        this->v.insert(vpFilterStyle, value.toInt());
        return*this;
    }

    virtual QVariant toVar()const{
        return this->v;
    };

    virtual const QVariantHash&toHash(){
        return this->v;
    };

    virtual QVariantMap toMap()const {
        return QVariant(this->v).toMap();
    };

private:
    QVariantHash v;
    T*___d=nullptr;
};


template <class T>
class  ModelDtoHeaders:public QStm::Object
{
public:

    enum FilterStyle{
        dsAutoDectect=DtoFilterStyle::dsAutoDectect
    };

    Q_ENUM(FilterStyle)


    explicit ModelDtoHeaders(T*dto, QObject*parent=nullptr):QStm::Object(parent){
        this->___d=dto;
    }

    explicit ModelDtoHeaders(T*dto, const QVariantHash&v, QObject*parent=nullptr):QStm::Object(parent){
        Q_UNUSED(v)
        this->___d=dto;
    }

    virtual ~ModelDtoHeaders(){
    }

    virtual ModelDtoHeaders<T>&clear(){
        this->___objectMap.clear();
        this->___objectList.clear();
        return*this;
    }

    virtual QVariant toVar()const{
        QVariantList vList;
        for(const auto&v:this->___objectList)
            vList<<v->toVar();
        return vList;
    };

    virtual QVariant toList()const{
        QVariantList vList;
        for(const auto&v:this->___objectList)
            vList<<v->toVar();
        return vList;
    };

    virtual T&d(){
        return*this->___d;
    }

    virtual ModelDtoHeader<T>* get(const QString&v){
        return this->___objectMap.value(v);
    }

    virtual ModelDtoHeaders&remove(const QString&v){
        if(this->___objectMap.contains(v)){
            auto object=this->___objectMap.take(v);
            this->___objectList.removeOne(object);
            delete object;
        }
        return*this;
    }

    virtual ModelDtoHeader<T>&value(const QString&v){
        return this->value(QVariantHash{{vpValue,v}});
    }

    virtual ModelDtoHeader<T>&value(const QVariantHash&v){
        auto value=v.value(vpValue).toString();
        auto object=this->___objectMap.value(value);
        if(object==nullptr){
            object=new ModelDtoHeader<T>(this->___d, this);
            object->order(this->___objectMap.count());
            this->___objectList<<object;
            this->___objectMap.insert(value, object);
        }
        object->type(v.value(vpType));
        object->value(v.value(vpValue));
        object->defaultValue(v.value(vpDefaultValue));
        object->defaultSelect(v.value(vpDefaultSelect));
        object->text(v.value(vpText));
        object->align(v.value(vpAlign));
        object->width(v.value(vpWidth));
        object->visible(v.value(vpVisible));
        object->readOnly(v.value(vpReadOnly));
        object->editable(v.value(vpEditable));
        object->sortable(v.value(vpSortable));
        object->length(v.value(vpLength));
        object->filtrable(v.value(vpFiltrable));
        object->filtrableStrategy(v.value(vpFiltrableStrategy));
        object->filterStyle(v.value(vpFilterStyle));
        object->inputType(v.value(vpInputType));
        object->inputMask(v.value(vpInputMask));
        object->inputLinks(v.value(vtInputLinks));
        return*object;
    }

    virtual ModelDtoHeader<T>&makeDefault(){
        static auto vAction=QVariantHash( { { vpValue, vpActions}, { vpText, qsl_null}, { vpSortable, false }, { vpVisible, true }, { vpWidth, qsl("0%") }});
        return this->value(vAction);
    }

    virtual ModelDtoHeaders&unMakeDefault(){
        return this->remove(vpActions);
    }

    virtual QList<ModelDtoHeader<T>*>&list(){
        this->reOrder();
        return this->___objectList;
    }

    virtual void reOrder(){
        auto i=0;
        for(auto&v:this->___objectList)
            v->setOrder(i++);
    }

protected:
    QMap<QString, ModelDtoHeader<T>*> ___objectMap;
    QList<ModelDtoHeader<T>*> ___objectList;
    T*___d=nullptr;
};


}
