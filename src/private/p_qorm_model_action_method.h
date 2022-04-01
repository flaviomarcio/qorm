#pragma once

#include "../qorm_model_dao.h"
#include "./p_qorm_model_crud_body.h"

namespace QOrm {

//!
//! \brief The ModelAction class
//!
class Q_ORM_EXPORT ModelAction : public QOrm::ObjectDb
{
public:
    Q_PROPERTY(QVariant Strategy READ strategy WRITE strategy NOTIFY strategyChanged )

    //!
    //! \brief ModelAction
    //! \param parent
    //!
    Q_INVOKABLE explicit ModelAction(QObject *parent = nullptr);

    //!
    //! \brief action
    //! \param vSource
    //! \return
    //!
    virtual ResultValue&action(const QVariant&vSource);

    //!
    //! \brief ~ModelAction
    //!
    ~ModelAction();

    //!
    //! \brief onActionBefore
    //! \param action
    //! \return
    //!
    virtual ModelAction&onActionBefore(ModelActionMethodPointer action);

    //!
    //! \brief onAction
    //! \param action
    //! \return
    //!
    virtual ModelAction&onAction(ModelActionMethodPointer action);

    //!
    //! \brief onActionAfter
    //! \param action
    //! \return
    //!
    virtual ModelAction&onActionAfter(ModelActionMethodPointer action);

private:
    void*p=nullptr;
signals:
    void strategyChanged();
};

}

