#-------------------------------------------------
#
# Project created by QtCreator 2015-10-10T23:45:58
#
#-------------------------------------------------

QT       -= gui

TARGET = ht6022bx
TEMPLATE = lib

DEFINES += HT6022BX_LIBRARY
INCLUDEPATH += ../../Driver/Inc/
SOURCES += ht6022bx.cpp \
    ../../Driver/Src/HT6022fw.c \
    ../../Driver/Src/HT6022BLfw.c \
    firmware6022bx.cpp

HEADERS += ht6022bx.h\
        ht6022bx_global.h \
    ../../Driver/Inc/HT6022fw.h \
    ../../Driver/Inc/HT6022BLfw.h \
    firmware6022bx.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
