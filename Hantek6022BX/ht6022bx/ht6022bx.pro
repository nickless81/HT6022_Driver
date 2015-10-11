#-------------------------------------------------
#
# Project created by QtCreator 2015-10-10T23:45:58
#
#-------------------------------------------------

QT       -= gui

TARGET = ht6022bx
TEMPLATE = lib

DEFINES += HT6022BX_LIBRARY

SOURCES += ht6022bx.cpp

HEADERS += ht6022bx.h\
        ht6022bx_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
