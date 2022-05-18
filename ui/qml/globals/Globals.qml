pragma Singleton

import QtQuick 2.4

Item {
    readonly property string fontName: _camptonLoader.status === FontLoader.Ready ?
                                           // use font from resources
                                           _camptonLoader.name :
                                           // otherwise use "Calibri" font as fallback
                                           "Calibri"

    readonly property int primaryFontSize: 18
    readonly property int secondaryFontSize: 14
    readonly property int titleFontSize: 26
    readonly property color primaryFontColor: "#FFFFFF"
    readonly property color inactiveColor: "#B3B3B3"
    readonly property color themeColor: "#C99FFF"
    readonly property color dividerColor: "#3D3D3D"

    FontLoader {
        id: fontCamptonBook
        source: "qrc:/campton-book.otf"
    }

    FontLoader {
        id: _camptonLoader
        source: "qrc:/campton-medium.otf"
    }
}

