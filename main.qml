import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

/*TODO:
  banner for incorrect pin;
*/

Window {
    id: window
    width: 640
    height: 480
    minimumHeight: 400
    minimumWidth: 250
    visible: true
    title: qsTr("Psswd mngr")

    property int defMargin: 5

    StackView {
        id: stackView
        anchors.fill: parent
        initialItem: loginPage
    }

    Login {
        id: loginPage
    }

    Data {
        id: credPage
        visible: false
    }

    Form {
        id: formPage
        visible: false
    }

    Connections {
        target: loginPage
        function onSuccessPin() {
            stackView.push(credPage);
        }
    }

    Connections {
        target: credPage
        function onEntryCreateClicked() {
            stackView.push(formPage)
        }
    }

    Connections {
        target: formPage
        function onSuccessCreated() {
            stackView.pop()
        }
    }
}
