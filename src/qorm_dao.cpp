#include "./qorm_dao.h"

namespace QOrm {

#define dPvt()\
    auto&p = *reinterpret_cast<DaoPrv*>(this->p)

class DaoPrv{
public:
    QOrm::SqlSuitableValue suitableValue;
    explicit DaoPrv(QObject*object)
    {
        Q_UNUSED(object)
    }
    virtual ~DaoPrv()
    {
    }
};

Dao::Dao(QObject *parent) : ObjectDb(parent)
{
    this->p = new DaoPrv(this);
}

Dao::Dao(const QSqlDatabase &connection, QObject *parent) : ObjectDb(connection, parent)
{
    this->p = new DaoPrv(this);
}

Dao::~Dao()
{
    dPvt();
    delete&p;
}

QOrm::SqlSuitableValue &Dao::format()
{
    dPvt();
    return p.suitableValue;
}

}
