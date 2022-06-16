#include "./p_qorm_model_dto_link_base.h"
#include "../../qstm/src/qstm_meta_types.h"
#include <QUuid>

namespace PrivateQOrm {

class ModelDtoLinkItemPvt : public QObject{
public:
    QUuid uuid=QUuid::createUuid();
    QVariantHash header;
    QVariant method;
    QVariant ref;
    QVariant hRef;
    QVariant parameters;
    QObject*parent=nullptr;
    QObject*___d=nullptr;
    explicit ModelDtoLinkItemPvt(QObject*parent):QObject{parent}
    {
    }
    ~ModelDtoLinkItemPvt()
    {
    }
};


class ModelDtoLinkBaseCollectionPvt : public QObject{
public:
    QObject*parent=nullptr;
    QObject*___d=nullptr;
    QHash<QString, ModelDtoLinkItem*> ___objectList;
    explicit ModelDtoLinkBaseCollectionPvt(QObject*parent):QObject{parent}
    {
    }
    ~ModelDtoLinkBaseCollectionPvt()
    {
    }
};

ModelDtoLinkItem::ModelDtoLinkItem(QObject *dtpParent, QObject *parent):QStm::Object{parent}
{
    this->p=new ModelDtoLinkItemPvt{this};

    p->___d=dtpParent;
}

ModelDtoLinkItem::~ModelDtoLinkItem()
{
//
//    delete&p;
}

QUuid &ModelDtoLinkItem::uuid() const
{

    return p->uuid;
}

QObject &ModelDtoLinkItem::d()
{

    return*p->___d;
}

bool ModelDtoLinkItem::isMethodGet() const
{

    return (p->method==qbl("get"));
}

bool ModelDtoLinkItem::isMethodPost() const
{

    return (p->method==qbl("post"));
}

bool ModelDtoLinkItem::isMethodPut() const
{

    return (p->method==qbl("put"));
}

bool ModelDtoLinkItem::isMethodDelete() const
{

    return (p->method==qbl("delete"));
}

QVariantHash &ModelDtoLinkItem::header() const
{

    return p->header;
}

ModelDtoLinkItem &ModelDtoLinkItem::setHeader(const QVariant &value)
{

    p->header=value.toHash();
    return*this;
}

QVariant &ModelDtoLinkItem::method() const
{

    return p->method;
}

ModelDtoLinkItem &ModelDtoLinkItem::setMethod(const QVariant &value)
{

    p->method=value.toString().trimmed();
    return*this;
}

QVariant &ModelDtoLinkItem::ref() const
{

    return p->ref;
}

ModelDtoLinkItem &ModelDtoLinkItem::setRef(const QVariant &value)
{

    p->ref=value;
    return*this;
}

QVariant &ModelDtoLinkItem::hRef() const
{

    return p->hRef;
}

ModelDtoLinkItem &ModelDtoLinkItem::setHRef(const QVariant &value)
{

    p->hRef=value;
    return*this;
}

QVariant &ModelDtoLinkItem::parameters() const
{

    return p->parameters;
}

ModelDtoLinkItem &ModelDtoLinkItem::setParameters(const QVariant &value)
{

    auto typeId=qTypeId(value);
    if(QMetaTypeUtilString.contains(typeId)){
        auto name=value.toString().trimmed();
        if(name.isEmpty())
            p->parameters=QVariantHash{{name, qsl("${%1}").arg(name)}};
    }
    p->parameters=value;
    return*this;
}

QVariant ModelDtoLinkItem::toVariant() const
{
    auto v=this->toHash();
    return v;
}

ModelDtoLinkCollection::ModelDtoLinkCollection(QObject *dtoParent, QObject *parent):QStm::Object{parent}
{
    this->p = new ModelDtoLinkBaseCollectionPvt{this};

    p->___d=dtoParent;
}

ModelDtoLinkCollection::~ModelDtoLinkCollection(){
}

QVariant ModelDtoLinkCollection::toVar() const
{

    QVariantList vList;
    for(auto &v:p->___objectList)
        vList<<v->toVariant();
    return vList;
}

QObject &ModelDtoLinkCollection::d()
{

    return*p->___d;
}

QObject &ModelDtoLinkCollection::ref(ModelDtoLinkItem *link)
{

    if(link==nullptr)
        link=new ModelDtoLinkItem(p->___d, this);
    else
        link->setParent(this);
    p->___objectList.insert(link->uuid().toString(), link);
    return*link;
}

ModelDtoLinkItem *ModelDtoLinkCollection::find(const QVariant &ref)
{

    auto _ref=ref.toString().toUtf8().toLower().trimmed();
    QHashIterator<QString, ModelDtoLinkItem*> i(p->___objectList);
    while (i.hasNext()) {
        i.next();
        auto iRef=i.value();
        if(iRef->ref().toString().toLower().trimmed()!=_ref)
            continue;
        return iRef;
    }
    return nullptr;
}

ModelDtoLinkCollection &ModelDtoLinkCollection::clear(){

    p->___objectList.clear();
    return*this;
}

QHash<QString, ModelDtoLinkItem *> &ModelDtoLinkCollection::list() const
{

    return p->___objectList;
}

}
