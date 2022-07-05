#pragma once

#include <QStm>
#include "../qorm_global.h"

namespace QOrm {

class HostPvt;
class Q_STM_EXPORT Host : public QStm::ObjectWrapper
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

    ~Host();

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

    const QString &protocol() const;
    void setProtocol(const QString &newProtocol);
    void resetProtocol();

    const QString &hostName() const;
    void setHostName(const QString &newHostName);
    void resetHostName();

    int port() const;
    void setPort(int newPort);
    void resetPort();

    QVariantHash &headers()const;
    void setHeaders(const QVariantHash &newHeaders);
    void addHeaders(const QVariantHash &newHeaders);
    void resetHeaders();

    const QString &basePath() const;
    void setBasePath(const QString &newBasePath);
    void resetBasePath();

private:
    HostPvt *p=nullptr;
signals:

    void protocolChanged();
    void hostNameChanged();
    void portChanged();
    void headersChanged();
    void basePathChanged();
};

} // namespace QOrm
