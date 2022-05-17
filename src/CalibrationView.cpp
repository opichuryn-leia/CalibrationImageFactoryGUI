#include "CalibrationView.h"
#include <QUrl>
#include <QPainter>
#include <QDebug>

CalibrationView::CalibrationView(QWindow *parent)
    : QOpenGLWindow(QOpenGLWindow::NoPartialUpdate, parent)
{}

void CalibrationView::paintGL()
{
    QPainter painter{this};
    qDebug() << "draw image " << m_image;


    QImage target = m_image;
    if (width() != m_image.width() || height() != m_image.height())
    {
        target = m_image.copy( 0, 0, qMin(width(), m_image.width()), qMin(height(), m_image.height()));
        painter.fillRect(QRect{0, 0, width(), height()}, Qt::black);
    }

    painter.drawImage(QRect{0, 0, target.width(), target.height()}, target);
}

void CalibrationView::resizeGL(int w, int h)
{
    qDebug() << "resizeGL " << w << "x" << h;
    update();
}

void CalibrationView::setPictureSource(const QImage& image)
{
    m_image = image;
    if (!m_image.isNull() && isVisible())
    {
        qDebug() << "setPictureSource update " << image;
        update();
    }
}

void CalibrationView::setPicturePath(const QString &path)
{
    const QUrl url{path};
    if (!url.isValid())
    {
        qWarning() << "Invalid picture url provided " << url;
        return;
    }

    const QString pictureSource = url.toDisplayString(QUrl::PreferLocalFile);
    qDebug() << "Display picture source " << pictureSource;
    setPictureSource(QImage{pictureSource});
}
