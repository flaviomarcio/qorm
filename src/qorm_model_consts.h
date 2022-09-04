#pragma once

struct EditObrigatory{
    const char *eoNo="eoNo";
    const char *eoYes="eoYes";
    const char *eoIfEdit="eoEdit";
    const char *eoIfFilter="eoFilter";
};

struct EditReadOnly{
    const char *erNo="erNo";
    const char *erYes="erYes";
    const char *erIfNew="erIfNew";
    const char *erIfExisting="erIfExisting";
};

static const auto vAuto="Auto";
static const auto vBool="Bool";
static const auto vCurrency="Currency";
static const auto vDate="Date";
static const auto vDateTime="DateTime";
static const auto vDouble="Double";
static const auto vInt="Int";
static const auto vNumber="Number";
static const auto vNumeric="Numeric";
static const auto vString="String";
static const auto vText="Text";
static const auto vTime="Time";
static const auto vUrl="Url";
static const auto vUuid="Uuid";
static const EditObrigatory vObrigatory;
static const EditReadOnly vReadOnly;


static const auto vaCenter="center";
static const auto vaEnd="end";
static const auto vaStart="start";

static const auto vlHorizontal="horizontal";
static const auto vlVertical="vertical";

static const auto vpActions="actions";
static const auto vpAlign="align";
static const auto vpColor="color";
static const auto vpColumns="columns";
static const auto vpDataType="dataType";
static const auto vpDefault="default";
static const auto vpDefaultSelect="defaultSelect";
static const auto vpDefaultValue="defaultValue";
static const auto vpDesign="design";
static const auto vpDisplayer="displayer";
static const auto vpDisplayText="displayText";
static const auto vpEdit="edit";
static const auto vpEditable="editable";
static const auto vpEndPoint="endPoint";
static const auto vpEndPoints="endPoints";
static const auto vpField="field";
static const auto vpFilters="filters";
static const auto vpFilterStyle="filterStyle";
static const auto vpFiltrable="filtrable";
static const auto vpFiltrableStrategy="searchStrategy";
static const auto vpFlags="flags";
static const auto vpFont="font";
static const auto vpFormat="format";
static const auto vpHeaders="headers";
static const auto vpHeight="height";
static const auto vpId="id";
static const auto vpInputFormat="inputFormat";
static const auto vpInputLink="inputLink";
static const auto vpInputMask="inputMask";
static const auto vpInputType="inputType";
static const auto vpActionType="actionType";
static const auto vpItems="items";
static const auto vpLayout="layout";
static const auto vpLength="length";
static const auto vpLinks="links";
static const auto vpMethod="method";
static const auto vpName="name";
static const auto vpObrigatory="obrigatory";
static const auto vpOptions="options";
static const auto vpOrder="order";
static const auto vpOutput="output";
static const auto vpPath="path";
static const auto vpPerfumery="perfumerys";
static const auto vpPrimaryKey="primaryKey";
static const auto vpReadOnly="readOnly";
static const auto vpResultInfo="resultInfo";
static const auto vpRoute="route";
static const auto vpRows="rows";
static const auto vpSearch="search";
static const auto vpSortable="sortable";
static const auto vpSortBy="sortBy";
static const auto vpSortDesc="sortDesc";
static const auto vpTitle="title";
static const auto vpType="type";
static const auto vpUuid="uuid";
static const auto vpVisible="visible";
static const auto vpWidth="width";


static const auto vList="list";
static const auto vObject="object";

static const auto vtInputAuto="InputAuto";
static const auto vtInputAddress="InputAddress";
static const auto vtInputCalendar="InputCalendar";
static const auto vtInputCheckBox="InputCheckBox";
static const auto vtInputComboBox="InputComboBox";
static const auto vtInputCurrency="InputCurrency";
static const auto vtInputDate="InputDate";
static const auto vtInputDateTime="InputDateTime";
static const auto vtInputDouble="InputDouble";
static const auto vtInputEmail="InputEmail";
static const auto vtInputInt="InputInt";
static const auto vtInputMd5="InputMd5";
static const auto vtInputNumber="InputNumber";
static const auto vtInputPhone="InputPhone";
static const auto vtInputRadioBox="InputRadioBox";
static const auto vtInputText="InputText";
static const auto vtInputTextArea="InputTextArea";
static const auto vtInputTextMask="InputTextMask";
static const auto vtInputTextSearch="InputTextSearch";
static const auto vtInputTime="InputTime";
static const auto vtInputUuid="InputUuid";
static const auto vtInputWeight="InputWeight";
static const auto vtInputPassWord="InputPassWord";
