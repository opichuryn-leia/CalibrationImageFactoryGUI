#include "CalibrationModel.h"
#include "CalibrationFactory.h"
#include <QDebug>
#include <QUrl>

namespace
{
using ModelItemPtr = QSharedPointer<CalibrationModelItem>;

QString getCalibationName(CalibrationModel::PatternType type)
{
    switch (type)
    {
    case CalibrationModel::RGB:
        return QStringLiteral("Rgb");
    case CalibrationModel::ACT:
        return QStringLiteral("Act");
    case CalibrationModel::ALIGN_BAR:
        return QStringLiteral("Abar");
    }
    return "Unknown";
}
} // namespace

CalibrationModel::CalibrationModel(QObject* parent)
    : QAbstractListModel(parent)
{
    const std::vector<PatternType> preset =
    {
        ACT,
        RGB,
        ALIGN_BAR
    };

    const auto metric = ApplicationDefaults::instance().metric();
    for(const auto& p : preset)
    {
        const QString name = getCalibationName(p);
        const QString fileName = getCalibrationImagePath(p, metric.m_width, metric.m_height, metric.m_rows, metric.m_columns);
        qDebug() << "pattern " << name << " -> " << fileName;
        auto* item = new CalibrationModelItem{};
        item->setObjectName(name + "PatternObj");
        item->setDescription(name + " Pattern");
        const QString fullPath = QUrl{}.fromLocalFile(fileName).toDisplayString();
        qDebug() << "Full path" << fullPath;
        item->setSource(fullPath);
        item->setPatternType(p);
        m_model.push_back(ModelItemPtr{item});
    }
}

ModelItemPtr CalibrationModel::get(int index) const
{
    if (index < 0 || static_cast<size_t>(index) >= m_model.size())
    {
        return {};
    }
    return m_model.at(index);
}

int CalibrationModel::rowCount(const QModelIndex &) const
{
    return m_model.size();
}

QVariant CalibrationModel::data(const QModelIndex &index, int) const
{
    auto ptr = get(index.row());
    if (!ptr)
    {
        return {};
    }
    return QVariant::fromValue(ptr.data());
}

QHash<int, QByteArray> CalibrationModel::roleNames() const
{
    QHash<int, QByteArray> names =
    {
        {Qt::DisplayRole, "modelItem"}
    };
    return names;
}

QString CalibrationModel::getCalibrationImagePath(CalibrationModel::PatternType type, int width, int height, int rows, int columns)
{
    // /tmp/act2880x1920_3x4.png
    const QString fileName = QDir{ApplicationDefaults::instance().dir()}.
            filePath(QString{"%1%2x%3_%4x%5.png"}.arg(getCalibationName(type).toLower()).
            arg(width).arg(height).
            arg(rows).arg(columns));

    if (QFileInfo::exists(fileName))
    {
        return fileName;
    }

    if (!CalibrationFactory::makePattern(static_cast<CalibrationFactory::PatternType>(type), fileName, width, height, rows, columns))
    {
        const auto message = QString{"generation failed %"}.arg(fileName);
        qFatal("%s", message.toLatin1().data());
    }
    return fileName;
}
