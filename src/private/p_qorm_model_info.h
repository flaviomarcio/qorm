#pragma once

#include <QMetaType>
#include <QMetaMethod>
#include <QMetaProperty>
#include <QObject>
#include <QVariant>
#include <QVariantHash>
#include "../qorm_global.h"
#include "../../qstm/src/qstm_vvm.h"
#include "../../qstm/src/qstm_object_wrapper.h"

namespace QOrm {
class ModelInfoPvt;
class Model;
class ModelFieldDescriptors;

//#define Q_ORM_MODEL_INFO_OBJECT

class Q_ORM_EXPORT ModelInfo
#ifdef Q_ORM_MODEL_INFO_OBJECT
    :public QStm::ObjectWrapper
    Q_OBJECT
public:
    Q_STM_OBJECT_WRAPPER(ModelInfo)
#else
    :public QVariant
#endif
{
    friend class ModelInfoPvt;
public:
    //!
    //! \brief ModelInfo
    //!
#ifdef Q_ORM_MODEL_INFO_OBJECT
    Q_INVOKABLE explicit ModelInfo(QObject *parent=nullptr);
#else
    explicit ModelInfo(QObject *parent=nullptr);
#endif
    //!
    //! \brief ModelInfo
    //! \param className
    //!
    explicit ModelInfo(const QByteArray &className, QObject *parent=nullptr);

    //!
    //! \brief ModelInfo
    //! \param metaObject
    //!
    explicit ModelInfo(const QMetaObject &metaObject, QObject *parent=nullptr);

#ifndef Q_ORM_MODEL_INFO_OBJECT
    //!
    //! \brief ~ModelInfo
    //!
    virtual ~ModelInfo();

#endif
    //!
    //! \brief uuid
    //! \return
    //!
    QUuid &uuid()const;

    //!
    //! \brief isValid
    //! \return
    //!
    bool isValid()const;

    //!
    //! \brief from
    //! \param v
    //! \return
    //!
    const static ModelInfo &from(const QVariant &v);
    const static ModelInfo &from(const QMetaObject &metaObject);

    //!
    //! \brief info
    //! \param v
    //! \return
    //!
    const static ModelInfo &info(const QByteArray &className);

    //!
    //! \brief Init
    //! \param staticMetaObject
    //! \return
    //!
    static ModelInfo &init(const QMetaObject &staticMetaObject);

    //!
    //! \brief instance
    //! \return
    //!
    //! espefic model converted in Model instance
    template<class T>
    static const T &instance(const QMetaObject &staticMetaObject)
    {
        auto model=QOrm::ModelInfo::internalinstance(staticMetaObject);
        auto __return=dynamic_cast<T*>(model);
        return *__return;
    }

    //!
    //! \brief toAttributes
    //! \param v
    //! \return
    //!
    QVariantHash toAttributes(const QVariant &v)const;

    //!
    //! \brief toAttributesFields
    //! \param v
    //! \return
    //!
    QVariantHash toAttributesFields(const QVariant &v)const;

    //!
    //! \brief modelName
    //! \return
    //!
    QString &name()const;
    ModelInfo &name(const QString &value);
    ModelInfo &setName(const QString &value);

    //!
    //! \brief modelDescription
    //! \return
    //!
    QString &description() const;
    ModelInfo &description(const QString &value);
    ModelInfo &setDescription(const QString &value);

    //!
    //! \brief staticMetaObjectModel
    //! \return
    //!
    QMetaObject &staticMetaObjectModel() const;

    //!
    //! \brief staticMetaObjectDescriptor
    //! \return
    //!
    QMetaObject &staticMetaObjectDescriptor() const;

    //!
    //! \brief propertyIgnoredList
    //! \return
    //!
    static const QStringList &propertyIgnoredList();

    //!
    //! \brief property
    //! \return
    //!
    QList<QMetaProperty> &property() const;

    //!
    //! \brief propertyByName
    //! \return
    //!
    QHash<QString, QMetaProperty> &propertyByName() const;

    //!
    //! \brief propertyByFieldName
    //! \return
    //!
    QHash<QString, QMetaProperty> &propertyByFieldName() const;

    //!
    //! \brief propertyByFieldName
    //! \param fieldName
    //! \return
    //!
    QMetaProperty propertyByFieldName(const QString &fieldName)const;

    //!
    //! \brief propertyFiltrable
    //! \return
    //!
    QHash<QString, QMetaProperty> &propertyFiltrable() const;

    //!
    //! \brief propertyFiltrableByLike
    //! \return
    //!
    QHash<QString, QMetaProperty> &propertyFiltrableByLike() const;

    //!
    //! \brief propertyByPropertyName
    //! \return
    //!
    QHash<QString, QMetaProperty> &propertyByPropertyName() const;

    //!
    //! \brief propertyByPropertyName
    //! \param propertyName
    //! \return
    //!
    QMetaProperty propertyByPropertyName(const QString &propertyName) const;

    //!
    //! \brief propertyList
    //! \return
    //!
    QStringList &propertyList() const;

    //!
    //! \brief propertyTableList
    //! \return
    //!
    QStringList &propertyTableList() const;

    //!
    //! \brief propertyTableVsShort
    //! \return
    //!
    QHash<QString, QString> &propertyTableVsShort() const;

    //!
    //! \brief propertyShortVsTable
    //! \return
    //!
    QHash<QString, QString> &propertyShortVsTable() const;

    //!
    //! \brief propertyPK
    //! \return
    //!
    QHash<QString, QMetaProperty> &propertyPK() const;

    //!
    //! \brief propertyForeignKeys
    //! \return
    //!
    QHash<QString, QMetaProperty> &propertyForeignKeys() const;

    //!
    //! \brief propertyDescriptors
    //! \return
    //!
    QVariantHash &propertyDescriptors() const;

    //!
    //! \brief propertyParserToTable
    //! \param v
    //! \return
    //!
    QVariantHash propertyParserToTable(const QVariant &v) const;

    //!
    //! \brief propertyParserToProperty
    //! \param v
    //! \return
    //!
    QVariantHash propertyParserToProperty(const QVariant &v) const;

    //!
    //! \brief propertyNameConvertMap
    //! \param propertyHash
    //! \return
    //!
    bool propertyNameConvertMap(QVariantHash &propertyHash);

    //!
    //! \brief propertyActivateField
    //! \return
    //!
    const QVariantHash &propertyActivateField() const;

    //!
    //! \brief propertyDeactivateField
    //! \return
    //!
    const QVariantHash &propertyDeactivateField() const;

    //!
    //! \brief tableSchema
    //! \return
    //!
    QString &tableSchema() const;

    //!
    //! \brief tablePrefix
    //! \return
    //!
    QString &tablePrefix() const;

    //!
    //! \brief tablePrefixSeparator
    //! \return
    //!
    QString tablePrefixSeparator() const;

    //!
    //! \brief tableName
    //! \return
    //!
    QString tableName() const;

    //!
    //! \brief tableNameFull
    //! \return
    //!
    QString tableNameFull() const;

    //!
    //! \brief tableSequence
    //! \return
    //!
    QVariantHash tableSequence() const;

    //!
    //! \brief tablePk
    //! \return
    //!
    QStringList tablePk() const;

    //!
    //! \brief tablePkField
    //! \return
    //!
    QVariantList tablePkField() const;

    //!
    //! \brief tableForeignKeys
    //! \return
    //!
    QVariantHash tableForeignKeys() const;

    //!
    //! \brief tableFiltrableField
    //! \return
    //!
    QVariantList tableFiltrableField() const;

    //!
    //! \brief tableOrderBy
    //! \return
    //!
    QStringList tableOrderBy() const;

    //!
    //! \brief tableOrderByField
    //! \return
    //!
    QVariantList tableOrderByField() const;

    //!
    //! \brief tableAutoSetFields
    //! \return
    //!
    QStringList tableAutoSetFields() const;

    //!
    //! \brief tablePkAutoGenerate
    //! \return
    //!
    bool tablePkAutoGenerate() const;

    //!
    //! \brief tablePkSingle
    //! \return
    //!
    QString tablePkSingle() const;

    //!
    //! \brief toHash
    //! \param object
    //! \return
    //!
    QVariantHash toHash(const QObject *object, bool nullValuesAdd=false) const;

    //!
    //! \brief toHashModel
    //! \param object
    //! \return
    //!
    QVariantHash toHashModel(const QObject *object, bool nullValuesAdd=false) const;

    //!
    //! \brief extractProperties
    //! \return
    //!
    QVariantHash extractProperties(const QVariantHash &values) const;

    //!
    //! \brief parserVVM
    //! \param vvm
    //! \return
    //!
    QVVM parserVVM(const QVariant &v)const;

private:
    ModelInfoPvt *p=nullptr;

    //!
    //! \brief internalinstance
    //! \return
    //!
    static Model *internalinstance(const QMetaObject &staticMetaObject);
};

}

Q_DECLARE_METATYPE(QOrm::ModelInfo)
Q_DECLARE_METATYPE(QOrm::ModelInfo*)

