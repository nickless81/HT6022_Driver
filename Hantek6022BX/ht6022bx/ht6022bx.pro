#-------------------------------------------------
#
# Project created by QtCreator 2015-10-10T23:45:58
#
#-------------------------------------------------

QT       -= gui
TARGET = ht6022bx
TEMPLATE = lib
CONFIG   += c++11
DEFINES += HT6022BX_LIBRARY
INCLUDEPATH += ../../Driver/Inc/
SOURCES += ht6022bx.cpp \
    ../../Driver/Src/HT6022fw.c \
    ../../Driver/Src/HT6022BLfw.c

HEADERS += ht6022bx.h\
        ht6022bx_global.h \
    ../../Driver/Inc/HT6022fw.h \
    ../../Driver/Inc/HT6022BLfw.h

# Settings for different operating systems
unix:!macx {
    INCLUDEPATH += /usr/include/libusb-1.0
}
unix {
    target.path = /usr/lib
    INSTALLS += target
}
LIBUSB_VERSION = 1
LIBS += -lusb-1.0
DEFINES += LIBUSB_VERSION=$${LIBUSB_VERSION}


win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../../openhantek/UsbWrapper/UsbWrapper/release/ -lUsbWrapper
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../../openhantek/UsbWrapper/UsbWrapper/debug/ -lUsbWrapper
else:unix: LIBS += -L$$OUT_PWD/../../../openhantek/UsbWrapper/UsbWrapper/ -lUsbWrapper

INCLUDEPATH += $$PWD/../../../openhantek/UsbWrapper/UsbWrapper
DEPENDPATH += $$PWD/../../../openhantek/UsbWrapper/UsbWrapper
