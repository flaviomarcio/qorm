#pragma once

#include <QtReforce/QStm>
#include "./qorm_global.h"

namespace QOrm {
class ModelDescriptorFieldPvt;
class Q_ORM_EXPORT ModelDescriptorField : public QStm::ObjectWrapper
{
    Q_OBJECT
    Q_PROPERTY(QVariant name READ name WRITE name RESET resetName NOTIFY nameChanged)
    Q_PROPERTY(QVariant title READ title WRITE title RESET resetTitle NOTIFY titleChanged)
    Q_PROPERTY(QVariant inputMask READ inputMask WRITE inputMask RESET resetInputMask NOTIFY inputMaskChanged)
    Q_PROPERTY(QVariant primaryKey READ primaryKey WRITE primaryKey RESET resetPrimaryKey NOTIFY primaryKeyChanged)
    Q_PROPERTY(QVariant inputType READ inputType WRITE inputType RESET resetInputType NOTIFY inputTypeChanged)
    Q_PROPERTY(QVariant inputTextSearch READ inputTextSearch WRITE inputTextSearch RESET resetInputTextSearch NOTIFY inputTextSearchChanged)
    Q_PROPERTY(QVariant length READ length WRITE length RESET resetLength NOTIFY lengthChanged)
    Q_PROPERTY(QVariant dataType READ dataType WRITE dataType RESET resetDataType NOTIFY dataTypeChanged)
    Q_PROPERTY(QVariant defaultValue READ defaultValue WRITE defaultValue RESET resetDefaultValue NOTIFY defaultValueChanged)
    Q_PROPERTY(QVariant defaultSelect READ defaultSelect WRITE defaultSelect RESET resetDefaultSelect NOTIFY defaultSelectChanged)
    Q_PROPERTY(bool obrigatory READ obrigatory WRITE obrigatory RESET resetObrigatory NOTIFY obrigatoryChanged)
    Q_PROPERTY(QVariant align READ align WRITE align RESET resetAlign NOTIFY alignChanged)
    Q_PROPERTY(int order READ order WRITE order RESET resetOrder NOTIFY orderChanged)
    Q_PROPERTY(bool displayer READ displayer WRITE displayer RESET resetDisplayer NOTIFY displayerChanged)
    Q_PROPERTY(bool displayText READ displayText WRITE displayText RESET resetDisplayText NOTIFY displayTextChanged)
    Q_PROPERTY(QVariant width READ width WRITE width RESET resetWidth NOTIFY widthChanged)
    Q_PROPERTY(QVariant color READ color WRITE color RESET resetColor NOTIFY colorChanged)
    Q_PROPERTY(QVariant font READ font WRITE font RESET resetFont NOTIFY fontChanged)
    Q_PROPERTY(bool filtrable READ filtrable WRITE filtrable RESET resetFiltrable NOTIFY filtrableChanged)
    Q_PROPERTY(QVariant filtrableStrategy READ filtrableStrategy WRITE filtrableStrategy RESET resetFiltrableStrategy NOTIFY filtrableStrategyChanged)
    Q_PROPERTY(bool editable READ editable WRITE editable RESET resetEditable NOTIFY editableChanged)
    Q_PROPERTY(bool visible READ visible WRITE visible RESET resetVisible NOTIFY visibleChanged)
    Q_PROPERTY(bool sortable READ sortable WRITE sortable RESET resetSortable NOTIFY sortableChanged)
    Q_PROPERTY(bool readOnly READ readOnly WRITE readOnly RESET resetReadOnly NOTIFY readOnlyChanged)
    Q_PROPERTY(QVariant output READ output WRITE output RESET resetOutput NOTIFY outputChanged)
    Q_PROPERTY(QVariant items READ items WRITE items RESET resetItems NOTIFY itemsChanged)
    Q_PROPERTY(QVariant inputLink READ inputLink WRITE inputLink RESET resetInputLink NOTIFY inputLinkChanged)
public:
    explicit ModelDescriptorField(QObject *parent = nullptr);

    //!
    //! \brief name
    //! \return
    //!
    const QVariant &name() const;
    ModelDescriptorField &name(const QVariant &newName);
    ModelDescriptorField &resetName();

    //!
    //! \brief title
    //! \return
    //!
    const QVariant &title() const;
    ModelDescriptorField &title(const QVariant &newTitle);
    ModelDescriptorField &resetTitle();

    //!
    //! \brief inputMask
    //! \return
    //!
    const QVariant &inputMask() const;
    ModelDescriptorField &inputMask(const QVariant &newInputMask);
    ModelDescriptorField &resetInputMask();

    //!
    //! \brief primaryKey
    //! \return
    //!
    const QVariant &primaryKey() const;
    ModelDescriptorField &primaryKey(const QVariant &newPrimaryKey);
    ModelDescriptorField &resetPrimaryKey();

    //!
    //! \brief inputType
    //! \return
    //!
    const QVariant &inputType() const;
    ModelDescriptorField &inputType(const QVariant &newInputType);
    ModelDescriptorField &resetInputType();

    //!
    //! \brief inputTextSearch
    //! \return
    //!
    const QVariant &inputTextSearch() const;
    ModelDescriptorField &inputTextSearch(const QVariant &newInputTextSearch);
    ModelDescriptorField &resetInputTextSearch();

    //!
    //! \brief length
    //! \return
    //!
    const QVariant &length() const;
    ModelDescriptorField &length(const QVariant &newLength);
    ModelDescriptorField &resetLength();

    //!
    //! \brief dataType
    //! \return
    //!
    const QVariant &dataType() const;
    ModelDescriptorField &dataType(const QVariant &newDataType);
    ModelDescriptorField &resetDataType();

    //!
    //! \brief defaultValue
    //! \return
    //!
    const QVariant &defaultValue() const;
    ModelDescriptorField &defaultValue(const QVariant &newDefaultValue);
    ModelDescriptorField &resetDefaultValue();

    //!
    //! \brief defaultSelect
    //! \return
    //!
    const QVariant &defaultSelect() const;
    ModelDescriptorField &defaultSelect(const QVariant &newDefaultSelect);
    ModelDescriptorField &resetDefaultSelect();

    //!
    //! \brief obrigatory
    //! \return
    //!
    bool obrigatory() const;
    ModelDescriptorField &obrigatory(bool newObrigatory);
    ModelDescriptorField &resetObrigatory();

    //!
    //! \brief align
    //! \return
    //!
    const QVariant &align() const;
    ModelDescriptorField &align(const QVariant &newAlign);
    ModelDescriptorField &resetAlign();

    //!
    //! \brief order
    //! \return
    //!
    int order() const;
    ModelDescriptorField &order(int newOrder);
    ModelDescriptorField &resetOrder();

    //!
    //! \brief displayer
    //! \return
    //!
    bool displayer() const;
    ModelDescriptorField &displayer(bool newDisplayer);
    ModelDescriptorField &resetDisplayer();

    //!
    //! \brief displayText
    //! \return
    //!
    bool displayText() const;
    ModelDescriptorField &displayText(bool newDisplayText);
    ModelDescriptorField &resetDisplayText();

    //!
    //! \brief width
    //! \return
    //!
    const QVariant &width() const;
    ModelDescriptorField &width(const QVariant &newWidth);
    ModelDescriptorField &resetWidth();

    //!
    //! \brief color
    //! \return
    //!
    const QVariant &color() const;
    ModelDescriptorField &color(const QVariant &newColor);
    ModelDescriptorField &resetColor();

    //!
    //! \brief font
    //! \return
    //!
    const QVariant &font() const;
    ModelDescriptorField &font(const QVariant &newFont);
    ModelDescriptorField &resetFont();

    //!
    //! \brief filtrable
    //! \return
    //!
    bool filtrable() const;
    ModelDescriptorField &filtrable(bool newFiltrable);
    ModelDescriptorField &resetFiltrable();

    //!
    //! \brief filtrableStrategy
    //! \return
    //!
    const QVariant &filtrableStrategy() const;
    ModelDescriptorField &filtrableStrategy(const QVariant &newFiltrableStrategy);
    ModelDescriptorField &resetFiltrableStrategy();

    //!
    //! \brief editable
    //! \return
    //!
    bool editable() const;
    ModelDescriptorField &editable(bool newEditable);
    ModelDescriptorField &resetEditable();

    //!
    //! \brief visible
    //! \return
    //!
    bool visible() const;
    ModelDescriptorField &visible(bool newVisible);
    ModelDescriptorField &resetVisible();

    //!
    //! \brief sortable
    //! \return
    //!
    bool sortable() const;
    ModelDescriptorField &sortable(bool newSortable);
    ModelDescriptorField &resetSortable();

    //!
    //! \brief readOnly
    //! \return
    //!
    bool readOnly() const;
    ModelDescriptorField &readOnly(bool newReadOnly);
    ModelDescriptorField &resetReadOnly();

    //!
    //! \brief output
    //! \return
    //!
    const QVariant &output() const;
    ModelDescriptorField &output(const QVariant &newOutput);
    ModelDescriptorField &resetOutput();

    //!
    //! \brief items
    //! \return
    //!
    const QVariant &items() const;
    ModelDescriptorField &items(const QVariant &newItems);
    ModelDescriptorField &resetItems();

    //!
    //! \brief inputLink
    //! \return
    //!
    const QVariant &inputLink() const;
    ModelDescriptorField &inputLink(const QVariant &newInputLink);
    ModelDescriptorField &resetInputLink();

signals:
    void nameChanged();

    void titleChanged();

    void inputMaskChanged();

    void primaryKeyChanged();

    void inputTypeChanged();

    void inputTextSearchChanged();

    void lengthChanged();

    void dataTypeChanged();

    void defaultValueChanged();

    void defaultSelectChanged();

    void obrigatoryChanged();

    void alignChanged();

    void startChanged();

    void orderChanged();

    void displayerChanged();

    void displayTextChanged();

    void widthChanged();

    void colorChanged();

    void fontChanged();

    void filtrableChanged();

    void filtrableStrategyChanged();

    void editableChanged();

    void visibleChanged();

    void sortableChanged();

    void readOnlyChanged();

    void outputChanged();

    void itemsChanged();

    void inputLinkChanged();

private:
    QVariant _name;
    QVariant _title;
    QVariant _inputMask;
    QVariant _primaryKey;
    QVariant _inputType;
    QVariant _inputTextSearch;
    QVariant _length;
    QVariant _dataType;
    QVariant _defaultValue;
    QVariant _defaultSelect;
    bool _obrigatory;
    QVariant _align;
    int _order=0;
    bool _displayer;
    bool _displayText;
    QVariant _width;
    QVariant _color;
    QVariant _font;
    bool _filtrable;
    QVariant _filtrableStrategy;
    bool _editable;
    bool _visible;
    bool _sortable;
    bool _readOnly;
    QVariant _output;
    QVariant _items;
    QVariant _inputLink;

};

} // namespace QOrm

