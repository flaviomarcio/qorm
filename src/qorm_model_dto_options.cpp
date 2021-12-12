#include "./qorm_model_dto_options.h"

namespace QOrm {

#define dPvt()\
auto&p = *reinterpret_cast<ModelDtoOptionsPvt*>(this->p)

          class ModelDtoOptionsPvt{
public:
    ModelDtoOptions*parent=nullptr;
    bool searchOnEmptyFilter=true;
    explicit ModelDtoOptionsPvt(ModelDtoOptions*parent){
        this->parent=parent;
    }
    virtual ~ModelDtoOptionsPvt(){
    }
};

ModelDtoOptions::ModelDtoOptions(QObject *parent):QObject(parent)
{
    this->p = new ModelDtoOptionsPvt(this);
}

ModelDtoOptions::~ModelDtoOptions()
{
    dPvt();
    delete&p;
}

bool ModelDtoOptions::searchOnEmptyFilter() const
{
    dPvt();
    return p.searchOnEmptyFilter;
}

void ModelDtoOptions::setSearchOnEmptyFilter(bool value)
{
    dPvt();
    p.searchOnEmptyFilter = value;
}

QOrm::ModelDtoOptions &QOrm::ModelDtoOptions::operator =(const QOrm::ModelDtoOptions &v)
{
    dPvt();
    p.searchOnEmptyFilter=v.searchOnEmptyFilter();
    return*this;
}

}
