#pragma once

#include <QObject>
#include <QVariantMap>
#include <QUrl>
#include "./p_qorm_model_dto_endpoint.h"

namespace QOrm {

class EndPointsPvt;

//!
//! \brief The EndPoints class
//!
class Q_STM_EXPORT EndPoints : public QStm::ObjectWrapper
{
    Q_OBJECT
    QSTM_OBJECT_WRAPPER(EndPoints)
    Q_PROPERTY(QVariant items READ items WRITE setItems RESET resetItems NOTIFY itemsChanged)
public:
    //!
    //! \brief EndPoints
    //! \param parent
    //!
    explicit EndPoints(QObject *parent = nullptr);

    //!
    //! \brief setValues
    //! \param v
    //! \return
    //!
    virtual bool setValues(const QVariant &v);

    //!
    //! \brief clear
    //!
    Q_INVOKABLE void clear();

    //!
    //! \brief count
    //! \return
    //!
    Q_INVOKABLE virtual int count() const;

    //!
    //! \brief value
    //! \param name
    //! \return
    //!
    virtual EndPoint *endpoint();

    //!
    //! \brief value
    //! \param v
    //! \return
    //!
    virtual EndPoint *value(const QString &name);

    //!
    //! \brief value
    //! \param v
    //! \return
    //!
    virtual EndPoint *method(const QVariant &method);

    //!
    //! \brief insert
    //! \param name
    //! \param link
    //!
    virtual void insert(const QString &name, EndPoint *endPoint);
    Q_INVOKABLE virtual void insert(const QVariant &endPoint);

    //!
    //! \brief remove
    //! \param name
    //!
    Q_INVOKABLE virtual void remove(const QString &name);

    //!
    //! \brief items
    //! \return
    //!
    QVariantList &items() const;
    void setItems(const QVariant &newItems);
    void resetItems();

    //!
    //! \brief toList
    //! \return
    //!
    const QVariantList toList();

    //!
    //! \brief toHash
    //! \return
    //!
    const QVariantHash toHash();

    //!
    //! \brief toObjectList
    //! \return
    //!
    QVector<EndPoint*> &toObjectList()const;

    //!
    //! \brief toObjectHash
    //! \return
    //!
    QHash<QString, EndPoint*> &toObjectHash()const;

signals:
    void itemsChanged();

private:
    EndPointsPvt *p=nullptr;
protected:
};

} // namespace QOrm
