import QtQuick 2.4
import QtQuick.Layouts 1.0

import globals 1.0 as GL

Item {
    id: root

    property alias text: _txt.text
    property alias icon: _icon.source

    property int paddingLeft: 10
    property int paddingRight: paddingLeft
    property int spacingMiddle: 0

    property alias textItem: _txt
    property alias iconItem: _icon
    property alias frameItem: _frame

    property alias hovered: _mouseArea.containsMouse
    signal triggered()

    implicitWidth: _row.implicitWidth
    implicitHeight: Math.max(_icon.implicitHeight, _txt.implicitHeight)

    RowLayout {
        id: _row

        anchors.fill: parent

        Item {
            id: _placeholderLeft
            width: root.paddingLeft
        }

        Item {
            Layout.fillWidth: _icon.source && _icon.status === Image.Ready
            Layout.fillHeight: Layout.fillWidth

            Image {
                id: _icon
                anchors.centerIn: parent
            }
        }

        Item {
            id: _placeholderMiddle
            width: _txt.text !== "" && _icon.status === Image.Ready ? root.spacingMiddle : 0
        }

        LFSText {
            id: _txt
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            Layout.fillWidth: true
            Layout.fillHeight: true
            elide: Text.ElideRight
            visible: text
            color: root.enabled ? "white" : GL.Globals.inactiveColor
        }

        Item {
            id: _placeholderRight
            width: root.paddingRight
        }
    }

    Rectangle {
        id: _frame

        anchors.fill: parent
        radius: 4
        color: "transparent"

        border {
            width: 2
            color: root.hovered ? "white" : GL.Globals.inactiveColor
        }
    }

    MouseArea {
        id: _mouseArea

        anchors.fill: parent
        hoverEnabled: true

        onClicked: {
            root.triggered();
        }
    }
}
