#include "./qorm_wrapper.h"

namespace QOrm {


class WrapperPvt:public QObject
{
public:
    QHash<QString, QString> wrapperNames;
    QVariant v;
    explicit WrapperPvt(QObject *parent=nullptr):QObject{parent} {}


    QVariant &wrapper()
    {
        QVariantList list;
        switch (v.typeId()) {
        case QMetaType::QVariantHash:
        case QMetaType::QVariantMap:
            list << this->v;
            break;
        case QMetaType::QVariantList:
        case QMetaType::QStringList:
            list = v.toList();
            break;
        default:
            break;
        }

        for (auto &v : list) {
            switch (v.typeId()) {
            case QMetaType::QVariantHash:
            case QMetaType::QVariantMap:
                break;
            default:
                continue; //next
            }

            auto vHash = v.toHash();
            if (vHash.isEmpty())
                continue;

            QVariantHash wrapperHash;
            QHashIterator<QString, QString> i(this->wrapperNames);
            while (i.hasNext()) {
                i.next();
                auto src = i.key();
                auto dst = i.value();

                auto value = vHash[src];
                if (!value.isValid())
                    continue;

                wrapperHash[dst] = value;
            }
            v = QVariant(wrapperHash);
        }

        switch (v.typeId()) {
        case QMetaType::QVariantHash:
        case QMetaType::QVariantMap:
            this->v = list.first();
            break;
        case QMetaType::QVariantList:
        case QMetaType::QStringList:
            this->v = list;
            break;
        default:
            break;
        }

        return this->v;
    }
};

Wrapper::Wrapper(const QVariant &v):QObject{},p{new WrapperPvt{this}}
{
    p->v = v;
}

Wrapper::Wrapper(ResultValue &v):QObject{},p{new WrapperPvt{this}}
{
    p->v = v.resultVariant();
}


Wrapper &Wrapper::w(const QString &propertySrc, const QString &propertyDestine)
{

    p->wrapperNames[propertySrc] = propertyDestine;
    return *this;
}

Wrapper &Wrapper::w(const QString &propertySrc)
{
    return this->w(propertySrc, propertySrc);
}

Wrapper &Wrapper::clear()
{
    p->wrapperNames.clear();
    return *this;
}

QVariant &Wrapper::v() const
{
    return p->wrapper();
}

} // namespace QOrm
