import QtQuick 2.4
import globals 1.0 as GL

Text {
    font.family: GL.Globals.fontName
    font.pixelSize: GL.Globals.primaryFontSize
    color: enabled ? GL.Globals.primaryFontColor : GL.Globals.inactiveColor
    elide: Text.ElideRight
}
