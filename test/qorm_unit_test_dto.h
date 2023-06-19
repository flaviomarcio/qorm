#pragma once

#include "./qorm_test.h"
#include "../src/qorm_model_dto.h"
#include "../src/qorm_model_consts.h"

namespace QOrm {

class UT_DTOTestUnit : public ObjectTest {
public:
    Q_INVOKABLE void UT_OutPut()
    {
        QVariantList records;
        records<<QVariantHash{{"uuid",QUuid::createUuid()},{"name",QUuid::createUuid()},{"enabled",true}};

        QOrm::ModelDto dto;

        dto.headers().item(QStringLiteral("uuid"      )).title(QStringLiteral("id")).dataType(vNumber   ).align(vaStart  ).width(QStringLiteral("10%")).editable(true );
        dto.headers().item(QStringLiteral("name"      )).title(QStringLiteral("na")).dataType(vText     ).align(vaStart  ).width(QStringLiteral("25%")).editable(false);
        dto.headers().item(QStringLiteral("enabled"   )).title(QStringLiteral("en")).dataType(vText     ).align(vaStart  ).width(QStringLiteral("15%")).editable(false);
        dto.items(records);

        auto toOutput=dto.toOutput().resultMap();
        QVERIFY(!toOutput.isEmpty());
        if(!toOutput.isEmpty()){
            auto headers=toOutput.value(vpHeaders).toList();
            //auto links=toOutput.value(vpLinks).toList();
            auto filters=toOutput.value(vpFilters).toList();
            //auto edit=toOutput.value(vpEdit).toList();
            auto items=toOutput.value(vpItems).toList();

            QVERIFY(!headers.isEmpty());
            QVERIFY(filters.isEmpty());
            QVERIFY(!items.isEmpty());


            QCOMPARE_EQ(headers.size() ,4);
            QCOMPARE_EQ(filters.size() ,0);
            QCOMPARE_EQ(items.size()   ,1);
        }
    }
};

}

