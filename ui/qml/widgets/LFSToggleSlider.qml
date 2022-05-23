import QtQuick 2.4

/*!
\qmltype LFSToggleSlider
\inqmlmodule widgets
\ingroup widgets
\inherits Slider
\brief Slider to xor binary state
\qml

ToggleSlider {
    // set initial value for text field
    value: 0 // 0 - false, 1 - true
    // 'checked' property indicates checked/unchecked states
}
*/

LFSSlider {
    id: root

    readonly property bool checked: value >= to
    active: checked

    function toggle() {
        if (value <= from) {
            value = to;
        } else {
            value = from;
        }
    }

    onPressedChanged: {
        if (pressed) {
            internal.counter = 0;
        } else if (!pressed && internal.counter <= 0) {
            toggle();
            internal.counter = 0;
        }
    }

    onValueChanged: {
        internal.counter += 1;
    }

    handle: Rectangle {
        x: root.leftPadding + root.visualPosition * (root.availableWidth - width)
        y: root.topPadding + root.availableHeight / 2 - height / 2
        height: root.height
        width:  height
        color: "white"
        radius: height

        Behavior on x {
            XAnimator { duration: 200 }
        }
    }

    QtObject {
        id: internal

        // value change counter between press <-> release
        property real counter: 0
    }
}
