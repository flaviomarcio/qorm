#include "./qorm_model_dto_options.h"

namespace QOrm {

class ModelDtoOptionsPvt:public QObject
{
public:
    ModelDtoOptions *parent = nullptr;
    bool searchOnEmptyFilter = true;
    explicit ModelDtoOptionsPvt(ModelDtoOptions *parent):QObject{parent} { this->parent = parent; }
};

ModelDtoOptions::ModelDtoOptions(QObject *parent) : QObject{parent}
{
    this->p = new ModelDtoOptionsPvt{this};
}

bool ModelDtoOptions::searchOnEmptyFilter() const
{
    return p->searchOnEmptyFilter;
}

void ModelDtoOptions::setSearchOnEmptyFilter(bool value)
{
    p->searchOnEmptyFilter = value;
}

QOrm::ModelDtoOptions &QOrm::ModelDtoOptions::operator=(const QOrm::ModelDtoOptions &v)
{
    p->searchOnEmptyFilter = v.searchOnEmptyFilter();
    return *this;
}

} // namespace QOrm
