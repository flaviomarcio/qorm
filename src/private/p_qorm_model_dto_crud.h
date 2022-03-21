#pragma once

#include "./p_qorm_model_dto_link_base.h"

namespace QOrm {


//!
//! \brief The ModelDtoCrudItem class
//!
template <class T>
class ModelDtoCrudItem : public QStm::Object{
public:

    //!
    //! \brief ModelDtoCrudItem
    //! \param dtoParent
    //! \param parent
    //!
    Q_INVOKABLE explicit ModelDtoCrudItem(QObject*dtoParent, QObject*parent=nullptr):QStm::Object{parent}, item(dtoParent, parent)
    {
    }

    //!
    //! \brief d
    //! \return
    //!
    virtual T&d()
    {
        auto object=&this->item.d();
        return*dynamic_cast<T*>(object);
    }
    PrivateQOrm::ModelDtoLinkItem item;
};

//!
//! \brief The ModelDtoCrud class
//!
template <class T>
class ModelDtoCrud:public PrivateQOrm::ModelDtoLinkCollection
{
public:
    //!
    //! \brief ModelDtoCrud
    //! \param dtoParent
    //! \param parent
    //!
    explicit ModelDtoCrud(QObject*dtoParent, QObject*parent=nullptr):PrivateQOrm::ModelDtoLinkCollection(dtoParent, parent)
    {
    }

    //!
    //! \brief d
    //! \return
    //!
    virtual T&d()
    {
        auto object=&PrivateQOrm::ModelDtoLinkCollection::d();
        return*dynamic_cast<T*>(object);
    }
};


}
