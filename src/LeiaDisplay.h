#pragma once

#include <QString>
#include <QObject>
#include <QtSerialPort/QSerialPort>

#include <memory>

class LeiaDisplay : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool isLegacyDisplay READ isLegacyDisplay WRITE setIsLegacyDisplay NOTIFY isLegacyDisplayChanged)
    Q_PROPERTY(QString comPort READ comPort WRITE setComPort NOTIFY comPortChanged)
    Q_PROPERTY(qreal centerView READ centerView WRITE setCenterView NOTIFY centerViewChanged)

public:
    static LeiaDisplay& instance();

    bool isLegacyDisplay() const;
    void setIsLegacyDisplay(bool value);

    QString comPort() const;
    void setComPort(const QString& value);


    qreal centerView() const;
    void setCenterView(double value);

    int initialize();
    int teardown();
    int switch2D();
    int switch3D();
    bool isConnected();
    QString getDisplayConfig();

public slots:
    void burnDisplay();
    void fetchDisplayProperties();

private slots:
    void slot_serialReadyRead();
    void slot_serialIssueCommand();

    void startSerialCommands();
    void startLeiaService();

signals:
    void isLegacyDisplayChanged();
    void comPortChanged();
    void centerViewChanged();

private:
    LeiaDisplay(QObject* parent = nullptr);
    void findComPortName();

    struct DisplayProperties
    {
        int numHorizontalDisplayViews{0};
        int numVerticalDisplayViews = 1;
        int screenWidth{0};
        int screenHeight{0};
        double act[9];
        int num_acts{0};
        double centerViewNum{0};
        double act_gamma{0};
        double act_beta{0};
        double pixelPitch{0};
        double n{0};
        double theta{0};
        double systemDisparity{0};
        double cameraPos[3]= {-37.880001068115234, 165.57200622558594, 0};
        double cameraRot[3] = {-5.40, 0, 0};
        int colorSlant{0};
        bool colorInversion{false};
        double p_over_du{0};
        double p_over_dv{0};
        double d_over_n{0};
        double s{0};

        double aa_coordinates[2][2] = {{0.45, 0.15}, {-0.45, -0.15}};
        double aa_weights[2] = {0.5, 0.5};
        QString displayClass;

        double cameraFovH = 86.0;
        double cameraFovV = 57.0;

        double predictParams[5] = {0.02, 0.05, 0.2, 0.1, 20.0};
    };

    void parseConfig(const QString& config);

    bool m_isLegacyDisplay = true;
    static constexpr int m_numCommands = 4;
    QString m_serialCommands[m_numCommands];
    QString m_comPortNum = "COM3";
    int m_serialCommand = 0;
    std::unique_ptr<QSerialPort> m_serialPort;
    DisplayProperties m_properties;
};
