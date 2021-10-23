#pragma once

#include "./qstm_result.h"

namespace QOrm {

    class Wrapper
    {
    public:
        Q_INVOKABLE explicit Wrapper(const QVariant&v);
        Q_INVOKABLE explicit Wrapper(ResultValue &v);
        Q_INVOKABLE ~Wrapper();
        Wrapper&w(const QString &propertySrc, const QString &propertyDestine);
        Wrapper&w(const QString &propertySrc);
        virtual Wrapper&clear();
        virtual QVariant &v() const;
    private:
        void*p=nullptr;
    };

} // namespace QOrm
