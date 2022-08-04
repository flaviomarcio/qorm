#pragma once

#include "../qorm_types.h"
#include "../qorm_object_db.h"
//#include "../qorm_model_dao.h"



namespace PrivateQOrm{
class CRUDBase;
}


namespace QOrm {
class ModelActionPvt;
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
    virtual ResultValue &action(const QVariant &vSource);

    //!
    //! \brief onActionBefore
    //! \param action
    //! \return
    //!
    virtual ModelAction &onActionBefore(ModelActionMethodPointer action);

    //!
    //! \brief onAction
    //! \param action
    //! \return
    //!
    virtual ModelAction &onAction(ModelActionMethodPointer action);

    //!
    //! \brief onActionAfter
    //! \param action
    //! \return
    //!
    virtual ModelAction &onActionAfter(ModelActionMethodPointer action);

    //!
    //! \brief crudBase
    //! \return
    //!
    virtual PrivateQOrm::CRUDBase *crudBase() const;
    virtual void setCrudBase(PrivateQOrm::CRUDBase *newCrudBase);
    virtual void resetCrudBase();

private:
    ModelActionPvt *p=nullptr;

signals:
    void strategyChanged();
};

}

