#pragma once

#include <QStm>
#include "../qorm_global.h"

namespace QOrm {

class Q_ORM_EXPORT Host : public QStm::ObjectWrapper
{
    Q_OBJECT
    QSTM_OBJECT_WRAPPER(Host)
    Q_PROPERTY(QString protocol READ protocol WRITE setProtocol RESET resetProtocol NOTIFY protocolChanged)

    Q_PROPERTY(QString hostName READ hostName WRITE setHostName RESET resetHostName NOTIFY hostNameChanged)

    Q_PROPERTY(int port READ port WRITE setPort RESET resetPort NOTIFY portChanged)

    Q_PROPERTY(QVariantHash headers READ headers WRITE setHeaders RESET resetHeaders NOTIFY headersChanged)

    Q_PROPERTY(QString basePath READ basePath WRITE setBasePath RESET resetBasePath NOTIFY basePathChanged)
public:
    Q_INVOKABLE explicit Host(QObject *parent = nullptr);

    //!
    //! \brief isValid
    //! \return
    //!
    Q_INVOKABLE bool isValid()const;

    //!
    //! \brief isEmpty
    //! \return
    //!
    Q_INVOKABLE bool isEmpty()const;

    //!
    //! \brief protocol
    //! \return
    //!
    const QString &protocol() const;
    void setProtocol(const QString &newProtocol);
    void resetProtocol();

    //!
    //! \brief hostName
    //! \return
    //!
    const QString &hostName() const;
    void setHostName(const QString &newHostName);
    void resetHostName();

    //!
    //! \brief port
    //! \return
    //!
    int port() const;
    void setPort(int newPort);
    void resetPort();

    //!
    //! \brief headers
    //! \return
    //!
    QVariantHash &headers();
    void setHeaders(const QVariantHash &newHeaders);
    void resetHeaders();

    //!
    //! \brief basePath
    //! \return
    //!
    const QString &basePath() const;
    void setBasePath(const QString &newBasePath);
    void resetBasePath();

private:
    QString _protocol;
    QString _hostName;
    int _port;
    QVariantHash _headers;
    QString _basePath;


signals:

    void protocolChanged();
    void hostNameChanged();
    void portChanged();
    void headersChanged();
    void basePathChanged();
};

} // namespace QMFE
