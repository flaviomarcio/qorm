#pragma once

#include <QObject>
#include <QVariantMap>
#include <QUrl>
#include "./p_qorm_model_dto_endpoint.h"
#include "./p_qorm_model_dto_host.h"

namespace QOrm {

class EndPointsPvt;

//!
//! \brief The EndPoints class
//!
class Q_ORM_EXPORT EndPoints : public QStm::ObjectWrapper
{
    Q_OBJECT
    Q_STM_OBJECT_WRAPPER(EndPoints)
    Q_PROPERTY(Host *host READ host WRITE setHost RESET resetHost NOTIFY hostChanged)
    Q_PROPERTY(QVariantList items READ items WRITE setItems RESET resetItems NOTIFY hostChanged)
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
    //! \brief host
    //! \return
    //!
    Host *host()const;
    void host(const Host *newHost);
    void host(const QVariant &newHost);
    void setHost(const Host *newHost);
    void resetHost();

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
    virtual EndPoint *value(const QUuid &uuid);

    //!
    //! \brief value
    //! \param v
    //! \return
    //!
    virtual EndPoint *method(const QVariant &method);

    //!
    //! \brief insert
    //! \param endPoint
    //!
    virtual void insert(EndPoint *endPoint);
    Q_INVOKABLE virtual void insert(const QVariant &endPoint);

    //!
    //! \brief remove
    //! \param name
    //!
    Q_INVOKABLE virtual void remove(const QUuid &uuid);

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
    const QVariantList &toList();

    //!
    //! \brief toHash
    //! \return
    //!
    const QVariantHash toHash() const;

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

    void hostChanged();

private:
    EndPointsPvt *p=nullptr;
protected:
};

} // namespace QOrm
