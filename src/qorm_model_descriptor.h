#pragma once


#include "./qstm_types.h"
#include "./private/p_qorm_model_controls.h"
#include <QVariant>
#include <QVariantHash>
#include <QVariantMap>

#define Q_ORM_MODEL_SET_SORT(value)\
    this->setSort(value);

#define Q_ORM_MODEL_SET_DESCRIPTION(value)\
    this->setDescription(value);

#define Q_ORM_MODEL_SET_DESCRIPTOR(propertyName, propertyValue)\
    this->addDescriptor(QStringLiteral(#propertyName), propertyValue);

#define Q_ORM_MODEL_SET_EDIT(propertyName, propertyValue)\
    this->addDescriptor(QStringLiteral(#propertyName), propertyValue);\
    this->addEdit(QStringLiteral(#propertyName), propertyValue);

#define Q_ORM_MODEL_SET_PERFUMERY(propertyName, propertyValue)\
    this->addDescriptor(QStringLiteral(#propertyName), propertyValue);\
    this->addPerfumery(QStringLiteral(#propertyName), propertyValue);

#define Q_ORM_MODEL_SET_FLAGS(propertyName, propertyValue)\
    this->addDescriptor(QStringLiteral(#propertyName), propertyValue);\
    this->addFlag(QStringLiteral(#propertyName), propertyValue);

#define Q_ORM_MODEL_SET_OPTIONS(propertyName, propertyValue)\
    this->addDescriptor(QStringLiteral(#propertyName), propertyValue);\
    this->addOption(QStringLiteral(#propertyName), propertyValue);

#define Q_ORM_DESCRIPTOR_CONSTRUCTOR(ClassName)\
public:\
    Q_INVOKABLE explicit ClassName(QObject *parent = nullptr):QOrm::ModelDescriptor(parent){};

namespace QOrm {

//!
//! \brief The ModelDescriptor class
//!
class Q_ORM_EXPORT ModelDescriptor : public QObject
{
    Q_OBJECT
    QORM_DESCRIPTOR_ORDERBY()
public:

    //!
    //! \brief ModelDescriptor
    //! \param parent
    //!
    Q_INVOKABLE explicit ModelDescriptor(QObject *parent = nullptr);

    //!
    //! \brief ~ModelDescriptor
    //!
    Q_INVOKABLE ~ModelDescriptor();
public:

    //!
    //! \brief isValid
    //! \return
    //!
    Q_INVOKABLE virtual bool isValid()const;

    //!
    //! \brief descriptorsInit
    //!
    Q_INVOKABLE virtual void descriptorsInit();

    //!
    //! \brief descriptors
    //! \return
    //!
    Q_INVOKABLE virtual QVariantList descriptors()const;

    //!
    //! \brief descriptor
    //! \param name
    //! \return
    //!
    Q_INVOKABLE virtual QVariant descriptor(const QString&name)const;

    //!
    //! \brief setDescriptor
    //! \param name
    //! \param v
    //!
    Q_INVOKABLE virtual void setDescriptor(const QString&name, const QVariantHash &v);

    //!
    //! \brief addDescriptor
    //! \param name
    //! \param v
    //!
    Q_INVOKABLE virtual void addDescriptor(const QString&name, const QVariantHash &v);

    //!
    //! \brief description
    //! \return
    //!
    Q_INVOKABLE virtual QString description();

    //!
    //! \brief setDescription
    //! \param v
    //!
    Q_INVOKABLE virtual void setDescription(const QString&v);

    //!
    //! \brief edit
    //! \return
    //!
    Q_INVOKABLE virtual QVariantHash&edit() const;

    //!
    //! \brief edit
    //! \param name
    //! \return
    //!
    Q_INVOKABLE virtual QVariant edit(const QString&name)const;

    //!
    //! \brief setEdit
    //! \param name
    //! \param v
    //!
    Q_INVOKABLE virtual void setEdit(const QString&name, const QVariantHash &v);

    //!
    //! \brief addEdit
    //! \param name
    //! \param v
    //!
    Q_INVOKABLE virtual void addEdit(const QString&name, const QVariantHash &v);

    //!
    //! \brief perfumerys
    //! \return
    //!
    Q_INVOKABLE virtual QVariantHash&perfumerys() const;

    //!
    //! \brief perfumery
    //! \param name
    //! \return
    //!
    Q_INVOKABLE virtual QVariant perfumery(const QString&name)const;

    //!
    //! \brief setPerfumery
    //! \param name
    //! \param v
    //!
    Q_INVOKABLE virtual void setPerfumery(const QString&name, const QVariantHash &v);

    //!
    //! \brief addPerfumery
    //! \param name
    //! \param v
    //!
    Q_INVOKABLE virtual void addPerfumery(const QString&name, const QVariantHash &v);

    //!
    //! \brief flags
    //! \return
    //!
    Q_INVOKABLE virtual QVariantHash&flags() const;

    //!
    //! \brief flag
    //! \param name
    //! \return
    //!
    Q_INVOKABLE virtual QVariant flag(const QString&name)const;

    //!
    //! \brief setFlag
    //! \param name
    //! \param v
    //!
    Q_INVOKABLE virtual void setFlag(const QString&name, const QVariantHash &v);

    //!
    //! \brief addFlag
    //! \param name
    //! \param v
    //!
    Q_INVOKABLE virtual void addFlag(const QString&name, const QVariantHash &v);

    //!
    //! \brief options
    //! \return
    //!
    Q_INVOKABLE virtual QVariantHash&options() const;

    //!
    //! \brief option
    //! \param name
    //! \return
    //!
    Q_INVOKABLE virtual QVariant option(const QString&name)const;

    //!
    //! \brief setOption
    //! \param name
    //! \param v
    //!
    Q_INVOKABLE virtual void setOption(const QString&name, const QVariantHash &v);

    //!
    //! \brief addOption
    //! \param name
    //! \param v
    //!
    Q_INVOKABLE virtual void addOption(const QString&name, const QVariantHash &v);

    //!
    //! \brief sort
    //! \return
    //!
    Q_INVOKABLE virtual QVariantHash &sort() const;

    //!
    //! \brief setSort
    //! \param value
    //!
    Q_INVOKABLE virtual void setSort(const QVariantHash &value);

private:
    void*p=nullptr;
};

}
