#pragma once

#include "./p_qorm_model_crud_body.h"
#include "../qorm_model_dao.h"

namespace QOrm {
//!
//! \brief ModelActionMethod
//!
typedef std::function<ResultValue*(QOrm::ObjectDb*controller, const QVariant&vBody)> ModelActionMethod;

//!
//! \brief The ModelAction class
//!
class Q_ORM_EXPORT ModelAction : public QOrm::ObjectDb
{
public:
    Q_PROPERTY(QVariant Strategy READ strategy WRITE strategy )

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
    Q_INVOKABLE ~ModelAction();

    //!
    //! \brief onActionBefore
    //! \param action
    //! \return
    //!
    virtual ModelAction&onActionBefore(ModelActionMethod action);

    //!
    //! \brief onAction
    //! \param action
    //! \return
    //!
    virtual ModelAction&onAction(ModelActionMethod action);

    //!
    //! \brief onActionAfter
    //! \param action
    //! \return
    //!
    virtual ModelAction&onActionAfter(ModelActionMethod action);

private:
    void*p=nullptr;
};

}

