#-------------------------------------------------
#
# Project created by QtCreator 2015-10-11T18:16:23
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = hantekfw
CONFIG   += console
CONFIG   -= app_bundle
CONFIG   += c++11
TEMPLATE = app

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../Hantek6022BX/ht6022bx/release/ -lht6022bx
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../Hantek6022BX/ht6022bx/debug/ -lht6022bx
else:unix: LIBS += -L$$OUT_PWD/../Hantek6022BX/ht6022bx/ -lht6022bx

INCLUDEPATH += $$PWD/../Hantek6022BX/ht6022bx
INCLUDEPATH += $$PWD/../Driver/Inc/
DEPENDPATH += $$PWD/../Hantek6022BX/ht6022bx

SOURCES += main.cpp

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../openhantek/UsbWrapper/UsbWrapper/release/ -lUsbWrapper
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../openhantek/UsbWrapper/UsbWrapper/debug/ -lUsbWrapper
else:unix: LIBS += -L$$OUT_PWD/../../openhantek/UsbWrapper/UsbWrapper/ -lUsbWrapper

INCLUDEPATH += $$PWD/../../openhantek/UsbWrapper/UsbWrapper
DEPENDPATH += $$PWD/../../openhantek/UsbWrapper/UsbWrapper
