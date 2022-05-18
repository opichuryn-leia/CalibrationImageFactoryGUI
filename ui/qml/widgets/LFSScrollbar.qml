import QtQuick 2.4
import globals 1.0 as GL

Item {
    id: root

    /*! Set to the list view which should be decorated by this scrollbar */
    property Flickable view : null

    /*! This property holds the list the widget is attached to */
    property Flickable attachTo: root.view

    property int topOffset: 0
    property int bottomOffset: 0

    readonly property real position: attachTo && attachTo.visibleArea ? attachTo.visibleArea.yPosition : 0
    readonly property real pageSize: attachTo && attachTo.visibleArea ? attachTo.visibleArea.heightRatio : 1.0

    //! calculated y position of scroll handle
    readonly property int yPosition:  Math.max(Math.min(root.position * _scrollbar.height, maximumYPosition), 0)
    //! maximum y position of scroll handle
    property real maximumYPosition: _scrollbar.height - _handle.height
    //! height of scroll handle
    property real scrollHandleHeight: root.pageSize * _scrollbar.height
    //! minimum height of scrollbar handle
    readonly property int minScrollHandleHeight: 50

    readonly property bool handleIsDragged: _ms.drag.active

    implicitWidth: 8
    visible: root.pageSize < 1.0

    Item {
        id: _scrollbar

        anchors {
            fill: parent
            topMargin: root.topOffset
            bottomMargin: root.bottomOffset
        }

        Rectangle {
            id: _background
            color: "gray"
            anchors.fill: parent
            radius: width / 2
        }

        MouseArea {
            id: _ms

            anchors.fill: _scrollbar

            drag {
                target: _handle
                maximumY: _ms.height - _handle.height
                minimumY: 0
            }

            Rectangle {
                id: _handle

                anchors.horizontalCenter: parent.horizontalCenter
                height:  root.scrollHandleHeight < root.minScrollHandleHeight ? root.minScrollHandleHeight : root.scrollHandleHeight
                width: _ms.drag.active? _background.width + 4 : _background.width - 2
                color: GL.Globals.themeColor
                radius: width / 2

                border {
                    width: 2
                    color: _ms.drag.active ? "white" : "transparent"
                }

                Binding {
                    target: _handle
                    when: !_ms.drag.active
                    property: "y"
                    value: root.yPosition
//                    restoreMode: Binding.RestoreNone
                }

                Binding {
                    target: root.attachTo
                    when: _ms.drag.active
                    property: "contentY"
                    value: _handle.y + _handle.height >= _scrollbar.height ?
                               root.attachTo.contentHeight - root.attachTo.height :
                               _handle.y / _ms.height * root.attachTo.contentHeight
//                    restoreMode: Binding.RestoreNone
                }

                Drag.active: _ms.drag.active
            }
        }
    }
}
