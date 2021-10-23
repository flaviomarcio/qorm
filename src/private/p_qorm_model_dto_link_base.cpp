#include "./p_qorm_model_dto_link_base.h"
#include <QUuid>

namespace PrivateQOrm {

#define dPvtI()\
    auto&p = *reinterpret_cast<ModelDtoLinkItemPvt*>(this->p)\

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
    explicit ModelDtoLinkItemPvt(QObject*parent):QObject(parent){
    }
    ~ModelDtoLinkItemPvt(){
    }
};

#define dPvtA()\
    auto&p = *reinterpret_cast<ModelDtoLinkBaseCollectionPvt*>(this->p)\

class ModelDtoLinkBaseCollectionPvt : public QObject{
public:
    QObject*parent=nullptr;
    QObject*___d=nullptr;
    QHash<QString, ModelDtoLinkItem*> ___objectList;
    explicit ModelDtoLinkBaseCollectionPvt(QObject*parent):QObject(parent){
    }
    ~ModelDtoLinkBaseCollectionPvt(){
    }
};

ModelDtoLinkItem::ModelDtoLinkItem(QObject *dtpParent, QObject *parent):QStm::Object(parent){
    this->p=new ModelDtoLinkItemPvt(this);
    dPvtI();
    p.___d=dtpParent;
}

ModelDtoLinkItem::~ModelDtoLinkItem(){
}

QUuid &ModelDtoLinkItem::uuid() const
{
    dPvtI();
    return p.uuid;
}

QObject &ModelDtoLinkItem::d()
{
    dPvtI();
    return*p.___d;
}

bool ModelDtoLinkItem::isMethodGet() const
{
    dPvtI();
    return (p.method==qbl("get"));
}

bool ModelDtoLinkItem::isMethodPost() const
{
    dPvtI();
    return (p.method==qbl("post"));
}

bool ModelDtoLinkItem::isMethodPut() const
{
    dPvtI();
    return (p.method==qbl("put"));
}

bool ModelDtoLinkItem::isMethodDelete() const
{
    dPvtI();
    return (p.method==qbl("delete"));
}

QVariantHash &ModelDtoLinkItem::header() const
{
    dPvtI();
    return p.header;
}

ModelDtoLinkItem &ModelDtoLinkItem::setHeader(const QVariant &value)
{
    dPvtI();
    p.header=value.toHash();
    return*this;
}

QVariant &ModelDtoLinkItem::method() const
{
    dPvtI();
    return p.method;
}

ModelDtoLinkItem &ModelDtoLinkItem::setMethod(const QVariant &value)
{
    dPvtI();
    p.method=value.toString().trimmed();
    return*this;
}

QVariant &ModelDtoLinkItem::ref() const
{
    dPvtI();
    return p.ref;
}

ModelDtoLinkItem &ModelDtoLinkItem::setRef(const QVariant &value)
{
    dPvtI();
    p.ref=value;
    return*this;
}

QVariant &ModelDtoLinkItem::hRef() const
{
    dPvtI();
    return p.hRef;
}

ModelDtoLinkItem &ModelDtoLinkItem::setHRef(const QVariant &value)
{
    dPvtI();
    p.hRef=value;
    return*this;
}

QVariant &ModelDtoLinkItem::parameters() const
{
    dPvtI();
    return p.parameters;
}

ModelDtoLinkItem &ModelDtoLinkItem::setParameters(const QVariant &value)
{
    dPvtI();
    if(value.type()==value.String || value.type()==value.ByteArray || value.type()==value.BitArray || value.type()==value.Char){
        auto name=value.toString().trimmed();
        if(name.isEmpty())
            p.parameters=QVariantHash({{name, qsl("${%1}").arg(name)}});
    }
    p.parameters=value;
    return*this;
}

QVariant ModelDtoLinkItem::toVariant() const{
    auto v=this->toHash();
    return v;
}

ModelDtoLinkCollection::ModelDtoLinkCollection(QObject *dtoParent, QObject *parent):QStm::Object(parent){
    this->p = new ModelDtoLinkBaseCollectionPvt(this);
    dPvtA();
    p.___d=dtoParent;
}

ModelDtoLinkCollection::~ModelDtoLinkCollection(){
}

QVariant ModelDtoLinkCollection::toVar() const
{
    dPvtA();
    QVariantList vList;
    for(auto&v:p.___objectList)
        vList<<v->toVariant();
    return vList;
}

QObject &ModelDtoLinkCollection::d(){
    dPvtA();
    return*p.___d;
}

QObject &ModelDtoLinkCollection::ref(ModelDtoLinkItem *link){
    dPvtA();
    if(link==nullptr)
        link=new ModelDtoLinkItem(p.___d, this);
    else
        link->setParent(this);
    p.___objectList.insert(link->uuid().toString(), link);
    return*link;
}

ModelDtoLinkItem *ModelDtoLinkCollection::find(const QVariant &ref)
{
    dPvtA();
    auto _ref=ref.toString().toUtf8().toLower().trimmed();
    QHashIterator<QString, ModelDtoLinkItem*> i(p.___objectList);
    while (i.hasNext()) {
        i.next();
        auto iRef=i.value();
        if(iRef->ref().toString().toLower().trimmed()==_ref){
            return iRef;
        }
    }
    return nullptr;
}

ModelDtoLinkCollection &ModelDtoLinkCollection::clear(){
    dPvtA();
    p.___objectList.clear();
    return*this;
}

QHash<QString, ModelDtoLinkItem *> &ModelDtoLinkCollection::list() const
{
    dPvtA();
    return p.___objectList;
}

}
