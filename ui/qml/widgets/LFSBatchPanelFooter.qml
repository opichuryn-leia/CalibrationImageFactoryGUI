import QtQuick 2.12
import QtQuick.Controls 1.4

import globals 1.0 as GL

Rectangle {
    id: root

    property bool expanded: false
    property int expandedHeight: 200
    property int collapsedHeight: 55

    readonly property bool hasJobs: videoModel.fsJobs.count > 0

    function getHeightValue() {
        return root.expanded ? root.expandedHeight : root.collapsedHeight
    }

    height: getHeightValue()
    color: "black"
    z: 5
    opacity: 0

    onHasJobsChanged: {
        if (hasJobs) {
            _closeTimer.stop();
            root.height = Qt.binding(getHeightValue);
            root.opacity = 1;
        } else {
            _closeTimer.start();
        }
    }

    Timer {
        id: _closeTimer
        interval: 1500
        onTriggered: {
            root.height = 0;
            root.opacity = 0;
            root.expanded = false;
        }
    }

    Behavior on height {
        NumberAnimation {}
    }

    Behavior on opacity {
        NumberAnimation { duration: 1000 }
    }

    LFSText {
        anchors {
            left: _indicatorArea.right
            right: _expander.left
            margins: 20
            verticalCenter: _indicatorArea.verticalCenter
        }

        text:  {
            if (root.hasJobs) {
                return "Loading " + videoModel.count;
            }
            return "Loaded " + videoModel.count
        }
    }

    Item {
        id: _indicatorArea

        width: 35
        height: width
        anchors {
            verticalCenter: _expander.verticalCenter
            left: parent.left
            leftMargin: 10
        }

        BusyIndicator {
            anchors {
                fill: parent
            }

            running: visible
            visible: root.hasJobs
        }

        Rectangle {
            anchors.fill: parent
            radius: width / 2
            color: "transparent"

            border {
                width: 2
                color: "white"
            }

            visible: !root.hasJobs

            Image {
                anchors.centerIn: parent
                source: "qrc:/checked.png"
            }
        }
    }

    LFSFlatButton {
        id: _expander

        anchors {
            top: parent.top
            right: parent.right
            margins: 10
        }

        enabled: root.hasJobs
        width: 35
        height: width
        icon: root.expanded  ? "qrc:/arrow_down.png" : "qrc:/arrow_top.png"
        onTriggered: {
            root.expanded = !root.expanded;
        }
    }

    LFSLineDivider {
        anchors {
            left: parent.left
            right: parent.right
            bottom: _coll.top
        }
    }

    Column {
        id: _coll

        anchors {
            top: _indicatorArea.bottom
            topMargin: 15
            left: parent.left
            right: parent.right
        }

        Repeater {
            model: Math.min(videoModel.fsJobs.count, 3)

            Item {
                width: parent.width
                height: 45

                LFSText {
                    anchors {
                        left: parent.left
                        right: _cancelBtn.left
                        margins: 10
                        verticalCenter: parent.verticalCenter
                    }

                    text: {
                        if (index == 2) {
                            return "+" + (videoModel.fsJobs.count - 2) + " more";
                        }
                        return videoModel.fsJobs.get(index).url;
                    }
                }

                LFSFlatButton {
                    id: _cancelBtn

                    visible: index < 2
                    anchors {
                        right: parent.right
                        rightMargin: 10
                        verticalCenter: parent.verticalCenter
                    }

                    width: 35
                    height: width
                    icon: "qrc:/close.png"

                    onTriggered: {
                        videoModel.cancelFSJob(videoModel.fsJobs.get(index).id);
                    }
                }

                LFSLineDivider {
                    anchors {
                        left: parent.left
                        right: parent.right
                        bottom: parent.bottom
                    }
                }
            }
        }
    }

    MouseArea {
        anchors.fill: parent
        z: -1
    }
}
