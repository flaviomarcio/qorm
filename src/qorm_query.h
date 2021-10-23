#pragma once

#include "./qorm_sql_suitable_builder.h"
#include <QSqlQuery>

namespace QOrm {
class Model;
class Q_ORM_EXPORT Query : public ObjectDb
{
public:
    Q_INVOKABLE explicit Query(QObject *parent = nullptr);
    Q_INVOKABLE explicit Query(const QSqlDatabase &db, QObject *parent=nullptr);
    Q_INVOKABLE ~Query();

    /**
     * @brief lastError
     * @return
     */
    virtual QSqlError &lastError() const;

    /**
     * @brief lr
     * @return
     */
    ResultValue &lr();

    /**
     * @brief clear
     */
    virtual void clear() const;

    /**
     * @brief close
     */
    virtual void close() const;

    /**
     * @brief setModel
     * @param metaObject
     * @return
     */
    virtual bool setModel(QMetaObject&metaObject);

    /**
     * @brief builder
     * @return
     */
    virtual SqlSuitableBuilder&builder();

    /**
     * @brief builder
     * @return
     */
    virtual SqlSuitableBuilder&b();

    /**
     * @brief sqlQuery
     * @return
     */
    virtual QSqlQuery &sqlQuery();

    /**
     * @brief sqlRecord
     * @return
     */
    virtual QSqlRecord &sqlRecord();

    /**
     * @brief makeRecordList
     * @return
     */
    virtual QVariantList makeRecordList();
    virtual QVariantList makeRecordList(const QMetaObject&metaObject);
    virtual QVariantList makeRecordList(const QOrm::ModelInfo&modelInfo);
    virtual QVariantHash makeRecord() const;
    virtual QVariantHash makeRecord(const QMetaObject&metaObject) const;
    virtual QVariantHash makeRecord(const QOrm::ModelInfo&modelInfo) const;

    /**
     * @brief modelSet
     * @param model
     * @return
     */
    virtual bool modelRead(QOrm::Model *model) const;

    template<class T>
    bool modelRead(T&model){
        return this->modelRead(&model);
    }

    /**
     * @brief close
     */
    virtual void close();

    /**
     * @brief next
     * @return
     */
    virtual bool next() const;

    /**
     * @brief prepare
     * @return
     */
    virtual bool prepare() const;

    /**
     * @brief prepareCache
     * @return
     */
    virtual bool prepareCache() const;

    /**
     * @brief exec
     * @return
     */
    virtual bool exec();

    /**
     * @brief exec
     * @param command
     * @return
     */
    virtual bool exec(const QVariant&command);

    /**
     * @brief execBatch
     * @param mode
     * @return
     */
    virtual bool execBatch(QSqlQuery::BatchExecutionMode mode = QSqlQuery::ValuesAsRows);

    /**
     * @brief bindValue
     * @param placeholder
     * @param val
     * @param type
     */
    virtual void bindValue(const QString& placeholder, const QVariant& val, QSql::ParamType type = QSql::In);

    /**
     * @brief bindValue
     * @param pos
     * @param val
     * @param type
     */
    virtual void bindValue(int pos, const QVariant& val, QSql::ParamType type = QSql::In);

    /**
     * @brief addBindValue
     * @param val
     * @param type
     */
    virtual void addBindValue(const QVariant& val, QSql::ParamType type = QSql::In);

    /**
     * @brief boundValues
     * @return
     */
    virtual QVariantMap boundValues() const;

    /**
     * @brief executedQuery
     * @return
     */
    virtual QString executedQuery() const;

    /**
     * @brief value
     * @param column
     * @return
     */
    virtual QVariant value(const int&column) const;

    /**
     * @brief value
     * @param columnName
     * @return
     */
    virtual QVariant value(QString &columnName) const;

    /**
     * @brief lastInsertId
     * @return
     */
    virtual QVariant lastInsertId() const;

    /**
     * @brief parser
     * @return
     */
    virtual QOrm::SqlSuitableKeyWord&parser();

private:
    void*p=nullptr;
};

}

