#include "./qorm_model_crud_request.h"
#include "./qstm_util_variant.h"
#include <QJsonDocument>

namespace QOrm {

    #define dPvt()\
        auto&p = *reinterpret_cast<CRUDRequestPvt*>(this->p)


    class CRUDRequestPvt{
    public:
        CRUDRequest*parent=nullptr;
        explicit CRUDRequestPvt(CRUDRequest*parent){
            this->parent=parent;
        }
        virtual ~CRUDRequestPvt(){
        }
    };

    CRUDRequest::CRUDRequest(const QVariant &other):QVariantHash(other.toHash())
    {
        this->p = new CRUDRequestPvt(this);
    }

    CRUDRequest::~CRUDRequest()
    {
        dPvt();
        delete&p;
    }

    CRUDRequest&CRUDRequest::setBody(const QVariant &v)
    {
        auto h=v.toHash();
        const auto&__path = h[qsl("path")].toString().trimmed().toLower();
        const auto&__header = h[qsl("header")].toHash();
        const auto&__method = h[qsl("method")].toString().trimmed().toLower();
        const auto&__source = h[qsl("source")];

        CRUDRequest __return;
        __return[qsl("path")]=__path;
        __return[qsl("header")]=__header;
        __return[qsl("method")]=__method;
        __return[qsl("source")]=__source;
        return*this;
    }

    CRUDRequest CRUDRequest::fromMap(const QVariant &v)
    {
        CRUDRequest r;
        return r.setBody(v);
    }

    CRUDRequest CRUDRequest::fromRequest(const QVariant &v)
    {
        auto h=v.toHash();
        const auto&__path = h[qsl("requestPath")].toString().trimmed().toLower();
        const auto&__header = h[qsl("requestHeader")].toHash();
        const auto&__method = h[qsl("requestMethod")].toString().trimmed().toLower();
        const auto&__body = h[qsl("requestBody")];
        const auto&__parameter = h[qsl("requestParameter")].toHash();

        CRUDRequest __return;
        __return[qsl("header")]=__header;
        __return[qsl("path")]=__path;
        __return[qsl("method")]=__method;
        if(!__parameter.isEmpty())
            __return[qsl("source")]=__parameter;
        else
            __return[qsl("source")]=__body;
        return __return;
    }

    const QVariantHash CRUDRequest::header()
    {
        auto&v=*dynamic_cast<QVariantHash*>(this);
        return v[__func__].toHash();
    }

    const QString CRUDRequest::path()
    {
        auto&v=*dynamic_cast<QVariantHash*>(this);
        return v[__func__].toString();
    }

    const QString CRUDRequest::method()
    {
        auto&v=*dynamic_cast<QVariantHash*>(this);
        return v[__func__].toString();
    }

    const QVariant CRUDRequest::source()
    {
        const auto&v=*dynamic_cast<QVariantHash*>(this);
        return v[__func__];
    }

    CRUDStrategy CRUDRequest::strategy()
    {
        auto strategy=this->method().trimmed();
        if(!strategy.isEmpty()){
            return QOrm::Undefined;
        }
        else{
            QVariant vFy=strategy;
            if(vFy.type()==vFy.String || vFy.type()==vFy.ByteArray || vFy.type()==vFy.Char || vFy.type()==vFy.BitArray){
                vFy=vFy.toString().toLower();
                vFy=__stringToStrategy.value(vFy.toString());
            }
            vFy=__listToStrategy.contains(vFy)?vFy:QOrm::Undefined;
            return QOrm::CRUDStrategy(vFy.toInt());
        }
    }

    bool CRUDRequest::isStrategy(const CRUDStrategy v)
    {
        auto s=this->strategy();
        return s==v;
    }


}
