#pragma once

#include "./p_qorm_model_dto_types.h"

namespace QOrm {

class ModelDto;
//!
//! \brief The ModelDtoHeader class
//!
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

    Q_PROPERTY(QVariant order READ order WRITE setOrder NOTIFY widthChanged)
    Q_PROPERTY(QVariant dataType READ dataType WRITE setDataType NOTIFY dataTypeChanged)
    Q_PROPERTY(QVariant value READ value WRITE setValue NOTIFY textChanged)
    Q_PROPERTY(QVariant defaultValue READ defaultValue WRITE setDefaultValue NOTIFY defaulChanged)
    Q_PROPERTY(QVariant text READ text WRITE setText NOTIFY valueChanged)
    Q_PROPERTY(QVariant length READ length WRITE setLength NOTIFY typeChanged)
    Q_PROPERTY(QVariant width READ width WRITE setWidth NOTIFY orderChanged)
    Q_PROPERTY(QVariant align READ align WRITE setAlign NOTIFY filtrableChanged)
    Q_PROPERTY(bool visible READ visible WRITE setVisible NOTIFY sortableChanged)
    Q_PROPERTY(bool editable READ editable WRITE setEditable NOTIFY editableChanged)
    Q_PROPERTY(bool sortable READ sortable WRITE setSortable NOTIFY visibleChanged)
    Q_PROPERTY(bool filtrable READ filtrable WRITE setFiltrable NOTIFY alignChanged)
    Q_PROPERTY(QVariant filterStyle READ filterStyle WRITE setFilterStyle NOTIFY filterStyleChanged)
    Q_PROPERTY(QVariant filtrableStrategy READ filtrableStrategy WRITE setFiltrableStrategy NOTIFY filtrableStrategyChanged)
    Q_PROPERTY(QVariant readOnly READ readOnly WRITE setReadOnly NOTIFY readOnlyChanged)
    Q_PROPERTY(QVariant defaultValue READ defaultValue WRITE setDefaultValue NOTIFY defaultValueChanged)
    Q_PROPERTY(QVariant defaultSelect READ defaultSelect WRITE setDefaultSelect NOTIFY defaultSelectChanged)
    Q_PROPERTY(QVariant inputType READ inputType WRITE setInputType NOTIFY inputTypeChanged)
    Q_PROPERTY(QVariant inputMask READ inputMask WRITE setInputMask NOTIFY inputMaskChanged)
    Q_PROPERTY(QVariant inputLinks READ inputLinks WRITE setInputLinks NOTIFY inputLinksChanged)

    //!
    //! \brief ModelDtoHeader
    //! \param dto
    //! \param parent
    //!
    Q_INVOKABLE explicit ModelDtoHeader(T*dto, QObject*parent=nullptr):QStm::Object{parent}
    {
        this->___d=dto;
    }

    //!
    //! \brief d
    //! \return
    //!
    T &d()
    {
        return*this->___d;
    }

    //!
    //! \brief order
    //! \return
    //!
    virtual int order() const
    {
        return this->v.value(vpOrder).toInt();
    }
    virtual ModelDtoHeader &order(const QVariant &order)
    {
        return this->setOrder(order);
    }
    virtual ModelDtoHeader &setOrder(const QVariant &order)
    {
        this->v.insert(vpOrder,order.toInt());
        return*this;
    }

    //!
    //! \brief type
    //! \param type
    //! \return
    //!
    virtual QVariant type() const
    {
        return this->v.value(vpOrder);
    }
    virtual ModelDtoHeader &dataType(const QVariant &type)
    {
        return this->setDataType(type);
    }
    virtual ModelDtoHeader &setDataType(const QVariant &type)
    {
        this->v.insert(vpDataType,type);
        return*this;
    }

    //!
    //! \brief value
    //! \return
    //!
    virtual QString value() const
    {
        return this->v.value(vpField).toByteArray();
    }
    virtual ModelDtoHeader &value(const QVariant &value)
    {
        return this->setValue(value);
    }
    virtual ModelDtoHeader &setValue(const QVariant &value)
    {
        this->v.insert(vpField, value.toByteArray());
        return*this;
    }

    //!
    //! \brief defaultValue
    //! \return
    //!
    virtual QString defaultValue() const
    {
        return this->v.value(vpDefaultValue).toByteArray();
    }
    virtual ModelDtoHeader &defaultValue(const QVariant &value)
    {
        return this->setDefaultValue(value);
    }
    virtual ModelDtoHeader &setDefaultValue(const QVariant &value)
    {
        this->v.insert(vpDefaultValue, value.toByteArray());
        return*this;
    }
    virtual QVariant defaultSelect() const
    {
        return this->v.value(vpDefaultSelect);
    }
    virtual ModelDtoHeader &defaultSelect(const QVariant &value)
    {
        this->v.insert(vpDefaultSelect, value);
        return*this;
    }

    //!
    //! \brief text
    //! \return
    //!
    virtual QString text() const
    {
        return this->v.value(vpTitle).toByteArray();
    }
    virtual ModelDtoHeader &text(const QVariant &value)
    {
        return this->setText(value);
    }
    virtual ModelDtoHeader &setText(const QVariant &value)
    {
        this->v.insert(vpTitle, value.toByteArray());
        return*this;
    }

    //!
    //! \brief align
    //! \return
    //!
    virtual QVariant align() const
    {
        return this->v.value(vpAlign);
    }
    virtual ModelDtoHeader &align(const QVariant & value)
    {
        return this->setAlign(value);
    }
    virtual ModelDtoHeader &setAlign(const QVariant & value)
    {
        this->v.insert(vpAlign, value);
        return*this;
    }

    //!
    //! \brief width
    //! \return
    //!
    virtual QVariant width() const
    {
        return this->v.value(vpWidth);
    }
    virtual ModelDtoHeader &width(const QVariant & value)
    {
        return this->setWidth(value);
    }
    virtual ModelDtoHeader &setWidth(const QVariant & value)
    {
        this->v.insert(vpWidth, value);
        return*this;
    }

    //!
    //! \brief inputType
    //! \return
    //!
    virtual QVariant inputType() const
    {
        return this->v.value(vpInputType);
    }
    virtual ModelDtoHeader &inputType(const QVariant &value)
    {
        this->v.insert(vpInputType, value);
        return*this;
    }

    //!
    //! \brief inputMask
    //! \return
    //!
    virtual QVariant inputMask() const
    {
        return this->v.value(vpInputType);
    }
    virtual ModelDtoHeader &inputMask(const QVariant &value)
    {
        this->v.insert(vpInputMask, value);
        return*this;
    }

    //!
    //! \brief inputLinks
    //! \return
    //!
    virtual QVariant inputLinks() const
    {
        return this->v.value(vtInputLinks);
    }
    virtual ModelDtoHeader &inputLinks(const QVariant &value)
    {
        this->v.insert(vtInputLinks, value);
        return*this;
    }

    //!
    //! \brief length
    //! \return
    //!
    virtual QVariant length() const
    {
        return this->v.value(vpLength);
    }
    virtual ModelDtoHeader &length(const QVariant &value)
    {
        this->v.insert(vpLength, value.toInt());
        return*this;
    }

    //!
    //! \brief visible
    //! \return
    //!
    virtual bool visible() const
    {
        return this->v.value(vpVisible).toBool();
    }
    virtual ModelDtoHeader &visible(const QVariant &value)
    {
        return this->setVisible(value);
    }
    virtual ModelDtoHeader &setVisible(const QVariant &value)
    {
        auto v=(value.isNull() || !value.isValid())?true:value.toBool();
        this->v.insert(vpVisible, v);
        return*this;
    }

    //!
    //! \brief readOnly
    //! \return
    //!
    virtual bool readOnly() const
    {
        return this->v.value(vpReadOnly).toBool();
    }
    virtual ModelDtoHeader &readOnly(const QVariant &value)
    {
        auto v=(value.isNull() || !value.isValid())?true:value.toBool();
        this->v.insert(vpReadOnly, v);
        return*this;
    }

    //!
    //! \brief editable
    //! \return
    //!
    virtual bool editable() const
    {
        return this->v.value(vpEditable).toBool();
    }
    virtual ModelDtoHeader &editable(const QVariant &value)
    {
        return this->setEditable(value);
    }
    virtual ModelDtoHeader &setEditable(const QVariant &value)
    {
        this->v.insert(vpEditable, value.toBool());
        return*this;
    }

    //!
    //! \brief sortable
    //! \return
    //!
    virtual bool sortable() const
    {
        return this->v.value(vpSortable).toBool();
    }
    virtual ModelDtoHeader &sortable(const QVariant &value)
    {
        return this->setSortable(value);
    }
    virtual ModelDtoHeader &setSortable(const QVariant &value)
    {
        this->v.insert(vpSortable, value.toBool());
        return*this;
    }

    //!
    //! \brief filtrable
    //! \return
    //!
    virtual bool filtrable() const
    {
        return this->v.value(vpFiltrable).toBool();
    }
    virtual ModelDtoHeader &filtrable(const QVariant &value)
    {
        return this->setFiltrable(value);
    }
    virtual ModelDtoHeader &setFiltrable(const QVariant &value)
    {
        this->v.insert(vpFiltrable, value.toBool());
        return*this;
    }

    //!
    //! \brief filtrableStrategy
    //! \return
    //!
    virtual QVariantHash filtrableStrategy() const
    {
        return this->v.value(vpFiltrableStrategy).toHash();
    }
    virtual ModelDtoHeader &filtrableStrategy(const QVariant &value)
    {
        return this->setFiltrableStrategy(value);
    }
    virtual ModelDtoHeader &setFiltrableStrategy(const QVariant &value)
    {
        this->v.insert(vpFiltrableStrategy, value);
        return*this;
    }

    //!
    //! \brief filterStyle
    //! \return
    //!
    virtual DtoFilterStyle filterStyle() const
    {
        return DtoFilterStyle(this->v.value(vpFilterStyle).toInt());
    }
    virtual ModelDtoHeader &filterStyle(const QVariant &value)
    {
        return this->setFilterStyle(value);
    }
    virtual ModelDtoHeader &setFilterStyle(const QVariant &value)
    {
        this->v.insert(vpFilterStyle, value.toInt());
        return*this;
    }

    //!
    //! \brief toVar
    //! \return
    //!
    virtual QVariant toVar()const
    {
        return this->v;
    }

    //!
    //! \brief toHash
    //! \return
    //!
    virtual QVariantHash toHash()const
    {
        return this->v;
    }

    //!
    //! \brief toMap
    //! \return
    //!
    virtual QVariantMap toMap()const
    {
        return QVariant(this->v).toMap();
    }

private:
    QVariantHash v;
    T*___d=nullptr;
};


//!
//! \brief The ModelDtoHeaders class
//!
template <class T>
class  ModelDtoHeaders:public QStm::Object
{
public:

    //!
    //! \brief The FilterStyle enum
    //!
    enum FilterStyle{
        dsAutoDectect=DtoFilterStyle::dsAutoDectect
    };
    Q_ENUM(FilterStyle)

    //!
    //! \brief ModelDtoHeaders
    //! \param dto
    //! \param parent
    //!
    explicit ModelDtoHeaders(T*dto, QObject*parent=nullptr):QStm::Object{parent}
    {
        this->___d=dto;
    }

    //!
    //! \brief ModelDtoHeaders
    //! \param dto
    //! \param v
    //! \param parent
    //!
    explicit ModelDtoHeaders(T*dto, const QVariantHash&v, QObject*parent=nullptr):QStm::Object{parent}
    {
        Q_UNUSED(v)
        this->___d=dto;
    }

    //!
    //! \brief clear
    //! \return
    //!
    virtual ModelDtoHeaders<T>&clear()
    {
        this->___objectMap.clear();
        this->___objectList.clear();
        return*this;
    }

    //!
    //! \brief toVar
    //! \return
    //!
    virtual QVariant toVar()const
    {
        QVariantList vList;
        for(const auto&v:this->___objectList)
            vList<<v->toVar();
        return vList;
    };

    //!
    //! \brief toList
    //! \return
    //!
    virtual QVariant toList()const
    {
        QVariantList vList;
        for(const auto&v:this->___objectList)
            vList<<v->toVar();
        return vList;
    };

    //!
    //! \brief d
    //! \return
    //!
    virtual T &d()
    {
        return*this->___d;
    }

    //!
    //! \brief get
    //! \param v
    //! \return
    //!
    virtual ModelDtoHeader<T>* get(const QString&v)
    {
        return this->___objectMap.value(v);
    }

    //!
    //! \brief remove
    //! \param v
    //! \return
    //!
    virtual ModelDtoHeaders&remove(const QString&v)
    {
        if(this->___objectMap.contains(v)){
            auto object=this->___objectMap.take(v);
            this->___objectList.removeOne(object);
            delete object;
        }
        return*this;
    }

    //!
    //! \brief value
    //! \param v
    //! \return
    //!
    virtual ModelDtoHeader<T> &value(const QString&v)
    {
        return this->value({{vpField,v}});
    }

    //!
    //! \brief value
    //! \param v
    //! \return
    //!
    virtual ModelDtoHeader<T> &value(const QVariantHash&v)
    {
        auto value=v.value(vpField).toString();
        auto object=this->___objectMap.value(value);
        if(object==nullptr){
            object=new ModelDtoHeader<T>(this->___d, this);
            object->order(this->___objectMap.count());
            this->___objectList<<object;
            this->___objectMap.insert(value, object);
        }
        object->dataType(v.value(vpDataType));
        object->value(v.value(vpField));
        object->order(v.value(vpOrder));
        object->defaultValue(v.value(vpDefaultValue));
        object->defaultSelect(v.value(vpDefaultSelect));
        object->text(v.value(vpTitle));
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

    //!
    //! \brief list
    //! \return
    //!
    virtual QList<ModelDtoHeader<T>*>&list()
    {
        this->reOrder();
        return this->___objectList;
    }

    //!
    //! \brief reOrder
    //!
    virtual void reOrder()
    {
        auto i=0;
        for(auto&v:this->___objectList){
            if(v->order()<0)
                v->setOrder(i++);
        }
    }

protected:
    QMap<QString, ModelDtoHeader<T>*> ___objectMap;
    QList<ModelDtoHeader<T>*> ___objectList;
    T*___d=nullptr;
signals:
    void orderChanged();
    void dataTypeChanged();
    void valueChanged();
    void defaulChanged();
    void textChanged();
    void lengthChanged();
    void widthChanged();
    void alignChanged();
    void visibleChanged();
    void editableChanged();
    void sortableChanged();
    void filtrableChanged();
    void filterStyleChanged();
    void filtrableStrategyChanged();
    void readOnlyChanged();
    void defaultValueChanged();
    void defaultSelectChanged();
    void inputTypeChanged();
    void inputMaskChanged();
    void inputLinksChanged();
};


}
