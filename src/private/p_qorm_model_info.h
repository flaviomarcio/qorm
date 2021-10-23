#pragma once

#include <QMetaType>
#include <QMetaMethod>
#include <QMetaProperty>
#include <QObject>
#include <QVariant>
#include <QVariantHash>
#include "../qorm_global.h"

namespace QOrm {

class Model;
class Q_ORM_EXPORT ModelInfo:public QVariant{
    friend class ModelInfoPvt;
public:
    /**
     * @brief ModelInfo
     */
    explicit ModelInfo();

    /**
     * @brief ~ModelInfo
     */
    virtual ~ModelInfo();

    /**
     * @brief modelInfo
     * @param metaObject
     * @return
     */
    const static ModelInfo &modelInfo(const QVariant&v);
    const static ModelInfo &modelInfo(const QMetaObject&metaObject);
    static ModelInfo&modelInfoInit(const QMetaObject&staticMetaObject);

    /**
     * @brief modelName
     * @return
     */
    virtual QString &modelName() const;
    virtual void setModelName(const QString &value);

    /**
     * @brief modelDescription
     * @return
     */
    virtual QString &modelDescription() const;
    virtual void setModelDescription(const QString &value);

    /**
     * @brief className
     * @return
     */
    QByteArray className() const;

    /**
     * @brief staticMetaObject
     * @return
     */
    QMetaObject &staticMetaObject() const;

    /**
     * @brief propertyIgnoredList
     * @return
     */
    static const QStringList &propertyIgnoredList();

    /**
     * @brief property
     * @return
     */
    virtual QList<QMetaProperty>&property() const;

    /**
     * @brief propertyByFieldName
     * @return
     */
    virtual QHash<QString, QMetaProperty>&propertyByFieldName() const;
    virtual QMetaProperty propertyByFieldName(const QString &fieldName)const;

    /**
     * @brief propertyByPropertyName
     * @return
     */
    virtual QHash<QString, QMetaProperty>&propertyByPropertyName() const;
    virtual QMetaProperty propertyByPropertyName(const QString &propertyName) const;

    /**
     * @brief propertyList
     * @return
     */
    virtual QStringList &propertyList() const;

    /**
     * @brief propertyTableList
     * @return
     */
    virtual QStringList &propertyTableList() const;

    /**
     * @brief propertyTableVsShort
     * @return
     */
    virtual QHash<QString, QString> &propertyTableVsShort() const;

    /**
     * @brief propertyShotVsTable
     * @return
     */
    virtual QHash<QString, QString> &propertyShortVsTable() const;

    /**
     * @brief propertyInfo
     * @return
     */
    virtual QHash<QString, QMetaProperty> &propertyInfo() const;

    /**
     * @brief propertyPK
     * @return
     */
    virtual QHash<QString, QMetaProperty> &propertyPK() const;

    /**
     * @brief propertyFK
     * @return
     */
    virtual QHash<QString, QMetaProperty> &propertyFK() const;

    /**
     * @brief propertyDescriptors
     * @return
     */
    virtual QVariantList propertyDescriptors() const;

    /**
     * @brief propertySort
     * @return
     */
    virtual QVariantHash propertySort() const;

    /**
     * @brief propertyParserToTable
     * @return
     */
    virtual QVariantHash propertyParserToTable(const QVariant &v) const;

    /**
     * @brief propertyParserToProperty
     * @return
     */
    virtual QVariantHash propertyParserToProperty(const QVariant &v) const;

    /**
     * @brief propertyNameConvertMap
     * @param propertyMap
     * @return
     */
    virtual bool propertyNameConvertMap(QVariantHash &propertyMap);

    /**
     * @brief propertyDeactivateField
     * @return
     */
    virtual QVariantHash propertyDeactivateField() const;


    /**
     * @brief tableSchema
     * @return
     */
    virtual QString &tableSchema() const;

    /**
     * @brief tablePrefix
     * @return
     */
    virtual QString &tablePrefix() const;

    /**
     * @brief tablePrefixSeparator
     * @return
     */
    virtual QString tablePrefixSeparator() const;

    /**
     * @brief tableName
     * @return
     */
    virtual QString tableName() const;

    /**
     * @brief tableName
     * @return
     */
    virtual QString tableNameFull() const;

    /**
     * @brief tableSequence
     * @return
     */
    virtual QVariantHash tableSequence() const;

    /**
     * @brief tablePk
     * @return
     */
    virtual QStringList tablePk() const;

    /**
     * @brief tablePkField
     * @return
     */
    virtual QVariantList tablePkField() const;

    /**
     * @brief tableForeignKey
     * @return
     */
    virtual QVariantHash tableForeignKey() const;

    /**
     * @brief tableOrderBy
     * @return
     */
    virtual QStringList tableOrderBy() const;

    /**
     * @brief orderByField
     * @param v
     * @return
     */
    virtual QVariantList tableOrderByField() const;

    /**
     * @brief tableDeactivateField
     * @param v
     * @return
     */
    virtual QVariantHash tableDeactivateField() const;

    /**
     * @brief tablePkAutoGenerate
     * @return
     */
    virtual bool tablePkAutoGenerate() const;

    /**
     * @brief tablePkSingle
     * @return
     */
    virtual QString tablePkSingle() const;

    /**
     * @brief toMap
     * @return
     */
    virtual QVariantMap toMap(const QObject *object) const;

    /**
     * @brief toMap
     * @param object
     * @return
     */
    virtual QVariantHash toHash(const QObject *object) const;

    /**
     * @brief toHashModel
     * @return
     */
    virtual QVariantMap toMapModel(const QObject *object) const;

    /**
     * @brief toHashModel
     * @return
     */
    virtual QVariantHash toHashModel(const QObject *object) const;

    /**
     * @brief toForeign
     * @param object
     * @return
     */
    virtual QVariantHash toForeign(const QVariant&vModel, const QVariant &v) const;


private:
    void*p=nullptr;
};

}

Q_DECLARE_METATYPE(QOrm::ModelInfo);
Q_DECLARE_METATYPE(QOrm::ModelInfo*);

