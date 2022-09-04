#pragma once

#include "./qorm_query.h"
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
    //!
    //! \brief ModelDao
    //! \param parent
    //!
    Q_INVOKABLE explicit ModelDao(QObject *parent = nullptr) : PrivateQOrm::ModelDao(parent) {}

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

    QVariantHash toPrepareSearch(const QOrm::ModelInfo &modelRef, const QVariant &vModel) const
    {
        return PrivateQOrm::ModelDao::toPrepareSearch(modelRef, vModel);
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
        QVariant value;
        if (v.isValid()) {
            SearchParameters vv{this->variantToParameters(v)};
            vv += this->p_modelInfo.tableDeactivateField();
            value = vv.buildVariant();
        }
        if (value.isValid())
            strategy.where().condition(value);
        if (!query.exec())
            return this->lr(query.lr());

        if (!query.next())
            return this->lr() = false;

        return this->lr(query.makeRecord(this->p_modelInfo));
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
        strategy.fieldsFrom(p_modelInfo);

        auto vHash=v.toHash();

        QHashIterator<QString, QVariant> i(this->p_modelInfo.tableDeactivateField());
        while (i.hasNext()) {
            i.next();
            if(vHash.contains(i.key())) continue;
            vHash.insert(i.key(), i.value());
        }

        auto vvm=this->p_modelInfo.parserVVM(vHash);
        if (!vvm.isEmpty())
            strategy.where(vvm);

        for (const auto &v : this->p_modelInfo.tableOrderByField())
            strategy.orderby().f(v);

        if (!query.exec())
            return this->lr(query.lr());

        return this->lr(query.makeRecordList());
    }

    //!
    //! \brief recordMap
    //! \return
    //!
    auto &recordMap() { return this->recordMap({}); }

    //!
    //! \brief recordMap
    //! \param v
    //! \return
    //!
    auto &recordMap(const QVariant &v)
    {
        auto tablePk = this->p_modelInfo.tablePk();
        if (tablePk.isEmpty())
            return this->lr().clear() = true;

        Query query{this};
        auto &strategy = query.builder().select();
        strategy.fieldsFrom(p_modelInfo);
        QVariant value(this->variantToParameters(v));
        if (value.isValid()) {
            strategy.where().condition(value);
        }

        QHashIterator<QString, QVariant> i(this->p_modelInfo.tableDeactivateField());
        while (i.hasNext()) {
            i.next();
            strategy.where().notEqual(i.key(), i.value());
        }

        if (!query.exec())
            return this->lr(query.lr());

        QVariantHash vHash;
        for (auto &v : query.makeRecordList()) {
            QVariantHash vMap(v.toHash());
            QStringList key;
            QObject model;
            for (auto &pkField : tablePk) {
                const auto property = p_modelInfo.propertyByFieldName(pkField);
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
    auto &exists() { return this->exists(QVariant()); }

    //!
    //! \brief exists
    //! \param model
    //! \return
    //!
    auto &exists(T &model)
    {
        auto vHash = model.toPKValues();
        if (vHash.isEmpty())
            vHash = model.toFKValues();
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

        auto vHash=v.toHash();
        QHashIterator<QString, QVariant> i(this->p_modelInfo.tableDeactivateField());
        while (i.hasNext()) {
            i.next();
            strategy.where().notEqual(i.key(), i.value());
        }

        auto vvm = this->p_modelInfo.parserVVM(vHash);

        if (!vvm.isEmpty())
            strategy.where().condition(vvm);

        if (!query.exec())
            return this->lr(query.lr());

        if (!query.next())
            return this->lr() = false;

        return this->lr() = true;
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
            vHash = model.toFKValues();
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
        if (value.isValid()) {
            strategy.where().condition(value);
        }
        QHashIterator<QString, QVariant> i(this->p_modelInfo.tableDeactivateField());
        while (i.hasNext()) {
            i.next();
            strategy.where().notEqual(i.key(), i.value());
        }

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
        auto vHash = model.toPKValues();
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

        if (!query.next())
            return this->lr() = false;

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

        if (!query.next())
            return this->lr() = false;

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
    const QOrm::ModelInfo &p_modelInfo = QOrm::ModelInfo::from(T::staticMetaObject);
};

} // namespace QOrm
