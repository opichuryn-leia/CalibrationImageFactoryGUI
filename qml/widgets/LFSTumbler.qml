import QtQuick 2.4
import QtQuick.Controls 2.4
import QtGraphicalEffects 1.0

import globals 1.0 as GL

/*!
\qmltype LFSTumbler
\inqmlmodule widgets
\ingroup widgets
\inherits Row
\brief Slider to xor binary state with text of left

\qml

FLSTumbler {
    // set initial value for text field
    checked: true
    // 'checked' property indicates checked/unchecked states
    text: "Caption:
}
*/

Row {
    id: root

    property bool checked: false
    property alias text: _textItem.text
    property alias textItem: _textItem

    spacing: 10

    LFSText {
        id: _textItem
        anchors.verticalCenter: root.verticalCenter
    }

    LFSToggleSlider {
        id: _slider

        anchors.verticalCenter: root.verticalCenter
        value: root.checked
        onCheckedChanged:  {
            root.checked = _slider.checked;
        }
    }
}
