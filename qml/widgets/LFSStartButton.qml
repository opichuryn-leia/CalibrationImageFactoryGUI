import QtQuick 2.12

Rectangle {

    signal toggle
    property bool isProcessing: false
    property bool isHovered: false
    property real progress: 0.00
    property string statusMsg: "Start conversion"


    property color primaryColor: "#FFC99FFF"
    property color opaqueColor: "#50C99FFF"

    color: "transparent"
    width: 220; height: width + 50

//    border { width: 1; color: "white" }

    Canvas {
        id: canvas
        anchors.top: parent.top
        anchors.horizontalCenter: parent.horizontalCenter
        width: 150; height: width
        antialiasing: true

        property real centerWidth: width / 2
        property real centerHeight: height / 2
        property real thickness: 6
        property real radius: Math.min(width, height) / 2 - thickness

        // this is the angle that splits the circle in two arcs
        // first arc is drawn from 0 radians to angle radians
        // second arc is angle radians to 2*PI radians
        property real angle: progress * 2 * Math.PI

        // we want both circle to start / end at 12 o'clock
        // without this offset we would start / end at 9 o'clock
        property real angleOffset: -Math.PI / 2

        onAngleChanged: requestPaint()

        onPaint: {
            var ctx = getContext("2d");
            ctx.save();

            ctx.clearRect(0, 0, width, height);

            ctx.beginPath();
            ctx.lineWidth = thickness;
            ctx.strokeStyle = opaqueColor;
            ctx.arc(centerWidth, centerHeight, radius, angleOffset + angle, angleOffset + 2*Math.PI);
            ctx.stroke();


            // Second, thicker arc
            // From 0 to angle

            ctx.beginPath();
            ctx.lineWidth = thickness;
            ctx.strokeStyle = primaryColor;
            ctx.arc(centerWidth, centerHeight, radius, angleOffset, angleOffset + angle);
            ctx.stroke();

            ctx.restore();
        }

        Rectangle {
            anchors.centerIn: parent
            width: 112; height: width; radius: height / 2
            color: primaryColor
            opacity: isHovered ? 1.0 : 0.9

            Image {
                width: isProcessing ? 34 : 44
                fillMode: Image.PreserveAspectFit
                anchors.centerIn: parent
                source: isProcessing ? "qrc:/preproc360_stop.png" : "qrc:/preproc360_start.png"
            }

        }

        MouseArea {
            anchors.fill: parent
            hoverEnabled: true
            onEntered: { isHovered = true; /*console.log("StartButton isProcessing " + isProcessing)*/ }
            onExited: isHovered = false
            onClicked: toggle()
        }

    }

    LFSText {
        anchors.top: canvas.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.topMargin: 10
        color: primaryColor
        text: statusMsg
    }

}
