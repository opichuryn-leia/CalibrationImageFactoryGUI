import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 1.4

import widgets 1.0 as WD
import globals 1.0 as GL
import Leia 1.0

Window {
    width: 640
    height: 480
    visible: true
    title: qsTr("CalibrationImageFactoryGUI")
    flags: Qt.WindowStaysOnTopHint|Qt.Sheet|Qt.WindowTitleHint|Qt.WindowCloseButtonHint|Qt.WindowMinimizeButtonHint

    onClosing: {
        calibrationView.close();
    }

    Rectangle {
        id: _scene

        anchors.fill: parent
        color: "#282828"

        Item {
            id: _topPlaceholder

            anchors {
                top: _scene.top
                left: parent.left
                right: parent.right
            }

            height: 30
        }

        WD.LFSText {
            id: _selectText

            anchors {
                top: _scene.top
                left: _scene.left
                leftMargin: 10
                verticalCenter: _topPlaceholder.verticalCenter
            }
            text: "Select Calibration Pattern: "
        }

        WD.LFSLineDivider {
            id: _divider1

            anchors {
                top: _topPlaceholder.bottom
                left: parent.left
                right: parent.right
            }
        }

        ListView {
            id: _listView

            anchors {
                top: _divider1.bottom
                bottom: _divider2.top
                left: _scene.left
//                right: _scene.right
                margins: 10
            }

            width: 250

            model: _model
            spacing: 20

            delegate: Rectangle {
                color: "transparent"
                border {
                    width: 2
                    color: "white"
                }

                width: Screen.width / 10
                height: Screen.height / 10

                Image {
                    id: _img
                    property string sss: modelItem.source
                    onSssChanged: {
                        console.log("source change " + sss)
                    }

                    source: modelItem.source
                    anchors.fill: parent
                    fillMode: Image.PreserveAspectFit

                    Rectangle {anchors.fill: parent; color: "blue"; opacity: 0.2 }


                    MouseArea {
                        id: _ms
                        anchors.fill: parent
                        hoverEnabled: true
                        onClicked: {
                            calibrationView.showFullScreen();
                            calibrationView.setPicturePath(_model.getCalibrationImagePath(modelItem.patternType,
                                                           _widthSpin.value, _heightSpin.value, _rowsSpin.value, _columsSpin.value));
                        }
                    }

                    Rectangle {
                        color: "black"
                        opacity: 0.5
                        height: 40
                        anchors {
                            top: _img.top
                            left: _img.left
                            right: _img.right
                        }

                        visible: _descText.visible
                    }

                    WD.LFSText {
                        id: _descText
                        text: modelItem.description
                        anchors{
                            horizontalCenter: _img.horizontalCenter
                            top: _img.top
                            topMargin: 10
                        }

                        visible: _ms.containsMouse
                    }

                    Rectangle {
                        anchors.fill: parent
                        color: "transparent"
                        radius: 4
                        border {
                            width: 4
                            color: _ms.containsMouse ? GL.Globals.themeColor : GL.Globals.primaryFontColor
                        }
                    }
                }

            }
        }

        Item {
            id: _settingsPanel

            anchors {
                top: _listView.top
                bottom: _listView.bottom
                left: _listView.right
                right: _scene.right
                leftMargin: 40
            }

            WD.LFSText {
                id: _manualText

                anchors {
                    top: _settingsPanel.top
                    left: _settingsPanel.left
                    leftMargin: 10
                }
                text: "Manual settings "
            }

            WD.LFSSpinBoxDouble {
                id: _widthSpin

                anchors {
                    top: _manualText.bottom
                    topMargin: 10
                    left: _settingsPanel.left
                    leftMargin: 40
                }

                title.width: 100
                step: 1
                min: 1
                max: 10000
                caption: "Width: "
                value: appDefaults.metric.width
                decimals: 0
            }

            WD.LFSSpinBoxDouble {
                id: _heightSpin

                anchors {
                    top: _widthSpin.bottom
                    topMargin: 10
                    left: _widthSpin.left
                }

                step: 1
                min: 1
                title.width: 100
                max: 10000
                caption: "Height: "
                value: appDefaults.metric.height
                decimals: 0
            }

            WD.LFSSpinBoxDouble {
                id: _rowsSpin

                anchors {
                    top: _heightSpin.bottom
                    topMargin: 10
                    left: _heightSpin.left
                }

                title.width: 100
                step: 1
                min: 1
                max: 10
                caption: "Rows: "
                value: appDefaults.metric.rows
                decimals: 0
            }

            WD.LFSSpinBoxDouble {
                id: _columsSpin

                anchors {
                    top: _rowsSpin.bottom
                    topMargin: 10
                    left: _rowsSpin.left
                }

                title.width: 100
                step: 1
                min: 1
                max: 10
                caption: "Columns: "
                value: appDefaults.metric.columns
                decimals: 0
            }

            WD.LFSFlatButton {
                id: _resetButton
                text: "Reset to defaults"
                anchors {
                    left: _manualText.left
                    top: _columsSpin.bottom
                    topMargin: 10
                }
                height: implicitHeight + 10

                onTriggered: {
                    _widthSpin.setValue(appDefaults.metric.width);
                    _heightSpin.setValue(appDefaults.metric.height);
                    _rowsSpin.setValue(appDefaults.metric.rows);
                    _columsSpin.setValue(appDefaults.metric.columns);
                }
            }

            WD.LFSText {
                id: _centerViewText
                anchors {
                    top: _resetButton.bottom
                    topMargin: 40
                    left: _resetButton.left
                }

                text: "CenterView"
            }

            WD.LFSSlider {
                id: _centerViewSpin

                anchors {
                    verticalCenter: _centerViewText.verticalCenter
                    left: _centerViewText.right
                    leftMargin: 10
                }

                stepSize: 0.05
                from: leiaDisplay.centerViewMin
                to: leiaDisplay.centerViewMax
                value: leiaDisplay.centerView
                width: 100
                height: 20

                onValueChanged: {
                    leiaDisplay.centerView = value;
                }
            }

            Connections {
                target: leiaDisplay
                function onCenterViewChanged() {
                    _centerViewSpin.value = leiaDisplay.centerView;
                    _spinner.setValue(leiaDisplay.centerView);
                }
            }

            WD.LFSSpinBoxDouble {
                id: _spinner
                anchors {
                    verticalCenter: _centerViewSpin.verticalCenter
                    left: _centerViewSpin.right
                    leftMargin: 10
                }

                step: 1
                min: leiaDisplay.centerViewMin
                max: leiaDisplay.centerViewMax
                value: leiaDisplay.centerView
                steppersVisile: false
                decimals: 2
                onValueChanged: {
                    leiaDisplay.centerView = value;
                }
            }

            WD.LFSTextField {
                id: _comPortField

                anchors {
                    left: _centerViewText.left
                    top: _centerViewText.bottom
                    topMargin: 10
                }

                width: 100
                readOnly: false
                text: leiaDisplay.comPort
            }

            WD.LFSText {
                id: _legacyText
                text: "IsLegacyDisplay"
                anchors {
                    verticalCenter: _comPortField.verticalCenter
                    left: _comPortField.right
                    leftMargin: 10
                }
            }

            WD.LFSToggleSlider {
                id: _legacySlider
                anchors {
                    verticalCenter: _legacyText.verticalCenter
                    left: _legacyText.right
                    leftMargin: 10
                }
                width: 35
                height: 20
                value: leiaDisplay.isLegacyDisplay
            }

            WD.LFSFlatButton {
                id: _burnButton
                text: "Burn display settings"
                anchors {
                    left: _comPortField.left
                    top: _comPortField.bottom
                    topMargin: 10
                }

                height: implicitHeight + 10

                onTriggered: {
                    console.log("burn clicked");
                    leiaDisplay.isLegacyDisplay = _legacySlider.checked;
                    leiaDisplay.centerView = _centerViewSpin.value;
                    leiaDisplay.comPort = _comPortField.text;
                    leiaDisplay.burnDisplay();
                }
            }
        }

        WD.LFSLineDivider {
            id: _divider2

            anchors {
                top: _bottomPlaceholder.top
                left: parent.left
                right: parent.right
            }
        }

        Item {
            id: _bottomPlaceholder

            anchors {
                bottom: _scene.bottom
                left: parent.left
                right: parent.right
            }

            height: 30
        }
    }

    CalibrationModel {
        id: _model
    }
}
