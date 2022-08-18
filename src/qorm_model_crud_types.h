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
    enum Strategy {
        Search = 0,
        Create = 1,
        Insert = 2,
        Update = 3,
        Upsert = 4,
        Remove = 5,
        Deactivate = 6
    };

    Q_ENUM(Strategy)

    explicit CRUDTypes(QObject *parent=nullptr):QObject{parent}{}

};



}
