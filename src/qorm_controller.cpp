#include "./qorm_controller.h"

namespace QOrm {

class ControllerPvt:public QObject
{
public:
    explicit ControllerPvt(QObject *object=nullptr)
        :QObject{object}
    {

    }
};

Controller::Controller(QObject *parent) : ObjectDb{parent}, p{new ControllerPvt{this}}
{
}

} // namespace QOrm
