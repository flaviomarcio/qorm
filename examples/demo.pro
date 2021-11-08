QT += testlib

CONFIG += testcase
CONFIG += console
CONFIG += silent
CONFIG -= debug_and_release
QT -= gui

TEMPLATE = app
TARGET = demo

include($$PWD/../../qstm/qstm.pri)
include($$PWD/../../qorm/qorm.pri)
include($$PWD/demo.pri)
