#pragma once

#include <QVariant>
#include <QVariantHash>
#include <QVariantHash>

#include "./qorm_global.h"

namespace QOrm {

    enum CRUDStrategy{
        Undefined=1, Search=2, Insert=4, Update=8, Upsert=16, Remove=32, Deactivate=64
    };

    static const QVariantList __listToStrategy={QOrm::Search, QOrm::Update, QOrm::Upsert, QOrm::Deactivate};
    static const QVariantHash __stringToStrategy={
        {QString::number(QOrm::Search       ),QOrm::Search      }, {qsl("search")       ,QOrm::Search       }, {qsl("get"),QOrm::Search}, {qsl("find"),QOrm::Search},
        {QString::number(QOrm::Insert       ),QOrm::Insert      }, {qsl("insert")       ,QOrm::Insert       },
        {QString::number(QOrm::Upsert       ),QOrm::Upsert      }, {qsl("upsert")       ,QOrm::Upsert       }, {qsl("post"),QOrm::Upsert}, {qsl("save"),QOrm::Upsert},
        {QString::number(QOrm::Update       ),QOrm::Update      }, {qsl("update")       ,QOrm::Update       }, {qsl("put"),QOrm::Update},
        {QString::number(QOrm::Remove       ),QOrm::Remove      }, {qsl("remove")       ,QOrm::Remove       },
        {QString::number(QOrm::Deactivate   ),QOrm::Deactivate  }, {qsl("deactivate")   ,QOrm::Deactivate   }, {qsl("delete"), QOrm::Deactivate}
    };

}
