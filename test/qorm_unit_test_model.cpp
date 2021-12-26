#ifndef Q_ORM_ModelTestUnit_H
#define Q_ORM_ModelTestUnit_H

#include "./qorm_test_unit.h"

namespace QOrm {

    class Q_ORM_ModelTestUnit : public SDKGoogleTestUnit {
    public:
    };

    TEST_F(Q_ORM_ModelTestUnit, serviceStart)
    {
        EXPECT_EQ(this->serviceStart(),true)<<"fail: service start";
    }

    TEST_F(Q_ORM_ModelTestUnit, serviceStop)
    {
        EXPECT_EQ(this->serviceStop(),true)<<"fail: service stop";
    }

}


#endif // ModelTestUnit_H
