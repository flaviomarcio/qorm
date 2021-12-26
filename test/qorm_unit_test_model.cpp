#ifndef Q_ORM_ModelTestUnit_H
#define Q_ORM_ModelTestUnit_H

#include "./qorm_test_unit.h"

namespace QOrm {

    class Q_ORM_ModelTestUnit : public SDKGoogleTestUnit {
    public:
    };

    TEST_F(Q_ORM_ModelTestUnit, serviceStart)
    {
        EXPECT_TRUE(this->serviceStart())<<"fail: service start";
    }

    TEST_F(Q_ORM_ModelTestUnit, serviceStop)
    {
        EXPECT_TRUE(this->serviceStop())<<"fail: service stop";
    }

}


#endif // ModelTestUnit_H
