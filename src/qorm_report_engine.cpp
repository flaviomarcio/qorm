#include "./qorm_report_engine.h"

namespace QOrm {

#define dPvt()\
    auto&p = *reinterpret_cast<ReportEnginePrv*>(this->p)

class ReportEnginePrv{
public:
    QMutex runnerLock;
    ReportRunner runnerDefault;
    QHash<QString, QVariantHash> settings;
    QHash<QString, ReportRunner*> runners;
    ReportEngine*parent=nullptr;
    explicit ReportEnginePrv(ReportEngine*parent){
        this->parent=parent;
    }
    virtual ~ReportEnginePrv(){
    }
};

ReportEngine::ReportEngine(QObject *parent) : ObjectDb(parent)
{
    this->p = new ReportEnginePrv(this);
}

ReportEngine::~ReportEngine()
{
    dPvt();
    delete&p;
}

ResultValue &ReportEngine::canResolver(const QString &reportName)
{
    auto&runner=this->resolver(reportName);
    if(!runner.isValid())
        return this->lr()=false;
    else
        return this->lr();
}

ResultValue &ReportEngine::canResolverExecute(const QString &reportName)
{
    auto&runner=this->resolver(reportName);
    if(!runner.isValid())
        return this->lr()=false;
    else
        return this->lr(runner.execute());
}

ReportRunner &ReportEngine::resolver(const QString &reportName)
{
    dPvt();
    QString report;
    QMutexLocker locker(&p.runnerLock);
    {
        auto reportPath=reportName.trimmed().toLower();
        const auto separator=QStringList()<<qsl("\\")<<qsl("//")<<qsl(".")<<qsl("|")<<qsl(";");

        for(auto&v:separator)
            reportPath=reportPath.replace(v,qsl("/"));

        auto list=reportName.split(qsl("/"));
        report=list.last();
    }

    auto runner=p.runners.value(report);

    if(runner==nullptr){
        auto setting=p.settings.value(report);
        runner=new ReportRunner(setting, this);
        p.runners.insert(report,runner);
    }
    return*runner;
}

}
