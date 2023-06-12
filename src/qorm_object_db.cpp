#include "./qorm_object_db.h"

namespace QOrm {

ObjectDb::ObjectDb(QObject *parent)
    : QOrm::Object{parent}, ObjectDbExtension{parent}
{
}

QByteArray ObjectDb::connectionId() const
{
    return ObjectDbExtension::connectionId();
}

} // namespace QOrm
