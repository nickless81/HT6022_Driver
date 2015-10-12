import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Layouts 1.1

Item {
    width: 460
    height: 200
    property alias buttonQuit: buttonQuit
    property alias buttonInstallFW: buttonInstallFW
    property alias buttonSearchDevice: buttonSearchDevice
    property alias buttonFWAvailable: buttonFWAvailable
    y: 0

    RowLayout {
        x: 26
        y: 41
        width: 360
        anchors.verticalCenterOffset: 65
        anchors.horizontalCenterOffset: 0
        spacing: 3
        anchors.centerIn: parent

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

        Button {
            id: buttonQuit
            text: qsTr("Quit")
        }
    }
}
