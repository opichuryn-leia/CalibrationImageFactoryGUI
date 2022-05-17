import QtQuick 2.0
import QtQuick.Controls 2.12
import QtQml 2.12
import Leia 1.0

import globals 1.0 as GL

/*!
\qmltype TextFieldDouble
\inqmlmodule widgets
\ingroup widgets
\inherits TextField
\brief Text field for double range-based input

\qml

TextFieldDouble {
    min: 1
    max: 100
    // set initial value for text field
    decimals: 1
    // 'value' property indicates entered number
}
*/
TextField {
    id: root

    // min bound for value
    property real min: 1
    // max bound for value
    property real max: 100
    // allowed number of values after the dot
    property int decimals: 2
    // locate for decimal value representation
    property var locale: defaultLocale
    // getter for current value
    readonly property alias value: internal.value

    function setValue(inValue) {
        internal.syncTextFromOuside(inValue);
    }

    function textFromValue(value, locale, delimals) {
        let result = "";
        try {
          result = Number(value).toLocaleString(locale, 'f', decimals);
        } catch (error) {}
        return result;
    }

    function valueFromText(text, locale) {
        let result = root.min;
        try {
          result = Number.fromLocaleString(locale, text);
        } catch (error) {}
        return result;
    }

    font.family: GL.Globals.fontName
    font.pixelSize: GL.Globals.primaryFontSize
    color: enabled ? GL.Globals.primaryFontColor : GL.Globals.inactiveColor
    selectByMouse: true

    validator: TextFieldDoubleValidator {
        top: root.max
        bottom: root.min
        decimals: root.decimals
        locale: root.locale
    }

    QtObject {
        id: internal

        property real value: {
            const value = valueFromText(root.text, root.locale);
            return isNaN(value) ? root.min : value;
        }

        function syncTextFromOuside(inValue) {
            root.text = textFromValue(inValue, root.locale, root.decimals);
        }

        function syncTextFromInside() {
            root.text = textFromValue(root.value, root.locale, root.decimals);
        }
    }

    onFocusChanged: {
        internal.syncTextFromInside();
    }

    Keys.onReturnPressed: {
        internal.syncTextFromInside();
    }

    Keys.onEnterPressed: {
        internal.syncTextFromInside();
    }
}
