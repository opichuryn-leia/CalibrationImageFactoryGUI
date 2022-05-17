import QtQuick 2.4

LFSFlatButton {
    id: root

    property bool underline: true

    frameItem {
        border {
            width: 2
            color: root.hovered ? "black" : "transparent"
        }
    }

    textItem {
        color: "black"
        font.underline: root.underline
    }
}
