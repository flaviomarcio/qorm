#pragma once

#include <QTest>
#include <QCoreApplication>
#include <QCryptographicHash>
#include <QDebug>
#include <QUuid>
#include <QStringList>
#include <QTimer>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QLocale>
#include <QJsonDocument>
#include <QTest>

namespace QOrm{

#define Q_ORM_OBJECT_TEST(OBJECT) \
public:\
    Q_INVOKABLE explicit OBJECT(QObject *parent=nullptr):QStm::ObjectTest{parent}{};

class ObjectTest : public QObject{
    Q_OBJECT
public:
    Q_INVOKABLE explicit ObjectTest(QObject *parent=nullptr);

    virtual void execute();
};

}
