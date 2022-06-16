#pragma once

#include "./p_qorm_model_dto_types.h"

namespace PrivateQOrm {
class ModelDtoLinkItemPvt;
class ModelDtoLinkBaseCollectionPvt;
//!
//! \brief The ModelDtoLinkItem class
//!
class ModelDtoLinkItem : public QStm::Object{
    Q_OBJECT
public:
    Q_PROPERTY(QVariantHash header READ header WRITE setHeader NOTIFY headerChanged)
    Q_PROPERTY(QVariant method READ method WRITE setMethod NOTIFY methodChanged)
    Q_PROPERTY(QVariant ref READ ref WRITE setRef NOTIFY refChanged)
    Q_PROPERTY(QVariant hRef READ hRef WRITE setHRef NOTIFY hRefChanged)
    Q_PROPERTY(QVariant parameters READ parameters WRITE setParameters NOTIFY parametersChanged)

    //!
    //! \brief ModelDtoLinkItem
    //! \param dtpParent
    //! \param parent
    //!
    explicit ModelDtoLinkItem(QObject*dtpParent, QObject*parent=nullptr);

    //!
    //!
    ~ModelDtoLinkItem();

    //!
    //! \brief uuid
    //! \return
    //!
    QUuid&uuid() const;

    //!
    //! \brief d
    //! \return
    //!
    QObject&d();

    //!
    //! \brief isMethodGet
    //! \return
    //!
    bool isMethodGet() const;

    //!
    //! \brief isMethodPost
    //! \return
    //!
    bool isMethodPost() const;

    //!
    //! \brief isMethodPut
    //! \return
    //!
    bool isMethodPut() const;

    //!
    //! \brief isMethodDelete
    //! \return
    //!
    bool isMethodDelete() const;

    //!
    //! \brief header
    //! \return
    //!
    QVariantHash&header() const;

    //!
    //! \brief setHeader
    //! \param value
    //! \return
    //!
    ModelDtoLinkItem&setHeader(const QVariant &value);

    //!
    //! \brief method
    //! \return
    //!
    QVariant&method() const;

    //!
    //! \brief setMethod
    //! \param value
    //! \return
    //!
    ModelDtoLinkItem&setMethod(const QVariant &value);

    //!
    //! \brief ref
    //! \return
    //!
    QVariant&ref() const;

    //!
    //! \brief setRef
    //! \param value
    //! \return
    //!
    ModelDtoLinkItem&setRef(const QVariant &value);

    //!
    //! \brief hRef
    //! \return
    //!
    QVariant&hRef() const;
    ModelDtoLinkItem&setHRef(const QVariant &value);

    //!
    //! \brief parameters
    //! \return
    //!
    QVariant&parameters() const;
    ModelDtoLinkItem&setParameters(const QVariant &value);

    //!
    //! \brief toVariant
    //! \return
    //!
    QVariant toVariant() const;
signals:
    void headerChanged();
    void methodChanged();
    void refChanged();
    void hRefChanged();
    void parametersChanged();
private:
    ModelDtoLinkItemPvt*p=nullptr;
};

//!
//! \brief The ModelDtoLinkCollection class
//!
class ModelDtoLinkCollection:public QStm::Object
{
    Q_OBJECT
public:

    //!
    //! \brief ModelDtoLinkCollection
    //! \param dtoParent
    //! \param parent
    //!
    explicit ModelDtoLinkCollection(QObject*dtoParent, QObject*parent=nullptr);

    //!
    //!
    ~ModelDtoLinkCollection();

    //!
    //! \brief toVar
    //! \return
    //!
    QVariant toVar() const;

    //!
    //! \brief d
    //! \return
    //!
    QObject&d();

    //!
    //! \brief ref
    //! \param link
    //! \return
    //!
    QObject&ref(ModelDtoLinkItem*link=nullptr);

    //!
    //! \brief find
    //! \param ref
    //! \return
    //!
    ModelDtoLinkItem*find(const QVariant&ref);

    //!
    //! \brief clear
    //! \return
    //!
    ModelDtoLinkCollection&clear();

    //!
    //! \brief list
    //! \return
    //!
    QHash<QString, ModelDtoLinkItem *> &list()const;
private:
    ModelDtoLinkBaseCollectionPvt*p=nullptr;
};

}
