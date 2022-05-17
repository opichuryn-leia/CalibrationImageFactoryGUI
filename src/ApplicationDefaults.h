#pragma once

#include <QObject>
#include <QRect>
#include <QTemporaryDir>

class AppMetric
{
    Q_GADGET

    Q_PROPERTY(int width MEMBER m_width)
    Q_PROPERTY(int height MEMBER m_height)
    Q_PROPERTY(int rows MEMBER m_rows)
    Q_PROPERTY(int columns MEMBER m_columns)

public:

    inline QRect geometry() const
    {
        QRect rect;
        rect.setWidth(m_width);
        rect.setHeight(m_height);
        return rect;
    }

    int m_width{0};
    int m_height{0};
    int m_rows{0};
    int m_columns{0};
};

class ApplicationDefaults : public QObject
{
    Q_OBJECT
    Q_PROPERTY(AppMetric metric READ metric CONSTANT)

public:
    static ApplicationDefaults& instance();

    AppMetric metric() const;
    QString dir() const;

private:
    explicit ApplicationDefaults(QObject *parent = nullptr);

    AppMetric m_metric;
    QTemporaryDir m_tempDir;
};

