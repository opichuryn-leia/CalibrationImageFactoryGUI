#include "CalibrationModelItem.h"

CalibrationModelItem::CalibrationModelItem(QObject* parent)
    : QObject(parent)
{}

const QString &CalibrationModelItem::source() const
{
    return m_source;
}

void CalibrationModelItem::setSource(const QString& value)
{
    m_source = value;
    emit sourceChanged();
}

const QString &CalibrationModelItem::description() const
{
    return m_description;
}

void CalibrationModelItem::setDescription(const QString& value)
{
    m_description = value;
    emit descriptionChanged();
}

int CalibrationModelItem::patternType() const
{
    return m_type;
}

void CalibrationModelItem::setPatternType(int value)
{
    if (m_type != value)
    {
        m_type = value;
        emit patternTypeChanged();
    }
}
