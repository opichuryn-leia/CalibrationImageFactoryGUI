#include "ApplicationDefaults.h"
#include <QScreen>
#include <QDebug>
#include <QGuiApplication>
#include <QMap>
#include <QStandardPaths>
#include <algorithm>

namespace
{

QPair<int, int> getDisplayDimensions(int width, int height)
{
    if (width == 2880 && height == 1880)
    {
        return {3, 4};
    }
    return {3, 3};
}

} // namespace

ApplicationDefaults& ApplicationDefaults::instance()
{
    static ApplicationDefaults app;
    return app;
}

AppMetric ApplicationDefaults::metric() const
{
    return m_metric;
}

QString ApplicationDefaults::dir() const
{
    if (m_tempDir.isValid())
    {
        return m_tempDir.path();
    }

    qWarning() << "No tmp dir available found";
    const auto list = QStandardPaths::standardLocations(QStandardPaths::PicturesLocation);
    auto it = std::find_if(list.begin(), list.end(), [](const QString& path)
    {
        return QDir{path}.exists();
    });
    if (it != list.end())
    {
        return *it;
    }

    qWarning() << "No standard picture location found";
    QString fallback = QGuiApplication::applicationDirPath();
    if (QDir{fallback}.exists())
    {
        return fallback;
    }
    qFatal("Cannot find default directory for images");
}

ApplicationDefaults::ApplicationDefaults(QObject *parent)
    : QObject(parent)
{
    constexpr int DEFAULT_SIZE = 1000;
    constexpr int DEFAULT_DIMENSION = 2;
    m_metric.m_width = m_metric.m_height = DEFAULT_SIZE;
    m_metric.m_rows = m_metric.m_columns = DEFAULT_DIMENSION;

    const QScreen* primaryScreen = QGuiApplication::primaryScreen();
    if (primaryScreen)
    {
        const QRect geometry = primaryScreen->geometry();
        m_metric.m_width = geometry.width();
        m_metric.m_height = geometry.height();
    }
    else
    {
        qWarning("No primary screen is found");
    }

    const auto dimensions = getDisplayDimensions(m_metric.m_width, m_metric.m_height);
    m_metric.m_rows = dimensions.first;
    m_metric.m_columns = dimensions.second;
    qDebug() << "Display " << m_metric.m_width << "x" << m_metric.m_height
             << "[" << m_metric.m_rows << ":" << m_metric.m_columns << "]";
}
