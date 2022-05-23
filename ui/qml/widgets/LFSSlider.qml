import QtQuick 2.4
import QtQuick.Controls 2.4
import QtGraphicalEffects 1.0

import globals 1.0 as GL

/*!
\qmltype LFSSlider
\inqmlmodule widgets
\ingroup widgets
\inherits Slider
\brief Slider to xor binary state
\qml

LFSSlider {
    // set initial value for text field
    value: 0 // 0 - false, 1 - true
}
*/

Slider {
    id: root

    property bool active: true

    implicitWidth: 48
    implicitHeight: 28

    leftPadding: 0
    rightPadding: 0

    stepSize: 1
    from: 0
    to: 1
    value: from
    live: true

    background: Item {
            width: root.width
            height: root.height / 2
            y: root.height / 4

            Rectangle {
                id: _grooveRect
                anchors.fill: parent
                color: root.active ? GL.Globals.themeColor : "#B3B3B3"
                radius: height / 2
            }

            // box-shadow: 2px 2px 8px 2px #11071D 34% inset;
            InnerShadow {
                anchors.fill: _grooveRect
                cached: true
                // -2 ... 2
                horizontalOffset: root.visualPosition * 4 - 2
                verticalOffset: 2
                radius: 8
                samples: 16
                spread: 0.8
                color: "#2211071D"
                smooth: true
                source: _grooveRect
            }
        }

    handle: Rectangle {
        x: root.leftPadding + root.visualPosition * (root.availableWidth - width)
        y: root.topPadding + root.availableHeight / 2 - height / 2
        height: root.height
        width:  height
        color: "white"
        radius: height
    }
}
