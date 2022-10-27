#pragma once

#include <QtReforce/QStm>
#include "./qorm_global.h"
#include "./qorm_model_field_descriptor.h"

namespace QOrm {

class ModelFieldCollectionPvt;
class Q_ORM_EXPORT ModelFieldCollection : public QStm::ObjectWrapper
{
    Q_OBJECT
    Q_STM_OBJECT_WRAPPER(ModelFieldCollection)
    Q_PROPERTY(QVariantList items READ items WRITE setItems RESET resetItems NOTIFY itemsChanged)
public:
    explicit ModelFieldCollection(QObject *parent = nullptr);

    //!
    //! \brief item
    //! \param fieldName
    //! \return
    //!
    ModelFieldDescriptor &item(const QString &fieldName);
    const ModelFieldCollection &item(const QString &fieldName, const QVariant &values)const;

    //!
    //! \brief remove
    //! \param fieldName
    //!
    void remove(const QString &fieldName);

    //!
    //! \brief list
    //! \return
    //!
    const QList<ModelFieldDescriptor*> &list()const;

    //!
    //! \brief toList
    //! \return
    //!
    const QVariantList &toList() const;

    //!
    //! \brief items
    //! \return
    //!
    const QVariantList &items() const;
    ModelFieldCollection &setItems(const QVariant &newItems);
    ModelFieldCollection &resetItems();

signals:
    void itemsChanged();

private:
    ModelFieldCollectionPvt *p=nullptr;
};

} // namespace QOrm

