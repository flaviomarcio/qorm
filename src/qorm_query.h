#pragma once

#include "./qorm_sql_suitable_builder.h"
#include <QSqlRecord>

namespace QOrm {
class Model;

//!
//! \brief The Query class
//!
class Q_ORM_EXPORT Query : public ObjectDb
{
public:

    //!
    //! \brief Query
    //! \param parent
    //!
    Q_INVOKABLE explicit Query(QObject *parent = nullptr);

    //!
    //! \brief Query
    //! \param db
    //! \param parent
    //!
    explicit Query(const QSqlDatabase &db, QObject *parent=nullptr);
    ~Query();

    //!
    //! \brief lastError
    //! \return
    //!
    virtual QSqlError &lastError() const;

    //!
    //! \brief lr
    //! \return
    //!
    ResultValue &lr();

    //!
    //! \brief clear
    //!
    virtual void clear() const;

    //!
    //! \brief close
    //!
    virtual void close() const;

    //!
    //! \brief setModel
    //! \param metaObject
    //! \return
    //!
    virtual bool setModel(QMetaObject&metaObject);

    //!
    //! \brief builder
    //! \return
    //!
    virtual SqlSuitableBuilder&builder();

    //!
    //! \brief b
    //! \return
    //!
    virtual SqlSuitableBuilder&b();

    //!
    //! \brief sqlRecord
    //! \return
    //!
    virtual QSqlRecord &sqlRecord();

    //!
    //! \brief makeRecordList
    //! \return
    //!
    virtual QVariantList makeRecordList();

    //!
    //! \brief makeRecordList
    //! \param metaObject
    //! \return
    //!
    virtual QVariantList makeRecordList(const QMetaObject&metaObject);

    //!
    //! \brief makeRecordList
    //! \param modelInfo
    //! \return
    //!
    virtual QVariantList makeRecordList(const QOrm::ModelInfo&modelInfo);

    //!
    //! \brief makeRecord
    //! \return
    //!
    virtual QVariantHash makeRecord() const;

    //!
    //! \brief makeRecord
    //! \param metaObject
    //! \return
    //!
    virtual QVariantHash makeRecord(const QMetaObject&metaObject) const;

    //!
    //! \brief makeRecord
    //! \param modelInfo
    //! \return
    //!
    virtual QVariantHash makeRecord(const QOrm::ModelInfo&modelInfo) const;

    //!
    //! \brief modelRead
    //! \param model
    //! \return
    //!
    virtual bool modelRead(QOrm::Model *model) const;

    //!
    //! \brief modelRead
    //! \param model
    //! \return
    //!
    template<class T>
    bool modelRead(T&model){
        return this->modelRead(&model);
    }

    //!
    //! \brief close
    //!
    virtual Query&close();

    //!
    //! \brief next
    //! \return
    //!
    virtual bool next() const;

    //!
    //! \brief prepare
    //! \return
    //!
    virtual bool prepare() const;

    //!
    //! \brief prepareCache
    //! \return
    //!
    virtual bool prepareCache() const;

    //!
    //! \brief exec
    //! \return
    //!
    virtual bool exec();

    //!
    //! \brief exec
    //! \param command
    //! \return
    //!
    virtual bool exec(const QVariant&command);

    //!
    //! \brief execBatch
    //! \return
    //!
    virtual bool execBatch();

    //!
    //! \brief execBatch
    //! \param mode
    //! \return
    //!
    virtual bool execBatch(int mode);

    //!
    //! \brief bindValue
    //! \param placeholder
    //! \param val
    //! \param type
    //!
    virtual void bindValue(const QString& placeholder, const QVariant& val, QSql::ParamType type = QSql::In);

    //!
    //! \brief bindValue
    //! \param pos
    //! \param val
    //! \param type
    //!
    virtual void bindValue(int pos, const QVariant& val, QSql::ParamType type = QSql::In);

    //!
    //! \brief addBindValue
    //! \param val
    //! \param type
    //!
    virtual void addBindValue(const QVariant& val, QSql::ParamType type = QSql::In);
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    //!
    //! \brief boundValues
    //! \return
    //!
    QMap<QString, QVariant> boundValues() const;
#else
    //!
    //! \brief boundValues
    //! \return
    //!
    virtual QVariantList boundValues() const;
#endif

    //!
    //! \brief executedQuery
    //! \return
    //!
    virtual QString executedQuery() const;

    //!
    //! \brief value
    //! \param column
    //! \return
    //!
    virtual QVariant value(const int&column) const;

    //!
    //! \brief value
    //! \param columnName
    //! \return
    //!
    virtual QVariant value(QString &columnName) const;

    //!
    //! \brief lastInsertId
    //! \return
    //!
    virtual QVariant lastInsertId() const;

    //!
    //! \brief parser
    //! \return
    //!
    virtual QOrm::SqlSuitableKeyWord&parser();

public:
    void*p=nullptr;
};

}

