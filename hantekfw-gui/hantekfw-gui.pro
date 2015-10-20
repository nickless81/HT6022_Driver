TEMPLATE = app

QT += qml quick widgets

SOURCES += main.cpp

RESOURCES += qml.qrc
CONFIG   += c++11
# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../Hantek6022BX/ht6022bx/release/ -lht6022bx
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../Hantek6022BX/ht6022bx/debug/ -lht6022bx
else:unix: LIBS += -L$$OUT_PWD/../Hantek6022BX/ht6022bx/ -lht6022bx

INCLUDEPATH += $$PWD/../Hantek6022BX/ht6022bx
INCLUDEPATH += $$PWD/../Driver/Inc/
DEPENDPATH += $$PWD/../Hantek6022BX/ht6022bx

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../openhantek/UsbWrapper/UsbWrapper/release/ -lUsbWrapper
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../openhantek/UsbWrapper/UsbWrapper/debug/ -lUsbWrapper
else:unix: LIBS += -L$$OUT_PWD/../../openhantek/UsbWrapper/UsbWrapper/ -lUsbWrapper

INCLUDEPATH += $$PWD/../../openhantek/UsbWrapper/UsbWrapper
DEPENDPATH += $$PWD/../../openhantek/UsbWrapper/UsbWrapper
