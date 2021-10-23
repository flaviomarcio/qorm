#pragma once

#include <QMetaType>
#include <QMetaMethod>
#include <QMetaProperty>
#include "./qorm_object.h"
#include "./qorm_query.h"
#include "./qorm_model_dto.h"
#include "./qorm_model_descriptor.h"
#include "./private/p_qorm_model_info.h"
#include "./private/p_qorm_sql_suitable_parser_strategy_options.h"

namespace QOrm {

class Query;
class Q_ORM_EXPORT Model:public QStm::Object
{
    Q_OBJECT
public:
    QORM_MODEL(QOrm::Model)
    QORM_OBJECT()
    QORM_DECLARE_DESCRIPTOR(ModelDescriptor)
    QORM_DECLARE_MODEL_DESCRIPTION()
    QORM_DECLARE_TABLE_SCHEMA()
    QORM_DECLARE_TABLE(,_,)
    QORM_DECLARE_TABLE_PRIMARY_KEY()
    QORM_DECLARE_TABLE_FOREIGN_KEY()
    QORM_DECLARE_TABLE_SEQUENCE()
    QORM_DECLARE_TABLE_PRIMARY_KEY_NON_AUTO()

    class Cursor{
    public:
        /**
         * @brief read
         * @param record
         * @return
         */
        explicit Cursor(void*p);
        virtual ~Cursor();

        bool next();
    private:
        void*p=nullptr;
    };

public:

    Q_INVOKABLE explicit Model(QObject *parent = nullptr);

    Q_INVOKABLE virtual ~Model();

    /**
     * @brief clear
     * @return
     */
    virtual Model&clear();

    /**
     * @brief storedMd5Make
     * @return
     */
    Q_INVOKABLE virtual QByteArray storedMd5Make() const;

    /**
     * @brief isModifier
     * @param vMap
     * @return
     */
    virtual bool isModifier(const QVariantHash&vMap);

    virtual QVariantList toList(ResultValue &vList);
    virtual QVariantList toList(const QVariantList &vList);
    virtual QVariantMap toMap() const;
    virtual QVariantHash toHash() const;
    virtual QVariantHash toHashModel() const;
    virtual QVariantHash toMapPKValues() const;
    virtual QVariantHash toMapFKValues() const;
    virtual QVariantHash toWrapper() const;
    virtual QVariantHash toWrapper(const QString&wrapperName)const;
    virtual QVariantList toWrapper(const QString&wrapperName, const QVariantList&v)const;
    virtual QVariantList toWrapper(const QString&wrapperName, const QVariantHash&v)const;
    virtual QVariantList toWrapper(const QString&wrapperName, ResultValue &v)const;

    virtual QVariantList toWrapper(const QVariantList&v)const;
    virtual QVariantList toWrapper(const QVariantHash&v)const;
    virtual QVariantList toWrapper(ResultValue &v)const;

    virtual bool addWrapper(const QString &wrapperName, const QString &src, const QString &dst);
    virtual bool writeToWrapper(QVariantHash&wrapper) const;
    virtual bool appendToWrapper(QVariantHash&wrapper) const;
    virtual bool appendToWrapper(QVariantHash&wrapper, const QString &fromWrapper) const;
    virtual bool appendToWrapper(QVariantHash&wrapper, const QString &fromWrapper, const QString &toWrapper) const;
    virtual bool isWrapperModifier(const QVariantHash &wrapper, const QString &fromWrapper);
    virtual bool isWrapperModifierSet(const QVariantHash &wrapper);
    virtual bool isWrapperModifierSet(const QVariantHash &wrapper, const QString &fromWrapper);

    /**
     * @brief propertyBeforeSet
     * @param property
     * @param value
     * @return
     */
    virtual bool propertyBeforeSet(const QByteArray&property, const QVariant&value) const;

    /**
     * @brief propertyAfterSet
     * @param property
     * @param value
     * @return
     */
    virtual bool propertyAfterSet(const QByteArray&property, const QVariant&value) const;


    /**
     * @brief setCursor
     * @param cursor
     * @return
     */
    virtual void setCursor(QVariantList&cursor);
    virtual void setCursor(QSqlQuery&cursor);
    virtual void setCursor(Query&cursor);

    /**
     * @brief cursor
     * @return
     */
    virtual Cursor&cursor();

    /**
     * @brief read
     * @param record
     * @return
     */
    virtual ResultValue&readFrom(const QVariant &record);
    virtual ResultValue&readFrom(QSqlQuery &record);
    virtual ResultValue&readFrom(Query &record);
    virtual ResultValue&readFrom(const ResultValue &resultValue);
    virtual ResultValue&readFrom(const Model &model);
    virtual ResultValue&readWrapper(const QString&wrapper, const QVariant&vMap);
    virtual ResultValue&readWrapper(const QVariant&vMap);
    virtual ResultValue&readWrapper(const QString&wrapper, ResultValue&resultValue);
    virtual ResultValue&readWrapper(ResultValue&resultValue);

    /**
     * @brief mergeFrom
     * @param record
     * @return
     */
    virtual ResultValue&mergeFrom(const QByteArray&record);
    virtual ResultValue&mergeFrom(const QVariantHash&record);
    virtual ResultValue&mergeFrom(const QVariant &record);
    virtual ResultValue&mergeFrom(QSqlQuery &record);
    virtual ResultValue&mergeFrom(ResultValue&resultValue);


    /**
     * @brief setProperty
     * @param name
     * @param value
     * @return
     */
    virtual bool setProperty(const char *name, const QVariant &value);
    virtual bool setProperty(const QMetaProperty&property, const QVariant &value);

    /**
     * @brief uuidSet
     * @return
     *
     * define new uuid in the primarykey property when it is null/invalid
     */
    virtual ResultValue&uuidSet();

    /**
     * @brief deactivateSetValues
     * @return
     */
    virtual ResultValue&deactivateSetValues();

    /**
     * @brief datetimeSet
     * @return
     *
     * define new datetime/date/time in the primarykey property when it is null/invalid
     */
    virtual ResultValue&datetimeSet();

    /**
     * @brief isValid
     * @return
     */
    virtual ResultValue&isValid();

    /**
     * @brief isActive
     * @return
     */
    virtual ResultValue&isActive();

    /**
     * @brief isEmptyPK
     * @return
     */
    virtual ResultValue&isEmptyPK();

    /**
     * @brief beforePost
     * @return
     */
    virtual bool beforePost() const;

    /**
     * @brief afterPost
     * @return
     */
    virtual bool afterPost() const;

    /**
     * @brief operator =
     * @param v
     * @return
     */
    Model&operator=(const QVariant&v);

private:
    void*p=nullptr;
signals:
    void changeProperty();
};

}

//Q_DECLARE_METATYPE(QOrm::Model);
