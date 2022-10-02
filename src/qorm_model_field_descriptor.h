#pragma once

#include <QtReforce/QStm>
#include "./qorm_global.h"

namespace QOrm {
class ModelFieldDescriptorPvt;
class Q_ORM_EXPORT ModelFieldDescriptor : public QStm::ObjectWrapper
{
    Q_OBJECT
    Q_STM_OBJECT_WRAPPER(ModelFieldDescriptor)
    Q_PROPERTY(QString field READ field WRITE field RESET resetField NOTIFY fieldChanged)
    Q_PROPERTY(QString title READ title WRITE title RESET resetTitle NOTIFY titleChanged)
    Q_PROPERTY(QString inputMask READ inputMask WRITE inputMask RESET resetInputMask NOTIFY inputMaskChanged)
    Q_PROPERTY(bool primaryKey READ primaryKey WRITE primaryKey RESET resetPrimaryKey NOTIFY primaryKeyChanged)
    Q_PROPERTY(QVariant inputType READ inputType WRITE inputType RESET resetInputType NOTIFY inputTypeChanged)
    Q_PROPERTY(QVariant inputTextSearch READ inputTextSearch WRITE inputTextSearch RESET resetInputTextSearch NOTIFY inputTextSearchChanged)
    Q_PROPERTY(QVariant length READ length WRITE length RESET resetLength NOTIFY lengthChanged)
    Q_PROPERTY(QVariant dataType READ dataType WRITE dataType RESET resetDataType NOTIFY dataTypeChanged)
    Q_PROPERTY(QVariant defaultValue READ defaultValue WRITE defaultValue RESET resetDefaultValue NOTIFY defaultValueChanged)
    Q_PROPERTY(QVariant defaultSelect READ defaultSelect WRITE defaultSelect RESET resetDefaultSelect NOTIFY defaultSelectChanged)
    Q_PROPERTY(EditObrigatory obrigatory READ obrigatory WRITE obrigatory RESET resetObrigatory NOTIFY obrigatoryChanged)
    Q_PROPERTY(QVariant align READ align WRITE align RESET resetAlign NOTIFY alignChanged)
    Q_PROPERTY(int order READ order WRITE order RESET resetOrder NOTIFY orderChanged)
    Q_PROPERTY(bool displayer READ displayer WRITE displayer RESET resetDisplayer NOTIFY displayerChanged)
    Q_PROPERTY(bool displayText READ displayText WRITE displayText RESET resetDisplayText NOTIFY displayTextChanged)
    Q_PROPERTY(QVariant displayWidth READ displayWidth WRITE setDisplayWidth RESET resetDisplayWidth NOTIFY displayWidthChanged)
    Q_PROPERTY(QVariant width READ width WRITE width RESET resetWidth NOTIFY widthChanged)
    Q_PROPERTY(QVariant color READ color WRITE color RESET resetColor NOTIFY colorChanged)
    Q_PROPERTY(QVariant font READ font WRITE font RESET resetFont NOTIFY fontChanged)
    Q_PROPERTY(bool filtrable READ filtrable WRITE filtrable RESET resetFiltrable NOTIFY filtrableChanged)
    Q_PROPERTY(QVariant filtrableStrategy READ filtrableStrategy WRITE filtrableStrategy RESET resetFiltrableStrategy NOTIFY filtrableStrategyChanged)
    Q_PROPERTY(bool editable READ editable WRITE editable RESET resetEditable NOTIFY editableChanged)
    Q_PROPERTY(bool visible READ visible WRITE visible RESET resetVisible NOTIFY visibleChanged)
    Q_PROPERTY(bool sortable READ sortable WRITE sortable RESET resetSortable NOTIFY sortableChanged)
    Q_PROPERTY(EditReadOnly readOnly READ readOnly WRITE readOnly RESET resetReadOnly NOTIFY readOnlyChanged)
    Q_PROPERTY(QVariant output READ output WRITE output RESET resetOutput NOTIFY outputChanged)
    Q_PROPERTY(QVariant items READ items WRITE items RESET resetItems NOTIFY itemsChanged)
    Q_PROPERTY(QVariant inputLink READ inputLink WRITE inputLink RESET resetInputLink NOTIFY inputLinkChanged)
public:
    enum EditReadOnly{erNo, erYes, erIfNew, erIfExisting};
    Q_ENUM(EditReadOnly)

    enum EditObrigatory{eoNo, eoYes, eoIfEdit, eoIfFilter};\
    Q_ENUM(EditObrigatory)

    explicit ModelFieldDescriptor(QObject *parent = nullptr);

    //!
    //! \brief field
    //! \return
    //!
    const QString &field() const;
    ModelFieldDescriptor &field(const QString &newField);
    ModelFieldDescriptor &resetField();

    //!
    //! \brief title
    //! \return
    //!
    const QString &title() const;
    ModelFieldDescriptor &title(const QString &newTitle);
    ModelFieldDescriptor &resetTitle();

    //!
    //! \brief inputMask
    //! \return
    //!
    const QString &inputMask() const;
    ModelFieldDescriptor &inputMask(const QString &newInputMask);
    ModelFieldDescriptor &resetInputMask();

    //!
    //! \brief primaryKey
    //! \return
    //!
    bool primaryKey() const;
    ModelFieldDescriptor &primaryKey(const bool newPrimaryKey);
    ModelFieldDescriptor &resetPrimaryKey();

    //!
    //! \brief inputType
    //! \return
    //!
    const QVariant &inputType() const;
    ModelFieldDescriptor &inputType(const QVariant &newInputType);
    ModelFieldDescriptor &resetInputType();

    //!
    //! \brief inputTextSearch
    //! \return
    //!
    const QVariant &inputTextSearch() const;
    ModelFieldDescriptor &inputTextSearch(const QVariant &newInputTextSearch);
    ModelFieldDescriptor &resetInputTextSearch();

    //!
    //! \brief length
    //! \return
    //!
    const QVariant &length() const;
    ModelFieldDescriptor &length(const QVariant &newLength);
    ModelFieldDescriptor &resetLength();

    //!
    //! \brief dataType
    //! \return
    //!
    const QVariant &dataType() const;
    ModelFieldDescriptor &dataType(const QVariant &newDataType);
    ModelFieldDescriptor &resetDataType();

    //!
    //! \brief defaultValue
    //! \return
    //!
    const QVariant &defaultValue() const;
    ModelFieldDescriptor &defaultValue(const QVariant &newDefaultValue);
    ModelFieldDescriptor &resetDefaultValue();

    //!
    //! \brief defaultSelect
    //! \return
    //!
    const QVariant &defaultSelect() const;
    ModelFieldDescriptor &defaultSelect(const QVariant &newDefaultSelect);
    ModelFieldDescriptor &resetDefaultSelect();

    //!
    //! \brief obrigatory
    //! \return
    //!
    QOrm::ModelFieldDescriptor::EditObrigatory obrigatory() const;
    ModelFieldDescriptor &obrigatory(const QVariant &newObrigatory);
    ModelFieldDescriptor &resetObrigatory();

    //!
    //! \brief align
    //! \return
    //!
    const QVariant &align() const;
    ModelFieldDescriptor &align(const QVariant &newAlign);
    ModelFieldDescriptor &resetAlign();

    //!
    //! \brief order
    //! \return
    //!
    int order() const;
    ModelFieldDescriptor &order(int newOrder);
    ModelFieldDescriptor &resetOrder();

    //!
    //! \brief displayer
    //! \return
    //!
    bool displayer() const;
    ModelFieldDescriptor &displayer(bool newDisplayer);
    ModelFieldDescriptor &resetDisplayer();

    //!
    //! \brief displayText
    //! \return
    //!
    bool displayText() const;
    ModelFieldDescriptor &displayText(bool newDisplayText);
    ModelFieldDescriptor &resetDisplayText();

    //!
    //! \brief displayWidth
    //! \return
    //!
    const QVariant &displayWidth() const;
    void setDisplayWidth(const QVariant &newDisplayWidth);
    void resetDisplayWidth();

    //!
    //! \brief width
    //! \return
    //!
    const QVariant &width() const;
    ModelFieldDescriptor &width(const QVariant &newWidth);
    ModelFieldDescriptor &resetWidth();

    //!
    //! \brief color
    //! \return
    //!
    const QVariant &color() const;
    ModelFieldDescriptor &color(const QVariant &newColor);
    ModelFieldDescriptor &resetColor();

    //!
    //! \brief font
    //! \return
    //!
    const QVariant &font() const;
    ModelFieldDescriptor &font(const QVariant &newFont);
    ModelFieldDescriptor &resetFont();

    //!
    //! \brief filtrable
    //! \return
    //!
    bool filtrable() const;
    ModelFieldDescriptor &filtrable(bool newFiltrable);
    ModelFieldDescriptor &resetFiltrable();

    //!
    //! \brief filtrableStrategy
    //! \return
    //!
    QVariant &filtrableStrategy() const;
    ModelFieldDescriptor &filtrableStrategy(const QVariant &newFiltrableStrategy);
    ModelFieldDescriptor &resetFiltrableStrategy();

    //!
    //! \brief editable
    //! \return
    //!
    bool editable() const;
    ModelFieldDescriptor &editable(bool newEditable);
    ModelFieldDescriptor &resetEditable();

    //!
    //! \brief visible
    //! \return
    //!
    bool visible() const;
    ModelFieldDescriptor &visible(bool newVisible);
    ModelFieldDescriptor &resetVisible();

    //!
    //! \brief sortable
    //! \return
    //!
    bool sortable() const;
    ModelFieldDescriptor &sortable(bool newSortable);
    ModelFieldDescriptor &resetSortable();

    //!
    //! \brief readOnly
    //! \return
    //!
    EditReadOnly readOnly() const;
    ModelFieldDescriptor &readOnly(const QVariant &newReadOnly);
    ModelFieldDescriptor &resetReadOnly();

    //!
    //! \brief output
    //! \return
    //!
    const QVariant &output() const;
    ModelFieldDescriptor &output(const QVariant &newOutput);
    ModelFieldDescriptor &resetOutput();

    //!
    //! \brief items
    //! \return
    //!
    const QVariant &items() const;
    ModelFieldDescriptor &items(const QVariant &newItems);
    ModelFieldDescriptor &resetItems();

    //!
    //! \brief inputLink
    //! \return
    //!
    const QVariant &inputLink() const;
    ModelFieldDescriptor &inputLink(const QVariant &newInputLink);
    ModelFieldDescriptor &resetInputLink();


signals:
    void fieldChanged();

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

    void displayWidthChanged();

private:
    ModelFieldDescriptorPvt *p=nullptr;
};

} // namespace QOrm

