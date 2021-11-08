QT += testlib

CONFIG += testcase
CONFIG += console
CONFIG += silent
CONFIG -= debug_and_release
QT -= gui

TEMPLATE = app
TARGET = QOrmTest

QORM_TEST_MODE=true
QMAKE_CXXFLAGS += -DQORM_TEST_MODE=\\\"$$QORM_TEST_MODE\\\"

LIBS += -lgmock
LIBS += -lgtest

include($$PWD/../qstm/test/qstm-test.pri)
#include($$PWD/qorm.pri)
include($$PWD/test/qorm-test.pri)

SOURCES += \
    $$PWD/test/main.cpp

