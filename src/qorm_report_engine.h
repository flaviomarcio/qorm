#pragma once

#include "./qorm_object_db.h"
#include "./qorm_report_strategy.h"
#include "./qorm_report_runner.h"

namespace QOrm {

class Q_ORM_EXPORT ReportEngine : public ObjectDb
{
public:
    Q_INVOKABLE explicit ReportEngine(QObject *parent = nullptr);
    Q_INVOKABLE ~ReportEngine();

    /**
     * @brief canResolver
     * @param reportName
     * @return
     */
    Q_INVOKABLE virtual ResultValue &canResolver(const QString&reportName);

    /**
     * @brief canResolverExecute
     * @param reportName
     * @return
     */
    Q_INVOKABLE virtual ResultValue &canResolverExecute(const QString&reportName);

    /**
     * @brief resolver
     * @param reportName
     * @return
     */
    virtual ReportRunner&resolver(const QString&reportName);
private:
    void*p=nullptr;
};

}
