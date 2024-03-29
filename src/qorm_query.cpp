#include "./qorm_query.h"
#include "./private/p_qorm_query.h"
#include "./qorm_model.h"
#include "./qorm_const.h"
#include "./qorm_macro.h"
#include <QCryptographicHash>

namespace QOrm {

Query::Query(QObject *parent) : ObjectDb{parent}, p{new QOrm::QueryPvt{this, {}}}
{
}

Query::Query(const QSqlDatabase &db, QObject *parent) : ObjectDb{parent}, p{new QOrm::QueryPvt{this, db}}
{
    if (!db.isValid() || !db.isOpen()) {
#if Q_ORM_LOG
        oWarning() << QStringLiteral("connection is not valid");
#endif
    }
}

ResultValue &Query::lr() const
{
    auto &lr=Object::lr();
    auto code = p->sqlError.nativeErrorCode().toUtf8().trimmed();
    if(!code.isEmpty()){
        auto message = p->sqlError.text().toUtf8().trimmed();
        if(code.isEmpty() && message.isEmpty())
            return lr.clear();
        lr.setCritical(code, message);
    }
    return lr;
}

const Query &Query::clear()
{
    p->clear();
    return *this;
}

const Query &Query::close()
{
    p->close();
    return *this;
}

QSqlQuery &Query::sqlQuery()const
{
    return p->sqlQuery;
}

QSqlRecord &Query::sqlRecord()
{
    if (p->sqlRecord.isEmpty())
        p->sqlRecord = p->sqlQuery.record();
    return p->sqlRecord;
}

Query &Query::setModel(QMetaObject &metaObject)
{
    p->makeModelMetaObject(metaObject);
    return *this;
}

SqlSuitableBuilder &Query::builder()
{
    return p->sqlBuilder;
}

QVariantList Query::makeRecordList()
{
    QVariantList records;

    while (p->next()) {
        QVariantHash record;
        auto &sqlRecord = this->sqlRecord();
        for (int col = 0; col < sqlRecord.count(); ++col) {
            const auto v = sqlRecord.value(col);
            record.insert(sqlRecord.fieldName(col),v);
        }
        records.append(record);
    }
    return records;
}

QVariantList Query::makeRecordList(const QMetaObject &metaObject)
{
    const auto &modelInfo = QOrm::ModelInfo::from(metaObject.className());
    return this->makeRecordList(modelInfo);
}

QVariantList Query::makeRecordList(const ModelInfo &modelInfo)
{
    const auto &metaObject = modelInfo.staticMetaObjectModel();
    QVariantList recordList;
    if (metaObject.methodCount() == 0) {
        while (this->next()) {
            QVariantHash record;
            auto &sqlRecord = this->sqlRecord();
            for (int col = 0; col < sqlRecord.count(); ++col) {
                const auto v = sqlRecord.value(col);
                record.insert(sqlRecord.fieldName(col),v);
            }
            recordList.append(record);
        }
        return recordList;
    }

    QHash<int, QByteArray> recordsIndex;
    auto propertyTableList = modelInfo.propertyShortVsTable();

    while (this->next()) {
        QVariantHash record;
        auto &sqlRecord = p->sqlRecord;

        if (recordsIndex.isEmpty()) {
            for (int col = 0; col < metaObject.propertyCount(); ++col) {
                auto property = metaObject.property(col);
                auto propertyName = QByteArray{property.name()}.toLower().trimmed();
                auto fieldName = propertyTableList.value(propertyName).trimmed();
                if (fieldName.isEmpty())
                    continue;
                auto index = sqlRecord.indexOf(fieldName);
                if (index < 0)
                    continue;
                recordsIndex.insert(index, property.name());
            }
        }

        QHashIterator<int, QByteArray> i(recordsIndex);
        while (i.hasNext()) {
            i.next();
            const auto v = sqlRecord.value(i.key());
            record.insert(i.value(), v);
        }
        recordList.append(record);
    }
    return recordList;
}

QVariantHash Query::makeRecord() const
{
    if (!p->initNext())
        return {};

    return p->makeRecord();
}

QVariantHash Query::makeRecord(const QMetaObject &metaObject) const
{
    const auto &modelInfo = QOrm::ModelInfo::from(metaObject.className());
    return this->makeRecord(modelInfo);
}

QVariantHash Query::makeRecord(const ModelInfo &modelInfo) const
{
    if (!p->initNext())
        return {};

    auto metaObject = modelInfo.staticMetaObjectModel();
    auto propertyTableList = modelInfo.propertyShortVsTable();
    if (metaObject.methodCount() == 0) {
        QVariantHash record;
        for (int col = 0; col < p->sqlRecord.count(); ++col)
            record.insert(p->sqlRecord.fieldName(col), p->sqlRecord.value(col));
        return record;
    }

    QList<int> recordsIndex;
    for (int col = 0; col < metaObject.propertyCount(); ++col) {
        auto property = metaObject.property(col);
        QString propertyName = QByteArray{property.name()}.toLower().trimmed();
        propertyName = propertyTableList.value(propertyName).trimmed();
        if (!propertyName.isEmpty()) {
            auto index = p->sqlRecord.indexOf(propertyName);
            if (index >= 0)
                recordsIndex.append(index);
        }
    }

    QVariantHash record;
    for (auto &col : recordsIndex)
        record.insert(p->sqlRecord.fieldName(col), p->sqlRecord.value(col));
    return record;
}

QVariantHash Query::makeRecordSplitted() const
{
    if (!p->initNext())
        return {};

    return p->makeRecord(true);
}

bool Query::modelRead(QOrm::Model *model) const
{
    bool __return = false;
    auto &metaObject = *model->metaObject();
    for (int col = 0; col < metaObject.propertyCount(); ++col) {
        auto property = metaObject.property(col);
        auto propertyName = QByteArray{property.name()}.toLower().trimmed();
        if (!p->sqlQueryFields.contains(propertyName))
            continue;
        auto index = p->sqlRecord.indexOf(propertyName);
        if (index < 0)
            continue;
        auto value = p->sqlRecord.value(index);
        if (!model->setProperty(property, value))
            return false;
        __return = true;
    }
    return __return;
}

bool Query::next() const
{
    if (!p->next())
        return false;

    if (p->sqlRecord.isEmpty())
        p->sqlRecord = p->sqlQuery.record();
    return true;
}

bool Query::prepare() const
{

    return p->prepareExec();
}

bool Query::prepareCache() const
{

    return p->prepareExecCache();
}

bool Query::exec()
{
    if (!p->prepareExec())
        return false;
    return p->exec({});
}

bool Query::exec(const QVariant &command)
{
    return p->exec(command);
}

bool Query::execBatch()
{
    this->prepare();
    return p->sqlQuery.execBatch(QSqlQuery::ValuesAsRows);
}

bool Query::execBatch(int mode)
{
    this->prepare();
    return p->sqlQuery.execBatch(QSqlQuery::BatchExecutionMode(mode));
}

Query &Query::bindValue(const QString &placeholder, const QVariant &val, QSql::ParamType type)
{
    this->prepare();
    p->sqlQuery.bindValue(placeholder, val, type);
    return *this;
}

Query &Query::bindValue(int pos, const QVariant &val, QSql::ParamType type)
{
    this->prepare();
    p->sqlQuery.bindValue(pos, val, type);
    return *this;
}

Query &Query::addBindValue(const QVariant &val, QSql::ParamType type)
{
    this->prepare();
    p->sqlQuery.addBindValue(val, type);
    return *this;
}

QVariantList Query::boundValues() const
{
    return p->sqlQuery.boundValues();
}

QString Query::executedQuery() const
{
    return p->sqlQuery.executedQuery();
}

QVariant Query::value(const int &column) const
{
    return p->sqlQuery.value(column);
}

QVariant Query::value(QString &columnName) const
{
    auto column = p->sqlRecord.indexOf(columnName);
    return p->sqlQuery.value(column);
}

QVariant Query::lastInsertId() const
{
    return p->sqlQuery.lastInsertId();
}

SqlSuitableKeyWord &Query::parser()
{
    if (p->parser == nullptr)
        p->parser = &QOrm::SqlSuitableKeyWord::parser(this->connection());

    if (p->parser == nullptr)
        return SqlSuitableKeyWord::parser(QSqlDriver::UnknownDbms);

    return *p->parser;
}

} // namespace QOrm
