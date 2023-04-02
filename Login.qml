import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

Page {
    id: loginPage
    spacing: 5

    signal checkPin(string pin)
    signal successPin()


    Popup {
        id: popup
        anchors.centerIn: Overlay.overlay
        width: 300
        height: 250
        modal: true
        focus: true

        ColumnLayout {
            anchors.fill: parent
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            Label {
                id: label
                Layout.alignment: Qt.AlignHCenter
                font.pixelSize: 18
                text: "Incorrect Password"
            }
        }
    }

    ColumnLayout {
        id: loginLayout
        anchors.centerIn: parent
        Layout.minimumWidth: 200

        Label {
           text: "Pin"
        }
        TextField {
            id: textField
            placeholderText: "Enter your pin"
            echoMode: TextInput.Password
            selectByMouse: true

        }
        Button {
            text: "Login"
            onClicked: {
                loginPage.checkPin(textField.text)
            }
        }
    }

    Connections {
        target: auth
        function onPinChecked(res) {
            if (res) {
                loginPage.successPin();
                return;
            }
            popup.open()
        }
    }

    Connections {
        target: auth
        function onAlertPopup(text) {
            label.text = text;
            popup.open()
            }
        }
}
