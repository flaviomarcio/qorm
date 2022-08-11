#include "qorm_model_descriptor_field.h"

namespace QOrm {

ModelDescriptorField::ModelDescriptorField(QObject *parent)
    : QStm::ObjectWrapper{parent}
{

}

const QVariant &ModelDescriptorField::name() const
{
    return _name;
}

ModelDescriptorField &ModelDescriptorField::name(const QVariant &newName)
{
    if (_name == newName)
        return *this;
    _name = newName;
    emit titleChanged();
    return *this;
}

ModelDescriptorField &ModelDescriptorField::resetName()
{
    return this->name({});

}

const QVariant &ModelDescriptorField::title() const
{
    return _title;
}

ModelDescriptorField &ModelDescriptorField::title(const QVariant &newTitle)
{
    if (_title == newTitle)
        return *this;
    _title = newTitle;
    emit titleChanged();
    return *this;
}

ModelDescriptorField &ModelDescriptorField::resetTitle()
{
    return this->title({});
}

const QVariant &ModelDescriptorField::inputMask() const
{
    return _inputMask;
}

ModelDescriptorField &ModelDescriptorField::inputMask(const QVariant &newInputMask)
{
    if (_inputMask == newInputMask)
        return *this;
    _inputMask = newInputMask;
    emit inputMaskChanged();
    return *this;
}

ModelDescriptorField &ModelDescriptorField::resetInputMask()
{
    return this->inputMask({});
}

const QVariant &ModelDescriptorField::primaryKey() const
{
    return _primaryKey;
}

ModelDescriptorField &ModelDescriptorField::primaryKey(const QVariant &newPrimaryKey)
{
    if (_primaryKey == newPrimaryKey)
        return *this;
    _primaryKey = newPrimaryKey;
    emit primaryKeyChanged();
    return *this;
}

ModelDescriptorField &ModelDescriptorField::resetPrimaryKey()
{
    return this->primaryKey({});
}

const QVariant &ModelDescriptorField::inputType() const
{
    return _inputType;
}

ModelDescriptorField &ModelDescriptorField::inputType(const QVariant &newInputType)
{
    if (_inputType == newInputType)
        return *this;
    _inputType = newInputType;
    emit inputTypeChanged();
    return *this;
}

ModelDescriptorField &ModelDescriptorField::resetInputType()
{
    return this->inputType({});
}

const QVariant &ModelDescriptorField::inputTextSearch() const
{
    return _inputTextSearch;
}

ModelDescriptorField &ModelDescriptorField::inputTextSearch(const QVariant &newInputTextSearch)
{
    if (_inputTextSearch == newInputTextSearch)
        return *this;
    _inputTextSearch = newInputTextSearch;
    emit inputTextSearchChanged();
    return *this;
}

ModelDescriptorField &ModelDescriptorField::resetInputTextSearch()
{
    return this->inputTextSearch({});
}

const QVariant &ModelDescriptorField::length() const
{
    return _length;
}

ModelDescriptorField &ModelDescriptorField::length(const QVariant &newLength)
{
    if (_length == newLength)
        return *this;
    _length = newLength;
    emit lengthChanged();
    return *this;
}

ModelDescriptorField &ModelDescriptorField::resetLength()
{
    return this->length({});
}

const QVariant &ModelDescriptorField::dataType() const
{
    return _dataType;
}

ModelDescriptorField &ModelDescriptorField::dataType(const QVariant &newDataType)
{
    if (_dataType == newDataType)
        return *this;
    _dataType = newDataType;
    emit dataTypeChanged();
    return *this;
}

ModelDescriptorField &ModelDescriptorField::resetDataType()
{
    return this->dataType({});
}

const QVariant &ModelDescriptorField::defaultValue() const
{
    return _defaultValue;
}

ModelDescriptorField &ModelDescriptorField::defaultValue(const QVariant &newDefaultValue)
{
    if (_defaultValue == newDefaultValue)
        return *this;
    _defaultValue = newDefaultValue;
    emit defaultValueChanged();
    return *this;
}

ModelDescriptorField &ModelDescriptorField::resetDefaultValue()
{
    return this->defaultValue({});
}

const QVariant &ModelDescriptorField::defaultSelect() const
{
    return _defaultSelect;
}

ModelDescriptorField &ModelDescriptorField::defaultSelect(const QVariant &newDefaultSelect)
{
    if (_defaultSelect == newDefaultSelect)
        return *this;
    _defaultSelect = newDefaultSelect;
    emit defaultSelectChanged();
    return *this;
}

ModelDescriptorField &ModelDescriptorField::resetDefaultSelect()
{
    return this->defaultSelect({});
}

bool ModelDescriptorField::obrigatory() const
{
    return _obrigatory;
}

ModelDescriptorField &ModelDescriptorField::obrigatory(bool newObrigatory)
{
    if (_obrigatory == newObrigatory)
        return *this;
    _obrigatory = newObrigatory;
    emit obrigatoryChanged();
    return *this;
}

ModelDescriptorField &ModelDescriptorField::resetObrigatory()
{
    return this->obrigatory({});
}

const QVariant &ModelDescriptorField::align() const
{
    return _align;
}

ModelDescriptorField &ModelDescriptorField::align(const QVariant &newAlign)
{
    if (_align == newAlign)
        return *this;
    _align = newAlign;
    emit alignChanged();
    return *this;
}

ModelDescriptorField &ModelDescriptorField::resetAlign()
{
    return this->align({});
}

int ModelDescriptorField::order() const
{
    return _order;
}

ModelDescriptorField &ModelDescriptorField::order(int newOrder)
{
    if (_order == newOrder)
        return *this;
    _order = newOrder;
    emit orderChanged();
    return *this;
}

ModelDescriptorField &ModelDescriptorField::resetOrder()
{
    return this->order({});
}

bool ModelDescriptorField::displayer() const
{
    return _displayer;
}

ModelDescriptorField &ModelDescriptorField::displayer(bool newDisplayer)
{
    if (_displayer == newDisplayer)
        return *this;
    _displayer = newDisplayer;
    emit displayerChanged();
    return *this;
}

ModelDescriptorField &ModelDescriptorField::resetDisplayer()
{
    return this->displayer({});
}

bool ModelDescriptorField::displayText() const
{
    return _displayText;
}

ModelDescriptorField &ModelDescriptorField::displayText(bool newDisplayText)
{
    if (_displayText == newDisplayText)
        return *this;
    _displayText = newDisplayText;
    emit displayTextChanged();
    return *this;
}

ModelDescriptorField &ModelDescriptorField::resetDisplayText()
{
    return this->displayText({});
}

const QVariant &ModelDescriptorField::width() const
{
    return _width;
}

ModelDescriptorField &ModelDescriptorField::width(const QVariant &newWidth)
{
    if (_width == newWidth)
        return *this;
    _width = newWidth;
    emit widthChanged();
    return *this;
}

ModelDescriptorField &ModelDescriptorField::resetWidth()
{
    return this->width({});
}

const QVariant &ModelDescriptorField::color() const
{
    return _color;
}

ModelDescriptorField &ModelDescriptorField::color(const QVariant &newColor)
{
    if (_color == newColor)
        return *this;
    _color = newColor;
    emit colorChanged();
    return *this;
}

ModelDescriptorField &ModelDescriptorField::resetColor()
{
    return this->color({});
}

const QVariant &ModelDescriptorField::font() const
{
    return _font;
}

ModelDescriptorField &ModelDescriptorField::font(const QVariant &newFont)
{
    if (_font == newFont)
        return *this;
    _font = newFont;
    emit fontChanged();
    return *this;
}

ModelDescriptorField &ModelDescriptorField::resetFont()
{
    return this->font({});
}

bool ModelDescriptorField::filtrable() const
{
    return _filtrable;
}

ModelDescriptorField &ModelDescriptorField::filtrable(bool newFiltrable)
{
    if (_filtrable == newFiltrable)
        return *this;
    _filtrable = newFiltrable;
    emit filtrableChanged();
    return *this;
}

ModelDescriptorField &ModelDescriptorField::resetFiltrable()
{
    return this->filtrable({});
}

const QVariant &ModelDescriptorField::filtrableStrategy() const
{
    return _filtrableStrategy;
}

ModelDescriptorField &ModelDescriptorField::filtrableStrategy(const QVariant &newFiltrableStrategy)
{
    if (_filtrableStrategy == newFiltrableStrategy)
        return *this;
    _filtrableStrategy = newFiltrableStrategy;
    emit filtrableStrategyChanged();
    return *this;
}

ModelDescriptorField &ModelDescriptorField::resetFiltrableStrategy()
{
    return this->filtrableStrategy({});
}

bool ModelDescriptorField::editable() const
{
    return _editable;
}

ModelDescriptorField &ModelDescriptorField::editable(bool newEditable)
{
    if (_editable == newEditable)
        return *this;
    _editable = newEditable;
    emit editableChanged();
    return *this;
}

ModelDescriptorField &ModelDescriptorField::resetEditable()
{
    return this->editable({});
}

bool ModelDescriptorField::visible() const
{
    return _visible;
}

ModelDescriptorField &ModelDescriptorField::visible(bool newVisible)
{
    if (_visible == newVisible)
        return *this;
    _visible = newVisible;
    emit visibleChanged();
    return *this;
}

ModelDescriptorField &ModelDescriptorField::resetVisible()
{
    return this->visible({});
}

bool ModelDescriptorField::sortable() const
{
    return _sortable;
}

ModelDescriptorField &ModelDescriptorField::sortable(bool newSortable)
{
    if (_sortable == newSortable)
        return *this;
    _sortable = newSortable;
    emit sortableChanged();
    return *this;
}

ModelDescriptorField &ModelDescriptorField::resetSortable()
{
    return this->sortable({});
}

bool ModelDescriptorField::readOnly() const
{
    return _readOnly;
}

ModelDescriptorField &ModelDescriptorField::readOnly(bool newReadOnly)
{
    if (_readOnly == newReadOnly)
        return *this;
    _readOnly = newReadOnly;
    emit readOnlyChanged();
    return *this;
}

ModelDescriptorField &ModelDescriptorField::resetReadOnly()
{
    return this->readOnly({});
}

const QVariant &ModelDescriptorField::output() const
{
    return _output;
}

ModelDescriptorField &ModelDescriptorField::output(const QVariant &newOutput)
{
    if (_output == newOutput)
        return *this;
    _output = newOutput;
    emit outputChanged();
    return *this;
}

ModelDescriptorField &ModelDescriptorField::resetOutput()
{
    return this->output({});
}

const QVariant &ModelDescriptorField::items() const
{
    return _items;
}

ModelDescriptorField &ModelDescriptorField::items(const QVariant &newItems)
{
    if (_items == newItems)
        return *this;
    _items = newItems;
    emit itemsChanged();
    return *this;
}

ModelDescriptorField &ModelDescriptorField::resetItems()
{
    return this->items({});
}

const QVariant &ModelDescriptorField::inputLink() const
{
    return _inputLink;
}

ModelDescriptorField &ModelDescriptorField::inputLink(const QVariant &newInputLink)
{
    if (_inputLink == newInputLink)
        return *this;
    _inputLink = newInputLink;
    emit inputLinkChanged();
    return *this;
}

ModelDescriptorField &ModelDescriptorField::resetInputLink()
{
    return this->inputLink({});
}

} // namespace QOrm
