#pragma once

#include <QDebug>
#include <QtGlobal>
#include <QDoubleValidator>

class TextFieldDoubleValidator : public QDoubleValidator {
    Q_OBJECT
    Q_PROPERTY(QLocale locale READ locale WRITE setLocale NOTIFY changed)

public:
    TextFieldDoubleValidator (QObject * parent = nullptr)
        : QDoubleValidator(parent)
    {}

    virtual QValidator::State validate(QString& s, int&) const override {
        const bool negativeRange = !qIsNaN(bottom()) && bottom() < 0;
        if (s.isEmpty() || (s.startsWith("-") && s.length() == 1 && negativeRange)) {
            // allow empty field or standalone minus sign
            return QValidator::Intermediate;
        }
        // check length of decimal places
        QChar point = locale().decimalPoint();
        if(s.indexOf(point) != -1) {
            int lengthDecimals = s.length() - s.indexOf(point) - 1;
            if (lengthDecimals > decimals()) {
                return QValidator::Invalid;
            }
        }
        // check range of value
        bool isNumber;
        double value = locale().toDouble(s, &isNumber);
        if (isNumber && bottom() <= value && value <= top()) {
            return QValidator::Acceptable;
        }
        return QValidator::Invalid;
    }
};
