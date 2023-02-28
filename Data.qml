import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Page {
    id: root

    ListView {
        id: dataList
        implicitHeight: 400
        implicitWidth: 250
        anchors.centerIn: parent
        clip: true

        model: MyModel

        delegate: RowLayout {
            width: parent.width

            TextEdit {
                text: model.site
                readOnly: true
                Layout.fillWidth: true
            }
            TextEdit {
                text: model.login
                readOnly: true
                Layout.fillWidth: true
            }
            TextEdit {
                text: model.password
                readOnly: true
                Layout.fillWidth: true
            }
        }
    }

}
