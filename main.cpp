#include <QQmlContext>
#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "src/CalibrationView.h"
#include <src/CalibrationModel.h>
#include "src/TextFieldDoubleValidator.h"
#include "src/ApplicationDefaults.h"

int main(int argc, char *argv[])
{
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
    engine.addImportPath("qrc:/qml/");
    engine.rootContext()->setContextProperty("calibrationView", &calibrationView);
    engine.rootContext()->setContextProperty("defaultLocale", locale);
    engine.rootContext()->setContextProperty("appDefaults", &ApplicationDefaults::instance());
    const QUrl url(QStringLiteral("qrc:/qml/main.qml"));

    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
