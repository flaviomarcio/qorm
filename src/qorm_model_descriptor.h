#pragma once

#include <QVariant>
#include <QVariantHash>
#include <QVariantMap>
#include "./qorm_types.h"
#include "./qorm_model_macro.h"
#include "./qorm_model_descriptor_field.h"
#include "./private/p_qorm_model_dto_endpoints.h"
//#include "../../qstm/src/qstm_types.h"
//#include "./private/p_qorm_model_controls.h"

#define QORM_DESCRIPTOR_CONSTRUCTOR(CLASSNAME) QORM_MODEL_DESCRIPTOR_CONSTRUCTOR(CLASSNAME, QOrm::ModelDescriptor)

//#define QORM_MODEL_SET_VALUES(W,H,R) this->setValues(QVariantHash({{vpWidth,W}, {vpHeight,H}, {vpRows,R}}))

#define QORM_MODEL_SET_FORM_DESIGN(W,H,C,R,L) this->setDesign(QVariantHash({{vpWidth,W}, {vpHeight,H}, {vpColumns,C}, {vpRows,R}, {vpLayout,L}}))

#define QORM_MODEL_SET_SORT(value) this->setSort(value)

#define QORM_MODEL_SET_DESCRIPTION(value) this->setDescription(value)

#define QORM_MODEL_FIELD_SET_DESCRIPTOR(propertyName, propertyValue) addFieldDescriptor(QStringLiteral(#propertyName), propertyValue)

#define QORM_MODEL_SET_FORM_TYPE(FORMTYPE) this->setType(FORMTYPE)

#define QORM_MODEL_FIELD_SET_EDIT(propertyName, propertyValue) \
    this->addFieldDescriptor(QStringLiteral(#propertyName), propertyValue); \
    this->addFieldEdit(QStringLiteral(#propertyName), propertyValue)

#define QORM_MODEL_FIELD_SET_DESIGN(propertyName, propertyValue) \
    this->addFieldDescriptor(QStringLiteral(#propertyName), propertyValue); \
    this->addFieldPerfumery(QStringLiteral(#propertyName), propertyValue)

#define QORM_MODEL_FIELD_SET_FLAGS(propertyName, propertyValue) \
    this->addFieldDescriptor(QStringLiteral(#propertyName), propertyValue); \
    this->addFieldFlag(QStringLiteral(#propertyName), propertyValue)

#define QORM_MODEL_FIELD_SET_OPTIONS(propertyName, propertyValue) \
    this->addFieldDescriptor(QStringLiteral(#propertyName), propertyValue); \
    this->addFieldOption(QStringLiteral(#propertyName), propertyValue)

#define QORM_MODEL_DECLARE_HOST(VALUES) this->addHost(VALUES)

#define QORM_MODEL_DECLARE_ENDPOINT(ENDPOINT)\
static const auto ENDPOINT##EndPoint=this->addEndPoint(#ENDPOINT, ENDPOINT).uuid();

#define QORM_MODEL_DECLARE_ENDPOINT_PRINCIPAL(ENDPOINT) this->setEndPoint(ENDPOINT)

#define QORM_MODEL_DESCRIPTOR_CONSTRUCTOR(CLASSNAME, DESCRIPTOR) \
public: \
    Q_INVOKABLE explicit CLASSNAME(QObject *parent = nullptr) : DESCRIPTOR(parent){};

namespace QOrm {
class ModelDescriptorPvt;
//!
//! \brief The ModelDescriptor class
//!
class Q_ORM_EXPORT ModelDescriptor : public QObject
{
    Q_OBJECT
    QORM_DESCRIPTOR_ORDERBY()
    QORM_MODEL_DECLARE_FORM_ENUMS
public:

    //!
    //! \brief ModelDescriptor
    //! \param parent
    //!
    Q_INVOKABLE explicit ModelDescriptor(QObject *parent = nullptr);

public:
    //!
    //! \brief isValid
    //! \return
    //!
    bool isValid() const;

    //!
    //! \brief descriptorsInit
    //!
    void descriptorsInit();

    //!
    //! \brief type
    //! \return
    //!
    FormType type() const;
    void type(const QVariant &type) const;
    void setType(const QVariant &type) const;

    //!
    //! \brief descriptors
    //! \return
    //!
    QVariantMap descriptors() const;

    //!
    //! \brief descriptor
    //! \param name
    //! \return
    //!
    QVariant descriptor(const QString &name) const;

    //!
    //! \brief addDescriptor
    //! \param fieldName
    //! \return
    //!
    ModelDescriptorField &addDescriptor(const QVariant &fieldName);

    //!
    //! \brief addFilter
    //! \param fieldName
    //! \return
    //!
    ModelDescriptorField &addFilter(const QVariant &fieldName);

    //!
    //! \brief setFieldDescriptor
    //! \param name
    //! \param v
    //!
    void setFieldDescriptor(const QString &name, const QVariantHash &v);

    //!
    //! \brief addDescriptor
    //! \param name
    //! \param v
    //!
    void addFieldDescriptor(const QString &name, const QVariantHash &v);

    //!
    //! \brief description
    //! \return
    //!
    Q_INVOKABLE QString description();

    //!
    //! \brief setDescription
    //! \param v
    //!
    void setDescription(const QString &v);

    //!
    //! \brief fieldEdit
    //! \return
    //!
    QVariantHash &fieldEdit() const;

    //!
    //! \brief fieldEdit
    //! \param name
    //! \return
    //!
    QVariant fieldEdit(const QString &name) const;

    //!
    //! \brief setFieldEdit
    //! \param name
    //! \param v
    //!
    void setFieldEdit(const QString &name, const QVariantHash &v);

    //!
    //! \brief addFieldEdit
    //! \param name
    //! \param v
    //!
    void addFieldEdit(const QString &name, const QVariantHash &v);

    //!
    //! \brief fieldPerfumerys
    //! \return
    //!
    QVariantHash &fieldPerfumerys() const;

    //!
    //! \brief fieldPerfumery
    //! \param name
    //! \return
    //!
    QVariant fieldPerfumery(const QString &name) const;

    //!
    //! \brief setFieldPerfumery
    //! \param name
    //! \param v
    //!
    void setFieldPerfumery(const QString &name, const QVariantHash &v);

    //!
    //! \brief addPerfumery
    //! \param name
    //! \param v
    //!
    void addFieldPerfumery(const QString &name, const QVariantHash &v);

    //!
    //! \brief fieldFlags
    //! \return
    //!
    QVariantHash &fieldFlags() const;

    //!
    //! \brief fieldFlag
    //! \param name
    //! \return
    //!
    QVariant fieldFlag(const QString &name) const;

    //!
    //! \brief setFieldFlag
    //! \param name
    //! \param v
    //!
    void setFieldFlag(const QString &name, const QVariantHash &v);

    //!
    //! \brief addFieldFlag
    //! \param name
    //! \param v
    //!
    void addFieldFlag(const QString &name, const QVariantHash &v);

    //!
    //! \brief options
    //! \return
    //!
    QVariantHash &fieldOptions() const;

    //!
    //! \brief option
    //! \param name
    //! \return
    //!
    QVariant fieldOption(const QString &name) const;

    //!
    //! \brief setFieldOption
    //! \param name
    //! \param v
    //!
    void setFieldOption(const QString &name, const QVariantHash &v);

    //!
    //! \brief addFieldOption
    //! \param name
    //! \param v
    //!
    void addFieldOption(const QString &name, const QVariantHash &v);

    //!
    //! \brief sort
    //! \return
    //!
    QVariantHash &sort() const;

    //!
    //! \brief setSort
    //! \param value
    //!
    void setSort(const QVariantHash &value);

    //!
    //! \brief design
    //! \return
    //!
    QVariantHash &design();
    void setDesign(const QVariantHash &value);

    //!
    //! \brief addEndPoint
    //! \param newEndPoint
    //! \return
    //!
    Host &addHost(Host *newHost);
    Host &addHost(const QVariant &newHost);

    //!
    //! \brief endPoints
    //! \return
    //!
    EndPoints &endPoints() const;
    void setEndPoints(const EndPoints &newEndPoints);
    void resetEndPoints();

    //!
    //! \brief addEndPoint
    //! \param newEndPoint
    //! \return
    //!
    EndPoint &addEndPoint(EndPoint *newEndPoint);
    EndPoint &addEndPoint(const QString &name, const QVariant &values);

    //!
    //! \brief endPoint
    //! \return
    //!
    EndPoint &endPoint() const;

    //!
    //! \brief endPoint
    //! \param name
    //! \return
    //!
    EndPoint *endPoint(const QUuid &uuid) const;

    //!
    //! \brief setEndPoint
    //! \param name
    //! \param v
    //!
    ModelDescriptor &setEndPoint(const EndPoint &v);
    ModelDescriptor &setEndPoint(const QVariant &v);

    //!
    //! \brief obrigatory
    //! \return
    //!
    bool obrigatory() const;
    void setObrigatory(bool newObrigatory);
    void resetObrigatory();

    //!
    //! \brief readonly
    //! \return
    //!
    bool readonly() const;
    void setReadonly(bool newReadonly);
    void resetReadonly();

signals:
    void cellRowsChanged();
    void endPointsChanged();
    void endPointChanged();
    void obrigatoryChanged();
    void readonlyChanged();
private:
    ModelDescriptorPvt *p = nullptr;
};

} // namespace QOrm
