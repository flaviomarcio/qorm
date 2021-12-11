#include "./qorm_query.h"
#include "./qorm_model.h"
#include "./private/p_qorm_query.h"

namespace QOrm {

#define dPvt()\
    auto&p = *reinterpret_cast<QueryPvt*>(this->p)

Query::Query(QObject *parent) : ObjectDb(parent)
{
    this->p = new QOrm::QueryPvt(this, QSqlDatabase());
}

Query::Query(const QSqlDatabase &db, QObject *parent) : ObjectDb(parent)
{
    this->p = new QOrm::QueryPvt(this, db);
    if(!db.isValid() || !db.isOpen()){
#if Q_ORM_LOG
        sWarning()<<qsl("connection is not valid");
#endif
    }
}

Query::~Query()
{
    dPvt();delete&p;
}

QSqlError&Query::lastError()const
{
    dPvt();
    return p.sqlError;
}

ResultValue &Query::lr()
{
    dPvt();
    return Object::lr(p.sqlError);
}

void Query::clear()const
{
    dPvt();
    p.clear();
}

void Query::close() const
{
    dPvt();
    p.close();
}

bool Query::setModel(QMetaObject &metaObject)
{
    dPvt();
    return p.makeModelMetaObject(metaObject);
}

SqlSuitableBuilder &Query::builder()
{
    dPvt();
    return p.sqlBuilder;
}

SqlSuitableBuilder &Query::b()
{
    dPvt();
    return p.sqlBuilder;
}

QSqlRecord &Query::sqlRecord()
{
    dPvt();
    if(p.sqlRecord.isEmpty())
        p.sqlRecord=p.sqlQuery.record();
    return p.sqlRecord;
}

QVariantList Query::makeRecordList()
{
    QVariantList records;
    dPvt();
    while(p.next()){
        QVariantHash record;
        auto&sqlRecord=this->sqlRecord();
        for (int col = 0; col < sqlRecord.count(); ++col){
            const auto v=sqlRecord.value(col);
            record[sqlRecord.fieldName(col)]=v;
        }
        records<<record;
    }
    return records;
}

QVariantList Query::makeRecordList(const QMetaObject &metaObject)
{
    const auto&modelInfo=QOrm::ModelInfo::modelInfo(metaObject.className());
    return this->makeRecordList(modelInfo);
}

QVariantList Query::makeRecordList(const ModelInfo &modelInfo)
{
    dPvt();
    auto metaObject = modelInfo.staticMetaObject();
    QVariantList recordList;
    if(metaObject.methodCount()==0){
        while(this->next()){
            QVariantHash record;
            auto&sqlRecord=this->sqlRecord();
            for (int col = 0; col < sqlRecord.count(); ++col){
                const auto v=sqlRecord.value(col);
                record[sqlRecord.fieldName(col)]=v;
            }
            recordList<<record;
        }
    }
    else{
        QHash<int,QByteArray> recordsIndex;
        QStringList propertys;
        auto propertyTableList = modelInfo.propertyShortVsTable();

        while(this->next()){
            QVariantHash record;
            auto&sqlRecord=p.sqlRecord;

            if(recordsIndex.isEmpty()){
                for(int col = 0; col < metaObject.propertyCount(); ++col) {
                    auto property = metaObject.property(col);
                    auto propertyName=QByteArray(property.name()).toLower().trimmed();
                    auto fieldName=propertyTableList.value(propertyName).trimmed();
                    if(!fieldName.isEmpty()){
                        auto index=sqlRecord.indexOf(fieldName);
                        if(index>=0)
                            recordsIndex[index]=property.name();
                    }
                }
            }


            QHashIterator<int, QByteArray> i(recordsIndex);
            while (i.hasNext()) {
                i.next();
                const auto v=sqlRecord.value(i.key());
                record[i.value()]=v;
            }
            recordList<<record;
        }
    }
    return recordList;
}

QVariantHash Query::makeRecord()const
{
    dPvt();
    if(p.initNext()){
        QVariantHash record;
        auto&sqlRecord=p.sqlRecord;
        for (int col = 0; col < p.sqlRecord.count(); ++col){
            const auto v=sqlRecord.value(col);
            record[sqlRecord.fieldName(col)]=v;
        }
        return record;
    }
    return {};
}

QVariantHash Query::makeRecord(const QMetaObject &metaObject) const
{
    const auto&modelInfo=QOrm::ModelInfo::modelInfo(metaObject.className());
    return this->makeRecord(modelInfo);
}

QVariantHash Query::makeRecord(const ModelInfo &modelInfo)const
{
    dPvt();
    if(p.initNext()){
        auto metaObject=modelInfo.staticMetaObject();
        auto propertyTableList=modelInfo.propertyShortVsTable();
        if(metaObject.methodCount()==0){
            QVariantHash record;
            for (int col = 0; col < p.sqlRecord.count(); ++col)
                record.insert(p.sqlRecord.fieldName(col), p.sqlRecord.value(col));
            return record;
        }
        else{
            QList<int> recordsIndex;
            QStringList propertys;
            for(int col = 0; col < metaObject.propertyCount(); ++col) {
                auto property = metaObject.property(col);
                QString propertyName=QByteArray(property.name()).toLower().trimmed();
                propertyName=propertyTableList.value(propertyName).trimmed();
                if(!propertyName.isEmpty()){
                    auto index=p.sqlRecord.indexOf(propertyName);
                    if(index>=0)
                        recordsIndex<<index;
                }
            }

            QVariantHash record;
            for (auto&col:recordsIndex)
                record.insert(p.sqlRecord.fieldName(col), p.sqlRecord.value(col));
            return record;
        }
    }
    return {};
}

bool Query::modelRead(QOrm::Model *model) const
{
    dPvt();
    bool RETURN = false;
    auto&metaObject=*model->metaObject();

    QStringList propertys;
    for(int col = 0; col < metaObject.propertyCount(); ++col) {
        auto property = metaObject.property(col);
        auto propertyName=QByteArray(property.name()).toLower().trimmed();
        if(p.sqlQueryFields.contains(propertyName)){
            auto index=p.sqlRecord.indexOf(propertyName);
            if(index>=0){
                auto value=p.sqlRecord.value(index);
                if(!model->setProperty(property, value))
                    return false;
                RETURN=true;
            }
        }
    }
    return RETURN;
}

Query &Query::close()
{
    dPvt();
    p.close();
    return*this;
}

bool Query::next()const
{
    dPvt();
    if(!p.next())
        return false;
    else{
        if(p.sqlRecord.isEmpty())
            p.sqlRecord=p.sqlQuery.record();
        return true;
    }
}

bool Query::prepare() const
{
    dPvt();
    return p.prepareExec();
}

bool Query::prepareCache() const
{
    dPvt();
    return p.prepareExecCache();
}

bool Query::exec()
{
    dPvt();
    if(!p.prepareExec())
        return false;
    return p.exec(QVariant());
}

bool Query::exec(const QVariant &command)
{
    dPvt();
    return p.exec(command);
}

bool Query::execBatch()
{
    dPvt();
    this->prepare();
    return p.sqlQuery.execBatch(QSqlQuery::ValuesAsRows);
}

bool Query::execBatch(int mode)
{
    dPvt();
    this->prepare();
    return p.sqlQuery.execBatch(QSqlQuery::BatchExecutionMode(mode));
}

void Query::bindValue(const QString &placeholder, const QVariant &val, QSql::ParamType type)
{
    dPvt();
    this->prepare();
    p.sqlQuery.bindValue(placeholder, val, type);
}

void Query::bindValue(int pos, const QVariant &val, QSql::ParamType type)
{
    dPvt();
    this->prepare();
    p.sqlQuery.bindValue(pos, val, type);
}

void Query::addBindValue(const QVariant &val, QSql::ParamType type)
{
    dPvt();
    this->prepare();
    p.sqlQuery.addBindValue(val, type);
}

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
QMap<QString, QVariant> Query::boundValues() const
#else
QVariantList Query::boundValues() const
#endif
{
    dPvt();
    return p.sqlQuery.boundValues();
}

QString Query::executedQuery() const
{
    dPvt();
    return p.sqlQuery.executedQuery();
}

QVariant Query::value(const int &column) const
{
    dPvt();
    return p.sqlQuery.value(column);
}

QVariant Query::value(QString &columnName) const
{
    dPvt();
    auto column=p.sqlRecord.indexOf(columnName);
    return p.sqlQuery.value(column);
}

QVariant Query::lastInsertId() const
{
    dPvt();
    return p.sqlQuery.lastInsertId();
}

SqlSuitableKeyWord &Query::parser()
{
    dPvt();
    if(p.parser==nullptr)
        p.parser=&QOrm::SqlSuitableKeyWord::parser(this->connection());

    if(p.parser==nullptr)
        return SqlSuitableKeyWord::parser(QSqlDriver::UnknownDbms);

    return *p.parser;
}


}
