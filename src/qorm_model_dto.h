#pragma once

#include "./private/p_qorm_model_controls.h"

namespace QOrm {
class ModelInfo;
class ModelDtoPvt;
//!
//! \brief The ModelDto class
//!
class Q_ORM_EXPORT ModelDto : public QStm::Object
{
    Q_OBJECT
public:
    QORM_MODEL_DECLARE_FORM_ENUMS
    //!
    //! \brief ModelDto
    //! \param parent
    //!
    Q_INVOKABLE explicit ModelDto(QObject *parent = nullptr);

    //!
    //! \brief ModelDto
    //! \param rows
    //! \param parent
    //!
    explicit ModelDto(const ResultValue &rows, QObject *parent = nullptr);

    //!
    //! \brief ModelDto
    //! \param rows
    //! \param parent
    //!
    explicit ModelDto(const ModelInfo *modelInfo, QObject *parent = nullptr);

    //!
    //! \brief type
    //! \return
    //!
    QVariant &type() const;

    //!
    //! \brief uuid
    //! \return
    //!
    virtual QUuid &uuid() const;
    virtual ModelDto &uuid(const QUuid &uuid);

    //!
    //! \brief name
    //! \return
    //!
    virtual QString &description() const;

    //!
    //! \brief sort
    //! \return
    //!
    virtual QVariantHash &sort() const;

    //!
    //! \brief controls
    //! \return
    //!
    virtual ModelDtoControls &controls();

    //!
    //! \brief headers
    //! \return
    //!
    virtual ModelFieldCollection &headers();

    //!
    //! \brief filters
    //! \return
    //!
    virtual ModelFieldCollection &filters();

    //!
    //! \brief Host
    //! \return
    //!
    virtual const Host &host()const;

    //!
    //! \brief Host
    //! \return
    //!
    virtual ModelDto &host(const Host &host);
    virtual ModelDto &host(const QVariant &host);

    //!
    //! \brief endPoint
    //! \return
    //!
    virtual EndPoint &endPoint()const;

    //!
    //! \brief endPoints
    //! \return
    //!
    virtual EndPoints &endPoints()const;

    //!
    //! \brief items
    //! \return
    //!
    virtual const QVariantList &items() const;

    //!
    //! \brief items
    //! \param v
    //! \return
    //!
    virtual ModelDtoControls &items(const QVariant &v);

    //!
    //! \brief items
    //! \param lr
    //! \return
    //!
    virtual ModelDtoControls &items(const ResultValue &lr);

    //!
    //! \brief resultInfo
    //! \return
    //!
    virtual QStm::ResultInfo &resultInfo();

    //!
    //! \brief resultInfo
    //! \param v
    //! \return
    //!
    virtual ModelDtoControls &resultInfo(const QVariant &v);

    //!
    //! \brief setResultInfo
    //! \param resultInfo
    //! \return
    //!
    virtual ModelDtoControls &setResultInfo(const QStm::ResultInfo &resultInfo);

    //!
    //! \brief setValue
    //! \param v
    //! \return
    //!
    virtual ModelDto &setValue(const QVariant &v);

    //!
    //! \brief setValue
    //! \param lr
    //! \return
    //!
    virtual ModelDto &setValue(const ResultValue &lr);

    //!
    //! \brief clear
    //! \return
    //!
    virtual ModelDto &clear();

    //!
    //! \brief o
    //! \return
    //!
    virtual ResultValue &o();

    //!
    //! \brief toOutput
    //! \return
    //!
    virtual ResultValue &toOutput();

    //!
    //! \brief initDescriptors
    //! \param object
    //! \return
    //!
    virtual ModelDto &initDescriptors(QObject *object);


signals:
    void typeChanged();

private:
    ModelDtoPvt *p = nullptr;
};

} // namespace QOrm
