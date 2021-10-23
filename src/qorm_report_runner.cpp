#include "./qorm_report_runner.h"
#include "./private/p_qorm_report_runner.h"

namespace QOrm {

#define dPvt()\
    auto&p = *reinterpret_cast<ReportRunnerPrv*>(this->p)

class ReportRunnerPrv{
public:
    ReportRunner*parent=nullptr;
    ReportItem item;
    explicit ReportRunnerPrv(ReportRunner*parent):item(parent){
        this->parent=parent;
    }
    virtual ~ReportRunnerPrv(){
    }

    ResultValue&execute(const QVariant&filterValues){
        Q_UNUSED(filterValues)
        ReportItem item(this->item.toHash(), this->parent);
        RunnerEngine engine(this->parent);
        return this->parent->lr(engine.execute(this->item));
    }

};

ReportRunner::ReportRunner(QObject *parent) : QStm::Object(parent)
{
    this->p = new ReportRunnerPrv(this);
}

ReportRunner::ReportRunner(const QVariant &item, QObject *parent):QStm::Object(parent)
{
    this->p = new ReportRunnerPrv(this);
    dPvt();
    p.item=item;
}

ReportRunner::~ReportRunner()
{
    dPvt();
    delete&p;
}

ReportRunner &ReportRunner::operator=(const QVariant &v)
{
    dPvt();
    p.item=v;
    return*this;
}

ResultValue &ReportRunner::isValid()
{
    dPvt();
    return this->lr(p.item.isValid());
}

ReportItem &ReportRunner::item()
{
    dPvt();
    return p.item;
}

ResultValue &ReportRunner::execute()
{
    return this->execute(QVariant());
}

ResultValue &ReportRunner::execute(const QVariant &filterValues)
{
    dPvt();
    return p.execute(filterValues);
}

}
