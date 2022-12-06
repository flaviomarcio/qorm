#include "qorm_model_field_descriptor.h"

namespace QOrm {

class ModelFieldDescriptorPvt:public QObject{
public:
    QString field;
    QString title;
    QString inputMask;
    bool primaryKey=false;
    QVariant inputType;
    QVariant inputTextSearch;
    QVariant length=0;
    QVariant dataType;
    QVariant defaultValue;
    QVariant defaultSelect;
    QStm::MetaEnum<ModelFieldDescriptor::EditObrigatory> obrigatory=ModelFieldDescriptor::eoNo;
    QVariant align;
    int order=0;
    bool displayer=false;
    bool displayText=false;
    QVariant displayWidth;
    QVariant width;
    QVariant color;
    QVariant font;
    bool filtrable=false;
    QVariant filtrableStrategy;
    bool editable=false;
    bool visible=false;
    bool sortable=false;
    QStm::MetaEnum<ModelFieldDescriptor::EditReadOnly> readOnly=ModelFieldDescriptor::erNo;
    QVariant output;
    QVariant items;
    QVariant inputLink;
    QString format={};
    QStm::MetaEnum<ModelFieldDescriptor::SummaryMode> summaryMode=ModelFieldDescriptor::None;
    bool grouping=false;
    explicit ModelFieldDescriptorPvt(QObject *parent):QObject{parent}{}
};

ModelFieldDescriptor::ModelFieldDescriptor(QObject *parent)
    : QStm::ObjectWrapper{parent}
{
    this->p=new ModelFieldDescriptorPvt{parent};
}

const QString &ModelFieldDescriptor::field() const
{
    return p->field;
}

ModelFieldDescriptor &ModelFieldDescriptor::field(const QString &newField)
{
    if (p->field == newField.trimmed())
        return *this;
    p->field = newField.trimmed();
    emit fieldChanged();
    return *this;
}

ModelFieldDescriptor &ModelFieldDescriptor::resetField()
{
    return this->field({});
}

const QString &ModelFieldDescriptor::title() const
{
    return p->title;
}

ModelFieldDescriptor &ModelFieldDescriptor::title(const QString &newTitle)
{
    if (p->title == newTitle.trimmed())
        return *this;
    p->title = newTitle.trimmed();
    emit titleChanged();
    return *this;
}

ModelFieldDescriptor &ModelFieldDescriptor::resetTitle()
{
    return this->title({});
}

const QString &ModelFieldDescriptor::inputMask() const
{
    return p->inputMask;
}

ModelFieldDescriptor &ModelFieldDescriptor::inputMask(const QString &newInputMask)
{
    if (p->inputMask == newInputMask)
        return *this;
    p->inputMask = newInputMask;
    emit inputMaskChanged();
    return *this;
}

ModelFieldDescriptor &ModelFieldDescriptor::resetInputMask()
{
    return this->inputMask({});
}

bool ModelFieldDescriptor::primaryKey() const
{
    return p->primaryKey;
}

ModelFieldDescriptor &ModelFieldDescriptor::primaryKey(const bool newPrimaryKey)
{
    if (p->primaryKey == newPrimaryKey)
        return *this;
    p->primaryKey = newPrimaryKey;
    emit primaryKeyChanged();
    return *this;
}

ModelFieldDescriptor &ModelFieldDescriptor::resetPrimaryKey()
{
    return this->primaryKey({});
}

const QVariant &ModelFieldDescriptor::inputType() const
{
    return p->inputType;
}

ModelFieldDescriptor &ModelFieldDescriptor::inputType(const QVariant &newInputType)
{
    if (p->inputType == newInputType)
        return *this;
    p->inputType = newInputType;
    emit inputTypeChanged();
    return *this;
}

ModelFieldDescriptor &ModelFieldDescriptor::resetInputType()
{
    return this->inputType({});
}

const QVariant &ModelFieldDescriptor::inputTextSearch() const
{
    return p->inputTextSearch;
}

ModelFieldDescriptor &ModelFieldDescriptor::inputTextSearch(const QVariant &newInputTextSearch)
{
    if (p->inputTextSearch == newInputTextSearch)
        return *this;
    p->inputTextSearch = newInputTextSearch;
    emit inputTextSearchChanged();
    return *this;
}

ModelFieldDescriptor &ModelFieldDescriptor::resetInputTextSearch()
{
    return this->inputTextSearch({});
}

const QVariant &ModelFieldDescriptor::length() const
{
    return p->length;
}

ModelFieldDescriptor &ModelFieldDescriptor::length(const QVariant &newLength)
{
    if (p->length == newLength)
        return *this;
    p->length = newLength;
    emit lengthChanged();
    return *this;
}

ModelFieldDescriptor &ModelFieldDescriptor::resetLength()
{
    return this->length({});
}

const QVariant &ModelFieldDescriptor::dataType() const
{
    return p->dataType;
}

ModelFieldDescriptor &ModelFieldDescriptor::dataType(const QVariant &newDataType)
{
    if (p->dataType == newDataType)
        return *this;
    p->dataType = newDataType;
    emit dataTypeChanged();
    return *this;
}

ModelFieldDescriptor &ModelFieldDescriptor::resetDataType()
{
    return this->dataType({});
}

const QVariant &ModelFieldDescriptor::defaultValue() const
{
    return p->defaultValue;
}

ModelFieldDescriptor &ModelFieldDescriptor::defaultValue(const QVariant &newDefaultValue)
{
    if (p->defaultValue == newDefaultValue)
        return *this;
    p->defaultValue = newDefaultValue;
    emit defaultValueChanged();
    return *this;
}

ModelFieldDescriptor &ModelFieldDescriptor::resetDefaultValue()
{
    return this->defaultValue({});
}

const QVariant &ModelFieldDescriptor::defaultSelect() const
{
    return p->defaultSelect;
}

ModelFieldDescriptor &ModelFieldDescriptor::defaultSelect(const QVariant &newDefaultSelect)
{
    if (p->defaultSelect == newDefaultSelect)
        return *this;
    p->defaultSelect = newDefaultSelect;
    emit defaultSelectChanged();
    return *this;
}

ModelFieldDescriptor &ModelFieldDescriptor::resetDefaultSelect()
{
    return this->defaultSelect({});
}

ModelFieldDescriptor::EditObrigatory ModelFieldDescriptor::obrigatory() const
{
    return p->obrigatory.type();
}

ModelFieldDescriptor &ModelFieldDescriptor::obrigatory(const QVariant &newObrigatory)
{
    if (p->obrigatory == newObrigatory)
        return *this;
    p->obrigatory = newObrigatory;
    emit obrigatoryChanged();
    return *this;
}

ModelFieldDescriptor &ModelFieldDescriptor::resetObrigatory()
{
    return this->obrigatory({});
}

const QVariant &ModelFieldDescriptor::align() const
{
    return p->align;
}

ModelFieldDescriptor &ModelFieldDescriptor::align(const QVariant &newAlign)
{
    if (p->align == newAlign)
        return *this;
    p->align = newAlign;
    emit alignChanged();
    return *this;
}

ModelFieldDescriptor &ModelFieldDescriptor::resetAlign()
{
    return this->align({});
}

int ModelFieldDescriptor::order() const
{
    return p->order;
}

ModelFieldDescriptor &ModelFieldDescriptor::order(int newOrder)
{
    if (p->order == newOrder)
        return *this;
    p->order = newOrder;
    emit orderChanged();
    return *this;
}

ModelFieldDescriptor &ModelFieldDescriptor::resetOrder()
{
    return this->order({});
}

bool ModelFieldDescriptor::displayer() const
{
    return p->displayer;
}

ModelFieldDescriptor &ModelFieldDescriptor::displayer(bool newDisplayer)
{
    if (p->displayer == newDisplayer)
        return *this;
    p->displayer = newDisplayer;
    emit displayerChanged();
    return *this;
}

ModelFieldDescriptor &ModelFieldDescriptor::resetDisplayer()
{
    return this->displayer({});
}

bool ModelFieldDescriptor::displayText() const
{
    return p->displayText;
}

ModelFieldDescriptor &ModelFieldDescriptor::displayText(bool newDisplayText)
{
    if (p->displayText == newDisplayText)
        return *this;
    p->displayText = newDisplayText;
    emit displayTextChanged();
    return *this;
}

ModelFieldDescriptor &ModelFieldDescriptor::resetDisplayText()
{
    return this->displayText({});
}

const QVariant &ModelFieldDescriptor::displayWidth() const
{
    return p->displayWidth;
}

void ModelFieldDescriptor::setDisplayWidth(const QVariant &newDisplayWidth)
{
    if (p->displayWidth == newDisplayWidth)
        return;
    p->displayWidth = newDisplayWidth;
    emit displayWidthChanged();
}

void ModelFieldDescriptor::resetDisplayWidth()
{
    setDisplayWidth({});
}

const QVariant &ModelFieldDescriptor::width() const
{
    return p->width;
}

ModelFieldDescriptor &ModelFieldDescriptor::width(const QVariant &newWidth)
{
    if (p->width == newWidth)
        return *this;
    p->width = newWidth;
    emit widthChanged();
    return *this;
}

ModelFieldDescriptor &ModelFieldDescriptor::resetWidth()
{
    return this->width({});
}

const QVariant &ModelFieldDescriptor::color() const
{
    return p->color;
}

ModelFieldDescriptor &ModelFieldDescriptor::color(const QVariant &newColor)
{
    if (p->color == newColor)
        return *this;
    p->color = newColor;
    emit colorChanged();
    return *this;
}

ModelFieldDescriptor &ModelFieldDescriptor::resetColor()
{
    return this->color({});
}

const QVariant &ModelFieldDescriptor::font() const
{
    return p->font;
}

ModelFieldDescriptor &ModelFieldDescriptor::font(const QVariant &newFont)
{
    if (p->font == newFont)
        return *this;
    p->font = newFont;
    emit fontChanged();
    return *this;
}

ModelFieldDescriptor &ModelFieldDescriptor::resetFont()
{
    return this->font({});
}

bool ModelFieldDescriptor::filtrable() const
{
    return p->filtrable;
}

ModelFieldDescriptor &ModelFieldDescriptor::filtrable(bool newFiltrable)
{
    if (p->filtrable == newFiltrable)
        return *this;
    p->filtrable = newFiltrable;
    emit filtrableChanged();
    return *this;
}

ModelFieldDescriptor &ModelFieldDescriptor::resetFiltrable()
{
    return this->filtrable({});
}

QVariant &ModelFieldDescriptor::filtrableStrategy() const
{
    return p->filtrableStrategy;
}

ModelFieldDescriptor &ModelFieldDescriptor::filtrableStrategy(const QVariant &newFiltrableStrategy)
{
    if (p->filtrableStrategy == newFiltrableStrategy)
        return *this;
    p->filtrableStrategy = newFiltrableStrategy;
    emit filtrableStrategyChanged();
    return *this;
}

ModelFieldDescriptor &ModelFieldDescriptor::resetFiltrableStrategy()
{
    return this->filtrableStrategy({});
}

bool ModelFieldDescriptor::editable() const
{
    return p->editable;
}

ModelFieldDescriptor &ModelFieldDescriptor::editable(bool newEditable)
{
    if (p->editable == newEditable)
        return *this;
    p->editable = newEditable;
    emit editableChanged();
    return *this;
}

ModelFieldDescriptor &ModelFieldDescriptor::resetEditable()
{
    return this->editable({});
}

bool ModelFieldDescriptor::visible() const
{
    return p->visible;
}

ModelFieldDescriptor &ModelFieldDescriptor::visible(bool newVisible)
{
    if (p->visible == newVisible)
        return *this;
    p->visible = newVisible;
    emit visibleChanged();
    return *this;
}

ModelFieldDescriptor &ModelFieldDescriptor::resetVisible()
{
    return this->visible({});
}

bool ModelFieldDescriptor::sortable() const
{
    return p->sortable;
}

ModelFieldDescriptor &ModelFieldDescriptor::sortable(bool newSortable)
{
    if (p->sortable == newSortable)
        return *this;
    p->sortable = newSortable;
    emit sortableChanged();
    return *this;
}

ModelFieldDescriptor &ModelFieldDescriptor::resetSortable()
{
    return this->sortable({});
}

ModelFieldDescriptor::EditReadOnly ModelFieldDescriptor::readOnly() const
{
    return p->readOnly.type();
}

ModelFieldDescriptor &ModelFieldDescriptor::readOnly(const QVariant &newReadOnly)
{
    if (p->readOnly == newReadOnly)
        return *this;
    p->readOnly = newReadOnly;
    emit readOnlyChanged();
    return *this;
}

ModelFieldDescriptor &ModelFieldDescriptor::resetReadOnly()
{
    return this->readOnly(this->erNo);
}

const QVariant &ModelFieldDescriptor::output() const
{
    return p->output;
}

ModelFieldDescriptor &ModelFieldDescriptor::output(const QVariant &newOutput)
{
    if (p->output == newOutput)
        return *this;
    p->output = newOutput;
    emit outputChanged();
    return *this;
}

ModelFieldDescriptor &ModelFieldDescriptor::resetOutput()
{
    return this->output({});
}

const QVariant &ModelFieldDescriptor::items() const
{
    return p->items;
}

ModelFieldDescriptor &ModelFieldDescriptor::items(const QVariant &newItems)
{
    if (p->items == newItems)
        return *this;
    p->items = newItems;
    emit itemsChanged();
    return *this;
}

ModelFieldDescriptor &ModelFieldDescriptor::resetItems()
{
    return this->items({});
}

const QVariant &ModelFieldDescriptor::inputLink() const
{
    return p->inputLink;
}

ModelFieldDescriptor &ModelFieldDescriptor::inputLink(const QVariant &newInputLink)
{
    if (p->inputLink == newInputLink)
        return *this;
    p->inputLink = newInputLink;
    emit inputLinkChanged();
    return *this;
}

ModelFieldDescriptor &ModelFieldDescriptor::resetInputLink()
{
    return this->inputLink({});
}

ModelFieldDescriptor::SummaryMode ModelFieldDescriptor::summaryMode() const
{
    if(p->summaryMode.equal(ModelFieldDescriptor::None))
        return {};

    return p->summaryMode.type();
}

ModelFieldDescriptor &ModelFieldDescriptor::summaryMode(const QVariant &newSummaryMode)
{
    if(p->summaryMode.equal(newSummaryMode))
        return *this;
    p->summaryMode=newSummaryMode;
    emit summaryModeChanged();
    return *this;
}

ModelFieldDescriptor &ModelFieldDescriptor::resetSummaryMode()
{
    return this->summaryMode(ModelFieldDescriptor::None);
}

bool ModelFieldDescriptor::grouping() const
{
    return p->grouping;
}

ModelFieldDescriptor &ModelFieldDescriptor::grouping(bool newGrouping)
{
    if(p->grouping==newGrouping)
        return *this;
    p->grouping=newGrouping;
    emit groupingChanged();
    return *this;
}

ModelFieldDescriptor &ModelFieldDescriptor::resetGrouping()
{
    return this->grouping({});
}

QString &ModelFieldDescriptor::format() const
{
    return p->format;
}

ModelFieldDescriptor &ModelFieldDescriptor::format(const QString &newFormat)
{
    if(p->format==newFormat)
        return *this;
    p->format=newFormat;
    return *this;
}

ModelFieldDescriptor &ModelFieldDescriptor::resetFormat()
{
    return this->format({});
}

} // namespace QOrm
