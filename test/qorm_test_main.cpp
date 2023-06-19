#include "qorm_test_main.h"
#include <QTest>

#include "./qorm_unit_test_dto.h"

#define Q_ORM_TEST_RUNNER(CLASS){CLASS var; var.execute();}

namespace QOrm{

void TestMain::execute()
{
    Q_ORM_TEST_RUNNER(UT_DTOTestUnit);
}

}
QTEST_MAIN(QOrm::TestMain)
#include "moc_qorm_test_main.cpp"


