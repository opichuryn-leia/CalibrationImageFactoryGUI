import QtQuick 2.12

/*!
\qmltype LFSSpinBoxDouble
\inqmlmodule widgets
\ingroup widgets
\inherits Row
\brief Up/Down widget for Double value

\qml

LFSSpinBoxDouble {
    min: 1
    max: 100
    // set initial value for text field
    inValue: 50
    // 'value' property indicates selected/entered number
}
*/
Row {
    id: root

    property real step: 0.1
    property alias min: _input.min
    property alias max: _input.max
    property alias font: _title.font
    property alias caption: _title.text
    property alias title: _title
    property real value
    property alias decimals: _input.decimals
    property bool steppersVisile: true

    signal inputAccepted();

    function increment() {
        _input.setValue(Math.min(root.max, root.value + root.step))
    }

    function decrement() {
        _input.setValue(Math.max(root.min, root.value - root.step));
    }

    function setValue(newValue)
    {
        _input.setValue(newValue);
    }

    width: childrenRect.width
    height: 30
    spacing: 8

    LFSText {
        id: _title
        anchors.verticalCenter: root.verticalCenter
    }

    Item {
        // spacer
        width: _title === "" ? 0 : 5
        height: root.height
    }

    Image {
        height: root.height - 5
        width: root.steppersVisile ? height : 0
        anchors.verticalCenter: root.verticalCenter
        source: "qrc:/minus.png"
        visible: root.steppersVisile

        MouseArea {
            id: _decrementBtn
            anchors.fill: parent
        }
    }

    Item {
        id: _inputArea
        height: parent.height
        width: Math.max(_minMetric.width, _maxMetric.width)

        TextMetrics {
            id: _minMetric
            font: _input.font
            text: _input.textFromValue(_input.min, _input.locale, _input.decimals);
        }

        TextMetrics {
            id: _maxMetric
            font: _input.font
            text: _input.textFromValue(_input.max, _input.locale, _input.decimals);
        }

        TextFieldDouble {
            id: _input

            leftPadding: 0
            rightPadding: 0
            horizontalAlignment: Text.AlignHCenter
            anchors.centerIn: parent
            font.preferShaping: true
            background: Item {}

            Keys.onUpPressed: { root.increment(); }
            Keys.onDownPressed: { root.decrement(); }

            onAccepted: {
                root.inputAccepted();
            }
        }
    }

    Image {
        height: root.height - 5
        width: root.steppersVisile ? height : 0
        anchors.verticalCenter: root.verticalCenter
        source: "qrc:/plus.png"
        visible: root.steppersVisile

        MouseArea {
            id: _incrementBtn
            anchors.fill: parent
        }
    }

    Timer {
        id: _timer

        interval: 50

        running: _incrementBtn.containsPress || _decrementBtn.containsPress
        repeat: true
        onTriggered: {
            if (_incrementBtn.containsPress) {
                root.increment();
            } else {
                root.decrement();
            }
        }
    }

    Component.onCompleted: {
        let copy = root.value;
        _input.setValue(copy);
        root.value = Qt.binding(() => _input.value);
    }
}
