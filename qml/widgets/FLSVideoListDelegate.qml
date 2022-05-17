import QtQuick 2.4
import QtQuick.Layouts 1.0
import QtQuick.Controls 2.12

import globals 1.0 as GL

Rectangle {
    id: root

    property int entryNumber: 0
    property string entryShortName: ""
    property string entryFullName: ""
    property real progress: 0
    property alias playIconSource: _playImage.source
    property alias statusText: _statusLabel.text
    property alias checkIndicatorVisible: _checkedBox.visible
    property bool checkIndicatorValue: false

    signal trashIconTriggered()
    signal playIconTriggered()
    signal entryTriggered()
    signal checkBoxTriggered();

    width: 100
    height: 70
    color: "transparent"
    radius: 5

    border {
        width: 2
        color: "white"
    }


    LFSFlatButton {
        id: _checkedBox

        anchors {
            left: root.left
            leftMargin: 10
            verticalCenter: parent.verticalCenter
        }

        height: 35
        width: visible ? height : 0
        icon: root.checkIndicatorValue ? Qt.resolvedUrl("qrc:/checked.png") : ""

        onTriggered: {
            root.checkBoxTriggered();
        }
    }

    LFSText {
        id: _fileName

        anchors {
            left: _checkedBox.right
            right: _trashIcon.left
            top: parent.top
            topMargin: 10
            leftMargin: 10
            rightMargin: 20
        }

        text: root.entryNumber + ". " + root.entryShortName
        elide: Text.ElideRight

        MouseArea {
            id: _fileNameMouseArea
            anchors.fill: parent
            hoverEnabled: true
        }
    }

    ToolTip {
        parent: _fileName
        delay: 500
        visible: _fileNameMouseArea.containsMouse
        text: root.entryFullName
    }

    LFSProgressBar {
        id: _progress

        anchors {
            top: _fileName.bottom
            topMargin: 10
            left: _fileName.left
        }
        value: root.progress
    }

    LFSText {
        id: _statusLabel

        anchors {
            left: _progress.right
            leftMargin: 10
            right: _fileName.right
            verticalCenter: _progress.verticalCenter
        }
        elide: Text.ElideRight
    }

    Image {
        id: _trashIcon

        anchors {
            verticalCenter: parent.verticalCenter
            right: _playIcon.left
            rightMargin: 20
        }

        source: Qt.resolvedUrl("qrc:/trash.png")

        MouseArea {
            anchors.fill: parent
            onClicked: {
                root.trashIconTriggered();
            }
        }
    }

    Rectangle {
        id: _playIcon

        anchors {
            right: parent.right
            rightMargin: 20
            verticalCenter: parent.verticalCenter
        }

        height: parent.height / 2
        width: height
        radius: height
        color: GL.Globals.themeColor
        border {
            width: 2
            color: _playMouseArea.containsMouse ? "white" : "transparent"
        }

        Image {
            id: _playImage
            anchors.centerIn: parent
            width: parent.width / 2
            height: width
            fillMode: Image.PreserveAspectFit
        }

        MouseArea {
            id: _playMouseArea
            anchors.fill: parent
            hoverEnabled: true
            onClicked: {
                root.playIconTriggered();
            }
        }
    }

    MouseArea {
        anchors.fill: parent
        propagateComposedEvents: true
        onPressed: {
            root.entryTriggered();
            mouse.accepted = false;
        }
    }
}
