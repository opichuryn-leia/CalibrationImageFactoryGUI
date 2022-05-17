#pragma once

#include <QOpenGLWindow>
#include <QImage>

class CalibrationView : public QOpenGLWindow
{
    Q_OBJECT

public:
    CalibrationView(QWindow *parent = nullptr);

public slots:
    void setPictureSource(const QImage& image);
    void setPicturePath(const QString& path);

protected:
    virtual void paintGL() override;
    virtual void resizeGL(int w, int h) override;

private:
    QImage m_image;
};
