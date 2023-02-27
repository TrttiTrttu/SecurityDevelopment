import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Window {
    id: window
    width: 640
    height: 480
    minimumHeight: 200
    minimumWidth: 200
    visible: true
    title: qsTr("Hello World")

    property int defMargin: 5

    StackView {
        id: stackView
        anchors.fill: parent
        initialItem: page1
    }

    ColumnLayout {
        id: page1
        anchors.fill: parent
        anchors.margins: defMargin
        spacing: 5

        ColumnLayout {
            id: loginLayout
//            Layout.minimumHeight: 200
            Layout.minimumWidth: 200
            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignCenter


            Label {
               text: "Pin"
            }
            TextField {
                placeholderText: "Enter your pin"
                echoMode: TextInput.Password
                selectByMouse: true

            }
            Button {
                text: "Login"
                onClicked: {
                    stackView.push(page2)
                }
            }
        }

    }

//    Simplepage {
//        id: page1
//        buttonText: "Login"
//        onButtonClicked: {
//            stackView.push(page2)
//        }
//    }
    Simplepage {
        id: page2
        visible: false
    }
}
