#pragma once

#include "./p_qorm_model_dto_types.h"

namespace QOrm {


//!
//! \brief The ModelDtoFilter class
//!
template <class T>
class ModelDtoFilter : public QStm::Object{
public:

    Q_PROPERTY(QVariant type READ type WRITE setType NOTIFY filteChanged)
    Q_PROPERTY(QVariant value READ value WRITE setValue NOTIFY widthChanged)
    Q_PROPERTY(QVariant defaultValue READ defaultValue WRITE setDefaultValue NOTIFY textChanged)
    Q_PROPERTY(QVariant comboValue READ comboValue WRITE setComboValue NOTIFY combChanged)
    Q_PROPERTY(QVariant text READ text WRITE setText NOTIFY defaulChanged)
    Q_PROPERTY(QVariant width READ width WRITE setWidth NOTIFY valueChanged)
    Q_PROPERTY(QVariant filterStyle READ filterStyle WRITE setFilterStyle NOTIFY typeChanged)
    Q_PROPERTY(Alignment align READ align WRITE setAlign NOTIFY alignChanged)
    Q_PROPERTY(bool sortable READ sortable WRITE setSortable NOTIFY sortableChanged)
    Q_PROPERTY(bool filtrable READ filtrable WRITE setFiltrable NOTIFY filtrableChanged)

    //!
    //! \brief ModelDtoFilter
    //! \param dto
    //! \param parent
    //!
    Q_INVOKABLE explicit ModelDtoFilter(T *dto, QObject *parent=nullptr):QStm::Object{parent}
    {
        this->___d=dto;
    }

    //!
    //! \brief d
    //! \return
    //!
    virtual T &d()
    {
        return*this->___d;
    }

    //!
    //! \brief type
    //! \return
    //!
    virtual QVariant type() const
    {
        return this->v.value(QStringLiteral("type"));
    }
    virtual ModelDtoFilter&type(const QVariant &type)
    {
        return this->setType(type);
    }
    virtual ModelDtoFilter&setType(const QVariant &type)
    {
        this->v.insert(QStringLiteral("type"),type);
        return *this;
    }

    //!
    //! \brief value
    //! \return
    //!
    virtual QByteArray value() const
    {
        return this->v.value(QStringLiteral("value")).toByteArray();
    }
    virtual ModelDtoFilter&value(const QVariant &value)
    {
        return this->setValue(value);
    }
    virtual ModelDtoFilter&setValue(const QVariant &value)
    {
        this->v.insert(QStringLiteral("value"), value);
        return *this;
    }

    //!
    //! \brief defaultValue
    //! \return
    //!
    virtual QString defaultValue() const
    {
        return this->v.value(QStringLiteral("defaultValue")).toByteArray();
    }
    virtual ModelDtoFilter&defaultValue(const QVariant &value)
    {
        return this->setDefaultValue(value);
    }
    virtual ModelDtoFilter&setDefaultValue(const QVariant &value)
    {
        this->v.insert(QStringLiteral("defaultValue"), value.toByteArray());
        return *this;
    }

    //!
    //! \brief comboValue
    //! \return
    //!
    virtual QString comboValue() const
    {
        return this->v.value(QStringLiteral("comboValue")).toByteArray();
    }
    virtual ModelDtoFilter&comboValue(const QVariant &value)
    {
        return this->setComboValue(value);
    }
    virtual ModelDtoFilter&setComboValue(const QVariant &value)
    {
        this->v.insert(QStringLiteral("comboValue"), value.toByteArray());
        return *this;
    }

    //!
    //! \brief text
    //! \return
    //!
    virtual QString text() const
    {
        return this->v.value(QStringLiteral("text")).toByteArray();
    }
    virtual ModelDtoFilter&text(const QVariant &value)
    {
        return this->setText(value);
    }
    virtual ModelDtoFilter&setText(const QVariant &value)
    {
        this->v.insert(QStringLiteral("text"), value.toByteArray());
        return *this;
    }

    //!
    //! \brief align
    //! \return
    //!
    virtual QVariant align() const
    {
        return this->v.value(QStringLiteral("align"));
    }
    virtual ModelDtoFilter&align(const QVariant & value)
    {
        return this->setAlign(value);
    }
    virtual ModelDtoFilter&setAlign(const QVariant & value)
    {
        this->v.insert(QStringLiteral("align"), value);
        return *this;
    }

    //!
    //! \brief width
    //! \return
    //!
    virtual QVariant width() const
    {
        return this->v.value(QStringLiteral("width")).toBool();
    }
    virtual ModelDtoFilter&width(const QVariant & value)
    {
        return this->setWidth(value);
    }
    virtual ModelDtoFilter&setWidth(const QVariant & value)
    {
        this->v.insert(QStringLiteral("width"), value);
        return *this;
    }

    //!
    //! \brief sortable
    //! \return
    //!
    virtual bool sortable() const
    {
        return this->v.value(QStringLiteral("sortable")).toBool();
    }
    virtual ModelDtoFilter&sortable(const QVariant &value)
    {
        return this->setSortable(value);
    }
    virtual ModelDtoFilter&setSortable(const QVariant &value)
    {
        this->v.insert(QStringLiteral("sortable"), value);
        return *this;
    }

    //!
    //! \brief filtrable
    //! \return
    //!
    virtual bool filtrable() const
    {
        return this->v.value(QStringLiteral("filtrable")).toBool();
    }
    virtual ModelDtoFilter&filtrable(const QVariant &value)
    {
        return this->setFiltrable(value);
    }
    virtual ModelDtoFilter&setFiltrable(const QVariant &value)
    {
        this->v.insert(QStringLiteral("filtrable"), value);
        return *this;
    }

    //!
    //! \brief filterStyle
    //! \return
    //!
    virtual DtoFilterStyle filterStyle() const
    {
        return DtoFilterStyle(this->v.value(QStringLiteral("filterStyle")).toInt());
    }
    virtual ModelDtoFilter&filterStyle(const DtoFilterStyle &value)
    {
        return this->setFilterStyle(value);
    }
    virtual ModelDtoFilter&setFilterStyle(const QVariant &value)
    {
        this->v.insert(QStringLiteral("filterStyle"), value);
        return *this;
    }

    //!
    //! \brief toVariant
    //! \return
    //!
    virtual QVariant toVariant()const
    {
        return this->v;
    };

private:
    QVariantHash v;
    T*___d=nullptr;
signals:
    void filteChanged();
    void widthChanged();
    void textChanged();
    void combChanged();
    void defaulChanged();
    void valueChanged();
    void typeChanged();
    void alignChanged();
    void sortableChanged();
    void filtrableChanged();
};

template <class T>
class ModelDtoFilters:public QStm::Object
{
public:

    explicit ModelDtoFilters(T*dto, QObject *parent=nullptr):QStm::Object{parent}
    {
        this->___d=dto;
    }

    virtual ~ModelDtoFilters()
    {
    }

    virtual QVariant toVar()const
    {
        QVariantList vList;
        for(const auto &v:this->___objectList)
            vList.append(v->toVariant());
        return vList;
    };

    virtual T &d()
    {
        return*this->___d;
    }

    virtual ModelDtoFilter<T>&value(const QString &v)
    {
        return this->value(QVariantHash{{vpType,v}});
    }

    virtual ModelDtoFilter<T>&value(const QVariantHash &v)
    {
        auto object=new ModelDtoFilter<T>(this->___d, this);
        object->setType(v.value(vpType));
        object->setValue(v.value(vpField));
        object->setDefaultValue(v.value(vpDefaultValue));
        object->setText(v.value(vpTitle));
        object->setAlign(v.value(vpAlign));
        object->setWidth(v.value(vpWidth));
        object->setSortable(v.value(vpSortable));
        this->___objectList.append(object);
        return*object;
    }

    virtual ModelDtoFilters<T>&clear()
    {
        this->___objectList.clear();
        return *this;
    }

    virtual QList<ModelDtoFilter<T>*>&list()
    {
        return this->___objectList;
    }

private:
    T*___d=nullptr;
    QList<ModelDtoFilter<T>*> ___objectList;
};


}
