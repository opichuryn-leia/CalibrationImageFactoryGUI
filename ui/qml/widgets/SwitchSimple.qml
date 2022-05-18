import QtQuick 2.12

Rectangle {
	property bool checked: false

	signal toggled

    color: "transparent"

    width: title.width + spacing + w; height: Math.max(sw.height, title.height);

    property alias font: title.font
    property alias caption: title.text
    property bool isSwitchNotButton: true
    property bool isHover: false
    property int w: 48
    property int h: 24
    property int hR: 28
    property int spacing: 20

    property color clr_purple: "#C99FFF"

    Text {
        id: title
        opacity: enabled ? 1.0 : 0.3
        color: (checked && isSwitchNotButton) ? clr_purple : "white"
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        font.pixelSize: 18
    }

    Rectangle {
        visible: isSwitchNotButton
        anchors.verticalCenter: parent.verticalCenter
        anchors.margins: spacing
        id: sw
        anchors.left: title.right
        width: w; height: h; radius: height / 2
        color: (checked) ? clr_purple : "#B3B3B3"

        Rectangle {
            anchors.verticalCenter: parent.verticalCenter

            color: "white"

            height: hR
            width: height
            radius: height / 2

            x: (checked) ? parent.width - width : 0
        }
    }


    Rectangle {
        anchors.left: title.right
        visible: !isSwitchNotButton
        width: 140
        height: h
        radius: height / 5
        color: isHover ? clr_purple : "black"
        border { width: 1; color: "white" }
        anchors.margins: spacing

        Text {
            anchors.centerIn: parent
            text: checked ? "Clockwise" : "AntiClockwise"
            color: "white"
            font.pixelSize: 18
        }

    }

    MouseArea {
        anchors.fill: parent
        hoverEnabled: true
        onEntered: isHover = true
        onExited: isHover = false
        onClicked: {
            checked = !checked
            toggled()
        }
    }
}

