#include "qorm_wrapper.h"

namespace QOrm {

#define dPvt()\
    auto&p = *reinterpret_cast<WrapperPvt*>(this->p)


class WrapperPvt{
public:
    QHash<QString,QString> wrapperNames;
    QVariant v;
    explicit WrapperPvt(){
    }
    QVariant&wrapper(){
        QVariantList list;
        if(qTypeId(v)==QMetaType_QVariantHash || qTypeId(v)==QMetaType_QVariantMap)
            list<<this->v;
        else if(qTypeId(v)==QMetaType_QVariantList)
            list=v.toList();

        for (int var = 0; var < list.count(); ++var) {
            auto&v=list[var];
            if(qTypeId(v)==QMetaType_QVariantHash || qTypeId(v)==QMetaType_QVariantMap){
                auto map=v.toHash();
                if(!map.isEmpty()){
                    QVariantHash wrapperMap;
                    QHashIterator<QString, QString> i(this->wrapperNames);
                    while (i.hasNext()) {
                        i.next();
                        auto src=i.key();
                        auto dst=i.value();

                        auto value=map[src];
                        if(value.isValid()){
                            wrapperMap[dst]=value;
                        }
                    }
                    v=QVariant(wrapperMap);
                }
            }
        }

        if(qTypeId(v)==QMetaType_QVariantHash || qTypeId(v)==QMetaType_QVariantMap)
            this->v=list.first();
        else if(qTypeId(v)==QMetaType_QVariantList)
            this->v=list;

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
