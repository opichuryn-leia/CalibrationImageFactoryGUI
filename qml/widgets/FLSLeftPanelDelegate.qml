import QtQuick 2.4
import globals 1.0 as GL

Rectangle {
    id: root

    property bool selected: false
    property alias text: _txt.text
    property alias iconItem: _icon
    readonly property alias hovered: _ms.containsMouse

    signal triggered()

    color: {
        if (root.selected) {
            return "#44C99FFF";
        } else if (root.hovered) {
            return "#5D5D5D";
        }
        return "transparent";
    }

    opacity: enabled ? (selected ? 0.75 : 1) : 0.1

    Column {
        anchors.centerIn: parent
        width: root.width
        spacing: 14

        Image {
            id: _icon
            anchors.horizontalCenter: parent.horizontalCenter
        }

        LFSText {
            id: _txt

            width: parent.width
            wrapMode: Text.WordWrap
            horizontalAlignment: Text.AlignHCenter
            color: "#c8a6f8"
            font.pixelSize: 14
        }
    }

    MouseArea {
        id: _ms

        anchors.fill: parent
        hoverEnabled: true
        onClicked: {
            root.triggered();
        }
    }
}
