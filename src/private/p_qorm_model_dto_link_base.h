#pragma once

#include "./p_qorm_model_dto_types.h"

namespace PrivateQOrm {

class ModelDtoLinkItem : public QStm::Object{
    Q_OBJECT
public:
    Q_PROPERTY(QVariantHash header      READ header     WRITE setHeader        )
    Q_PROPERTY(QVariant     method      READ method     WRITE setMethod      )
    Q_PROPERTY(QVariant     ref         READ ref        WRITE setRef         )
    Q_PROPERTY(QVariant     hRef        READ hRef       WRITE setHRef        )
    Q_PROPERTY(QVariant     parameters  READ parameters WRITE setParameters  )

    Q_INVOKABLE explicit ModelDtoLinkItem(QObject*dtpParent, QObject*parent=nullptr);

    Q_INVOKABLE ~ModelDtoLinkItem();

    QUuid&uuid() const;

    QObject&d();

    bool isMethodGet() const;

    bool isMethodPost() const;

    bool isMethodPut() const;

    bool isMethodDelete() const;

    QVariantHash&header() const;

    ModelDtoLinkItem&setHeader(const QVariant&value);

    QVariant&method() const;

    ModelDtoLinkItem&setMethod(const QVariant&value);

    QVariant&ref() const;

    ModelDtoLinkItem&setRef(const QVariant&value);

    QVariant&hRef() const;

    ModelDtoLinkItem&setHRef(const QVariant&value);

    QVariant&parameters() const;

    ModelDtoLinkItem&setParameters(const QVariant&value);

    QVariant toVariant() const;

private:
    void*p=nullptr;
};

class ModelDtoLinkCollection:public QStm::Object
{
    Q_OBJECT
public:

    explicit ModelDtoLinkCollection(QObject*dtoParent, QObject*parent=nullptr);

    Q_INVOKABLE ~ModelDtoLinkCollection();

    QVariant toVar() const;

    QObject&d();

    QObject&ref(ModelDtoLinkItem*link=nullptr);

    ModelDtoLinkItem*find(const QVariant&ref);

    ModelDtoLinkCollection&clear();

    QHash<QString, ModelDtoLinkItem *> &list()const;
private:
    void*p=nullptr;
};

}
