#pragma once

#include <QObject>
#include "./qorm_global.h"

namespace QOrm {

    class Q_ORM_EXPORT ModelDtoOptions:public QObject
    {
        Q_OBJECT
    public:
        Q_INVOKABLE explicit ModelDtoOptions(QObject *parent=nullptr);
        Q_INVOKABLE virtual ~ModelDtoOptions();

        ModelDtoOptions&operator=(const ModelDtoOptions&v);

        /**
         * @brief searchOnEmptyFilter
         * @return
         */
        Q_INVOKABLE virtual bool searchOnEmptyFilter() const;
        Q_INVOKABLE virtual void setSearchOnEmptyFilter(bool value);

    private:
        void*p=nullptr;
    };

}
