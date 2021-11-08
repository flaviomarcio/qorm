#pragma once

#include "./qorm_object.h"

namespace QOrm {

//!
//! \brief The ModelDtoResultInfo class
//!
//! class page navigator
class Q_ORM_EXPORT ModelDtoResultInfo:public QOrm::Object{
    Q_OBJECT

    Q_PROPERTY(QVariantList errors      READ errors      WRITE setErrors      )
    Q_PROPERTY(QVariantList messages    READ messages    WRITE setMessages    )
    Q_PROPERTY(bool         success     READ success     WRITE setSuccess     )
    Q_PROPERTY(int          page        READ page        WRITE setPage        )
    Q_PROPERTY(int          per_page    READ per_page    WRITE setPer_page    )
    Q_PROPERTY(int          count       READ count       WRITE setCount       )
    Q_PROPERTY(int          total_count READ total_count WRITE setTotal_count )
    Q_PROPERTY(int          total_pages READ total_pages WRITE setTotal_pages )

public:
    //!
    //! \brief ModelDtoResultInfo constructor
    //! \param parent
    //!
    Q_INVOKABLE ModelDtoResultInfo(QObject*parent=nullptr);

    //! \brief ModelDtoResultInfo destructor
    //!
    ~ModelDtoResultInfo();

    //!
    //! \brief clear
    //!
    //!
    //! clear all values
    void clear();

    //!
    //! \brief errors
    //! \return
    //! activity resource
    virtual bool enabled();
    virtual void setEnabled(bool value);

    //!
    //! \brief errors
    //! \return
    //! return errors to server
    virtual QVariantList&errors();
    virtual void setErrors(const QVariantList&value);

    //!
    //! \brief messages
    //! \return
    //!
    //!
    //! return message to server
    virtual QVariantList&messages();
    void setMessages(const QVariantList&value);

    //!
    //! \brief success
    //! \return
    //!
    //! return success true ou false to request
    int success() const;
    void setSuccess(bool value);

    //!
    //! \brief page
    //! \return
    //!
    //! currenty page
    int page() const;
    void setPage(int value);

    //!
    //! \brief per_page
    //! \return
    //!
    //! rows per page
    int per_page() const;
    void setPer_page(int value);

    //!
    //! \brief count
    //! \return
    //!
    //! returneds row per page
    int count() const;
    void setCount(int value);

    //!
    //! \brief total_count
    //! \return
    //!
    //! total rows to all pages
    int total_count() const;
    void setTotal_count(int value);

    //!
    //! \brief total_pages
    //! \return
    //!
    //! total pages
    int total_pages() const;
    void setTotal_pages(int value);

    //!
    //! \brief toRequestHash
    //! \return
    //!
    virtual const QVariantHash toRequestHash() const;

private:
    void*p=nullptr;
};

}
