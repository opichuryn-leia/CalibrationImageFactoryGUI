import QtQuick 2.0
import QtQuick.Controls 2.12
import globals 1.0 as GL

/*!
\qmltype LFSTextField
\inqmlmodule widgets
\ingroup widgets
\inherits TextField
\brief Text field for double range-based input

\qml

LFSTextField {
    min: 1
    max: 100
    // set initial value for text field
    inValue: 50
    decimals: 1
    // 'value' property indicates entered number
}
*/
TextField {
    id: root

    property alias tooltipVisible: _tooltip.visible

    implicitWidth: 200
    implicitHeight: 40

    background: Rectangle { color: "black"; radius: 5 }

    horizontalAlignment: Text.AlignLeft
    verticalAlignment: Text.AlignVCenter
    font.family: GL.Globals.fontName
    font.pixelSize: GL.Globals.secondaryFontSize
    color: enabled ? GL.Globals.primaryFontColor : GL.Globals.inactiveColor
    readOnly: true
    selectByMouse: true
    hoverEnabled: true

    ToolTip {
        id: _tooltip
        parent: root
        delay: 500
        visible: root.hovered && root.text
        text: root.text
    }
}
