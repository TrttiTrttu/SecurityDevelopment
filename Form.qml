import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
Page {
    id: formPage

    signal checkCreateEntry(string site, string login, string password)
    signal successCreated()

    ColumnLayout {
        id: formLayout
        anchors.centerIn: parent
        Layout.minimumWidth: 200

        Label {
           text: "site"
        }
        TextField {
            id: siteField
            text: ""
            placeholderText: "Enter site url"
            selectByMouse: true
        }
        Label {
           text: "login"
        }
        TextField {
            id: loginField
            text: ""
            placeholderText: "Enter login"
            selectByMouse: true
        }

        Label {
           text: "password"
        }
        TextField {
            id: passwordField
            text: ""
            placeholderText: "Enter password"
            echoMode: TextInput.Password
            selectByMouse: true
        }

        Button {
            text: "Create"
            onClicked: {
                formPage.checkCreateEntry(siteField.text, loginField.text, passwordField.text)
            }
        }
    }

    Connections {
        target: ListController
        function onEntryCreated() {
            formPage.successCreated();
            return;
        }
    }

}

