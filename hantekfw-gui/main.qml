import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2

ApplicationWindow {
    id: window
    title: qsTr("Hello World")
    width: 460
    height: 200
    visible: true
    signal qmlGetDevicesInfo()
    signal qmlSearchDevices(string deviceName)
    signal qmlInstallFW(string deviceName)
    function enableDownload(){
        console.log("Received Signal")
        mainForm.buttonInstallFW.enabled = true
    }
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
        id: mainForm
        anchors.fill: parent
        buttonFWAvailable.onClicked:
        {
            window.qmlGetDevicesInfo()
        }
        buttonSearchDevice.onClicked:
        {
            window.qmlSearchDevices("6022BL");
        }
        buttonInstallFW.onClicked:
        {
            //onDeviceConnected: console.log("Error code:",errorCode)
            window.qmlInstallFW("6022BL")
        }
        buttonQuit.onClicked: Qt.quit();
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
/*
        Button {
            id: buttonFWAvailable
            text: qsTr("Firmware Available")
        }

        Button {
            id: buttonSearchDevice
            text: qsTr("Search Device")
        }

        Button {
            id: buttonInstallFW
            text: qsTr("Install Firmware")
            enabled: false
        }
*/
