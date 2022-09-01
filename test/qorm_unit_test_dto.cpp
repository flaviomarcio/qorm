#ifndef Q_ORM_DTOTestUnit_H
#define Q_ORM_DTOTestUnit_H

#include "./qorm_test_unit.h"
#include "../src/qorm_model_dto.h"

namespace QOrm {

class Q_ORM_DTOTestUnit : public SDKGoogleTestUnit {
public:
};

TEST_F(Q_ORM_DTOTestUnit, serviceTestOutPut)
{
    QVariantList records;
    records<<QVariantHash{{"uuid",QUuid::createUuid()},{"name",QUuid::createUuid()},{"enabled",true}};

    QOrm::ModelDto dto;
    dto
        .headers().value(QStringLiteral("uuid"      )).text(QStringLiteral("id")).type(vNumber   ).align(vaStart  ).width(QStringLiteral("10%")).editable(true ).d()
        .headers().value(QStringLiteral("name"      )).text(QStringLiteral("na")).type(vText     ).align(vaStart  ).width(QStringLiteral("25%")).editable(false).d()
        .headers().value(QStringLiteral("enabled"   )).text(QStringLiteral("en")).type(vText     ).align(vaStart  ).width(QStringLiteral("15%")).editable(false).d()
        .links().ref(QStringLiteral("endpoint_1")).method(vGET).hRef("/endpoint_1").d()
        .links().ref(QStringLiteral("endpoint_1")).method(vPOST).hRef("/endpoint_2").d()
        .links().ref(QStringLiteral("endpoint_2")).method(vPOST).hRef("/endpoint_2").d()
        .filters().d()
        .items(records);

    auto toOutput=dto.toOutput().resultMap();
    EXPECT_EQ(toOutput.isEmpty() ,false)<<"invalid headers";
    if(!toOutput.isEmpty()){
        auto headers=toOutput.value(vpHeaders).toList();
        auto links=toOutput.value(vpLinks).toList();
        auto filters=toOutput.value(vpFilters).toList();
        //auto edit=toOutput.value(vpEdit).toList();
        auto items=toOutput.value(vpItems).toList();

        EXPECT_EQ(headers.isEmpty() ,false)<<"invalid headers";
        EXPECT_EQ(links.isEmpty()   ,false)<<"invalid links";
        EXPECT_EQ(filters.isEmpty() ,true )<<"invalid filters";
        //EXPECT_EQ(edit.isEmpty()    ,false)<<"invalid fields";
        EXPECT_EQ(items.isEmpty()   ,false)<<"invalid items";


        EXPECT_EQ(headers.size() ,4)<<"invalid headers";
        EXPECT_EQ(links.size()   ,2)<<"invalid links";
        EXPECT_EQ(filters.size() ,0)<<"invalid filters";
        //EXPECT_EQ(edit.size()    ,2)<<"invalid fields";
        EXPECT_EQ(items.size()   ,1)<<"invalid items";
    }
}

}


#endif // DTOTestUnit_H
