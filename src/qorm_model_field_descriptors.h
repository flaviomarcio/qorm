#pragma once

#include <QVariant>
#include <QVariantHash>
#include <QVariantMap>
#include "./qorm_types.h"
#include "./qorm_model_macro.h"
#include "./qorm_model_field_descriptor.h"
#include "./qorm_model_field_collection.h"
#include "./private/p_qorm_model_dto_endpoints.h"
#include "./private/p_qorm_model_dto_design.h"

#define QORM_MODEL_SET_SORT(value) this->setSort(value)

#define QORM_MODEL_SET_DESCRIPTION(value) this->setDescription(value)

#define QORM_MODEL_FIELD_SET_DESCRIPTOR(NAME, VALUES) addDescriptor(QStringLiteral(#NAME), VALUES)

#define QORM_MODEL_FIELD_SET_EDIT(NAME, VALUES) addDescriptor(QStringLiteral(#NAME), VALUES);

#define QORM_MODEL_FIELD_SET_DESIGN(NAME, VALUES) addDescriptor(QStringLiteral(#NAME), VALUES);

#define QORM_MODEL_FIELD_SET_FLAGS(NAME, VALUES) addDescriptor(QStringLiteral(#NAME), VALUES);

#define QORM_MODEL_FIELD_SET_OPTIONS(NAME, VALUES) addDescriptor(QStringLiteral(#NAME), VALUES);

#define QORM_MODEL_DECLARE_HOST(VALUES) this->addHost(VALUES)

#define QORM_MODEL_DECLARE_ENDPOINT(ENDPOINT) static const auto ENDPOINT##EndPoint=this->addEndPoint(#ENDPOINT, ENDPOINT).uuid();

#define QORM_MODEL_DECLARE_ENDPOINT_PRINCIPAL(ENDPOINT) setEndPoint(ENDPOINT)

#define QORM_MODEL_DESCRIPTOR_CONSTRUCTOR(CLASSNAME, DESCRIPTOR) Q_INVOKABLE explicit CLASSNAME(QObject *parent = nullptr) : DESCRIPTOR{parent}{};

#define QORM_DESCRIPTOR_CONSTRUCTOR(CLASSNAME) public: QORM_MODEL_DESCRIPTOR_CONSTRUCTOR(CLASSNAME, QOrm::ModelFieldDescriptors)

namespace QOrm {
class ModelDescriptorPvt;

//!
//! \brief The ModelFieldDescriptors class
//!
class Q_ORM_EXPORT ModelFieldDescriptors : public QStm::ObjectWrapper
{
    Q_OBJECT
    QORM_DESCRIPTOR_ORDERBY()
    QORM_MODEL_DECLARE_FORM_ENUMS

    Q_PROPERTY(QByteArray className READ className WRITE setClassName  RESET resetClassName NOTIFY classNameChanged)
    Q_PROPERTY(QUuid uuid READ uuid WRITE setUuid  RESET resetUuid NOTIFY uuidChanged)
    Q_PROPERTY(QString description READ description RESET resetDescription WRITE setDescription RESET resetDescription NOTIFY descriptionChanged)
    Q_PROPERTY(ModelFieldCollection *descriptors READ descriptors WRITE setDescriptors RESET resetDescriptors NOTIFY descriptorsChanged)
    Q_PROPERTY(ModelFieldCollection *filters READ filters WRITE setFilters RESET resetFilters NOTIFY filtersChanged)
    Q_PROPERTY(Design *design READ design WRITE setDesign RESET resetDesign NOTIFY designChanged)
    Q_PROPERTY(EndPoint *endPoint READ endPoint WRITE setEndPoint NOTIFY endPointChanged)
    Q_PROPERTY(EndPoints *endPoints READ endPoints WRITE setEndPoints RESET resetEndPoints NOTIFY endPointsChanged)
    Q_PROPERTY(Host *host READ host WRITE setHost RESET resetHost NOTIFY hostChanged)
    Q_PROPERTY(bool obrigatory READ obrigatory WRITE setObrigatory NOTIFY obrigatoryChanged)
    Q_PROPERTY(bool readOnly READ readOnly WRITE setReadOnly NOTIFY readOnlyChanged)
    Q_PROPERTY(QVariantHash sort READ sort WRITE setSort RESET resetSort NOTIFY sortChanged)
public:

    //!
    //! \brief ModelFieldDescriptors
    //! \param parent
    //!
    Q_INVOKABLE explicit ModelFieldDescriptors(QObject *parent = nullptr);

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
    //! \brief makeDescriptorToFilters
    //!
    virtual ModelFieldDescriptors &makeDescriptorToFilters();

    //!
    //! \brief className
    //! \return
    //!
    QByteArray className()const;
    ModelFieldDescriptors &setClassName(const QByteArray &value);
    ModelFieldDescriptors &resetClassName();

    //!
    //! \brief uuid
    //! \return
    //!
    QUuid &uuid()const;
    ModelFieldDescriptors &setUuid(const QUuid &value);
    ModelFieldDescriptors &resetUuid();

    //!
    //! \brief descriptors
    //! \return
    //!
    ModelFieldCollection *descriptors();
    ModelFieldCollection &descriptorsCollection() const;
    ModelFieldDescriptors &setDescriptors(const ModelFieldCollection *newDescriptors);
    ModelFieldDescriptors &resetDescriptors();

    //!
    //! \brief filters
    //! \return
    //!
    ModelFieldCollection *filters();
    ModelFieldCollection &filtersCollection() const;
    ModelFieldDescriptors &setFilters(const ModelFieldCollection *newFilters);
    ModelFieldDescriptors &resetFilters();

    //!
    //! \brief addDescriptor
    //! \param fieldName
    //! \return
    //!
    ModelFieldDescriptor &addDescriptor(const QString &fieldName);
    ModelFieldDescriptors &addDescriptor(const QString &fieldName, const QVariant &values);

    //!
    //! \brief addFilter
    //! \param fieldName
    //! \return
    //!
    ModelFieldDescriptor &addFilter(const QString &fieldName);
    ModelFieldDescriptors &addFilter(const QString &fieldName, const QVariant &values);

    //!
    //! \brief description
    //! \return
    //!
    Q_INVOKABLE QString &description();
    ModelFieldDescriptors &setDescription(const QString &v);
    ModelFieldDescriptors &resetDescription();

    //!
    //! \brief sort
    //! \return
    //!
    QVariantHash &sort() const;
    ModelFieldDescriptors &setSort(const QVariantHash &value);
    ModelFieldDescriptors &resetSort();

    //!
    //! \brief design
    //! \return
    //!
    Design *design();
    ModelFieldDescriptors &setDesign(const Design *value);
    ModelFieldDescriptors &setDesign(const QVariantHash &value);
    ModelFieldDescriptors &resetDesign();

    //!
    //! \brief host
    //! \return
    //!
    Host *host();
    Host &setHost(const Host *newHost);
    Host &setHost(const QVariant &newHost);
    ModelFieldDescriptors &resetHost();

    //!
    //! \brief endPoints
    //! \return
    //!
    EndPoints *endPoints() const;
    ModelFieldDescriptors &setEndPoints(const EndPoints *newEndPoints);
    ModelFieldDescriptors &resetEndPoints();
    ModelFieldDescriptors &addEndPoint(EndPoint *newEndPoint);
    EndPoint &addEndPoint(const QString &name, const QVariant &values);

    //!
    //! \brief endPoint
    //! \return
    //!
    EndPoint *endPoint() const;

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
    ModelFieldDescriptors &setEndPoint(const EndPoint *v);
    ModelFieldDescriptors &setEndPoint(const QVariant &v);

    //!
    //! \brief obrigatory
    //! \return
    //!
    bool obrigatory() const;
    ModelFieldDescriptors &setObrigatory(bool newObrigatory);
    ModelFieldDescriptors &resetObrigatory();

    //!
    //! \brief readOnly
    //! \return
    //!
    bool readOnly() const;
    ModelFieldDescriptors &setReadOnly(bool newReadonly);
    ModelFieldDescriptors &resetReadOnly();

signals:
    void classNameChanged();
    void uuidChanged();
    void descriptionChanged();
    void descriptorsChanged();
    void filtersChanged();
    void designChanged();
    void endPointChanged();
    void endPointsChanged();
    void hostChanged();
    void obrigatoryChanged();
    void readOnlyChanged();
    void sortChanged();
    void typeChanged();
private:
    ModelDescriptorPvt *p = nullptr;
};

} // namespace QOrm
