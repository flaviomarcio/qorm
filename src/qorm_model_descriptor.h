#pragma once

#include <QVariant>
#include <QVariantHash>
#include <QVariantMap>
#include "./qorm_types.h"
#include "./qorm_model_macro.h"
#include "./private/p_qorm_model_dto_endpoints.h"
//#include "../../qstm/src/qstm_types.h"
//#include "./private/p_qorm_model_controls.h"

#define QORM_DESCRIPTOR_CONSTRUCTOR(CLASSNAME) QORM_MODEL_DESCRIPTOR_CONSTRUCTOR(CLASSNAME, QOrm::ModelDescriptor)

//#define QORM_MODEL_SET_VALUES(W,H,R) this->setValues(QVariantHash({{vpWidth,W}, {vpHeight,H}, {vpRows,R}}))

#define QORM_MODEL_SET_FORM_DESIGN(W,H,C,R,L) this->setDesign(QVariantHash({{vpWidth,W}, {vpHeight,H}, {vpColumns,C}, {vpRows,R}, {vpLayout,L}}))

#define QORM_MODEL_SET_SORT(value) this->setSort(value)

#define QORM_MODEL_SET_DESCRIPTION(value) this->setDescription(value)

#define QORM_MODEL_SET_DESCRIPTOR(propertyName, propertyValue) addDescriptor(QStringLiteral(#propertyName), propertyValue)

#define QORM_MODEL_SET_FORM_TYPE(FORMTYPE) this->setType(FORMTYPE)

#define QORM_MODEL_SET_EDIT(propertyName, propertyValue) \
    this->addDescriptor(QStringLiteral(#propertyName), propertyValue); \
    this->addEdit(QStringLiteral(#propertyName), propertyValue)

#define QORM_MODEL_SET_DESIGN(propertyName, propertyValue) \
    this->addDescriptor(QStringLiteral(#propertyName), propertyValue); \
    this->addPerfumery(QStringLiteral(#propertyName), propertyValue)

#define QORM_MODEL_SET_FLAGS(propertyName, propertyValue) \
    this->addDescriptor(QStringLiteral(#propertyName), propertyValue); \
    this->addFlag(QStringLiteral(#propertyName), propertyValue)

#define QORM_MODEL_SET_OPTIONS(propertyName, propertyValue) \
    this->addDescriptor(QStringLiteral(#propertyName), propertyValue); \
    this->addOption(QStringLiteral(#propertyName), propertyValue)

#define QORM_MODEL_DECLARE_HOST(VALUES) this->addHost(VALUES)

#define QORM_MODEL_DECLARE_ENDPOINT(NAME)\
const auto NAME##EndPoint=this->addEndPoint(QStringLiteral(#NAME), QVariantHash{{vpPath, NAME}}).uuid().toString()

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
    virtual bool isValid() const;

    //!
    //! \brief descriptorsInit
    //!
    virtual void descriptorsInit();

    //!
    //! \brief type
    //! \return
    //!
    virtual FormType type() const;
    virtual void type(const QVariant &type) const;
    virtual void setType(const QVariant &type) const;

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
   virtual  void addDescriptor(const QString &name, const QVariantHash &v);

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
    //! \brief addEndPoint
    //! \param newEndPoint
    //! \return
    //!
    virtual Host &addHost(Host *newHost);
    virtual Host &addHost(const QVariant &newHost);

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
    virtual EndPoint &addEndPoint(EndPoint *newEndPoint);
    virtual EndPoint &addEndPoint(const QString &name, const QVariant &values);

    //!
    //! \brief endPoint
    //! \return
    //!
    virtual EndPoint &endPoint() const;

    //!
    //! \brief endPoint
    //! \param name
    //! \return
    //!
    virtual EndPoint *endPoint(const QUuid &uuid) const;

    //!
    //! \brief setEndPoint
    //! \param name
    //! \param v
    //!
    virtual ModelDescriptor &setEndPoint(const EndPoint &v);
    virtual ModelDescriptor &setEndPoint(const QVariant &v);

    //!
    //! \brief obrigatory
    //! \return
    //!
    virtual bool obrigatory() const;
    virtual void setObrigatory(bool newObrigatory);
    virtual void resetObrigatory();

    //!
    //! \brief readonly
    //! \return
    //!
    virtual bool readonly() const;
    virtual void setReadonly(bool newReadonly);
    virtual void resetReadonly();

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
