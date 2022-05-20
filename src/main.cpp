#include <QQmlContext>
#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "CalibrationView.h"
#include "CalibrationModel.h"
#include "TextFieldDoubleValidator.h"
#include "ApplicationDefaults.h"
#include "LeiaDisplay.h"
#include <iostream>

namespace
{

struct FileLogger
{
    FileLogger()
        : ts(&helper.file)
    {}

    bool valid() const
    {
        return helper.ok;
    }

    struct Helper
    {
        Helper()
        {
            ok = file.open(QIODevice::WriteOnly | QIODevice::Truncate);
        }
        QFile file{"calibration.log"};
        bool ok;
    };


    Helper helper;
    QTextStream ts;
} fileLogger;

const char* logLevel(QtMsgType type)
{
    switch (type) {
    case QtDebugMsg: return "[dbg]";
    case QtWarningMsg: return "[wrn]";
    case QtCriticalMsg: return "[crt]";
    case QtFatalMsg: return "[ftl]";
    case QtInfoMsg: return "[inf]";
    }
    return "[xxx]";
}

FILE* logStream(QtMsgType type)
{
    switch (type)
    {
    case QtWarningMsg:
    case QtCriticalMsg:
    case QtFatalMsg:
        return stderr;
    default:
        return stdout;
    }
}

void messageHandler(QtMsgType type, const QMessageLogContext &, const QString &msg)
{
    QString fullMessage = QString{"%1 %2"}.arg(logLevel(type), msg);
    if (fileLogger.valid())
    {
        fileLogger.ts << fullMessage << "\n";
        fileLogger.ts.flush();
    }

    QByteArray data = fullMessage.toLocal8Bit();
    FILE* ss = logStream(type);
    fprintf(ss, "%s\n", data.constData());
    fflush(ss);
}
} // namespace

int main(int argc, char *argv[])
{
    qInstallMessageHandler(messageHandler);
    auto locale = QLocale::c();
    locale.setNumberOptions(QLocale::RejectGroupSeparator|QLocale::OmitGroupSeparator);
    QLocale::setDefault(locale);

    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    qmlRegisterType<TextFieldDoubleValidator>("Leia", 1, 0, "TextFieldDoubleValidator");
    qmlRegisterType<CalibrationModelItem>("Leia", 1, 0, "CalibrationModelItem");
    qmlRegisterType<CalibrationModel>("Leia", 1, 0, "CalibrationModel");
    qRegisterMetaType<AppMetric>();

    CalibrationView calibrationView;
    QQmlApplicationEngine engine;
    engine.addImportPath("qrc:/");
    engine.rootContext()->setContextProperty("calibrationView", &calibrationView);
    engine.rootContext()->setContextProperty("defaultLocale", locale);
    engine.rootContext()->setContextProperty("appDefaults", &ApplicationDefaults::instance());
    engine.rootContext()->setContextProperty("leiaDisplay", &LeiaDisplay::instance());
    const QUrl url(QStringLiteral("qrc:/main.qml"));

    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
