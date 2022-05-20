#include "LeiaDisplay.h"

#include <iostream>
#include <windows.h>
#include <tchar.h>
#include <string>
#include <QString>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QTimer>
#include <QFileInfo>
#include <QDir>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QProcess>

typedef void (WINAPI *PGNSI)(LPSYSTEM_INFO);
typedef void(__cdecl *MINT_VOID) (int); // a pointer to a process that takes 1 int arg and returns a void
typedef bool(__cdecl *MATOMIC_BOOL) (); // a pointer to a process that takes no args and returns a bool
typedef char*(__cdecl *MATOMIC_PCHAR) (); // a pointer to a process that takes no args and returns a char*
typedef int*(__cdecl *MATOMIC_PINT)(); // a pointer to a process that takes no args and returns an int*
typedef float*(__cdecl *MATOMIC_PFLOAT)(); // a pointer to a process that takes no args and returns a float*

static HMODULE hmodKernel32 = nullptr;
static HMODULE hmodDisplaySdkCpp = nullptr;
static HMODULE hmodDisplayParams = nullptr;
static MATOMIC_PCHAR fPtrPlatform = nullptr;
static MATOMIC_BOOL fPtrConnected = nullptr;
static MINT_VOID fPtrRequestBacklight = nullptr;
static MATOMIC_PCHAR fPtrGetDisplayConfigString = nullptr;

static const char* invokeFunc(MATOMIC_PCHAR func) {
    __try {
        return func();
    }
    __except  (true) {
        return nullptr;
    }
    return nullptr;
}
static int invokeFunc(MATOMIC_BOOL func, bool &result) {
    __try {
        result = func();
        return 0;
    }
    __except  (true) {
        return 1;
    }
    return 1;
}

static int invokeFunc(MINT_VOID func, int value) {
    __try {
        func(value);
        return 0;
    }
    __except  (true) {
        return 1;
    }
    return 1;
}

LeiaDisplay::LeiaDisplay(QObject* parent)
    : QObject(parent)
{
    fetchDisplayProperties();
    findComPortName();
}

LeiaDisplay &LeiaDisplay::instance()
{
    static LeiaDisplay self;
    return self;
}

int LeiaDisplay::initialize()
{
    hmodKernel32 = LoadLibrary(_T("kernel32.dll"));
    if (hmodKernel32 == nullptr) {
        qWarning() << "could not load kernel32";
        return 1;
    }
    hmodDisplaySdkCpp = LoadLibrary(_T("LeiaDisplaySdkCpp.dll"));
    if (hmodDisplaySdkCpp == nullptr) {
        qWarning() << "could not load LeiaDisplaySdkCpp";
        return 1;
    }
    else {
        qDebug() << "Loaded LeiaDisplaySdkCpp";

        fPtrPlatform = (MATOMIC_PCHAR) GetProcAddress(hmodDisplaySdkCpp, "getPlatformVersion");
        if (fPtrPlatform == nullptr) {
            qWarning() << "\tCould not load function getPlatformVersion";
        }
        else {
            qDebug() << "getPlatformVersion pointer is " << fPtrPlatform;
            const char *result = invokeFunc(fPtrPlatform);
            if(result == nullptr) {
                qWarning() << "fPtrPlatform failed";
                return 1;
            }
            qWarning() << "result is " << QString(result);
        }

        bool isDisplayConnected = false;
        fPtrConnected = (MATOMIC_BOOL)GetProcAddress(hmodDisplaySdkCpp, "isDisplayConnected");
        if (fPtrConnected != nullptr) {
            qWarning() << "isDisplayConnected pointer is " << fPtrConnected;
            if(invokeFunc(fPtrConnected, isDisplayConnected) != 0) {
                qWarning() << "\tCould not execute function isDisplayConnected";
                return 1;
            }
            qWarning() << "result of isDisplayConnected test is " << isDisplayConnected;
        }
        else {
            qWarning() << "\tCould not load function isDisplayConnected";
            return 1;
        }

        if (!isDisplayConnected) {
            qWarning() << "Leia Display not connected";
            return 1;
        }

        fPtrRequestBacklight = (MINT_VOID)GetProcAddress(hmodDisplaySdkCpp, "requestBacklightMode");
        if (fPtrRequestBacklight == nullptr) {
            qWarning() << "\tDisplay was not connected or could not load function requestBacklightMode";
            return 1;
        }
    }

    qWarning() << "before attempting to load LeiaDisplayParams, last code is " << GetLastError();
    hmodDisplayParams = LoadLibrary(_T("LeiaDisplayParams.dll"));

    if (hmodDisplayParams == nullptr) {
        qWarning() << "Could not load LeiaDisplayParams.DLL";
        qWarning() << "last error code " << GetLastError();
        return 1;
    }

    fPtrGetDisplayConfigString = (MATOMIC_PCHAR)GetProcAddress(hmodDisplayParams, "getDisplayConfigString");
    if(fPtrGetDisplayConfigString == nullptr) {
        qWarning() << "\tDisplay was not connected or could not load function requestBacklightMode";
        return 1;
    }

    return 0;
}

int LeiaDisplay::teardown()
{
    if(hmodKernel32 != nullptr)
        FreeLibrary(hmodKernel32);
    if(hmodDisplaySdkCpp != nullptr)
        FreeLibrary(hmodDisplaySdkCpp);
    if(hmodDisplayParams != nullptr)
        FreeLibrary(hmodDisplayParams);
    hmodKernel32 = NULL;
    hmodDisplaySdkCpp = NULL;
    hmodDisplayParams = NULL;
    fPtrPlatform = nullptr;
    fPtrConnected = nullptr;
    fPtrRequestBacklight = nullptr;
    fPtrGetDisplayConfigString = nullptr;
    return 0;
}

int LeiaDisplay::switch2D()
{
    if(fPtrRequestBacklight == nullptr)
        return 1;
    return invokeFunc(fPtrRequestBacklight, 0);
}

int LeiaDisplay::switch3D()
{
    if(fPtrRequestBacklight == nullptr)
        return 1;
    return invokeFunc(fPtrRequestBacklight, 1);
}

bool LeiaDisplay::isConnected()
{
    if(fPtrConnected == nullptr)
        return false;
    bool isDisplayConnected = false;
    if(invokeFunc(fPtrConnected, isDisplayConnected) != 0) {
        qWarning() << "\tCould not execute function isDisplayConnected";
        return false;
    }
    return isDisplayConnected;
}

QString LeiaDisplay::getDisplayConfig()
{
    qDebug() << "getDisplayConfig STEP#1";
    if(fPtrGetDisplayConfigString == nullptr) {
        // Try to reconnect
        teardown();
        initialize();
    }
    qDebug() << "getDisplayConfig STEP#2";
    if(fPtrGetDisplayConfigString == nullptr)
        return "";

    qDebug() << "getDisplayConfig STEP#3";
    const char *result = invokeFunc(fPtrGetDisplayConfigString);
    qDebug() << "getDisplayConfig STEP#4";
    if(result == nullptr)
        return "";
    qDebug() << "getDisplayConfig STEP#5";
    return QString(result);
}

void LeiaDisplay::fetchDisplayProperties()
{
    qDebug() << "fetchDisplayProperties";
    parseConfig(getDisplayConfig());
}

//https://leia3d.atlassian.net/wiki/spaces/LIT/pages/2912878612/TINY2PLUS+Command+List?atlOrigin=eyJpIjoiMDE0ZWRjZGMzYmZmNDdjN2IzOWUzZjBhMzlmMTUyYTciLCJwIjoiY29uZmx1ZW5jZS1jaGF0cy1pbnQifQ
void LeiaDisplay::burnDisplay()
{
//    if(player->getIs3D())
//        switch2D();

    QString command1, command2, command3, command4;
    if(!m_isLegacyDisplay) {
command1 = QString(
"setDisplayConfig  actCoefficients : {\n\
 \"value\" : [%1,%2,%3,%4,%5,%6],\n\
 \"beta\" : %7,\n\
 \"gamma\" : %8\n\
} # \r\n")
            .arg(QString::number(m_properties.act[0], 'g', 6))
            .arg(QString::number(m_properties.act[1], 'g', 6))
            .arg(QString::number(m_properties.act[2], 'g', 6))
            .arg(QString::number(m_properties.act[3], 'g', 6))
            .arg(QString::number(m_properties.act[4], 'g', 6))
            .arg(QString::number(m_properties.act[5], 'g', 6))
            .arg(QString::number(m_properties.act_beta, 'g', 6))
            .arg(QString::number(m_properties.act_gamma, 'g', 6));


command2 = QString("setDisplayConfig  displayGeometry : {\n\
   \"centerView\" : %1,\n\
   \"s\" : %2,\n\
   \"d_over_n\" : %3,\n\
   \"n\" : %4,\n\
   \"pixelPitch\" : %5,\n\
   \"theta\" : %6,\n\
   \"systemDisparity\" : %7,\n\
   \"colorSlant\" : %8,\n\
   \"colorInversion\" : %9,\n\
   \"p_over_du\" : %10,\n\
   \"p_over_dv\" : %11\n\
} # \r\n")
.arg(QString::number(m_properties.centerViewNum, 'g', 6))
.arg(QString::number(m_properties.s, 'g', 6))
.arg(QString::number(m_properties.d_over_n, 'g', 6))
.arg(QString::number(m_properties.n, 'g', 6))
.arg(QString::number(m_properties.pixelPitch, 'g', 6))
.arg(QString::number(m_properties.theta, 'g', 6))
.arg(QString::number(m_properties.systemDisparity, 'g', 6))
.arg(m_properties.colorSlant)
.arg(m_properties.colorInversion)
.arg(QString::number(m_properties.p_over_du, 'g', 6))
.arg(QString::number(m_properties.p_over_dv, 'g', 6));


command3 = QString("setDisplayConfig  trackingCamera : {\n\
    \"cameraT\": [%1, %2, %3],\n\
    \"cameraR\": [%4, %5, %6],\n\
    \"FOV\" : [%7, %8],\n\
    \"predictParams\" : [%9, %10, %11, %12, %13]\n\
} # \r\n")
.arg(QString::number(m_properties.cameraPos[0], 'g', 6))
.arg(QString::number(m_properties.cameraPos[1], 'g', 6))
.arg(QString::number(m_properties.cameraPos[2], 'g', 6))
.arg(QString::number(m_properties.cameraRot[0], 'g', 6))
.arg(QString::number(m_properties.cameraRot[1], 'g', 6))
.arg(QString::number(m_properties.cameraRot[2], 'g', 6))
.arg(QString::number(m_properties.cameraFovH, 'g', 6))
.arg(QString::number(m_properties.cameraFovV, 'g', 6))
.arg(QString::number(m_properties.predictParams[0], 'g', 6))
.arg(QString::number(m_properties.predictParams[1], 'g', 6))
.arg(QString::number(m_properties.predictParams[2], 'g', 6))
.arg(QString::number(m_properties.predictParams[3], 'g', 6))
.arg(QString::number(m_properties.predictParams[4], 'g', 6));

//,

command4 = QString(
"setDisplayConfig  antiAliasing : {\n\
 \"coordinates\" : [[%1, %2],[%3,%4]],\n\
 \"weights\" : [%5, %6]\n\
} # \r\n")
            .arg(QString::number(m_properties.aa_coordinates[0][0]))
            .arg(QString::number(m_properties.aa_coordinates[0][1]))
            .arg(QString::number(m_properties.aa_coordinates[1][0]))
            .arg(QString::number(m_properties.aa_coordinates[1][1]))
            .arg(QString::number(m_properties.aa_weights[0]))
            .arg(QString::number(m_properties.aa_weights[1]));
    }
    else
    {
        command1 = "setDisplayConfig ActCoefficientsX [";
        for(int i=0; i<6; ++i)
            command1 += QString::number(m_properties.act[i], 'f', 3)+",";
        command1 += "0,0,0] \r\n";

        const float versionNum = 2.5;
        command2 = "setDisplayConfig InterlacingMatrix1 [";
        command2 += QString::number(versionNum);
        command2 += ",";
        command2 += QString::number(m_properties.centerViewNum);
        command2 += ",";
        command2 += QString::number(m_properties.n);
        command2 += ",";
        command2 += QString::number(m_properties.theta);
        command2 += ",";
        command2 += QString::number(m_properties.s);
        command2 += ",";
        command2 += QString::number(m_properties.d_over_n);
        command2 += ",";
        command2 += QString::number(m_properties.p_over_du);
        command2 += ",";
        command2 += QString::number(m_properties.p_over_dv);
        command2 += "]";
        command2 += " \r\n";

        command3 = "setDisplayConfig InterlacingMatrix2 [";
        command3 += QString::number(m_properties.colorSlant);
        command3 += ",";
        command3 += QString::number(m_properties.colorInversion ? 1 : 0);
        command3 += ",";
        command3 += QString::number(m_properties.cameraPos[0]);
        command3 += ",";
        command3 += QString::number(m_properties.cameraPos[1]);
        command3 += ",";
        command3 += QString::number(m_properties.act_beta);
        command3 += ",";
        command3 += QString::number(m_properties.act_gamma);
        command3 += ",";
        command3 += QString::number(m_properties.cameraRot[0]);
        command3 += ",";
        command3 += QString::number(m_properties.cameraFovH);
        command3 += "]";
        command3 += " \r\n";

        command4 = "setDisplayConfig ActCoefficientsY [";
        command4 += QString::number(m_properties.cameraPos[2], 'f', 3)+",";
        command4 += QString::number(m_properties.cameraRot[1], 'f', 3)+",";
        command4 += QString::number(m_properties.cameraRot[2], 'f', 3)+",";
        command4 += QString::number(m_properties.aa_coordinates[0][0], 'f', 3)+",";
        command4 += QString::number(m_properties.aa_coordinates[0][1], 'f', 3)+",";
        command4 += QString::number(m_properties.aa_coordinates[1][0], 'f', 3)+",";
        command4 += QString::number(m_properties.aa_coordinates[1][1], 'f', 3)+",";
        command4 += QString::number(m_properties.aa_weights[0], 'f', 3)+",";
        command4 += QString::number(m_properties.aa_weights[1], 'f', 3)+",";
        command4 += "] \r\n";
    }

    qDebug() << "Serial Command" << command1;
    qDebug() << "Serial Command" << command2;
    qDebug() << "Serial Command" << command3;
    qDebug() << "Serial Command" << command4;

//    findComPortName();

//    if(player->getIs3D())
//         switch3D();

//    player->saveDisplayMetaData();


    m_serialCommand = 0;
    m_serialCommands[0] = command1;
    m_serialCommands[1] = command2;
    m_serialCommands[2] = command3;
    m_serialCommands[3] = command4;

    // Stop Service
    qDebug() << "Stop LeiaService";
    QProcess::execute("sc stop LeiaService");
    QTimer::singleShot(5000, this, &LeiaDisplay::startSerialCommands);
}

void LeiaDisplay::findComPortName() {
    QString portName = "";
    QString logFilePath = "C:/Program Files/LeiaInc/LeiaService/Logs";
    if(QDir(logFilePath).exists()) {
        qDebug() << "Searching Logs for COM port number";
        QStringList namesFilter;
        namesFilter << "*.txt";
        QFileInfoList list = QDir(logFilePath).entryInfoList(namesFilter, QDir::NoFilter, QDir::Time | QDir::Reversed);
        qDebug() << "log files" << list;
        if(list.size()==0) {
            qDebug() << "No log LeiaService files found";
        }
        else {
            QFileInfo f = list[0];
            QFile file(f.absoluteFilePath());
            qDebug() << "Open LOG FILE" << f.absoluteFilePath();
            if(file.open(QIODevice::ReadOnly)) {
                QTextStream in(&file);
                QString logContents = in.readAll();
                file.close();

                int index = logContents.indexOf("COM");
                if(index == -1) {
                    qDebug() << "LeiaService LOG file could not find COM port";
                }
                else {
                    portName = logContents.mid(index, 4);
                    qDebug() << "Found COM Port in LOG file" << portName;
                }
            }
            else {
                qDebug() << "LeiaService LOG file cannot be opened";
            }
        }
    }
    if(portName == "") {
        portName = "COM3";
    }
    setComPort(portName);
}

void LeiaDisplay::startSerialCommands()
{
    if(!m_serialPort)
    {
        m_serialPort.reset(new QSerialPort());
        connect(m_serialPort.get(), &QSerialPort::readyRead, this, &LeiaDisplay::slot_serialReadyRead);

        qDebug() << "LeiaService COM port" << m_comPortNum;

        m_serialPort->setPortName(m_comPortNum);
        m_serialPort->setBaudRate(QSerialPort::Baud115200);
        m_serialPort->setDataBits(QSerialPort::Data8);
        m_serialPort->setParity(QSerialPort::NoParity);
        m_serialPort->setStopBits(QSerialPort::OneStop);
        m_serialPort->setFlowControl(QSerialPort::NoFlowControl);
        if (m_serialPort->open(QIODevice::ReadWrite))
            qDebug() << "Serial Ready";
        else {
            qWarning() << "Serial Not ready";
//            if(player->getIs3D())
//                switch2D();
//            if(player->getIs3D())
//                switch3D();
//            m_serialPortProgress->hide();
            m_serialPort.reset();
            return;
        }
    }

    QByteArray bytes;

    bytes = m_serialCommands[m_serialCommand].toUtf8();
    m_serialPort->write(bytes);
    m_serialPort->flush();
    qDebug() << "Execute" << m_serialCommands[m_serialCommand];
    QTimer::singleShot(2000, this, &LeiaDisplay::slot_serialIssueCommand);
}

void LeiaDisplay::slot_serialIssueCommand()
{
    qDebug() << "Starting to Issue Serial Command" << m_serialCommand;
    if(!m_serialPort) {
        qDebug() << "SERIAL PORT IS NOT AVAILABLE!!";
        return;
    }
    m_serialCommand++;
    if(m_serialCommand >= m_numCommands)
    {
        qDebug() << "SERIAL PORT too many commands issued!!!";
        return;
    }
    QByteArray bytes = m_serialCommands[m_serialCommand].toUtf8();
    m_serialPort->clear();
    m_serialPort->write(bytes);
    m_serialPort->flush();
    qDebug() << "Execute" << m_serialCommands[m_serialCommand] << m_serialCommand;
    if(m_serialCommand<m_numCommands-1)
    {
        qDebug() << "Prep next command";
        QTimer::singleShot(1500, this, &LeiaDisplay::slot_serialIssueCommand);
    }
    else
    {
        qDebug() << "Queue of commands empty";
    }
}

void LeiaDisplay::slot_serialReadyRead()
{
    const QByteArray data = m_serialPort->readAll();
    qDebug() << "serial read" << data << m_serialCommand;
    if(m_serialCommand==m_numCommands-1)
    {
        m_serialPort->flush();
        m_serialPort->close();
        m_serialPort.reset();
        m_serialPort = nullptr;
        m_serialCommand = 0;
        QTimer::singleShot(5000, this, &LeiaDisplay::startLeiaService);
    }
}

void LeiaDisplay::startLeiaService()
{
    // Stop Service
    qDebug() << "Start LeiaService";
    QProcess::execute("sc start LeiaService");
}

bool LeiaDisplay::isLegacyDisplay() const
{
    return m_isLegacyDisplay;
}

void LeiaDisplay::setIsLegacyDisplay(bool value)
{
    if (m_isLegacyDisplay != value)
    {
        m_isLegacyDisplay = value;
        emit isLegacyDisplayChanged();
    }
}

QString LeiaDisplay::comPort() const
{
    return m_comPortNum;
}

void LeiaDisplay::setComPort(const QString& value)
{
    if (m_comPortNum != value)
    {
        m_comPortNum = value;
        emit comPortChanged();
    }
}

qreal LeiaDisplay::centerView() const
{
    return m_properties.centerViewNum;
}

void LeiaDisplay::setCenterView(double value)
{
    if (m_properties.centerViewNum != value)
    {
        m_properties.centerViewNum = value;
        emit centerViewChanged();
    }
}

void LeiaDisplay::parseConfig(const QString& displayJson)
{
    LeiaDisplay::DisplayProperties properties;

    qDebug() << "Display JSON =" << displayJson;

    QByteArray data = displayJson.toUtf8();
    QJsonParseError errorPtr;
    QJsonDocument doc = QJsonDocument::fromJson(data, &errorPtr);
    if (doc.isNull() || doc.isEmpty() || errorPtr.error != QJsonParseError::NoError) {
        qWarning() << "displayJson isNull or isEmpty " << errorPtr.errorString();
        return;
    }

    QJsonObject rootObj = doc.object();
    if(!displayJson.contains("InterlacingMatrix")) {
        qDebug() << "not legacy due to InterlacingMatrix";
        LeiaDisplay::setIsLegacyDisplay(false);
    }

    properties.numHorizontalDisplayViews = 12;
    properties.numVerticalDisplayViews = 1;

    QJsonObject jNumViews = rootObj.value("NumViews").toObject();
    if(!jNumViews.isEmpty()) {
        properties.numHorizontalDisplayViews = jNumViews.value("X").toInt();
        properties.numVerticalDisplayViews = jNumViews.value("Y").toInt();
    } else {
        qWarning() << "NumViews not found";
    }

    qDebug() << "Leia Display numHorizViews=" << properties.numHorizontalDisplayViews;
    qDebug() << "Leia Display numVertViews=" << properties.numVerticalDisplayViews;

    properties.screenWidth = 3840;
    properties.screenHeight = 2160;
    QJsonObject jPanelResolution = rootObj.value("PanelResolution").toObject();
    if(!jPanelResolution.isEmpty()) {
        properties.screenWidth = jPanelResolution.value("X").toInt();
        properties.screenHeight = jPanelResolution.value("Y").toInt();
    } else {
        qWarning() << "PanelResolution not found";
    }
    qDebug() << "Leia Display Screen Width=" << properties.screenWidth;
    qDebug() << "Leia Display Screen Height=" << properties.screenHeight;

    properties.centerViewNum = 0.f;
    if(rootObj.contains("CenterViewNumber")) {
        QJsonValue jCenterViewNumber = rootObj.value("CenterViewNumber");
        properties.centerViewNum = jCenterViewNumber.toDouble();
    } else {
        qWarning() << "CenterViewNumber not found";
    }
    qDebug() << "Leia Display Center View #=" << properties.centerViewNum;

    double viewBoxSizeX{0};
    double convergenceDistance{0};

    QJsonObject jViewboxSize = rootObj.value("ViewboxSize").toObject();
    if(!jViewboxSize.isEmpty()) {
        viewBoxSizeX = jViewboxSize.value("X").toDouble();
    } else {
        qWarning() << "ViewboxSize not found";
    }

    qDebug() << "Leia Display I_0=" << viewBoxSizeX;
    convergenceDistance = 600.0;
    if(rootObj.contains("ConvergenceDistance")) {
        QJsonValue jConvergenceDistance = rootObj.value("ConvergenceDistance");
        convergenceDistance = jConvergenceDistance.toDouble();
    } else {
        qWarning() << "ConvergenceDistance not found";
    }
    qDebug() << "Leia Display Convergence Distance=" << convergenceDistance;

    QJsonObject jDotPitchInMm = rootObj.value("DotPitchInMm").toObject();
    if(!jDotPitchInMm.isEmpty()) {
        properties.pixelPitch = jDotPitchInMm.value("X").toDouble();
    } else {
        qWarning() << "DotPitchInMm not found";
    }
    qDebug() << "Leia Display Pixel Pitch=" << properties.pixelPitch;

    properties.n = 1.47;
    properties.theta = 0.0;
    properties.cameraPos[0] =-37.88;
    properties.cameraPos[1] = 165.57;
    properties.cameraPos[2] = 0.0;
    properties.cameraRot[0] = 0.0;
    properties.cameraRot[1] = 0.0;
    properties.cameraRot[2] = 0.0;
    properties.colorSlant = 0;
    properties.colorInversion = false;
    properties.p_over_du = 3.0;
    properties.p_over_dv = 1.0;
    properties.act_beta = 2.0;
    properties.s = 11.0;
    properties.d_over_n = 0.5;
    properties.cameraFovH = 86.0;
    properties.cameraFovV = 57.0;

    if(rootObj.contains("Beta")) {
        QJsonValue jBeta = rootObj.value("Beta");
        properties.act_beta = jBeta.toDouble();
    } else {
        qWarning() << "Beta not found";
    }

    properties.act_gamma = 2.0;
    if(rootObj.contains("Gamma")) {
        QJsonValue jGamma = rootObj.value("Gamma");
        properties.act_gamma = jGamma.toDouble();
    } else {
        qWarning() << "Gamma not found";
    }

    if(rootObj.contains("IsSlanted")) {
        QJsonValue jIsSlanted = rootObj.value("IsSlanted");
        properties.p_over_dv = jIsSlanted.toBool() ? 1.0 : -1.0;
    } else {
        qWarning() << "IsSlanted not found";
    }

    QJsonObject jact_coefficients = rootObj.value("ViewSharpeningCoefficients").toObject();
    QJsonArray jInterlacingMatrix1 = rootObj.value("InterlacingMatrix").toArray();

    if (!jact_coefficients.isEmpty() && !jInterlacingMatrix1.isEmpty())
    {
        qDebug() << "jact_coefficients and jInterlacingMatrix1 are valid";
        QJsonArray row1 = jInterlacingMatrix1[0].toArray();
        QJsonArray row2 = jInterlacingMatrix1[1].toArray();
        QJsonArray row3 = jInterlacingMatrix1[2].toArray();
        QJsonArray row4 = jInterlacingMatrix1[3].toArray();
        double versionNum = row1[0].toDouble();
        if(versionNum>=2.0) {
            //qDebug() << "jInterlacingMatrix1" << jInterlacingMatrix1;

            properties.centerViewNum = row1[1].toDouble();
            properties.n = row1[2].toDouble();
            properties.theta = row1[3].toDouble();

            properties.s = row2[0].toDouble();
            properties.d_over_n = row2[1].toDouble();
            properties.p_over_du = row2[2].toDouble();
            properties.p_over_dv = row2[3].toDouble();

            properties.colorSlant = ((int)row3[0].toDouble());
            properties.colorInversion = fabs(row3[1].toDouble()) > 0.001;
            properties.cameraPos[0] = row3[2].toDouble();
            properties.cameraPos[1] = row3[3].toDouble();
            properties.act_beta = row4[0].toDouble();
            properties.act_gamma = row4[1].toDouble();
            properties.cameraRot[0] = row4[2].toDouble();

            if(versionNum>=2.5) {
                properties.cameraFovH = row4[3].toDouble();
            }

        }
        else if(!jact_coefficients.isEmpty()) {
            QJsonArray act_values = jact_coefficients["Y"].toArray();
            if(act_values[0].toDouble() >= 1000.0 ) {
                properties.n = act_values[0].toDouble()/1000.f;
                properties.theta = act_values[1].toDouble();
                properties.cameraPos[0] = act_values[2].toDouble();
                properties.cameraPos[1] = act_values[3].toDouble();
                if(act_values[4].toDouble() > 1.0)
                    properties.act_beta = act_values[4].toDouble();
                if(act_values[5].toDouble() > 0.0)
                    viewBoxSizeX = act_values[5].toDouble();
            }
            double IO = viewBoxSizeX;
            double D = convergenceDistance;
            double du = properties.pixelPitch/properties.p_over_du;
            properties.d_over_n = du * D / IO;
            properties.s = (du / IO) * double(properties.screenWidth)*properties.p_over_du;
        }

        if(versionNum>=2.1)
        {
            QJsonArray act_values = jact_coefficients["Y"].toArray();
            properties.cameraPos[2] = act_values[0].toDouble();
            properties.cameraRot[1] = act_values[1].toDouble();
            properties.cameraRot[2] = act_values[2].toDouble();
            properties.aa_coordinates[0][0] = act_values[3].toDouble();
            properties.aa_coordinates[0][1] = act_values[4].toDouble();
            properties.aa_coordinates[1][0] = act_values[5].toDouble();
            properties.aa_coordinates[1][1] = act_values[6].toDouble();
            properties.aa_weights[0] = act_values[7].toDouble();
            properties.aa_weights[1] = act_values[8].toDouble();
        }
        else {
            // Old style axis convention for tracking - flip
            properties.cameraPos[0] = -properties.cameraPos[0];
            properties.cameraRot[0] = -properties.cameraRot[0];
        }

    }

    qDebug() << "Leia Display n=" << properties.n;
    qDebug() << "Leia Display theta=" << properties.theta;
    qDebug() << "Leia Display Gamma=" << properties.act_gamma;
    qDebug() << "Leia Display Beta=" << properties.act_beta;
    qDebug() << "Leia Display Color Slant=" << properties.colorSlant;
    qDebug() << "Leia Display Color Inversion=" << properties.colorInversion;
    qDebug() << "Leia Display p_over_du=" << properties.p_over_du;
    qDebug() << "Leia Display p_over_dv=" << properties.p_over_dv;

    properties.num_acts = 0;
     if(!jact_coefficients.isEmpty()) {
        QJsonArray act_values = jact_coefficients["X"].toArray();
        if(!act_values.isEmpty()) {
            for(int i=0; i<act_values.size(); ++i) {
                properties.act[i] = act_values[i].toDouble();
                qDebug() << "Leia Display act[" << i << "]=" << properties.act[i];
            }
            properties.num_acts = act_values.size();
        }
    }

     properties.systemDisparity = 4.0;
    if(rootObj.contains("SystemDisparity")) {
        QJsonValue jSystemDisparity = rootObj.value("SystemDisparity");
        properties.systemDisparity = jSystemDisparity.toDouble();
    }
    qDebug() << "Leia Display System Disparity=" << properties.systemDisparity;
    qDebug() << "Leia Display Camera fov=" << properties.cameraFovH << properties.cameraFovV;

    qDebug() << "Assign properties";
    m_properties = properties;
    qDebug() << "fetched centerView()" << centerView();
    emit centerViewChanged();
}
