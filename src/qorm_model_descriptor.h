#pragma once

#include <QVariant>
#include <QVariantHash>
#include <QVariantMap>
#include "./qorm_model_macro.h"
#include "./private/p_qorm_model_dto_endpoints.h"
//#include "../../qstm/src/qstm_types.h"
//#include "./private/p_qorm_model_controls.h"

//#define Q_ORM_MODEL_SET_VALUES(W,H,R) this->setValues(QVariantHash({{vpWidth,W}, {vpHeight,H}, {vpRows,R}}));

#define Q_ORM_MODEL_SET_DESIGN(W,H,C,R,L) this->setDesign(QVariantHash({{vpWidth,W}, {vpHeight,H}, {vpColumns,C}, {vpRows,R}, {vpLayout,L}}));

#define Q_ORM_MODEL_SET_SORT(value) this->setSort(value);

#define Q_ORM_MODEL_SET_DESCRIPTION(value) this->setDescription(value);

#define Q_ORM_MODEL_SET_DESCRIPTOR(propertyName, propertyValue) addDescriptor(QStringLiteral(#propertyName), propertyValue);

#define Q_ORM_MODEL_SET_EDIT(propertyName, propertyValue) \
    this->addDescriptor(QStringLiteral(#propertyName), propertyValue); \
    this->addEdit(QStringLiteral(#propertyName), propertyValue);

#define Q_ORM_MODEL_SET_PERFUMERY(propertyName, propertyValue) \
    this->addDescriptor(QStringLiteral(#propertyName), propertyValue); \
    this->addPerfumery(QStringLiteral(#propertyName), propertyValue);

#define Q_ORM_MODEL_SET_FLAGS(propertyName, propertyValue) \
    this->addDescriptor(QStringLiteral(#propertyName), propertyValue); \
    this->addFlag(QStringLiteral(#propertyName), propertyValue);

#define Q_ORM_MODEL_SET_OPTIONS(propertyName, propertyValue) \
    this->addDescriptor(QStringLiteral(#propertyName), propertyValue); \
    this->addOption(QStringLiteral(#propertyName), propertyValue);

#define Q_ORM_MODEL_DECLARE_HOST(VALUES) this->addHost(VALUES);

#define Q_ORM_MODEL_DECLARE_ENDPOINT(NAME, VALUES)\
const auto NAME##EndPoint=this->addEndPoint(QStringLiteral(#NAME), VALUES).uuid().toString();


#define Q_ORM_DESCRIPTOR_CONSTRUCTOR(ClassName) \
public: \
    Q_INVOKABLE explicit ClassName(QObject *parent = nullptr) : QOrm::ModelDescriptor(parent){};

namespace QOrm {
class ModelDescriptorPvt;
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
    ~ModelDescriptor();

public:
    //!
    //! \brief isValid
    //! \return
    //!
    virtual bool isValid() const;

    //!
    //! \brief descriptorsInit
    //!
    virtual void descriptorsInit();

    //!
    //! \brief descriptors
    //! \return
    //!
    virtual QVariantMap descriptors() const;

    //!
    //! \brief descriptor
    //! \param name
    //! \return
    //!
    virtual QVariant descriptor(const QString &name) const;

    //!
    //! \brief setDescriptor
    //! \param name
    //! \param v
    //!
    virtual void setDescriptor(const QString &name, const QVariantHash &v);

    //!
    //! \brief addDescriptor
    //! \param name
    //! \param v
    //!
    virtual void addDescriptor(const QString &name, const QVariantHash &v);

    //!
    //! \brief description
    //! \return
    //!
    Q_INVOKABLE virtual QString description();

    //!
    //! \brief setDescription
    //! \param v
    //!
    virtual void setDescription(const QString &v);

    //!
    //! \brief edit
    //! \return
    //!
    virtual QVariantHash &edit() const;

    //!
    //! \brief edit
    //! \param name
    //! \return
    //!
    virtual QVariant edit(const QString &name) const;

    //!
    //! \brief setEdit
    //! \param name
    //! \param v
    //!
    virtual void setEdit(const QString &name, const QVariantHash &v);

    //!
    //! \brief addEdit
    //! \param name
    //! \param v
    //!
    virtual void addEdit(const QString &name, const QVariantHash &v);

    //!
    //! \brief perfumerys
    //! \return
    //!
    virtual QVariantHash &perfumerys() const;

    //!
    //! \brief perfumery
    //! \param name
    //! \return
    //!
    virtual QVariant perfumery(const QString &name) const;

    //!
    //! \brief setPerfumery
    //! \param name
    //! \param v
    //!
    virtual void setPerfumery(const QString &name, const QVariantHash &v);

    //!
    //! \brief addPerfumery
    //! \param name
    //! \param v
    //!
    virtual void addPerfumery(const QString &name, const QVariantHash &v);

    //!
    //! \brief flags
    //! \return
    //!
    virtual QVariantHash &flags() const;

    //!
    //! \brief flag
    //! \param name
    //! \return
    //!
    virtual QVariant flag(const QString &name) const;

    //!
    //! \brief setFlag
    //! \param name
    //! \param v
    //!
    virtual void setFlag(const QString &name, const QVariantHash &v);

    //!
    //! \brief addFlag
    //! \param name
    //! \param v
    //!
    virtual void addFlag(const QString &name, const QVariantHash &v);

    //!
    //! \brief options
    //! \return
    //!
    virtual QVariantHash &options() const;

    //!
    //! \brief option
    //! \param name
    //! \return
    //!
    virtual QVariant option(const QString &name) const;

    //!
    //! \brief setOption
    //! \param name
    //! \param v
    //!
    virtual void setOption(const QString &name, const QVariantHash &v);

    //!
    //! \brief addOption
    //! \param name
    //! \param v
    //!
    virtual void addOption(const QString &name, const QVariantHash &v);

    //!
    //! \brief sort
    //! \return
    //!
    virtual QVariantHash &sort() const;

    //!
    //! \brief setSort
    //! \param value
    //!
    virtual void setSort(const QVariantHash &value);

    //!
    //! \brief design
    //! \return
    //!
    virtual QVariantHash &design();
    virtual void setDesign(const QVariantHash &value);

    //!
    //! \brief endPoints
    //! \return
    //!
    virtual EndPoints &endPoints() const;
    virtual void setEndPoints(const EndPoints &newEndPoints);
    virtual void resetEndPoints();

    //!
    //! \brief addEndPoint
    //! \param newEndPoint
    //! \return
    //!
    virtual Host &addHost(Host *newHost);
    virtual Host &addHost(const QVariant &newHost);

    //!
    //! \brief addEndPoint
    //! \param newEndPoint
    //! \return
    //!
    virtual EndPoint &addEndPoint(EndPoint *newEndPoint);
    virtual EndPoint &addEndPoint(const QString &name, const QVariant &values);

signals:
    void cellRowsChanged();
    void endPointsChanged();

private:
    ModelDescriptorPvt *p = nullptr;
};

} // namespace QOrm
