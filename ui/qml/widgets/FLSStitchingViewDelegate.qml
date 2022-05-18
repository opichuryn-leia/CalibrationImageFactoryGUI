import QtQuick 2.12
import QtQuick.Layouts 1.0
import QtQuick.Controls 2.12

import views 1.0 as VW
import globals 1.0 as GL
import widgets 1.0 as WD

import Leia 1.0

Item {
    id: root

    property alias frontImageSource: _frontImage.source
    property alias browsePanelVisible: _browsePanel.visible
    property bool uploadInProgress: false
    property int filesLoaded: 0
    readonly property bool hovered: _ms.containsMouse || _footer.trashIconHovered

    signal fileDropped(var drop);
    signal triggered();
    signal closeButtonTriggered();

    VW.LFSBrowsePanel {
        id: _browsePanel

        anchors {
            fill: parent
        }

        firstLineText: "Drag & Drop your images"
        vectorIconBottomMargin: 40
        textVeticalOffset: height / 5
    }

    Image {
        id: _frontImage

        anchors {
            fill: parent
            margins: 3
        }

        fillMode: Image.PreserveAspectFit
    }

    DropArea {
        id: _dropArea

        anchors.fill: parent
        onDropped: {
            root.fileDropped(drop);
        }
    }

    MouseArea {
        id: _ms
        anchors.fill: parent
        hoverEnabled: true
        onClicked: {
            root.triggered();
        }
    }

    LFSStitchingDelegateFooter {
        id: _footer
        processing: root.uploadInProgress
        filesLoaded: root.filesLoaded

        anchors {
            left: root.left
            right: root.right
            bottom: root.bottom
        }

        onProcessingChanged: {
            if (processing) {
                open();
            }
        }

        onFilesLoadedChanged: {
            if (filesLoaded === 0) {
                close();
            }
        }

        onCloseButtonTriggered: {
            close();
            root.closeButtonTriggered();
        }
    }
}
