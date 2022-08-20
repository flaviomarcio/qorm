#pragma once

#include "./private/p_qorm_model_report.h"
#include "./qorm_model_dao.h"
#include "./qorm_model.h"

namespace QOrm {

//!
//! \brief The ModelReport class
//!
template<class T = Model>
class ModelReport : public PrivateQOrm::ModelReportBase
{
public:
    //!
    //! \brief ModelReport
    //! \param parent
    //!
    Q_INVOKABLE explicit ModelReport(QObject *parent = nullptr)
        : PrivateQOrm::ModelReportBase(parent), p_dao{this}, p_model{this}
    {
        this->init();
    }

    //!
    //! \brief ModelReport
    //! \param reportBody
    //! \param parent
    //!
    explicit ModelReport(const QVariant &reportBody, QObject *parent = nullptr)
        : PrivateQOrm::ModelReportBase(reportBody, parent), p_dao{this}, p_model{this}
    {
        this->init();
    }

    //!
    //! \brief reportfy
    //! \return
    //!
    virtual ResultValue &reportfy()
    {
        this->p_dto.type(this->type());
        return PrivateQOrm::ModelReportBase::reportfy();
    }

private:
    ModelDao<T> p_dao;
    T p_model;
    ModelDto &p_dto = this->dto();

    //!
    //! \brief init
    //!
    void init()
    {
        this->dto().initDescriptors(&p_model);
        const auto &modelInfo = this->modelInfo();
        this->reportName(modelInfo.modelName()).reportDescription(modelInfo.modelDescription());
    }

protected:
    //!
    //! \brief modelInfo
    //! \return
    //!
    virtual const ModelInfo &modelInfo()
    {
        const auto &modelInfo = this->p_model.modelInfo();
        return modelInfo;
    }

    //!
    //! \brief search
    //! \param model
    //! \return
    //!
    virtual ResultValue &search(const T &model)
    {
        auto value = model.toPKValues();
        VariantUtil util;
        if (!this->options().searchOnEmptyFilter() && util.vIsEmpty(value))
            return this->lr();

        if (!this->p_dao.recordList(value))
            return this->lr(this->p_dao.lr());

        auto vList = this->p_dao.lr().resultList();
        vList = this->p_model.toList(vList);
        return this->lr(vList);
    }

    //!
    //! \brief search
    //! \param value
    //! \return
    //!
    virtual ResultValue &search(const QVariant &value)
    {
        VariantUtil util;
        if (!this->options().searchOnEmptyFilter() && util.vIsEmpty(value))
            return this->lr();

        if (!this->p_dao.recordList(value))
            return this->lr(this->p_dao.lr());

        auto vList = this->p_dao.lr().resultList();
        vList = this->p_model.toList(vList);
        return this->lr(vList);
    }

    //!
    //! \brief search
    //! \return
    //!
    virtual ResultValue &search()
    {
        static const auto __operator=QStringLiteral("operator");
        static const auto __format=QStringLiteral("format");
        static const auto __arg1=QStringLiteral("%1");

        T model(this->source());
        auto mapSource = this->source().toHash();
        SearchParameters map;
        if (!mapSource.isEmpty()) {
            const ModelInfo &modelInfo = this->modelInfo();
            const auto &propertyShortVsTable = modelInfo.propertyShortVsTable();
            QHashIterator<QString, QVariant> i(model.toHash());
            while (i.hasNext()) {
                i.next();
                auto header = &this->p_dto.headers().item(i.key());
                if (header) {
                    const auto vHash = header->filtrableStrategy().toHash();
                    const auto keywordOperator = vHash.value(__operator);
                    QString format = vHash.value(__format).toString().trimmed();
                    QVariant v_value;
                    if (format.contains(__arg1))
                        v_value = format.arg(i.value().toString());
                    else
                        v_value = i.value();
                    if (mapSource.contains(i.key())) {
                        auto v_key = propertyShortVsTable.value(i.key());
                        auto a = SqlParserItem::createObject(v_key);
                        auto b = SqlParserItem::createValue(v_value);
                        map.insert(a, b, keywordOperator);
                    }
                }
            }
        }
        auto v = map.buildVariant();
        return this->search(v);
    }

    //!
    //! \brief onBefore
    //! \param method
    //! \return
    //!
    auto &onBefore(ModelActionMethod method)
    {
        auto obj = PrivateQOrm::ModelReportBase::onBefore(method);
        return *(dynamic_cast<ModelReport<T> *>(obj));
    }

    //!
    //! \brief onSuccess
    //! \param method
    //! \return
    //!
    auto &onSuccess(ModelActionMethod method)
    {
        auto obj = PrivateQOrm::ModelReportBase::onSuccess(method);
        return *(dynamic_cast<ModelReport<T> *>(obj));
    }

    //!
    //! \brief onFailed
    //! \param method
    //! \return
    //!
    auto &onFailed(ModelActionMethod method)
    {
        auto obj = PrivateQOrm::ModelReportBase::onFailed(method);
        return *(dynamic_cast<ModelReport<T> *>(obj));
    }
};

} // namespace QOrm
