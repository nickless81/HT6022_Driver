import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2

ApplicationWindow {
    id: window
    title: qsTr("Hello World")
    width: 640
    height: 480
    visible: true
    signal qmlGetDevicesInfo()
    signal qmlSearchDevices(string deviceName)
    signal qmlInstallFW(string deviceName)
    menuBar: MenuBar {
        Menu {
            title: qsTr("&File")
            MenuItem {
                text: qsTr("&Open")
                onTriggered: messageDialog.show(qsTr("Open action triggered"));
            }
            MenuItem {
                text: qsTr("E&xit")
                onTriggered: Qt.quit();
            }
        }
    }
    MainForm {
        //var texto = qsTr("6022BL")
        anchors.fill: parent
        button1.onClicked:
        {
            //messageDialog.show(qsTr("Button 1 pressed"))
            window.qmlGetDevicesInfo()
        }
        button2.onClicked:
        {
            //text: qsTr("6022BL")
            //messageDialog.show(qsTr("Button 2 pressed"))
            window.qmlSearchDevices("6022BL");
        }
        button3.onClicked:
        {
            //text: qsTr("6022BL")
            //messageDialog.show(qsTr("Button 3 pressed"))
            window.qmlInstallFW("6022BL")
        }
    }

    MessageDialog {
        id: messageDialog
        title: qsTr("May I have your attention, please?")
        function show(caption) {
            messageDialog.text = caption;
            messageDialog.open();
        }
    }
}
/**
signals:
    void deviceConnected(HT6022_ErrorTypeDef errorCode);
    void deviceReady(HT6022_ErrorTypeDef errorCode);
    void sendDevicesInfo(QList<HT6022BX_Info> *DeviceName);
    qml slots:
**/
