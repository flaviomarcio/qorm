#pragma once

#include <QMetaType>
#include <QMetaMethod>
#include <QMetaProperty>
#include <QObject>
#include <QVariant>
#include <QVariantHash>
#include <QVariantMap>
#include "../qorm_global.h"

namespace QOrm {
class ModelInfoPvt;
class Model;
class ModelDescriptor;
class Q_ORM_EXPORT ModelInfo:public QVariant
{
    friend class ModelInfoPvt;
public:
    //!
    //! \brief ModelInfo
    //!
    explicit ModelInfo();

    //!
    //! \brief ModelInfo
    //! \param className
    //!
    explicit ModelInfo(const QByteArray &className);

    //!
    //! \brief ModelInfo
    //! \param metaObject
    //!
    explicit ModelInfo(const QMetaObject &metaObject);

    //!
    //! \brief ~ModelInfo
    //!
    virtual ~ModelInfo();

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
    //! \brief modelInfoInit
    //! \param staticMetaObject
    //! \return
    //!
    static ModelInfo &modelInfoInit(const QMetaObject &staticMetaObject);

    //!
    //! \brief descritpr
    //! \return
    //!
    const ModelDescriptor *descritor()const;

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
    //! \brief staticMetaObject
    //! \return
    //!
    QMetaObject &staticMetaObject() const;

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
    //! \brief propertyInfo
    //! \return
    //!
    QHash<QString, QMetaProperty> &propertyInfo() const;

    //!
    //! \brief propertyPK
    //! \return
    //!
    QHash<QString, QMetaProperty> &propertyPK() const;

    //!
    //! \brief propertyFK
    //! \return
    //!
    QHash<QString, QMetaProperty> &propertyFK() const;

    //!
    //! \brief propertyDescriptors
    //! \return
    //!
    QVariantMap propertyDescriptors() const;

    //!
    //! \brief propertyEndPoints
    //! \return
    //!
    QVariantList propertyEndPoints() const;

    //!
    //! \brief propertySort
    //! \return
    //!
    QVariantHash propertySort() const;

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
    //! \brief propertyDeactivateField
    //! \return
    //!
    QVariantHash propertyDeactivateField() const;


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

    /**
     * @brief tableName
     * @return
     */
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
    //! \brief tableForeignKey
    //! \return
    //!
    QVariantHash tableForeignKey() const;

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
    //! \brief tableDeactivateField
    //! \return
    //!
    QVariantHash tableDeactivateField() const;

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
    //! \brief toMap
    //! \param object
    //! \return
    //!
    QVariantMap toMap(const QObject *object) const;

    //!
    //! \brief toHash
    //! \param object
    //! \return
    //!
    QVariantHash toHash(const QObject *object) const;

    //!
    //! \brief toMapModel
    //! \param object
    //! \return
    //!
    QVariantMap toMapModel(const QObject *object) const;

    //!
    //! \brief toHashModel
    //! \param object
    //! \return
    //!
    QVariantHash toHashModel(const QObject *object) const;

private:
    ModelInfoPvt *p=nullptr;
};

}

Q_DECLARE_METATYPE(QOrm::ModelInfo)
Q_DECLARE_METATYPE(QOrm::ModelInfo*)

