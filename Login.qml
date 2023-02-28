import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

Page {
    id: loginPage
    spacing: 5

    signal checkPin(string pin)
    signal successPin()

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
        }
    }
}
