import QtQuick 2.12
import QtQuick.Controls 1.4

import globals 1.0 as GL

Rectangle {
    id: root

    property int expandedHeight: 55
    property int collapsedHeight: 0
    property bool processing: false
    property int filesLoaded: 0
    readonly property alias trashIconHovered: _expander.hovered

    function open() {
        root.height = root.expandedHeight;
        root.opacity = 1;
    }

    function close() {
        root.height = root.collapsedHeight;
        root.opacity = 0;
    }

    signal closeButtonTriggered();

    height: 0
    color: "black"
    clip: true

    Behavior on height {
        NumberAnimation {}
    }

    Behavior on opacity {
        NumberAnimation { duration: 1000 }
    }

    LFSText {
        anchors {
            left: _indicatorArea.right
            right: _expander.left
            margins: 20
            verticalCenter: _indicatorArea.verticalCenter
        }

        text:  {
            if (root.processing) {
                return "Loading " + root.filesLoaded;
            }
            return "Loaded " + root.filesLoaded;
        }
    }

    Item {
        id: _indicatorArea

        width: 35
        height: width
        anchors {
            verticalCenter: _expander.verticalCenter
            left: parent.left
            leftMargin: 10
        }

        BusyIndicator {
            anchors {
                fill: parent
            }

            running: visible
            visible: root.processing
        }

        Rectangle {
            anchors.fill: parent
            radius: width / 2
            color: "transparent"

            border {
                width: 2
                color: "white"
            }

            visible: !root.processing

            Image {
                anchors.centerIn: parent
                source: "qrc:/checked.png"
            }
        }
    }

    LFSFlatButton {
        id: _expander

        anchors {
            top: parent.top
            right: parent.right
            margins: 10
        }

        width: 35
        height: width
        icon: "qrc:/trash.png"
        onTriggered: {
            root.closeButtonTriggered();
        }
    }

    MouseArea {
        anchors.fill: parent
        z: -1
    }
}
