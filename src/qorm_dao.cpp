#include "./qorm_dao.h"

namespace QOrm {

class DaoPvt:public QObject
{
public:
    QOrm::SqlSuitableValue suitableValue;
    explicit DaoPvt(QObject *parent):QObject{parent} {}
};

Dao::Dao(QObject *parent) : ObjectDb{parent}, p{new DaoPvt{this}}
{
}

QOrm::SqlSuitableValue &Dao::format()
{
    return p->suitableValue;
}

} // namespace QOrm
