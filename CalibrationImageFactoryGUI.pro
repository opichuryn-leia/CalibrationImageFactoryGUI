QT += quick

CONFIG += c++14

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        main.cpp \
        src/ApplicationDefaults.cpp \
        src/CalibrationModelItem.cpp \
        src/CalibrationModel.cpp \
        src/CalibrationFactory.cpp \
        src/CalibrationView.cpp

HEADERS += \
        src/ApplicationDefaults.h \
        src/CalibrationModelItem.h \
        src/CalibrationModel.h \
        src/CalibrationView.h \
        src/CalibrationFactory.h \
        src/TextFieldDoubleValidator.h

RESOURCES += qml.qrc \
    images/images.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

QML_IMPORT_PATH += $$_PRO_FILE_PWD_/qml