#include "StoragePage.h"

#include "../Components/MetricCard.h"

#include <QFrame>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QStringList>
#include <QStyle>
#include <QVBoxLayout>

namespace
{
QLabel *createKpiValue(QWidget *parent, const QString &value)
{
    auto *label = new QLabel(value, parent);
    label->setObjectName(QStringLiteral("kpiValue"));
    return label;
}
}

StoragePage::StoragePage(QWidget *parent)
    : QWidget(parent)
{
    setObjectName(QStringLiteral("storagePage"));
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    auto *contentLayout = new QVBoxLayout(this);
    contentLayout->setContentsMargins(30, 28, 30, 30);
    contentLayout->setSpacing(20);

    auto *headerLayout = new QHBoxLayout;
    auto *titleLayout = new QVBoxLayout;
    titleLayout->setSpacing(3);

    auto *title = new QLabel(QStringLiteral("ADAPTO Storage"), this);
    title->setObjectName(QStringLiteral("pageTitle"));
    titleLayout->addWidget(title);

    auto *subtitle = new QLabel(QStringLiteral("Storage simulator and carrier location control"), this);
    subtitle->setObjectName(QStringLiteral("pageSubtitle"));
    titleLayout->addWidget(subtitle);
    headerLayout->addLayout(titleLayout);
    headerLayout->addStretch();

    auto *systemLabel = new QLabel(QStringLiteral("ADAPTO  |  SIMULATION ACTIVE"), this);
    systemLabel->setObjectName(QStringLiteral("systemLabel"));
    headerLayout->addWidget(systemLabel, 0, Qt::AlignTop);
    contentLayout->addLayout(headerLayout);

    auto *kpiGrid = new QGridLayout;
    kpiGrid->setContentsMargins(0, 0, 0, 0);
    kpiGrid->setHorizontalSpacing(16);
    kpiGrid->setVerticalSpacing(16);
    kpiGrid->addWidget(createKpiCard(QStringLiteral("Stored Today"), QStringLiteral("248"), QStringLiteral("stored"), storedTodayValue), 0, 0);
    kpiGrid->addWidget(createKpiCard(QStringLiteral("Retrieved Today"), QStringLiteral("186"), QStringLiteral("retrieved"), retrievedTodayValue), 0, 1);
    kpiGrid->addWidget(createKpiCard(QStringLiteral("Block Utilization"), QStringLiteral("78%"), QStringLiteral("utilization"), utilizationValue), 0, 2);
    kpiGrid->addWidget(createKpiCard(QStringLiteral("Active Storage Orders"), QStringLiteral("12"), QStringLiteral("orders"), activeOrdersValue), 0, 3);
    contentLayout->addLayout(kpiGrid);

    auto *bottomLayout = new QHBoxLayout;
    bottomLayout->setSpacing(18);
    bottomLayout->addWidget(createOrderCard(), 3);
    bottomLayout->addWidget(createExceptionsCard(), 2);
    contentLayout->addLayout(bottomLayout, 1);

    setMessage(QStringLiteral("Ready for storage order"), QStringLiteral("ready"));
}

QFrame *StoragePage::createKpiCard(const QString &title, const QString &value,
                                    const QString &theme, QLabel *&valueLabel)
{
    auto *card = new MetricCard(title, value, theme, this);
    valueLabel = card->valueLabel();
    return card;
}

QFrame *StoragePage::createOrderCard()
{
    auto *card = new QFrame(this);
    card->setObjectName(QStringLiteral("contentCard"));
    auto *layout = new QVBoxLayout(card);
    layout->setContentsMargins(22, 20, 22, 20);
    layout->setSpacing(10);

    auto *title = new QLabel(QStringLiteral("Storage Order Information"), card);
    title->setObjectName(QStringLiteral("cardTitle"));
    layout->addWidget(title);

    auto *carrierLabel = new QLabel(QStringLiteral("Product Carrier"), card);
    carrierLabel->setObjectName(QStringLiteral("fieldLabel"));
    layout->addWidget(carrierLabel);
    carrierInput = new QLineEdit(card);
    carrierInput->setObjectName(QStringLiteral("carrierInput"));
    carrierInput->setPlaceholderText(QStringLiteral("Enter carrier ID"));
    layout->addWidget(carrierInput);

    auto *subIndexLabel = new QLabel(QStringLiteral("SubIndex"), card);
    subIndexLabel->setObjectName(QStringLiteral("fieldLabel"));
    layout->addWidget(subIndexLabel);
    subIndexInput = new QLineEdit(card);
    subIndexInput->setObjectName(QStringLiteral("subIndexInput"));
    subIndexInput->setPlaceholderText(QStringLiteral("Enter subindex"));
    layout->addWidget(subIndexInput);

    auto *orderDetails = new QVBoxLayout;
    orderDetails->setSpacing(0);
    orderDetails->addWidget(createOrderRow(QStringLiteral("Assigned Block"), assignedBlockValue));
    orderDetails->addWidget(createOrderRow(QStringLiteral("Assigned Bin"), assignedBinValue));
    layout->addLayout(orderDetails);
    layout->addStretch();

    messageLabel = new QLabel(card);
    messageLabel->setObjectName(QStringLiteral("messageLabel"));
    layout->addWidget(messageLabel);

    auto *buttonLayout = new QHBoxLayout;
    buttonLayout->setSpacing(8);
    auto *findButton = new QPushButton(QStringLiteral("Find Location"), card);
    findButton->setObjectName(QStringLiteral("secondaryButton"));
    auto *storeButton = new QPushButton(QStringLiteral("Store Carrier"), card);
    storeButton->setObjectName(QStringLiteral("primaryButton"));
    auto *retrieveButton = new QPushButton(QStringLiteral("Retrieve Carrier"), card);
    retrieveButton->setObjectName(QStringLiteral("secondaryButton"));
    findButton->setCursor(Qt::PointingHandCursor);
    storeButton->setCursor(Qt::PointingHandCursor);
    retrieveButton->setCursor(Qt::PointingHandCursor);
    buttonLayout->addWidget(findButton);
    buttonLayout->addWidget(storeButton);
    buttonLayout->addWidget(retrieveButton);
    layout->addLayout(buttonLayout);

    connect(findButton, &QPushButton::clicked, this, [this] { findLocation(); });
    connect(storeButton, &QPushButton::clicked, this, [this] { storeCarrier(); });
    connect(retrieveButton, &QPushButton::clicked, this, [this] { retrieveCarrier(); });
    return card;
}

QFrame *StoragePage::createExceptionsCard()
{
    auto *card = new QFrame(this);
    card->setObjectName(QStringLiteral("contentCard"));
    auto *layout = new QVBoxLayout(card);
    layout->setContentsMargins(22, 20, 22, 20);
    layout->setSpacing(8);

    auto *title = new QLabel(QStringLiteral("Storage Exceptions"), card);
    title->setObjectName(QStringLiteral("cardTitle"));
    layout->addWidget(title);

    const QStringList exceptions{QStringLiteral("BIN FULL"), QStringLiteral("BIN EMPTY"),
                                 QStringLiteral("No Storage Location"), QStringLiteral("Unknown Carrier")};
    const QStringList descriptions{QStringLiteral("Capacity limit reached"), QStringLiteral("No carrier detected"),
                                   QStringLiteral("No suitable bin assigned"), QStringLiteral("Carrier is not registered")};
    const QStringList themes{QStringLiteral("warning"), QStringLiteral("ready"), QStringLiteral("error"), QStringLiteral("error")};

    for (int index = 0; index < exceptions.size(); ++index) {
        auto *exceptionRow = new QWidget(card);
        auto *rowLayout = new QHBoxLayout(exceptionRow);
        rowLayout->setContentsMargins(0, 10, 0, 10);
        rowLayout->setSpacing(12);

        auto *indicator = new QLabel(exceptionRow);
        indicator->setObjectName(QStringLiteral("exceptionIndicator"));
        indicator->setProperty("theme", themes.at(index));
        rowLayout->addWidget(indicator, 0, Qt::AlignTop);

        auto *textLayout = new QVBoxLayout;
        textLayout->setContentsMargins(0, 0, 0, 0);
        textLayout->setSpacing(2);
        auto *exceptionTitle = new QLabel(exceptions.at(index), exceptionRow);
        exceptionTitle->setObjectName(QStringLiteral("exceptionTitle"));
        textLayout->addWidget(exceptionTitle);
        auto *description = new QLabel(descriptions.at(index), exceptionRow);
        description->setObjectName(QStringLiteral("exceptionDescription"));
        textLayout->addWidget(description);
        rowLayout->addLayout(textLayout, 1);
        layout->addWidget(exceptionRow);
    }
    layout->addStretch();
    return card;
}

QWidget *StoragePage::createOrderRow(const QString &label, QLabel *&valueLabel)
{
    auto *row = new QWidget(this);
    auto *rowLayout = new QHBoxLayout(row);
    rowLayout->setContentsMargins(0, 0, 0, 0);

    auto *labelWidget = new QLabel(label, row);
    labelWidget->setObjectName(QStringLiteral("orderLabel"));
    rowLayout->addWidget(labelWidget);
    valueLabel = new QLabel(QStringLiteral("-"), row);
    valueLabel->setObjectName(QStringLiteral("orderValue"));
    valueLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    rowLayout->addWidget(valueLabel);
    return row;
}

void StoragePage::findLocation()
{
    if (carrierInput->text().trimmed().isEmpty()) {
        setMessage(QStringLiteral("Enter a product carrier first"), QStringLiteral("warning"));
        carrierInput->setFocus();
        return;
    }

    assignedBlockValue->setText(QStringLiteral("B-04"));
    assignedBinValue->setText(QStringLiteral("B-04-018"));
    setMessage(QStringLiteral("Location found for carrier"), QStringLiteral("success"));
}

void StoragePage::storeCarrier()
{
    if (carrierInput->text().trimmed().isEmpty()) {
        setMessage(QStringLiteral("Enter a product carrier first"), QStringLiteral("warning"));
        carrierInput->setFocus();
        return;
    }

    assignedBlockValue->setText(QStringLiteral("B-04"));
    assignedBinValue->setText(QStringLiteral("B-04-018"));
    ++storedToday;
    ++activeOrders;
    storedTodayValue->setText(QString::number(storedToday));
    activeOrdersValue->setText(QString::number(activeOrders));
    setMessage(QStringLiteral("Carrier queued for storage"), QStringLiteral("success"));
}

void StoragePage::retrieveCarrier()
{
    if (carrierInput->text().trimmed().isEmpty()) {
        setMessage(QStringLiteral("Enter a product carrier first"), QStringLiteral("warning"));
        carrierInput->setFocus();
        return;
    }

    ++retrievedToday;
    activeOrders = qMax(0, activeOrders - 1);
    retrievedTodayValue->setText(QString::number(retrievedToday));
    activeOrdersValue->setText(QString::number(activeOrders));
    setMessage(QStringLiteral("Carrier retrieval requested"), QStringLiteral("success"));
}

void StoragePage::setMessage(const QString &message, const QString &theme)
{
    messageLabel->setProperty("theme", theme);
    messageLabel->setText(message);
    messageLabel->style()->unpolish(messageLabel);
    messageLabel->style()->polish(messageLabel);
}
