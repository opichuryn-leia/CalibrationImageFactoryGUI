#pragma once

#include <QSharedPointer>
#include <QAbstractListModel>

#include "CalibrationFactory.h"
#include "ApplicationDefaults.h"
#include "CalibrationModelItem.h"

#include <vector>

class CalibrationModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum PatternType
    {
        RGB = CalibrationFactory::RGB,
        ACT = CalibrationFactory::ACT,
        ALIGN_BAR = CalibrationFactory::ALIGN_BAR
    };
    Q_ENUM(PatternType)

    CalibrationModel(QObject * parent = nullptr);

    QSharedPointer<CalibrationModelItem> get(int index) const;

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    Q_INVOKABLE static QString getCalibrationImagePath(CalibrationModel::PatternType type, int width, int height, int rows, int columns);

private:
    std::vector<QSharedPointer<CalibrationModelItem>> m_model;
};

