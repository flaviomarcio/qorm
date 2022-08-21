#include "p_qorm_model_dto_design.h"
#include "../../../qstm/src/qstm_meta_enum.h"

namespace QOrm {

static const auto __percent30 = "30%";
static const auto __percent40 = "40%";

class DesignPvt:public QObject
{
public:
    Design *parent=nullptr;
    QVariant width=__percent30;
    QVariant height=__percent40;
    int rows=1;
    int columns=1;
    QStm::MetaEnum<Design::Layout> layout=Design::Vertical;
    QStm::MetaEnum<Design::FormType> type=Design::RegisterForm;
    QVariant typeName;
    explicit DesignPvt(Design *parent):QObject{parent}{ this->parent=parent; }
};

Design::Design(QObject *parent)
    : QStm::ObjectWrapper{parent}
{
    this->p=new DesignPvt{this};
}

QVariant &Design::width()
{
    return p->width;
}

Design &Design::setWidth(const QVariant &value)
{
    if (p->width == value)
        return *this;
    p->width = value;
    emit widthChanged();
    return *this;
}

Design &Design::resetWidth()
{
    return setWidth(__percent40);
}

QVariant &Design::height()
{
    return p->height;
}

Design &Design::setHeight(const QVariant &value)
{
    if (p->height == value)
        return *this;
    p->height = value;
    emit heightChanged();
    return *this;
}

Design &Design::resetHeight()
{
    return setHeight(__percent30);
}

int Design::rows() const
{
    return p->rows;
}

Design &Design::setRows(int value)
{
    if (p->rows == value)
        return *this;
    p->rows = value;
    emit rowsChanged();
    return *this;
}

Design &Design::resetRows()
{
    return setRows(2);
}

int Design::columns() const
{
    return p->columns;
}

Design &Design::setColumns(int value)
{
    if (p->columns == value)
        return *this;
    p->columns = value;
    emit columnsChanged();
    return *this;
}

Design &Design::resetColumns()
{
    return setColumns({});
}

Design::Layout Design::layout() const
{
    return p->layout.type();
}

Design &Design::setLayout(const QVariant &value)
{
    if (p->layout == value)
        return *this;
    p->layout = value;
    emit layoutChanged();
    return *this;
}

Design &Design::resetLayout()
{
    return setLayout({});
}

QVariant &Design::type() const
{
    p->typeName=p->type.name();
    return p->typeName;
}

Design &Design::setType(const QVariant &value)
{
    if (p->type == value)
        return *this;
    p->type = value;
    emit typeChanged();
    return *this;
}

Design &Design::resetType()
{
    return setType({});
}

} // namespace QOrm
