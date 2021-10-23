#pragma once

#include "./qorm_object_db.h"
#include "./qorm_sql_suitable_value.h"
#include "./qorm_wrapper.h"

namespace QOrm {

class Q_ORM_EXPORT Dao : public ObjectDb
{
public:
    Q_INVOKABLE explicit Dao(QObject *parent = nullptr);
    Q_INVOKABLE explicit Dao(const QSqlDatabase&connection, QObject *parent = nullptr);
    Q_INVOKABLE ~Dao();
    QOrm::SqlSuitableValue&format();
    QOrm::SqlSuitableValue&f=format();
private:
    void*p=nullptr;
};

}
