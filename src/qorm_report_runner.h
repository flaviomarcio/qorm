#pragma once

#include "./qorm_object.h"
#include "./qorm_report_item.h"

namespace QOrm {

class Q_ORM_EXPORT ReportRunner : public QStm::Object
{
public:
    Q_INVOKABLE explicit ReportRunner(QObject *parent = nullptr);
    Q_INVOKABLE explicit ReportRunner(const QVariant&setting, QObject *parent = nullptr);
    Q_INVOKABLE ~ReportRunner();

    ReportRunner&operator=(const QVariant&v);

    /**
     * @brief isValid
     * @return
     */
    Q_INVOKABLE virtual ResultValue &isValid();

    /**
     * @brief item
     * @return
     */
    virtual ReportItem&item();

    /**
     * @brief execute
     * @param filter
     * @return
     */
    Q_INVOKABLE virtual ResultValue &execute();
    Q_INVOKABLE virtual ResultValue &execute(const QVariant&filterValues);

private:
    void*p=nullptr;
};

}
