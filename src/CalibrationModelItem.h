#pragma once

#include <QObject>

class CalibrationModelItem : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString source READ source NOTIFY sourceChanged)
    Q_PROPERTY(QString description READ description NOTIFY descriptionChanged)
    // See CalibrationModel::PatternType
    Q_PROPERTY(int patternType READ patternType NOTIFY patternTypeChanged)

public:
    CalibrationModelItem(QObject * parent = nullptr);

    const QString& source() const;
    void setSource(const QString& value);

    const QString& description() const;
    void setDescription(const QString& value);

    int patternType() const;
    void setPatternType(int value);

signals:
    void sourceChanged();
    void descriptionChanged();
    void patternTypeChanged();

private:
    QString m_source;
    QString m_description;
    int m_type{0};
};
