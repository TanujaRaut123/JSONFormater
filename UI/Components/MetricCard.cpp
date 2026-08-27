#include "MetricCard.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QVBoxLayout>

MetricCard::MetricCard(const QString &title, const QString &value, const QString &theme,
                       QWidget *parent, QStyle::StandardPixmap icon)
    : QFrame(parent)
{
    setObjectName(QStringLiteral("metricCard"));
    setProperty("theme", theme);
    setMinimumHeight(108);

    auto *layout = new QHBoxLayout(this);
    layout->setContentsMargins(18, 14, 18, 14);
    layout->setSpacing(14);

    if (icon != QStyle::SP_CustomBase) {
        auto *iconLabel = new QLabel(this);
        iconLabel->setObjectName(QStringLiteral("metricIcon"));
        iconLabel->setAlignment(Qt::AlignCenter);
        iconLabel->setFixedSize(44, 44);
        iconLabel->setPixmap(style()->standardIcon(icon).pixmap(22, 22));
        layout->addWidget(iconLabel, 0, Qt::AlignTop);
    }

    auto *textLayout = new QVBoxLayout;
    textLayout->setContentsMargins(0, 1, 0, 0);
    textLayout->setSpacing(4);
    auto *titleLabel = new QLabel(title, this);
    titleLabel->setObjectName(QStringLiteral("metricTitle"));
    textLayout->addWidget(titleLabel);
    auto *valueLabelWidget = new QLabel(value, this);
    valueLabelWidget->setObjectName(QStringLiteral("metricValue"));
    textLayout->addWidget(valueLabelWidget);
    textLayout->addStretch();
    layout->addLayout(textLayout, 1);
}

QLabel *MetricCard::valueLabel() const
{
    return findChild<QLabel *>(QStringLiteral("metricValue"));
}
