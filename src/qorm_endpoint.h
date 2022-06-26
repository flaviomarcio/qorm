#pragma once

#include "../common/qmfe_object_wrapper.h"
#include "./qmfe_host.h"
#include "./qmfe_data_body.h"
#include "./qmfe_network_types.h"
#include <QUuid>

namespace QMFE {
class EndPointPvt;
//!
//! \brief The EndPoint class
//!
class Q_MFE_EXPORT EndPoint : public ObjectWrapper
{
    Q_OBJECT

    QMFE_OBJECT_WRAPPER(EndPoint)

    Q_PROPERTY(QVariant method READ method WRITE setMethod RESET resetMethod NOTIFY methodChanged)

    Q_PROPERTY(QUuid uuid READ uuid WRITE setUuid RESET resetUuid NOTIFY uuidChanged)

    Q_PROPERTY(Host *host READ host WRITE setHost RESET resetHost NOTIFY hostChanged)

    Q_PROPERTY(DataBody *paramerters READ paramerters WRITE setParamerters RESET resetParamerters NOTIFY paramertersChanged)

    Q_PROPERTY(DataBody *body READ body WRITE setBody RESET resetBody NOTIFY bodyChanged)

    Q_PROPERTY(QString path READ path WRITE setPath RESET resetPath NOTIFY pathChanged)

    Q_PROPERTY(QString basePath READ basePath WRITE setBasePath RESET resetBasePath NOTIFY basePathChanged)

    Q_PROPERTY(QString url READ url NOTIFY urlChanged)

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
    Q_INVOKABLE void addContentType(const QString &applicationType) const;

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
    Q_INVOKABLE void addAuthBasic(const QString &userName, const QString &password) const;

    //!
    //! \brief addAuthBearer
    //! \param credentials
    //!
    Q_INVOKABLE void addAuthBearer(const QString &credentials) const;

    //!
    //! \brief uuid
    //! \return
    //!
    QUuid &uuid() const;
    void setUuid(const QUuid &newUuid);
    void resetUuid();

    //!
    //! \brief host
    //! \return
    //!
    Host *host();
    void setHost(Host *newHost);
    void resetHost();

    //!
    //! \brief paramerters
    //! \return
    //!
    DataBody *paramerters() const;
    void setParamerters(const DataBody *newParamerters);
    void setParamerters(const QVariant &newParamerters);
    void resetParamerters();

    //!
    //! \brief paramerters
    //! \return
    //!
    DataBody *body() const;
    void setBody(const DataBody *newBody);
    void setBody(const QVariant &newBody);
    void resetBody();

    //!
    //! \brief path
    //! \return
    //!
    const QString &path() const;
    void setPath(const QString &newPath);
    void resetPath();

    //!
    //! \brief basePath
    //! \return
    //!
    const QString &basePath() const;
    void setBasePath(const QString &newBasePath);
    void resetBasePath();

    //!
    //! \brief method
    //! \return
    //!
    Network::Method method();
    void setMethod(const QVariant &newMethod);
    void resetMethod();


    //!
    //! \brief url
    //! \return
    //!
    QString &url() const;

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
};

} // namespace QMFE
