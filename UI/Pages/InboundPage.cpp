#include "InboundPage.h"

#include <QDateTime>
#include <QFrame>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QStringList>
#include <QStyle>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QVBoxLayout>

namespace
{
QLabel *createValueLabel(QWidget *parent)
{
    auto *label = new QLabel(QStringLiteral("-"), parent);
    label->setObjectName(QStringLiteral("resultValue"));
    label->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    return label;
}

QTableWidgetItem *createTableItem(const QString &text)
{
    auto *item = new QTableWidgetItem(text);
    item->setFlags(item->flags() & ~Qt::ItemIsEditable);
    return item;
}
}

InboundPage::InboundPage(QWidget *parent)
    : QWidget(parent)
{
    setObjectName(QStringLiteral("inboundPage"));
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    auto *contentLayout = new QVBoxLayout(this);
    contentLayout->setContentsMargins(30, 28, 30, 30);
    contentLayout->setSpacing(20);

    auto *headerLayout = new QHBoxLayout;
    auto *titleLayout = new QVBoxLayout;
    titleLayout->setSpacing(3);

    auto *title = new QLabel(QStringLiteral("Inbound Scanner"), this);
    title->setObjectName(QStringLiteral("pageTitle"));
    titleLayout->addWidget(title);

    auto *subtitle = new QLabel(QStringLiteral("Scan and validate cartons as they arrive at the facility"), this);
    subtitle->setObjectName(QStringLiteral("pageSubtitle"));
    titleLayout->addWidget(subtitle);
    headerLayout->addLayout(titleLayout);
    headerLayout->addStretch();

    auto *stationLabel = new QLabel(QStringLiteral("STATION 01  |  ONLINE"), this);
    stationLabel->setObjectName(QStringLiteral("stationLabel"));
    headerLayout->addWidget(stationLabel, 0, Qt::AlignTop);
    contentLayout->addLayout(headerLayout);

    auto *bodyLayout = new QHBoxLayout;
    bodyLayout->setSpacing(18);

    auto *scannerCard = new QFrame(this);
    scannerCard->setObjectName(QStringLiteral("contentCard"));
    auto *scannerLayout = new QVBoxLayout(scannerCard);
    scannerLayout->setContentsMargins(22, 20, 22, 20);
    scannerLayout->setSpacing(12);

    auto *scannerTitle = new QLabel(QStringLiteral("Carton Scan"), scannerCard);
    scannerTitle->setObjectName(QStringLiteral("cardTitle"));
    scannerLayout->addWidget(scannerTitle);

    auto *fieldLabel = new QLabel(QStringLiteral("SSCC Barcode"), scannerCard);
    fieldLabel->setObjectName(QStringLiteral("fieldLabel"));
    scannerLayout->addWidget(fieldLabel);

    auto *scanRow = new QHBoxLayout;
    scanRow->setSpacing(10);
    barcodeInput = new QLineEdit(scannerCard);
    barcodeInput->setObjectName(QStringLiteral("barcodeInput"));
    barcodeInput->setPlaceholderText(QStringLiteral("Enter or scan SSCC barcode"));
    barcodeInput->setClearButtonEnabled(true);
    scanRow->addWidget(barcodeInput, 1);

    auto *scanButton = new QPushButton(QStringLiteral("Scan"), scannerCard);
    scanButton->setObjectName(QStringLiteral("primaryButton"));
    scanButton->setIcon(style()->standardIcon(QStyle::SP_DialogApplyButton));
    scanButton->setCursor(Qt::PointingHandCursor);
    scanRow->addWidget(scanButton);
    scannerLayout->addLayout(scanRow);

    auto *resultTitle = new QLabel(QStringLiteral("Scan Result"), scannerCard);
    resultTitle->setObjectName(QStringLiteral("subsectionTitle"));
    scannerLayout->addWidget(resultTitle);

    auto *resultLayout = new QVBoxLayout;
    resultLayout->setSpacing(0);
    resultLayout->addWidget(createResultRow(QStringLiteral("Barcode Status"), barcodeStatusValue));
    resultLayout->addWidget(createResultRow(QStringLiteral("First Seen Time"), firstSeenValue));
    resultLayout->addWidget(createResultRow(QStringLiteral("Length"), lengthValue));
    resultLayout->addWidget(createResultRow(QStringLiteral("Width"), widthValue));
    resultLayout->addWidget(createResultRow(QStringLiteral("Height"), heightValue));
    resultLayout->addWidget(createResultRow(QStringLiteral("Weight"), weightValue));
    resultLayout->addWidget(createResultRow(QStringLiteral("Carton Status"), cartonStatusValue));
    scannerLayout->addLayout(resultLayout);
    scannerLayout->addStretch();

    auto *actionLayout = new QHBoxLayout;
    actionLayout->setSpacing(8);
    auto *validateButton = new QPushButton(QStringLiteral("Validate"), scannerCard);
    validateButton->setObjectName(QStringLiteral("primaryButton"));
    auto *rescanButton = new QPushButton(QStringLiteral("Re-Scan"), scannerCard);
    rescanButton->setObjectName(QStringLiteral("secondaryButton"));
    auto *createButton = new QPushButton(QStringLiteral("Create Carton"), scannerCard);
    createButton->setObjectName(QStringLiteral("secondaryButton"));
    validateButton->setCursor(Qt::PointingHandCursor);
    rescanButton->setCursor(Qt::PointingHandCursor);
    createButton->setCursor(Qt::PointingHandCursor);
    actionLayout->addWidget(validateButton);
    actionLayout->addWidget(rescanButton);
    actionLayout->addWidget(createButton);
    scannerLayout->addLayout(actionLayout);

    bodyLayout->addWidget(scannerCard, 3);
    bodyLayout->addWidget(createStatisticsCard(), 2);
    contentLayout->addLayout(bodyLayout, 3);
    contentLayout->addWidget(createHistoryCard(), 2);

    connect(scanButton, &QPushButton::clicked, this, [this] { scanBarcode(); });
    connect(validateButton, &QPushButton::clicked, this, [this] { validateCarton(); });
    connect(rescanButton, &QPushButton::clicked, this, [this] { clearScan(); });
    connect(createButton, &QPushButton::clicked, this, [this] { createCarton(); });
    connect(barcodeInput, &QLineEdit::returnPressed, this, [this] { scanBarcode(); });

    setResultStatus(QStringLiteral("Waiting for scan"), QStringLiteral("ready"));
    cartonStatusValue->setText(QStringLiteral("Not scanned"));
}

QWidget *InboundPage::createResultRow(const QString &label, QLabel *&valueLabel)
{
    auto *row = new QWidget(this);
    auto *rowLayout = new QHBoxLayout(row);
    rowLayout->setContentsMargins(0, 0, 0, 0);

    auto *labelWidget = new QLabel(label, row);
    labelWidget->setObjectName(QStringLiteral("resultLabel"));
    rowLayout->addWidget(labelWidget);

    valueLabel = createValueLabel(row);
    rowLayout->addWidget(valueLabel);
    return row;
}

QFrame *InboundPage::createStatisticsCard()
{
    auto *card = new QFrame(this);
    card->setObjectName(QStringLiteral("contentCard"));

    auto *layout = new QVBoxLayout(card);
    layout->setContentsMargins(22, 20, 22, 20);
    layout->setSpacing(15);

    auto *title = new QLabel(QStringLiteral("Inbound Statistics"), card);
    title->setObjectName(QStringLiteral("cardTitle"));
    layout->addWidget(title);

    auto *statsGrid = new QGridLayout;
    statsGrid->setHorizontalSpacing(18);
    statsGrid->setVerticalSpacing(22);

    const QStringList labels{QStringLiteral("Total Scanned"), QStringLiteral("Successful"),
                             QStringLiteral("Failed"), QStringLiteral("Waiting")};
    QLabel **valuePointers[]{&totalScannedValue, &successfulValue, &failedValue, &waitingValue};
    const QStringList themes{QStringLiteral("total"), QStringLiteral("success"), QStringLiteral("error"), QStringLiteral("waiting")};

    for (int index = 0; index < labels.size(); ++index) {
        auto *stat = new QWidget(card);
        auto *statLayout = new QVBoxLayout(stat);
        statLayout->setContentsMargins(0, 0, 0, 0);
        statLayout->setSpacing(3);

        auto *value = new QLabel(stat);
        value->setObjectName(QStringLiteral("statValue"));
        value->setText(QString::number(index == 0 ? totalScanned : index == 1 ? successful : index == 2 ? failed : waiting));
        if (index > 0) {
            value->setProperty("theme", themes.at(index));
            value->setObjectName(QStringLiteral("statAccent"));
        }
        *valuePointers[index] = value;
        statLayout->addWidget(value);

        auto *label = new QLabel(labels.at(index), stat);
        label->setObjectName(QStringLiteral("statLabel"));
        statLayout->addWidget(label);
        statsGrid->addWidget(stat, index / 2, index % 2);
    }

    layout->addLayout(statsGrid);
    layout->addStretch();
    return card;
}

QFrame *InboundPage::createHistoryCard()
{
    auto *card = new QFrame(this);
    card->setObjectName(QStringLiteral("contentCard"));

    auto *layout = new QVBoxLayout(card);
    layout->setContentsMargins(22, 16, 22, 16);
    layout->setSpacing(8);

    auto *title = new QLabel(QStringLiteral("Scan History"), card);
    title->setObjectName(QStringLiteral("cardTitle"));
    layout->addWidget(title);

    historyTable = new QTableWidget(2, 3, card);
    historyTable->setObjectName(QStringLiteral("historyTable"));
    historyTable->setHorizontalHeaderLabels({QStringLiteral("SSCC"), QStringLiteral("Time"), QStringLiteral("Status")});
    historyTable->verticalHeader()->setVisible(false);
    historyTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    historyTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    historyTable->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    historyTable->setSelectionMode(QAbstractItemView::NoSelection);
    historyTable->setFocusPolicy(Qt::NoFocus);
    historyTable->setShowGrid(false);
    historyTable->setMinimumHeight(135);

    const QStringList barcodes{QStringLiteral("003456789012345678"), QStringLiteral("003456789012345661")};
    const QStringList times{QStringLiteral("08:42:16"), QStringLiteral("08:37:04")};
    const QStringList statuses{QStringLiteral("Successful"), QStringLiteral("Waiting")};
    for (int row = 0; row < barcodes.size(); ++row) {
        historyTable->setItem(row, 0, createTableItem(barcodes.at(row)));
        historyTable->setItem(row, 1, createTableItem(times.at(row)));
        historyTable->setItem(row, 2, createTableItem(statuses.at(row)));
    }

    layout->addWidget(historyTable, 1);
    return card;
}

void InboundPage::scanBarcode()
{
    if (barcodeInput->text().trimmed().isEmpty()) {
        barcodeInput->setText(QStringLiteral("003456789012345678"));
    }

    ++totalScanned;
    ++successful;
    waiting = qMax(0, waiting - 1);
    totalScannedValue->setText(QString::number(totalScanned));
    successfulValue->setText(QString::number(successful));
    waitingValue->setText(QString::number(waiting));

    firstSeenValue->setText(QDateTime::currentDateTime().toString(QStringLiteral("hh:mm:ss")));
    lengthValue->setText(QStringLiteral("400 mm"));
    widthValue->setText(QStringLiteral("300 mm"));
    heightValue->setText(QStringLiteral("250 mm"));
    weightValue->setText(QStringLiteral("8.4 kg"));
    cartonStatusValue->setText(QStringLiteral("Received"));
    setResultStatus(QStringLiteral("Valid barcode"), QStringLiteral("success"));
    addHistoryEntry(barcodeInput->text(), QStringLiteral("Successful"));
}

void InboundPage::clearScan()
{
    barcodeInput->clear();
    firstSeenValue->setText(QStringLiteral("-"));
    lengthValue->setText(QStringLiteral("-"));
    widthValue->setText(QStringLiteral("-"));
    heightValue->setText(QStringLiteral("-"));
    weightValue->setText(QStringLiteral("-"));
    cartonStatusValue->setText(QStringLiteral("Not scanned"));
    setResultStatus(QStringLiteral("Waiting for scan"), QStringLiteral("ready"));
    barcodeInput->setFocus();
}

void InboundPage::validateCarton()
{
    if (barcodeInput->text().trimmed().isEmpty()) {
        setResultStatus(QStringLiteral("Scan required"), QStringLiteral("warning"));
        return;
    }

    cartonStatusValue->setText(QStringLiteral("Validated"));
    setResultStatus(QStringLiteral("Validated"), QStringLiteral("success"));
}

void InboundPage::createCarton()
{
    if (barcodeInput->text().trimmed().isEmpty()) {
        setResultStatus(QStringLiteral("Scan required"), QStringLiteral("warning"));
        return;
    }

    cartonStatusValue->setText(QStringLiteral("Created"));
    addHistoryEntry(barcodeInput->text(), QStringLiteral("Carton created"));
}

void InboundPage::addHistoryEntry(const QString &barcode, const QString &status)
{
    if (historyTable == nullptr) {
        return;
    }

    historyTable->insertRow(0);
    historyTable->setItem(0, 0, createTableItem(barcode));
    historyTable->setItem(0, 1, createTableItem(QDateTime::currentDateTime().toString(QStringLiteral("hh:mm:ss"))));
    historyTable->setItem(0, 2, createTableItem(status));
}

void InboundPage::setResultStatus(const QString &status, const QString &theme)
{
    barcodeStatusValue->setObjectName(QStringLiteral("statusValue"));
    barcodeStatusValue->setProperty("theme", theme);
    barcodeStatusValue->setText(status);
    barcodeStatusValue->style()->unpolish(barcodeStatusValue);
    barcodeStatusValue->style()->polish(barcodeStatusValue);
}
