#include "./qorm_dao.h"

namespace QOrm {

class DaoPvt:public QObject
{
public:
    QOrm::SqlSuitableValue suitableValue;
    explicit DaoPvt(QObject *parent=nullptr):QObject{parent} {}
    virtual ~DaoPvt() {}
};

Dao::Dao(QObject *parent) : ObjectDb{parent}
{
    this->p = new DaoPvt{this};
}

Dao::Dao(const QSqlDatabase &connection, QObject *parent) : ObjectDb(connection, parent)
{
    this->p = new DaoPvt{this};
}

Dao::~Dao()
{
}

QOrm::SqlSuitableValue &Dao::format()
{
    return p->suitableValue;
}

} // namespace QOrm
