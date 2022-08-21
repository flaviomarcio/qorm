#pragma once

#include <QVariant>
#include "../../../qstm/src/qstm_object_wrapper.h"
#include "../qorm_global.h"
#include "../qorm_types.h"

namespace QOrm {
class DesignPvt;
class Q_ORM_EXPORT Design : public QStm::ObjectWrapper
{
    Q_OBJECT
    QSTM_OBJECT_WRAPPER(Design)

    QORM_MODEL_DECLARE_FORM_ENUMS

    Q_PROPERTY(QVariant width READ width WRITE setWidth RESET resetWidth NOTIFY widthChanged)
    Q_PROPERTY(QVariant height READ height WRITE setHeight RESET resetHeight NOTIFY heightChanged)
    Q_PROPERTY(int rows READ rows WRITE setRows RESET resetRows NOTIFY rowsChanged)
    Q_PROPERTY(int columns READ columns WRITE setColumns RESET resetColumns NOTIFY columnsChanged)
    Q_PROPERTY(Layout layout READ layout WRITE setLayout RESET resetLayout NOTIFY layoutChanged)
    Q_PROPERTY(QVariant type READ type WRITE setType RESET resetType NOTIFY typeChanged)
public:

    //!
    //! \brief InputDesign
    //! \param parent
    //!
    Q_INVOKABLE explicit Design(QObject *parent = nullptr);

    //!
    //! \brief width
    //! \return
    //!
    QVariant &width();
    Design &width(const QVariant &value){return this->setWidth(value);};
    Design &setWidth(const QVariant &value);
    Design &resetWidth();

    //!
    //! \brief height
    //! \return
    //!
    QVariant &height();
    Design &height(const QVariant &value){return this->setHeight(value);};
    Design &setHeight(const QVariant &value);
    Design &resetHeight();

    //!
    //! \brief rows
    //! \return
    //!
    int rows() const;
    Design &rows(int value){return this->setRows(value);};
    Design &setRows(int value);
    Design &resetRows();

    //!
    //! \brief columns
    //! \return
    //!
    int columns() const;
    Design &columns(int value){ return this->setColumns(value);};
    Design &setColumns(int value);
    Design &resetColumns();

    //!
    //! \brief layout
    //! \return
    //!
    QOrm::Design::Layout layout() const;
    Design &layout(const QVariant &value){ return this->setLayout(value);};
    Design &setLayout(const QVariant &value);
    Design &resetLayout();

    //!
    //! \brief formType
    //! \return
    //!
    QVariant &type() const;
    Design &type(const QVariant &value){ return this->setType(value);};
    Design &setType(const QVariant &value);
    Design &resetType();

private:
    DesignPvt *p=nullptr;
signals:
    void widthChanged();
    void heightChanged();
    void rowsChanged();
    void columnsChanged();
    void layoutChanged();
    void typeChanged();
};

} // namespace QOrm

