#pragma once

#include <QCoreApplication>
#include <QCryptographicHash>
#include <QDebug>
#include <QHash>
#include <QStringList>
#include <QTimer>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QLocale>
#include <QJsonDocument>
#include <gtest/gtest.h>

#define CRUD_TESTER_BODY(vCrud){\
auto ___hash=QVariant(vCrud).toHash();\
    for(auto&property:Q_ORM_CRUD_PROPERTY_LIST){\
        bool contains=___hash.contains(property);\
        EXPECT_EQ(contains,true);\
    }\
}

#define CRUD_TESTER_REQUEST(request, body, method)\
request.call(QRpc::Get, method, body);\
EXPECT_EQ(request.response().isOk(), true);\
if(request.response().isOk())\
    CRUD_TESTER_BODY(request.response().bodyHash());

namespace QOrm {

class SDKGoogleTest : public testing::Test{
public:

    explicit SDKGoogleTest();

    //!
    //! \brief SetUpTestCase
    //!
    static void SetUpTestCase()
    {
    }

    //!
    //! \brief SetUp
    //!
    virtual void SetUp()
    {
    }

    //!
    //! \brief TearDown
    //!
    virtual void TearDown()
    {
    }

    //!
    //! \brief TearDownTestCase
    //!
    static void TearDownTestCase()
    {
    }

    //!
    //! \brief clear
    //! \return
    //!
    virtual bool clear();

    //!
    //! \brief arguments
    //! \return
    //!
    virtual QStringList arguments()const;

    //!
    //! \brief toMd5
    //! \param v
    //! \return
    //!
    static const QByteArray toMd5(const QVariant &v);

    //!
    //! \brief toVar
    //! \param v
    //! \return
    //!
    static const QVariant toVar(const QVariant &v);

public:


};
}
