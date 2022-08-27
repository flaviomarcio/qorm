#pragma once

#include "../../../qstm/src/qstm_object_wrapper.h"
#include "../qorm_global.h"
#include "./p_qorm_model_dto_host.h"

namespace QOrm {
class EndPointPvt;
//!
//! \brief The EndPoint class
//!
class Q_ORM_EXPORT EndPoint : public QStm::ObjectWrapper
{
    Q_OBJECT

    Q_STM_OBJECT_WRAPPER(EndPoint)

    Q_PROPERTY(QVariant method READ method WRITE setMethod RESET resetMethod NOTIFY methodChanged)

    Q_PROPERTY(QUuid uuid READ uuid WRITE setUuid RESET resetUuid NOTIFY uuidChanged)

    Q_PROPERTY(Host *host READ host WRITE setHost RESET resetHost NOTIFY hostChanged)

    Q_PROPERTY(QByteArray path READ path WRITE setPath RESET resetPath NOTIFY pathChanged)

    Q_PROPERTY(QByteArray basePath READ basePath WRITE setBasePath RESET resetBasePath NOTIFY basePathChanged)

    Q_PROPERTY(QByteArray url READ url NOTIFY urlChanged)

    Q_PROPERTY(QByteArray name READ name WRITE setName RESET resetName NOTIFY nameChanged)
public:

    //!
    //! \brief EndPoint
    //! \param parent
    //!
    explicit EndPoint(QObject *parent = nullptr);

    //!
    //! \brief isValid
    //! \return
    //!
    Q_INVOKABLE bool isValid()const;

    //!
    //! \brief addAuthBasic
    //! \param userName
    //! \param password
    //!
    Q_INVOKABLE void addContentType(const QByteArray &applicationType) const;

    Q_INVOKABLE void addContentTypeJSON() const;

    Q_INVOKABLE void addContentTypeXML() const;

    Q_INVOKABLE void addContentTypeTextXML() const;

    Q_INVOKABLE void addContentTypeX_WWW_FORT_URLENCODED() const;

    Q_INVOKABLE void addContentTypeDefault() const;

    Q_INVOKABLE QVariantHash contentTypeDefault();

    Q_INVOKABLE QVariantHash &headers() const;

    //!
    //! \brief addAuthBasic
    //! \param userName
    //! \param password
    //!
    Q_INVOKABLE void addAuthBasic(const QByteArray &userName, const QByteArray &password) const;

    //!
    //! \brief addAuthBearer
    //! \param credentials
    //!
    Q_INVOKABLE void addAuthBearer(const QByteArray &credentials) const;

    //!
    //! \brief uuid
    //! \return
    //!
    const QUuid &uuid() const;
    void setUuid(const QUuid &newUuid);
    void resetUuid();

    //!
    //! \brief host
    //! \return
    //!
    Host *host() const;
    void setHost(Host *newHost);
    void resetHost();

    //!
    //! \brief path
    //! \return
    //!
    const QByteArray &path() const;
    void setPath(const QByteArray &newPath);
    void resetPath();

    //!
    //! \brief basePath
    //! \return
    //!
    const QByteArray &basePath() const;
    void setBasePath(const QByteArray &newBasePath);
    void resetBasePath();

    //!
    //! \brief method
    //! \return
    //!
    QVariant &method() const;
    void setMethod(const QVariant &newMethod);
    void resetMethod();


    //!
    //! \brief url
    //! \return
    //!
    QByteArray &url() const;

    const QByteArray &name() const;
    void setName(const QByteArray &newName);
    void resetName();

private:
    EndPointPvt *p=nullptr;
signals:
    void uuidChanged();
    void hostChanged();
    void paramertersChanged();
    void bodyChanged();
    void pathChanged();
    void basePathChanged();
    void methodChanged();
    void urlChanged();
    void nameChanged();
};

} // namespace QOrm
