#include "./qorm_test.h"

namespace QOrm{

ObjectTest::ObjectTest(QObject *parent):QObject{parent}
{
    QLocale::setDefault(QLocale(QLocale::Portuguese, QLocale::Brazil));
}

void ObjectTest::execute()
{
    static const QByteArray prefix="test_";

    for (int i = 0; i < this->metaObject()->methodCount(); ++i) {
        auto method=this->metaObject()->method(i);

        if(method.methodType()!=QMetaMethod::Method)
            continue;

        if(method.parameterCount()>0)
            continue;

        if(!method.name().startsWith(prefix))
            continue;

        method.invoke(this, Qt::DirectConnection);
    }
}

}
