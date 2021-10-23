include($$PWD/../qorm.pri)

INCLUDEPATH+=$$PWD

CONFIG += console
CONFIG -= debug_and_release
CONFIG += testcase
LIBS += -L/usr/local/lib -lgmock
LIBS += -L/usr/local/lib -lgtest

Q_ORM_TEST=true
QMAKE_CXXFLAGS += -DQ_ORM_TEST=\\\"$$Q_ORM_TEST\\\"

HEADERS += \
    $$PWD/qorm_test.h \
    $$PWD/qorm-test.pri \
    $$PWD/qorm_test_unit.h

SOURCES += \
    $$PWD/qorm_unit_test_object.cpp \
    $$PWD/qorm_unit_test_model.cpp \
    $$PWD/qorm_unit_test_dto.cpp \
    $$PWD/qorm_unit_test_crud.cpp \
    $$PWD/qorm_unit_test_connection_notify.cpp




