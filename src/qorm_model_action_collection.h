#pragma once

#include <QtReforce/QStm>
#include "./qorm_global.h"
#include "./qorm_model_action_descriptor.h"

namespace QOrm {

class ModelActionCollectionPvt;
class Q_ORM_EXPORT ModelActionCollection : public QStm::ObjectWrapper
{
    Q_OBJECT
    Q_STM_OBJECT_WRAPPER(ModelActionCollection)
    Q_PROPERTY(QVariantList items READ items WRITE setItems RESET resetItems NOTIFY itemsChanged)
public:
    explicit ModelActionCollection(QObject *parent = nullptr);

    //!
    //! \brief item
    //! \param fieldName
    //! \return
    //!
    ModelActionDescriptor &item(const QString &actionName);
    const ModelActionCollection &item(const QString &actionName, const QVariant &values)const;

    //!
    //! \brief list
    //! \return
    //!
    const QList<ModelActionDescriptor*> &list()const;

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
    ModelActionCollection &setItems(const QVariant &newItems);
    ModelActionCollection &resetItems();

signals:
    void itemsChanged();

private:
    ModelActionCollectionPvt *p=nullptr;
};

} // namespace QOrm

