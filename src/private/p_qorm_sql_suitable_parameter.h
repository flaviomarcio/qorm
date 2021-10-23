#pragma once

#include <QVariant>
#include <QVariantHash>
#include <QVariantHash>

#include "../qorm_global.h"
#include "../qorm_sql_suitable_types.h"

class Q_ORM_EXPORT SearchParam:public QVariantHash
{
public:
    explicit SearchParam(const QVariant&v=QVariant());
    explicit SearchParam(const QVariant&valueA, const QVariant &valueB, const QVariant&keywordOperator, const QVariant&keywordLogical);
    explicit SearchParam(const QVariant&valueA, const QVariant &valueB, const QVariant &valueC, const QVariant&keywordOperator, const QVariant&keywordLogical);
    virtual ~SearchParam();
    bool isValid();
    QString key()const;
    QVariant valueA()const;
    QVariant valueB()const;
    QVariant valueC()const;
    QOrm::KeywordOperator keywordOperator()const;
    QOrm::KeywordLogical keywordLogical()const;

    static SearchParam from(const QVariant&value);

};

class Q_ORM_EXPORT SearchParameters:public QVariant
{
public:
    explicit SearchParameters(const QVariant &other=QVariant());
    virtual ~SearchParameters();



    /**
     * @brief canRead
     * @param v
     * @return
     */
    virtual bool canRead(const QVariant&v);

    /**
     * @brief isEmpty
     * @return
     */
    virtual bool isEmpty() const;

    /**
     * @brief operator =s
     * @param v
     * @return
     */
    SearchParameters&operator=(const QVariant &v);

    /**
     * @brief operator +=
     * @param v
     * @return
     */
    SearchParameters&operator+=(const QVariant &v);

    /**
     * @brief from
     * @param v
     * @return
     */
    static SearchParameters from(const QVariantHash &v);
    static SearchParameters from(const QVariantMap &v);

    /**
     * @brief insert
     * @param valueA
     * @param valueB
     * @param valueC
     * @param keywordOperator
     * @param keywordLogical
     * @return
     */
    virtual SearchParameters &insert(const QVariant&valueA, const QVariant&valueB);
    virtual SearchParameters &insert(const QVariant&valueA, const QVariant&valueB, const QVariant &keywordOperator);
    virtual SearchParameters &insert(const QVariant&valueA, const QVariant&valueB, const QVariant &keywordOperator, const QVariant &keywordLogical);
    virtual SearchParameters &insert(const QVariant&valueA, const QVariant&valueB, const QVariant&valueC, const QVariant &keywordOperator, const QVariant &keywordLogical);

    /**
     * @brief buildList
     * @return
     */
    virtual QList<SearchParam> build() const;

    /**
     * @brief buildVariant
     * @return
     */
    virtual QVariant buildVariant() const;

private:
    void*p=nullptr;
};

Q_DECLARE_METATYPE(SearchParam)
Q_DECLARE_METATYPE(SearchParam*)

Q_DECLARE_METATYPE(SearchParameters)
Q_DECLARE_METATYPE(SearchParameters*)
