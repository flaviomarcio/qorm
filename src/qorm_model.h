#pragma once

#include <QMetaType>
#include <QMetaMethod>
#include <QMetaProperty>
#include <QDateTime>
#include <QVariant>
#include <QVariantHash>
#include <QUuid>
#include <QSqlQuery>
#include "./qorm_global.h"
#include "./qorm_object.h"
#include "./qorm_model_macro.h"
#include "./qorm_model_field_descriptors.h"
#include "./qorm_sql_suitable_types.h"
#include "./private/p_qorm_model_info.h"

class QSqlQuery;

namespace QOrm {
class ModelPvt;
class Query;
//!
//! \brief The Model class
//!
class Q_ORM_EXPORT Model:public QOrm::Object
{
    Q_OBJECT
public:
    QORM_MODEL(QOrm::Model)
    QORM_DECLARE_DESCRIPTOR(ModelFieldDescriptors)
    QORM_DECLARE_MODEL_DESCRIPTION()
    QORM_DECLARE_TABLE_SCHEMA()
    QORM_DECLARE_TABLE(,_,)
    QORM_DECLARE_TABLE_PRIMARY_KEY()
    QORM_DECLARE_TABLE_SEQUENCE()
    QORM_DECLARE_TABLE_PRIMARY_KEY_NON_AUTO()
    QORM_DECLARE_FILTRABLE_FIELD({})

public:
    //!
    //! \brief Model
    //! \param parent
    //!
    Q_INVOKABLE explicit Model(QObject *parent = nullptr);

public:
    //!
    //! \brief tablePkCompuser
    //! \return
    //!
    Q_INVOKABLE virtual QVariant tablePkCompuser()const;
public:
    //!
    //! \brief clear
    //! \return
    //!
    virtual Model &clear();

    //!
    //! \brief makeUuid
    //! \return
    //!
    virtual bool makeUuid();

    //!
    //! \brief autoMakeUuid
    //!
    virtual bool autoMakeUuid();

    //!
    //! \brief storedMd5Make
    //! \return
    //!
    virtual QByteArray storedMd5Make() const;

    //!
    //! \brief isModifier
    //! \param vMap
    //! \return
    //!
    virtual bool isModifier(const QVariantHash &vMap);

    //!
    //! \brief toList
    //! \param vList
    //! \return
    //!
    virtual QVariantList toList(ResultValue &vList);

    //!
    //! \brief toList
    //! \param vList
    //! \return
    //!
    virtual QVariantList toList(const QVariantList &vList);

    //!
    //! \brief toHash
    //! \return
    //!
    virtual QVariantHash toHash() const;

    //!
    //! \brief toHashModel
    //! \return
    //!
    virtual QVariantHash toHashModel(bool nullValuesAdd=true) const;

    //!
    //! \brief toPKValues
    //! \return
    //!
    virtual QVariantHash toPKValues() const;

    //!
    //! \brief toPKValuesValid
    //! \return
    //!
    virtual QVariantHash toPKValuesValid() const;

    //!
    //! \brief toFKValues
    //! \return
    //!
    virtual QVariantHash toFKValues() const;

    //!
    //! \brief toWrapper
    //! \return
    //!
    virtual QVariantHash toWrapper() const;

    //!
    //! \brief toWrapper
    //! \param wrapperName
    //! \return
    //!
    virtual QVariantHash toWrapper(const QString &wrapperName)const;

    //!
    //! \brief toWrapper
    //! \param wrapperName
    //! \param v
    //! \return
    //!
    virtual QVariantList toWrapper(const QString &wrapperName, const QVariantList &v)const;

    //!
    //! \brief toWrapper
    //! \param wrapperName
    //! \param v
    //! \return
    //!
    virtual QVariantList toWrapper(const QString &wrapperName, const QVariantHash &v)const;

    //!
    //! \brief toWrapper
    //! \param wrapperName
    //! \param v
    //! \return
    //!
    virtual QVariantList toWrapper(const QString &wrapperName, ResultValue &v)const;

    //!
    //! \brief toWrapper
    //! \param v
    //! \return
    //!
    virtual QVariantList toWrapper(const QVariantList &v)const;

    //!
    //! \brief toWrapper
    //! \param v
    //! \return
    //!
    virtual QVariantList toWrapper(const QVariantHash &v)const;

    //!
    //! \brief toWrapper
    //! \param v
    //! \return
    //!
    virtual QVariantList toWrapper(ResultValue &v)const;

    //!
    //! \brief addWrapper
    //! \param wrapperName
    //! \param src
    //! \param dst
    //! \return
    //!
    virtual bool addWrapper(const QString &wrapperName, const QString &src, const QString &dst);

    //!
    //! \brief writeToWrapper
    //! \param wrapper
    //! \return
    //!
    virtual bool writeToWrapper(QVariantHash &wrapper) const;

    //!
    //! \brief appendToWrapper
    //! \param wrapper
    //! \return
    //!
    virtual bool appendToWrapper(QVariantHash &wrapper) const;

    //!
    //! \brief appendToWrapper
    //! \param wrapper
    //! \param fromWrapper
    //! \return
    //!
    virtual bool appendToWrapper(QVariantHash &wrapper, const QString &fromWrapper) const;

    //!
    //! \brief appendToWrapper
    //! \param wrapper
    //! \param fromWrapper
    //! \param toWrapper
    //! \return
    //!
    virtual bool appendToWrapper(QVariantHash &wrapper, const QString &fromWrapper, const QString &toWrapper) const;

    //!
    //! \brief isWrapperModifier
    //! \param wrapper
    //! \param fromWrapper
    //! \return
    //!
    virtual bool isWrapperModifier(const QVariantHash &wrapper, const QString &fromWrapper);

    //!
    //! \brief isWrapperModifierSet
    //! \param wrapper
    //! \return
    //!
    virtual bool isWrapperModifierSet(const QVariantHash &wrapper);

    //!
    //! \brief isWrapperModifierSet
    //! \param wrapper
    //! \param fromWrapper
    //! \return
    //!
    virtual bool isWrapperModifierSet(const QVariantHash &wrapper, const QString &fromWrapper);

    //!
    //! \brief propertyBeforeSet
    //! \param property
    //! \param value
    //! \return
    //!
    virtual bool propertyBeforeSet(const QByteArray &property, const QVariant &value) const;

    //!
    //! \brief propertyAfterSet
    //! \param property
    //! \param value
    //! \return
    //!
    virtual bool propertyAfterSet(const QByteArray &property, const QVariant &value) const;

    //!
    //! \brief readFrom
    //! \param record
    //! \return
    //!
    virtual ResultValue &readFrom(const QVariant &values);

    //!
    //! \brief readFrom
    //! \param record
    //! \return
    //!
    virtual ResultValue &readFrom(const QSqlQuery *values);

    //!
    //! \brief readFrom
    //! \param record
    //! \return
    //!
    virtual ResultValue &readFrom(const Query *values);

    //!
    //! \brief readFrom
    //! \param resultValue
    //! \return
    //!
    virtual ResultValue &readFrom(const ResultValue &resultValue);

    //!
    //! \brief readFrom
    //! \param model
    //! \return
    //!
    virtual ResultValue &readFrom(const Model &model);

    //!
    //! \brief readWrapper
    //! \param wrapper
    //! \param vMap
    //! \return
    //!
    virtual ResultValue &readWrapper(const QString &wrapper, const QVariant &values);

    //!
    //! \brief readWrapper
    //! \param vMap
    //! \return
    //!
    virtual ResultValue &readWrapper(const QVariant &values);

    //!
    //! \brief readWrapper
    //! \param wrapper
    //! \param resultValue
    //! \return
    //!
    virtual ResultValue &readWrapper(const QString &wrapper, ResultValue &resultValue);

    //!
    //! \brief readWrapper
    //! \param resultValue
    //! \return
    //!
    virtual ResultValue &readWrapper(ResultValue &resultValue);

    //!
    //! \brief mergeWrapper
    //! \param value
    //! \return
    //!
    virtual ResultValue &mergeWrapper(const QVariant &values);

    //!
    //! \brief mergeFrom
    //! \param record
    //! \return
    //!
    virtual ResultValue &mergeFrom(const QByteArray &values);

    //!
    //! \brief mergeFrom
    //! \param record
    //! \return
    //!
    virtual ResultValue &mergeFrom(const QVariantHash &values);

    //!
    //! \brief mergeFrom
    //! \param record
    //! \return
    //!
    virtual ResultValue &mergeFrom(const QVariant &values);

    //!
    //! \brief mergeFrom
    //! \param record
    //! \return
    //!
    virtual ResultValue &mergeFrom(const QSqlQuery *values);

    //!
    //! \brief mergeFrom
    //! \param resultValue
    //! \return
    //!
    virtual ResultValue &mergeFrom(ResultValue &resultValue);

    //!
    //! \brief setProperty
    //! \param name
    //! \param value
    //! \return
    //!
    virtual bool setProperty(const char *name, const QVariant &value);

    //!
    //! \brief setProperty
    //! \param property
    //! \param value
    //! \return
    //!
    virtual bool setProperty(const QMetaProperty &property, const QVariant &value);

    //!
    //! \brief autoSet
    //! \return
    //!
    virtual ResultValue &autoSet();

    //!
    //! \brief uuidSet
    //! \return
    //!define new uuid in the primarykey property when it is null/invalid
    virtual ResultValue &uuidSet();

    //!
    //! \brief datetimeSet
    //! \return
    //!define new datetime/date/time in the primarykey property when it is null/invalid
    virtual ResultValue &datetimeSet();

    //!
    //! \brief activateSetValues
    //! \return
    //!
    virtual ResultValue &activateSetValues();

    //!
    //! \brief deactivateSetValues
    //! \return
    //!
    virtual ResultValue &deactivateSetValues();

    //!
    //! \brief isValid
    //! \return
    //!
    virtual ResultValue &isValid();

    //!
    //! \brief isActive
    //! \return
    //!
    virtual ResultValue &isActive();

    //!
    //! \brief isEmptyPK
    //! \return
    //!
    virtual ResultValue &isEmptyPK();

    //!
    //! \brief beforePost
    //! \return
    //!
    virtual bool beforePost() const;

    //!
    //! \brief afterPost
    //! \return
    //!
    virtual bool afterPost() const;

    //!
    //! \brief operator =
    //! \param v
    //! \return
    //!
    Model &operator=(const QVariant &v);

private:
    ModelPvt *p=nullptr;
signals:
    void changeProperty();
};

}

//Q_DECLARE_METATYPE(QOrm::Model);
