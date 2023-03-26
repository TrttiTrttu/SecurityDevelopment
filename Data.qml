import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Page {
    id: dataPage

    signal entryCreateClicked()
    signal entryDeleteClicked()

    ListView {
        id: dataList
        implicitHeight: 400
        implicitWidth: 300
        anchors.centerIn: parent
        clip: true

        model: MyModel

        delegate: RowLayout {
            width: parent.width

            TextEdit {
                text: model.site
                readOnly: true
//                Layout.fillWidth: true
                horizontalAlignment: Text.AlignLeft
            }
            TextEdit {
                text: model.login
                readOnly: true
//                Layout.fillWidth: true
                horizontalAlignment: Text.AlignLeft
            }
            TextEdit {
                text: model.password
                readOnly: true
//                Layout.fillWidth: true
                horizontalAlignment: Text.AlignLeft
            }
        }
    }

    GridLayout {
        anchors.top: dataList.bottom
        anchors.horizontalCenter: dataList.horizontalCenter
        Button {
            text: "Create"
            onClicked: {
                dataPage.entryCreateClicked()
            }
        }
        Button {
            text: "Delete"
            onClicked: {
                dataPage.entryDeleteClicked()
            }
        }
    }



}
