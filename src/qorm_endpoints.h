#pragma once

#include <QMFECommon>
#include <QObject>
#include <QVariantMap>
#include <QUrl>
#include "./qmfe_endpoint.h"

namespace QMFE {

class EndPointsPvt;

//!
//! \brief The EndPoints class
//!
class Q_MFE_EXPORT EndPoints : public ObjectWrapper
{
    Q_OBJECT
    QMFE_OBJECT_WRAPPER(EndPoints)
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
    virtual EndPoint *method(const Network::Method &method);

    //!
    //! \brief insert
    //! \param name
    //! \param link
    //!
    virtual void insert(const QString &name, EndPoint *link);
    Q_INVOKABLE virtual void insert(const QString &name, const QVariant &link);

    //!
    //! \brief remove
    //! \param name
    //!
    Q_INVOKABLE virtual void remove(const QString &name);


private:
    EndPointsPvt *p=nullptr;
protected:
};

} // namespace QMFE
