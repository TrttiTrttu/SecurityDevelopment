import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Page {
    id: dataPage

    signal entryCreateClicked()
    signal entryDeleteClicked()

    signal copyToBuffer(int num, string type)

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
                text: qsTr("•").repeat(5)
                font.pixelSize: 26

                verticalAlignment: Text.AlignVCenter
                Layout.fillHeight: true
                Layout.preferredWidth: parent.width * 0.3

                MouseArea {
                    anchors.fill: parent
                    acceptedButtons: Qt.AllButtons
                    onClicked: dataPage.copyToBuffer(model.row, "login")
                }
            }
            Label  {
                id: password
                text: qsTr("•").repeat(5)
                font.pixelSize: 26

                verticalAlignment: Text.AlignVCenter
                Layout.fillHeight: true
                Layout.preferredWidth: parent.width * 0.3

                MouseArea {
                    anchors.fill: parent
                    onClicked: dataPage.copyToBuffer(model.row, "password")
                }
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
