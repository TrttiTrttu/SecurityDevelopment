import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Page {
    id: dataPage

    signal entryCreateClicked()
    signal entryDeleteClicked()

    TextField {
        id: searchField
        anchors.horizontalCenter: parent.horizontalCenter
        Layout.fillWidth: true
        Layout.preferredHeight: 40
        placeholderText: "Search..."
        font.pixelSize: 15
        onDisplayTextChanged: searchFilterModel.onSearchInputChanged(displayText)

    }

    ListView {
        id: dataList
        implicitHeight: 400
        implicitWidth: 300
        anchors.centerIn: parent
        clip: true

        model: searchFilterModel

        delegate: RowLayout {
            width: parent.width

            Label {
                id: site
                text: model.site
                font.pixelSize: 16
                verticalAlignment: Text.AlignVCenter

                Layout.fillHeight: true
                Layout.fillWidth: true
            }
            Label {
                id: login
                text: qsTr("•").repeat(model.login.length)
                font.pixelSize: 26

                verticalAlignment: Text.AlignVCenter
                Layout.fillHeight: true
                Layout.preferredWidth: parent.width * 0.3
            }
            Label  {
                id: password
                text: qsTr("•").repeat(model.password.length)
                font.pixelSize: 26

                verticalAlignment: Text.AlignVCenter
                Layout.fillHeight: true
                Layout.preferredWidth: parent.width * 0.3
            }
            CheckBox {
                checked: model.deleted
                onClicked: model.deleted = checked
                Layout.fillHeight: true
            }
        }
    }

    GridLayout {
        anchors.top: dataList.bottom
        anchors.horizontalCenter: dataList.horizontalCenter
        Button {
            text: "Create"
            onClicked: dataPage.entryCreateClicked()
        }
        Button {
            text: "Delete"
            onClicked: dataPage.entryDeleteClicked()
        }
    }



}
