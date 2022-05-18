set QT_MSVC_DIR=C:\root\programming\Qt\Qt5.12.1\5.15.2\msvc2015_64\bin
set QT_CREATOR_DIR=C:\root\programming\Qt\Qt5.12.1\Tools\QtCreator\bin
set QT_PLUGIN_PATH=C:\root\programming\Qt\Qt5.12.1\5.15.2\msvc2015_64\plugins
set QML2_IMPORT_PATH=C:\root\programming\Qt\Qt5.12.1\5.15.2\msvc2015_64\qml

set PATH=%QT_MSVC_DIR%;%QT_CREATOR_DIR%;%PATH%;%CD%\build\src

set VS_PATH="C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvars64.bat"
start /D %CD% cmd /k %VS_PATH%
