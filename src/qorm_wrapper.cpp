#include "./qorm_wrapper.h"
#include "../../qstm/src/qstm_types.h"

namespace QOrm {

#define dPvt()\
    auto&p = *reinterpret_cast<WrapperPvt*>(this->p)


class WrapperPvt{
public:
    QHash<QString,QString> wrapperNames;
    QVariant v;
    explicit WrapperPvt()
    {
    }

    virtual ~WrapperPvt()
    {
    }

    QVariant&wrapper()
    {
        QVariantList list;
        switch (qTypeId(v)) {
        case QMetaType_QVariantHash:
        case QMetaType_QVariantMap:
            list<<this->v;
            break;
        case QMetaType_QVariantList:
        case QMetaType_QStringList:
            list=v.toList();
            break;
        default:
            break;
        }

        for(auto&v:list){
            switch (qTypeId(v)) {
            case QMetaType_QVariantHash:
            case QMetaType_QVariantMap:
                break;
            default:
                continue;//next
            }

            auto vHash=v.toHash();
            if(vHash.isEmpty())
                continue;

            QVariantHash wrapperHash;
            QHashIterator<QString, QString> i(this->wrapperNames);
            while (i.hasNext()) {
                i.next();
                auto src=i.key();
                auto dst=i.value();

                auto value=vHash[src];
                if(!value.isValid())
                    continue;

                wrapperHash[dst]=value;
            }
            v=QVariant(wrapperHash);
        }

        switch (qTypeId(v)) {
        case QMetaType_QVariantHash:
        case QMetaType_QVariantMap:
            this->v=list.first();
            break;
        case QMetaType_QVariantList:
        case QMetaType_QStringList:
            this->v=list;
            break;
        default:
            break;
        }

        return this->v;
    }
};


Wrapper::Wrapper(const QVariant &v)
{
    this->p = new WrapperPvt();
    dPvt();
    p.v = v;
}

Wrapper::Wrapper(ResultValue &v)
{
    this->p = new WrapperPvt();
    dPvt();
    p.v = v.resultVariant();
}

Wrapper::~Wrapper()
{
    dPvt();
    delete&p;
}

Wrapper &Wrapper::w(const QString &propertySrc, const QString &propertyDestine)
{
    dPvt();
    p.wrapperNames[propertySrc]=propertyDestine;
    return*this;
}

Wrapper &Wrapper::w(const QString &propertySrc)
{
    return this->w(propertySrc, propertySrc);
}

Wrapper &Wrapper::clear()
{
    dPvt();
    p.wrapperNames.clear();
    return*this;
}

QVariant&Wrapper::v() const
{
    dPvt();
    return p.wrapper();
}

}
