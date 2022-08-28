#pragma once

#include <QObject>
#include <QVariant>
#include <QVariantHash>
#include <QVariantList>
//#include "./qorm_global.h"

namespace QOrm {


class CRUDTypes:public QObject{
    Q_OBJECT
public:
    enum Strategy {Search, Create, Upsert, Remove, Deactivate, Apply, Execute, Finalize, Print};
    Q_ENUM(Strategy)
    explicit CRUDTypes(QObject *parent=nullptr):QObject{parent}{}

};



}
