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
//!
//! \brief The Model class
//!
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
    QORM_DECLARE_FILTRABLE_FIELD(qvl{})

    //!
    //! \brief The Cursor class
    //!
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
    //!
    //! \brief Model
    //! \param parent
    //!
    Q_INVOKABLE explicit Model(QObject *parent = nullptr);

    //!
    //! \brief ~Model
    //!
    Q_INVOKABLE virtual ~Model();

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
    virtual Model&clear();

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
    Q_INVOKABLE virtual QByteArray storedMd5Make() const;

    //!
    //! \brief isModifier
    //! \param vMap
    //! \return
    //!
    virtual bool isModifier(const QVariantHash&vMap);

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
    //! \brief toMap
    //! \return
    //!
    virtual QVariantMap toMap() const;

    //!
    //! \brief toHash
    //! \return
    //!
    virtual QVariantHash toHash() const;

    //!
    //! \brief toHashModel
    //! \return
    //!
    virtual QVariantHash toHashModel() const;

    //!
    //! \brief toMapPKValues
    //! \return
    //!
    virtual QVariantHash toMapPKValues() const;

    //!
    //! \brief toMapFKValues
    //! \return
    //!
    virtual QVariantHash toMapFKValues() const;

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
    virtual QVariantHash toWrapper(const QString&wrapperName)const;

    //!
    //! \brief toWrapper
    //! \param wrapperName
    //! \param v
    //! \return
    //!
    virtual QVariantList toWrapper(const QString&wrapperName, const QVariantList&v)const;

    //!
    //! \brief toWrapper
    //! \param wrapperName
    //! \param v
    //! \return
    //!
    virtual QVariantList toWrapper(const QString&wrapperName, const QVariantHash&v)const;

    //!
    //! \brief toWrapper
    //! \param wrapperName
    //! \param v
    //! \return
    //!
    virtual QVariantList toWrapper(const QString&wrapperName, ResultValue &v)const;

    //!
    //! \brief toWrapper
    //! \param v
    //! \return
    //!
    virtual QVariantList toWrapper(const QVariantList&v)const;

    //!
    //! \brief toWrapper
    //! \param v
    //! \return
    //!
    virtual QVariantList toWrapper(const QVariantHash&v)const;

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
    virtual bool writeToWrapper(QVariantHash&wrapper) const;

    //!
    //! \brief appendToWrapper
    //! \param wrapper
    //! \return
    //!
    virtual bool appendToWrapper(QVariantHash&wrapper) const;

    //!
    //! \brief appendToWrapper
    //! \param wrapper
    //! \param fromWrapper
    //! \return
    //!
    virtual bool appendToWrapper(QVariantHash&wrapper, const QString &fromWrapper) const;

    //!
    //! \brief appendToWrapper
    //! \param wrapper
    //! \param fromWrapper
    //! \param toWrapper
    //! \return
    //!
    virtual bool appendToWrapper(QVariantHash&wrapper, const QString &fromWrapper, const QString &toWrapper) const;

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
    virtual bool propertyBeforeSet(const QByteArray&property, const QVariant&value) const;

    //!
    //! \brief propertyAfterSet
    //! \param property
    //! \param value
    //! \return
    //!
    virtual bool propertyAfterSet(const QByteArray&property, const QVariant&value) const;

    //!
    //! \brief setCursor
    //! \param cursor
    //!
    virtual void setCursor(QVariantList&cursor);

    //!
    //! \brief setCursor
    //! \param cursor
    //!
    virtual void setCursor(QSqlQuery&cursor);

    //!
    //! \brief setCursor
    //! \param cursor
    //!
    virtual void setCursor(Query&cursor);

    //!
    //! \brief cursor
    //! \return
    //!
    virtual Cursor&cursor();

    //!
    //! \brief readFrom
    //! \param record
    //! \return
    //!
    virtual ResultValue&readFrom(const QVariant &record);

    //!
    //! \brief readFrom
    //! \param record
    //! \return
    //!
    virtual ResultValue&readFrom(QSqlQuery &record);

    //!
    //! \brief readFrom
    //! \param record
    //! \return
    //!
    virtual ResultValue&readFrom(Query &record);

    //!
    //! \brief readFrom
    //! \param resultValue
    //! \return
    //!
    virtual ResultValue&readFrom(const ResultValue &resultValue);

    //!
    //! \brief readFrom
    //! \param model
    //! \return
    //!
    virtual ResultValue&readFrom(const Model &model);

    //!
    //! \brief readWrapper
    //! \param wrapper
    //! \param vMap
    //! \return
    //!
    virtual ResultValue&readWrapper(const QString&wrapper, const QVariant&vMap);

    //!
    //! \brief readWrapper
    //! \param vMap
    //! \return
    //!
    virtual ResultValue&readWrapper(const QVariant&vMap);

    //!
    //! \brief readWrapper
    //! \param wrapper
    //! \param resultValue
    //! \return
    //!
    virtual ResultValue&readWrapper(const QString&wrapper, ResultValue&resultValue);

    //!
    //! \brief readWrapper
    //! \param resultValue
    //! \return
    //!
    virtual ResultValue&readWrapper(ResultValue&resultValue);

    //!
    //! \brief mergeFrom
    //! \param record
    //! \return
    //!
    virtual ResultValue&mergeFrom(const QByteArray&record);

    //!
    //! \brief mergeFrom
    //! \param record
    //! \return
    //!
    virtual ResultValue&mergeFrom(const QVariantHash&record);

    //!
    //! \brief mergeFrom
    //! \param record
    //! \return
    //!
    virtual ResultValue&mergeFrom(const QVariant &record);

    //!
    //! \brief mergeFrom
    //! \param record
    //! \return
    //!
    virtual ResultValue&mergeFrom(QSqlQuery &record);

    //!
    //! \brief mergeFrom
    //! \param resultValue
    //! \return
    //!
    virtual ResultValue&mergeFrom(ResultValue&resultValue);

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
    virtual bool setProperty(const QMetaProperty&property, const QVariant &value);

    //!
    //! \brief uuidSet
    //! \return
    //!define new uuid in the primarykey property when it is null/invalid
    virtual ResultValue&uuidSet();

    //!
    //! \brief deactivateSetValues
    //! \return
    //!
    virtual ResultValue&deactivateSetValues();

    //!
    //! \brief datetimeSet
    //! \return
    //!define new datetime/date/time in the primarykey property when it is null/invalid
    virtual ResultValue&datetimeSet();

    //!
    //! \brief isValid
    //! \return
    //!
    virtual ResultValue&isValid();

    //!
    //! \brief isActive
    //! \return
    //!
    virtual ResultValue&isActive();

    //!
    //! \brief isEmptyPK
    //! \return
    //!
    virtual ResultValue&isEmptyPK();

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
    Model&operator=(const QVariant&v);

private:
    void*p=nullptr;
signals:
    void changeProperty();
};

}

//Q_DECLARE_METATYPE(QOrm::Model);
