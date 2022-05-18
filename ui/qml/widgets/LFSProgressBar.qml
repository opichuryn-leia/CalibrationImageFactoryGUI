import QtQuick 2.4
import QtQuick.Controls 2.0

import globals 1.0 as GL

ProgressBar {
    id: root

    implicitWidth: 200
    implicitHeight: 6

    from: 0
    to: 100
    value: 0

    background: Rectangle {
        width: root.width
        height: root.height
        color: "white"
        radius: 3
    }

    contentItem: Item {
        width: root.width
        height: root.height * 0.8

        Rectangle {
            width: root.visualPosition * parent.width
            height: parent.height
            radius: 2
            color: GL.Globals.themeColor
        }
    }
}
