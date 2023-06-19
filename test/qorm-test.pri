CONFIG += console
CONFIG -= debug_and_release
CONFIG += testcase
QT += testlib

LC_ALL=en_US.UTF-8

include($$PWD/../../qstm/qstm.pri)
include($$PWD/../qorm.pri)

Q_STM_TEST=true
QMAKE_CXXFLAGS += -DQ_STM_TEST=\\\"$$Q_STM_TEST\\\"

INCLUDEPATH+=$$PWD/src

HEADERS += \
    $$PWD/qorm_test.h \
    $$PWD/qorm_test_main.h \
    $$PWD/qorm_unit_test_dto.h \
#    $$PWD/qorm_unit_test_object.h \
#    $$PWD/qorm_unit_test_model.h \
#    $$PWD/qorm_unit_test_dto.h \
#    $$PWD/qorm_unit_test_crud.h \
#    $$PWD/qorm_unit_test_connection_notify.h \

SOURCES += \
    $$PWD/qorm_test.cpp \
    $$PWD/qorm_test_main.cpp \
