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
#include "./qstm_types.h"

static const QVector<QByteArray> Q_ORM_CRUD_PROPERTY_LIST=QVector<QByteArray>{"resultInfo","items","links","text","filters","type","layout","headers","id"};

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

        explicit SDKGoogleTest(){
            this->clear();
            QLocale::setDefault(QLocale(QLocale::Portuguese, QLocale::Brazil));
        }

        virtual bool clear(){
            return true;
        }

        virtual bool serviceStart(){
            return this->clear();
        }

        virtual bool serviceStop(){
            return this->clear();
        }

        virtual QStringList arguments(){
            return qApp->arguments();
        }

        static QByteArray toMd5(const QVariant&v){
            QByteArray bytes;
            if(QStmTypesObjectMetaData.contains(qTypeId(v)))
                bytes=QJsonDocument::fromVariant(v).toJson(QJsonDocument::Compact);
            else
                bytes=v.toByteArray();
            return QCryptographicHash::hash(bytes, QCryptographicHash::Md5).toHex();
        }

        static QVariant toVar(const QVariant&v){
            if(QStmTypesListString.contains(qTypeId(v)))
                return QJsonDocument::fromJson(v.toByteArray()).toVariant();
            return v;
        }

        static void SetUpTestCase()
        {
        }

        virtual void SetUp()
        {
        }

        virtual void TearDown()
        {
        }

        static void TearDownTestCase()
        {
        }

    public:

    };

}
