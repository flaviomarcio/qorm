#pragma once

#include "./qorm_query.h"
#include "./qorm_model.h"
#include "./private/p_qorm_model_dao.h"
#include "./private/p_qorm_sql_suitable_parser_strategy_options.h"

namespace QOrm {

typedef SqlParserConditions<SqlParserCommand> DaoConditions;

#define likeL(v)QStringLiteral("%")+v
#define likeR(v)v+QStringLiteral("%")
#define likeLR(v)QStringLiteral("%")+v+QStringLiteral("%")

#define vlikeL(v)likeL(QVariant(v).toString())
#define vlikeR(v)likeR(QVariant(v).toString())
#define vlikeLR(v)likeLR(QVariant(v).toString())

#define vpsFiltrableStrategyLikeLR \
QVariantHash{QStringLiteral("operator"),QOrm::koLike}, {QStringLiteral("format"),QStringLiteral("%%1cS%")}}

#define vpsFiltrableStrategyLikeL \
QVariantHash{{QStringLiteral("operator"),QOrm::koLike}, {QStringLiteral("format"),QStringLiteral("%%1")}}

#define vpsFiltrableStrategyLikeR \
QVariantHash{{QStringLiteral("operator"),QOrm::koLike}, {QStringLiteral("format"),QStringLiteral("%1%")}}


//!
//! \brief The ModelDao class
//!
template<class T>
class ModelDao : public PrivateQOrm::ModelDao
{
public:

    enum Action{None, Insert, Update, Upsert, Search};
    Q_ENUM(Action)


    //!
    //! \brief ModelDao
    //! \param parent
    //!
    Q_INVOKABLE explicit ModelDao(QObject *parent = nullptr) : PrivateQOrm::ModelDao(parent) {}

    //!
    //! \brief deactivateField
    //! \return
    //!
    bool deactivateField()
    {
        return _deactivateField;
    }

    //!
    //! \brief setDeactivateField
    //! \param value
    //!
    void setDeactivateField(bool value)
    {
        _deactivateField=value;
    }

    //!
    //! \brief variantToParameters
    //! \param value
    //! \return
    //!
    QVariant variantToParameters(const QVariant &value) const
    {
        return PrivateQOrm::ModelDao::variantToParameters(this->p_modelInfo, value);
    }

    //!
    //! \brief toPrepareForeignWrapper
    //! \param vModelFK
    //! \param vModelPK
    //! \return
    //!
    QVariantList toPrepareForeignWrapper(const QVariant &vModelFK, const QVariant &vModelPK) const
    {
        return PrivateQOrm::ModelDao::toPrepareForeignWrapper(this->p_modelInfo, vModelFK, vModelPK);
    }

    //!
    //! \brief variantToParameters
    //! \param value
    //! \return
    //!
    QVariantHash toPrepareForeign(const QVariant &vModelFK) const
    {
        return PrivateQOrm::ModelDao::toPrepareForeign(this->p_modelInfo, vModelFK);
    }

    //!
    //! \brief toPrepareSearch
    //! \param modelRef
    //! \param vModel
    //! \return
    //!
    QVariantHash toPrepareSearch(const QOrm::ModelInfo &modelRef, const QVariant &vModel) const
    {
        return PrivateQOrm::ModelDao::toPrepareSearch(modelRef, vModel);
    }

    //!
    //! \brief actionExec
    //! \param value
    //! \param newPropertyModel
    //! \return
    //!
    auto &actionExec(const QVariant &value, const T &newPropertyModel)
    {
        auto newPropertyValues=newPropertyModel.toHashModel(false);
        return this->actionExec(value, newPropertyValues);
    }
    auto &actionExec(const ResultValue &value, const T &newPropertyModel)
    {
        auto newPropertyValues=newPropertyModel.toHashModel(false);
        return this->actionExec(value.resultVariant(), newPropertyValues);
    }
    auto &actionExec(const ResultValue &value, QVariantHash &newPropertyValues)
    {
        return this->actionExec(value.resultVariant(), newPropertyValues);
    }
    auto &actionExec(const QVariant &value, QVariantHash &newPropertyValues)
    {
        if (value.isNull() || !value.isValid())
            return this->lr();

        QVariantList list;
        if(!newPropertyValues.isEmpty()){
            switch (value.typeId()) {
            case QMetaType::QVariantList:
                list=value.toList();
                break;
            case QMetaType::QVariantHash:
            case QMetaType::QVariantMap:
                list.append(value);
                break;
            default:
                const T *model=value.value<T*>();
                if(model)
                    list.append(model->toHashModel());
                break;
            }
            //merge new values values
            for (auto &v : list) {
                T model{v};
                model.mergeFrom(newPropertyValues);
                v=model.toHashModel();
            }
        }

        if(list.isEmpty())
            return this->lr();

        switch (this->_action){
        case Insert:
            return this->insert(list);
        case Update:
            return this->update(list);
        case Upsert:
            return this->upsert(list);
        case Search:
            return this->recordList(list);
        default:
            return this->lr();
        }
    }

    //!
    //! \brief record
    //! \return
    //!
    auto &record() { return this->record(QVariant{}); }

    //!
    //! \brief record
    //! \param v
    //! \return
    //!
    auto &record(const QVariant &v)
    {
        Query query{this};
        auto &strategy = query.builder().select();
        strategy.limit(1).fieldsFrom(p_modelInfo);
        if (v.isValid()) {
            QVariant value;
            SearchParameters vv{this->variantToParameters(v)};
            value = vv.buildVariant();
            if (value.isValid())
                strategy.where().condition(value);
        }

        if(this->_deactivateField){
            auto vvm=this->p_modelInfo.parserVVM(this->p_modelInfo.propertyActivateField());
            strategy.where(vvm);
        }

        if (!query.exec())
            return this->lr(query.lr());

        if (!query.next())
            return this->lr() = false;

        return this->lr(query.makeRecord(this->p_modelInfo));
    }

    //!
    //! \brief recordCount
    //! \return
    //!
    auto &recordCount() { return this->recordCount({}); }

    //!
    //! \brief recordCount
    //! \param v
    //! \return
    //!
    auto &recordCount(const QVariant &v){
        Query query{this};
        auto &strategy = query.builder().select();
        strategy
            .fields().count();
        strategy
            .from(p_modelInfo);
        if (v.isValid()) {
            QVariant value;
            SearchParameters vv{this->variantToParameters(v)};
            value = vv.buildVariant();
            if (value.isValid())
                strategy.where().condition(value);
        }

        if(this->_deactivateField){
            auto vvm=this->p_modelInfo.parserVVM(this->p_modelInfo.propertyActivateField());
            strategy.where(vvm);
        }

        if (!query.exec())
            return this->lr(query.lr());

        if (!query.next())
            return this->lr() = false;

        return this->lr(query.makeRecord());
    }

    //!
    //! \brief recordList
    //! \return
    //!
    auto &recordList() { return this->recordList({}); }

    //!
    //! \brief recordList
    //! \param v
    //! \return
    //!
    auto &recordList(const QVariant &v)
    {
        Query query{this};
        auto &strategy = query.builder().select();

        auto &rInfo=this->lr().resultInfo();
        if(rInfo.perPage()>0)
            strategy.offset(rInfo.perPage(),rInfo.offSetRecords());

        strategy.fieldsFrom(p_modelInfo);

        auto vvm=this->p_modelInfo.parserVVM(v);
        if (!vvm.isEmpty())
            strategy.where(vvm);

        if(this->_deactivateField){
            auto vvm=this->p_modelInfo.parserVVM(this->p_modelInfo.propertyActivateField());
            strategy.where(vvm);
        }

        for (const auto &v : this->p_modelInfo.tableOrderByField())
            strategy.orderby().f(v);

        if (!query.exec())
            return this->lr(query.lr());

        return this->lr(query.makeRecordList(this->p_modelInfo));
    }

    //!
    //! \brief recordHash
    //! \return
    //!
    auto &recordHash() { return this->recordHash({}); }

    //!
    //! \brief recordHash
    //! \param v
    //! \return
    //!
    auto &recordHash(const QVariant &v)
    {
        auto tablePk = this->p_modelInfo.tablePk();
        if (tablePk.isEmpty())
            return this->lr().clear() = true;

        Query query{this};
        auto &strategy = query.builder().select();
        strategy.fieldsFrom(p_modelInfo);
        QVariant value(this->variantToParameters(v));
        if (value.isValid())
            strategy.where(value);

        if(this->_deactivateField){
            auto vvm=this->p_modelInfo.parserVVM(this->p_modelInfo.propertyActivateField());
            strategy.where(vvm);
        }

        if (!query.exec())
            return this->lr(query.lr());

        QVariantHash vHash;
        for (auto &v : query.makeRecordList(this->p_modelInfo)) {
            QVariantHash vMap(v.toHash());
            QStringList key;
            QObject model;
            auto &propertyByName=p_modelInfo.propertyByName();
            for (auto &pkField : tablePk) {
                const auto property = propertyByName.value(pkField);
                const auto vMapValue = vMap.value(pkField);
                const auto vType = property.typeId();
                if (vType == vMapValue.Invalid || !vMapValue.isValid() || vMapValue.isNull())
                    key.append("");
                else if (vType == vMapValue.Uuid)
                    key.append(vMapValue.toUuid().toString());
                else if (vType == vMapValue.Url)
                    key.append(vMapValue.toUrl().toString());
                else
                    key.append(vMapValue.toString());
            }
            vHash.insert(key.join('.'), v);
        }
        return this->lr(QVariant(vHash));
    }

    //!
    //! \brief exists
    //! \return
    //!
    auto &exists() { return this->exists(QVariant{}); }

    //!
    //! \brief exists
    //! \param model
    //! \return
    //!
    auto &exists(T &model)
    {
        auto vHash = model.toPKValues();
        return this->exists(vHash);
    }

    //!
    //! \brief exists
    //! \param v
    //! \return
    //!
    auto &exists(const QVariant &v)
    {
        QOrm::Query query{this};
        auto &strategy = query.builder().select();
        strategy.fromExists(p_modelInfo);

        auto vvm = this->p_modelInfo.parserVVM(v);
        if (!vvm.isEmpty())
            strategy.where(vvm);

        if(this->_deactivateField){
            auto vvm=this->p_modelInfo.parserVVM(this->p_modelInfo.propertyActivateField());
            strategy.where(vvm);
        }

        if (!query.exec())
            return this->lr(query.lr());

        if (!query.next())
            return this->lr() = false;

        return this->lr() = true;
    }

    //!
    //! \brief insert
    //! \return
    //!
    auto &insert()
    {
        this->_action=Insert;
        return *static_cast<ModelDao<T>*>(this);
    }

    //!
    //! \brief insert
    //! \param value
    //! \return
    //!
    auto &insert(T &model) { return this->insert(model.toHashModel()); }

    //!
    //! \brief insert
    //! \param value
    //! \return
    //!
    auto &insert(const QVariant &value)
    {
        if (value.isNull() || !value.isValid())
            return this->lr() = false;

        QVariantList list;
        switch (value.typeId()) {
        case QMetaType::QVariantList: {
            for (auto &v : value.toList()) {
                T model{v};
                model.autoMakeUuid();
                model.autoSet();
                list.append(model.toHashModel());
            }
            break;
        }
        default:
            T model{value};
            model.autoMakeUuid();
            model.autoSet();
            list.append(model.toHashModel());
        }

        for (auto &v : list) {
            QOrm::Query query{this};
            query.builder().insert().destine(p_modelInfo).values(v);

            if (!query.exec())
                return this->lr(query.lr());
        }
        return this->lr() = true;
    }

    //!
    //! \brief update
    //! \return
    //!
    auto &update()
    {
        this->_action=Update;
        return *static_cast<ModelDao<T>*>(this);
    }

    //!
    //! \brief update
    //! \param value
    //! \return
    //!
    auto &update(T &value)
    {
        Q_UNUSED(value)
        return this->update(value.toHashModel());
    }

    //!
    //! \brief update
    //! \param value
    //! \return
    //!
    auto &update(const QVariant &value)
    {
        if (value.isNull() || !value.isValid())
            return this->lr() = false;
        QVariantList list;
        switch (value.typeId()) {
        case QMetaType::QVariantList: {
            for (auto &v : value.toList()) {
                T model{v};
                list.append(model.toHashModel());
            }
            break;
        }
        default:
            list.append(value);
        }
        for (auto &v : list) {
            QOrm::Query query{this};
            query.builder().update().destine(p_modelInfo).values(v);

            if (!query.exec())
                return this->lr(query.lr());
        }
        if(list.isEmpty())
            return this->lr();

        return this->lr(list.size()==1?list.first():list) = true;
    }

    //!
    //! \brief upsert
    //! \return
    //!
    auto &upsert()
    {
        this->_action=Upsert;
        return *static_cast<ModelDao<T>*>(this);
    }

    //!
    //! \brief upsert
    //! \param value
    //! \return
    //!
    auto &upsert(T &value)
    {
        auto vHash = value.toHashModel();
        if (this->upsert(vHash)) {
            value.readFrom(this->lr());
            return this->lr() = true;
        }
        return this->lr() = false;
    }

    //!
    //! \brief upsert
    //! \param value
    //! \return
    //!
    auto &upsert(const QVariant &value)
    {
        if (value.isNull() || !value.isValid())
            return this->lr() = false;
        QVariantList list;
        switch (value.typeId()) {
        case QMetaType::QVariantList: {
            for (auto &v : value.toList()) {
                T model{v};
                model.autoMakeUuid();
                model.autoSet();
                list.append(model.toHashModel());
            }
            break;
        }
        default:
            T model{value};
            model.autoMakeUuid();
            model.autoSet();
            list.append(model.toHashModel());
        }

        for (auto &v : list) {
            QOrm::Query query{this};
            query.builder().upsert().destine(p_modelInfo).values(v);

            if (!query.exec())
                return this->lr(query.lr());
        }

        if(list.isEmpty())
            return this->lr();

        return this->lr(list.size()==1?list.first():list) = true;
    }

    //!
    //! \brief deactivate
    //! \param value
    //! \return
    //!
    auto &deactivate(T &value)
    {
        if (!value.deactivateSetValues())
            return this->lr(value.lr()) = false;

        auto vHash = value.toHashModel();

        if (this->upsert(vHash)) {
            value.readFrom(vHash);
            return this->lr() = true;
        }

        return this->lr() = false;
    }

    //!
    //! \brief deactivate
    //! \param value
    //! \return
    //!
    auto &deactivate(const QVariant &value)
    {
        QVariantList list;
        switch (value.typeId()) {
        case QMetaType::QVariantList: {
            for (auto &v : value.toList()) {
                T model{v};
                list.append(model.toHashModel());
            }
            break;
        }
        default:
            list.append(value);
        }
        for (auto &v : list) {
            T model{this, v};
            if (!this->deactivate(model))
                return this->lr();
        }
        return this->lr() = true;
    }

    //!
    //! \brief deactivate
    //! \return
    //!
    auto &deactivate()
    {
        auto list = this->lr().resultList();
        if (list.isEmpty())
            return this->lr() = true;

        for (auto &v : list) {
            T model{this, v};
            if (!this->deactivate(model))
                return this->lr() = false;
        }
        return this->lr() = true;
    }

    //!
    //! \brief remove
    //! \param model
    //! \return
    //!
    auto &remove(T &model)
    {
        auto vHash = model.toPKValues();
        if (vHash.isEmpty())
            return this->lr() = false;
        return this->remove(vHash);
    }

    //!
    //! \brief remove
    //! \param v
    //! \return
    //!
    auto &remove(const QVariant &v)
    {
        Query query{this};
        auto &strategy = query.builder().remove();
        strategy.from(p_modelInfo);
        QVariant value(this->variantToParameters(v));
        if (value.isValid())
            strategy.where().condition(value);

        if (!query.exec())
            return this->lr(query.lr());

        return this->lr() = true;
    }

    //!
    //! \brief remove
    //! \return
    //!
    auto &remove()
    {
        auto list = this->lr().resultList();
        if (list.isEmpty())
            return this->lr() = true;

        for (auto &v : list) {
            T model{this, v};
            if (!this->remove(model))
                return this->lr() = false;
        }
        return this->lr() = true;
    }

    //!
    //! \brief reload
    //! \param model
    //! \return
    //!
    auto &reload(T &model)
    {
        auto vHash = model.toPKValuesValid();
        if (vHash.isEmpty())
            vHash = model.toFKValues();
        if (vHash.isEmpty())
            return this->lr() = false;
        return this->reload(model, vHash);
    }

    //!
    //! \brief reload
    //! \param model
    //! \param v
    //! \return
    //!
    auto &reload(T &model, const QVariant &v)
    {
        auto record = this->record(v).resultVariant();
        if (!record.isValid()) {
            model.clear();
            return this->lr() = false;
        }

        if (!model.readFrom(record))
            return this->lr() = false;

        return this->lr(record) = true;
    }

    //!
    //! \brief lock
    //! \param model
    //! \return
    //!
    auto &lock(T &model)
    {
        auto vHash = model.toPKValues();
        if (vHash.isEmpty())
            vHash = model.toFKValues();
        if (!this->lock(vHash))
            return this->lr() = false;

        if (!model.readFrom(this->lr()))
            return this->lr() = false;

        if (!model.storedProperty())
            return this->lr() = false;

        return this->lr() = true;
    }

    //!
    //! \brief lock
    //! \param model
    //! \param value
    //! \return
    //!
    auto &lock(T &model, const QVariant &value)
    {
        if (!this->lock(value))
            return this->lr() = false;

        if (!model.readFrom(this->lr()))
            return this->lr() = false;

        if (!model.storedProperty())
            return this->lr() = false;

        return this->lr() = true;
    }

    //!
    //! \brief lock
    //! \param v
    //! \return
    //!
    auto &lock(const QVariant &v)
    {
        Query query{this};
        auto &strategy = query.builder().select();
        strategy.lock().fieldsFrom(p_modelInfo);
        QVariant value(this->variantToParameters(v));
        if (value.isValid()) {
            strategy.where().condition(value);
        }
        if (!query.exec())
            return this->lr(query.lr());
        auto vList = query.makeRecordList(this->p_modelInfo);
        if (vList.isEmpty())
            return this->lr() = false;

        return this->lr((vList.size() == 1) ? vList.first() : vList);
    }

    //!
    //! \brief truncateTable
    //! \return
    //!
    auto &truncateTable()
    {
        Query query{this};
        query.builder().structure().truncateTable(this->p_modelInfo);
        if (!query.exec())
            return this->lr(query.lr());
        return this->lr() = true;
    }

    //!
    //! \brief truncateTableCascade
    //! \return
    //!
    auto &truncateTableCascade()
    {
        Query query{this};
        query.builder().structure().truncateTableCascade(this->p_modelInfo);
        if (!query.exec())
            return this->lr(query.lr());
        return this->lr() = true;
    }

    //!
    //! \brief nextVal
    //! \return
    //!
    auto &nextVal() { return this->nextVal(this->p_modelInfo.tableSequence()); }

    //!
    //! \brief nextVal
    //! \param v
    //! \return
    //!
    auto &nextVal(const QVariant &v)
    {
        Query query{this};
        query.builder().function().nextVal(v);
        if (!query.exec())
            return this->lr().clear() = query.lr();

        if (!query.next())
            return this->lr().clear() = false;

        return this->lr(query.value(0));
    }

    //!
    //! \brief values
    //!
    QVariantList values()
    {
        auto vListRecord = this->lr().resultList();
        if (vListRecord.isEmpty())
            return {};

        auto tablePk = this->p_modelInfo.tablePkField();
        if (tablePk.size() == 1)
            return this->values(tablePk.first());

        QVariantList __return;
        for (auto &v : vListRecord) {
            auto map = v.toHash();
            QVariantHash record;
            for (auto &v : tablePk) {
                auto vField = SqlParserItem::from(v);
                auto f_name = vField.name().toString();
                auto f_value = map.value(f_name);
                if (!record.contains(f_name))
                    record.insert(f_name, f_value);
            }
            __return.append(record);
        }
        return __return;
    }

    //!
    //! \brief values
    //! \param field
    //!
    QVariantList values(const SqlParserItem &field)
    {
        auto vField = SqlParserItem::from(field);
        if (!vField.isObject())
            return {};

        auto vListRecord = this->lr().resultList();
        QVariantList __return;
        for (auto &v : vListRecord) {
            auto map = v.toHash();
            ;
            auto f_value = map.value(vField.name().toString());
            if (!__return.contains(f_value))
                __return.append(f_value);
        }
        return __return;
    }

    //!
    //! \brief setRecordsValues
    //! \param field
    //! \param values
    //! \return
    //!
    auto &setRecordsValues(const SqlParserItem &field, QVariantList &values)
    {
        values = this->values(field);
        return this->lr();
    }

    //!
    //! \brief setRecords
    //! \param values
    //! \return
    //!
    auto &setRecords(QVariantList &values)
    {
        values = this->lr().resultList();
        return this->lr();
    }

    const QOrm::ModelInfo &modelInfo()const
    {
        return p_modelInfo;
    }

private:
    Action _action=Action::None;
    bool _deactivateField=true;
    const QOrm::ModelInfo &p_modelInfo = QOrm::ModelInfo::from(T::staticMetaObject);
};

} // namespace QOrm
