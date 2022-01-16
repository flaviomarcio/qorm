#pragma once

#include <QOrm>

//!
//! \brief The PersonController class
//!
//! inherits of QOrm::Controller
class PersonController : public QOrm::Controller
{
    Q_OBJECT
public:
    //!
    //! \brief personController
    //! \param parent
    //!
    //! constructor controller to pass current connection
    QORM_CONTROLLER_CONSTRUCTOR(PersonController)

    //!
    //! \brief crud
    //! \param vBody
    //! \return
    //! example of CRUD
    ResultValue &crud(const QVariant &vBody);

    //!
    //! \brief upsert
    //! \param v
    //! \return
    //!
    //! example of insert/update object
    ResultValue &upsert(const QVariant &v);

    //!
    //! \brief remove
    //! \param v
    //! \return
    //!
    //! example of remove object
    ResultValue &remove(const QVariant &v);

    //!
    //! \brief get
    //! \param v
    //! \return
    //!
    //! example of get object
    ResultValue &get(const QVariant&v);

    //!
    //! \brief list
    //! \param vFilters
    //! \return
    //!
    //! example of list object
    ResultValue &list(const QVariant &vFilters);


    //!
    //! \brief listPerson
    //! \param v
    //! \return
    //! example of query builder
    ResultValue &listPerson(const QVariant &v);

    //!
    //! \brief listPersonParent
    //! \param v
    //! \return
    //! example of join
    ResultValue &listPersonParent(const QVariant &v);

    //!
    //! \brief listPersonCheck
    //! \param v
    //! \return
    //!
    //!example of manipulation objects
    ResultValue &listPersonCheck(const QVariant &v);

    //!
    //! \brief runMultiTasks
    //! \return
    //!
    //! example of execute multi tasks
    ResultValue &runMultiTasks();

};
