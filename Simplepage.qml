import QtQuick
import QtQuick.Controls

Page {
    id: root
    property alias buttonText: navButton.text
    signal buttonClicked();

    Button {
        id: navButton
        anchors.centerIn: parent
        visible: text.length > 0
        onClicked: {
            root.buttonClicked();
        }
    }
}
