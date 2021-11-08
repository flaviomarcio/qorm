#include "./qorm_model_dto_resullt_info.h"
#include <QMetaProperty>

namespace QOrm {

#define dPvt()\
    auto&p = *reinterpret_cast<ModelDtoResultInfoPvt*>(this->p)\

class ModelDtoResultInfoPvt{
public :
    ModelDtoResultInfo *parent=nullptr;
    bool enabled=false;
    QVariantList errors;
    QVariantList messages;
    bool success=false;
    int page=0;
    int per_page=9999999;
    int count=0;
    int total_count=0;
    int total_pages=0;

    explicit ModelDtoResultInfoPvt(ModelDtoResultInfo *parent){
        this->parent=parent;
    }
    virtual ~ModelDtoResultInfoPvt(){
    }

    void clear(){
        success=false;
        page=0;
        per_page=9999999;
        count=0;
        total_count=0;
        total_pages=0;
    }
};

ModelDtoResultInfo::ModelDtoResultInfo(QObject *parent):QOrm::Object(parent)
{
    this->p=new ModelDtoResultInfoPvt(this);
}

ModelDtoResultInfo::~ModelDtoResultInfo()
{
    dPvt();
    delete&p;
}

void ModelDtoResultInfo::clear()
{
    dPvt();
    p.clear();
}

bool ModelDtoResultInfo::enabled()
{
    dPvt();
    return p.enabled;
}

void ModelDtoResultInfo::setEnabled(bool value)
{
    dPvt();
    p.enabled=value;
}

QVariantList &ModelDtoResultInfo::errors()
{
    dPvt();
    return p.errors;
}

void ModelDtoResultInfo::setErrors(const QVariantList &value)
{
    dPvt();
    p.errors=value;
}

QVariantList &ModelDtoResultInfo::messages()
{
    dPvt();
    return p.messages;
}

void ModelDtoResultInfo::setMessages(const QVariantList &value)
{
    dPvt();
    p.messages=value;
}

int ModelDtoResultInfo::success() const
{
    dPvt();
    return p.success;
}

void ModelDtoResultInfo::setSuccess(bool value)
{
    dPvt();
    p.success=value;
}

int ModelDtoResultInfo::page() const
{
    dPvt();
    return p.page;
}

void ModelDtoResultInfo::setPage(int value)
{
    dPvt();
    p.page = value;
}

int ModelDtoResultInfo::per_page() const
{
    dPvt();
    return p.per_page;
}

void ModelDtoResultInfo::setPer_page(int value)
{
    dPvt();
    p.per_page = value;
}

int ModelDtoResultInfo::count() const
{
    dPvt();
    return p.count;
}

void ModelDtoResultInfo::setCount(int value)
{
    dPvt();
    p.count = value;
}

int ModelDtoResultInfo::total_count() const
{
    dPvt();
    return p.total_count;
}

void ModelDtoResultInfo::setTotal_count(int value)
{
    dPvt();
    p.total_count = value;
}

int ModelDtoResultInfo::total_pages() const
{
    dPvt();
    return p.total_pages;
}

void ModelDtoResultInfo::setTotal_pages(int value)
{
    dPvt();
    p.total_pages = value;
}

const QVariantHash ModelDtoResultInfo::toRequestHash() const
{
    QVariantHash v;
    v[QT_STRINGIFY2(page)]=this->page();
    v[QT_STRINGIFY2(per_page)]=this->per_page();
    return v;
}

}
