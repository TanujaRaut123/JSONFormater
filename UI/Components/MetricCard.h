#pragma once

#include <QFrame>
#include <QStyle>

class QLabel;
class QString;

class MetricCard : public QFrame
{
public:
    explicit MetricCard(const QString &title,
                        const QString &value,
                        const QString &theme,
                        QWidget *parent = nullptr,
                        QStyle::StandardPixmap icon = QStyle::SP_CustomBase);

    QLabel *valueLabel() const;
};
