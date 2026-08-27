#include "PickingPage.h"

#include <QAbstractItemView>
#include <QFrame>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QProgressBar>
#include <QPushButton>
#include <QStringList>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QVBoxLayout>

namespace
{
QTableWidgetItem *createTableItem(const QString &text)
{
    auto *item = new QTableWidgetItem(text);
    item->setFlags(item->flags() & ~Qt::ItemIsEditable);
    return item;
}
}

PickingPage::PickingPage(QWidget *parent)
    : QWidget(parent)
{
    setObjectName(QStringLiteral("pickingPage"));
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    auto *contentLayout = new QVBoxLayout(this);
    contentLayout->setContentsMargins(30, 28, 30, 30);
    contentLayout->setSpacing(20);

    auto *headerLayout = new QHBoxLayout;
    auto *titleLayout = new QVBoxLayout;
    titleLayout->setSpacing(3);

    auto *title = new QLabel(QStringLiteral("GTP Picking Workstation"), this);
    title->setObjectName(QStringLiteral("pageTitle"));
    titleLayout->addWidget(title);

    auto *subtitle = new QLabel(QStringLiteral("Pick, verify, and complete your active order tasks"), this);
    subtitle->setObjectName(QStringLiteral("pageSubtitle"));
    titleLayout->addWidget(subtitle);
    headerLayout->addLayout(titleLayout);
    headerLayout->addStretch();

    auto *stationLabel = new QLabel(QStringLiteral("GTP-04  |  READY"), this);
    stationLabel->setObjectName(QStringLiteral("stationLabel"));
    headerLayout->addWidget(stationLabel, 0, Qt::AlignTop);
    contentLayout->addLayout(headerLayout);

    auto *topLayout = new QHBoxLayout;
    topLayout->setSpacing(18);
    topLayout->addWidget(createOrderCard(), 2);
    topLayout->addWidget(createTaskCard(), 3);
    topLayout->addWidget(createExceptionsCard(), 2);
    contentLayout->addLayout(topLayout, 2);
    contentLayout->addWidget(createTaskTableCard(), 3);

    connect(taskTable, &QTableWidget::currentCellChanged, this,
            [this](int currentRow, int, int, int) { updateTask(currentRow); });

    taskTable->selectRow(0);
}

QWidget *PickingPage::createInfoRow(const QString &label, QLabel *&valueLabel)
{
    auto *row = new QWidget(this);
    auto *rowLayout = new QHBoxLayout(row);
    rowLayout->setContentsMargins(0, 0, 0, 0);

    auto *labelWidget = new QLabel(label, row);
    labelWidget->setObjectName(QStringLiteral("sectionLabel"));
    rowLayout->addWidget(labelWidget);

    valueLabel = new QLabel(QStringLiteral("-"), row);
    valueLabel->setObjectName(QStringLiteral("infoValue"));
    valueLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    rowLayout->addWidget(valueLabel);
    return row;
}

QFrame *PickingPage::createOrderCard()
{
    auto *card = new QFrame(this);
    card->setObjectName(QStringLiteral("contentCard"));
    auto *layout = new QVBoxLayout(card);
    layout->setContentsMargins(20, 20, 20, 20);
    layout->setSpacing(5);

    auto *title = new QLabel(QStringLiteral("Order Information"), card);
    title->setObjectName(QStringLiteral("cardTitle"));
    layout->addWidget(title);
    layout->addSpacing(8);
    layout->addWidget(createInfoRow(QStringLiteral("Order ID"), orderIdValue));
    layout->addWidget(createInfoRow(QStringLiteral("Pack Group"), packGroupValue));
    layout->addWidget(createInfoRow(QStringLiteral("Workstation"), workstationValue));
    layout->addStretch();

    orderIdValue->setText(QStringLiteral("ORD-20260827-041"));
    packGroupValue->setText(QStringLiteral("PG-07"));
    workstationValue->setText(QStringLiteral("GTP-04"));
    return card;
}

QFrame *PickingPage::createTaskCard()
{
    auto *card = new QFrame(this);
    card->setObjectName(QStringLiteral("contentCard"));
    auto *layout = new QVBoxLayout(card);
    layout->setContentsMargins(22, 20, 22, 20);
    layout->setSpacing(8);

    auto *title = new QLabel(QStringLiteral("Picking Task"), card);
    title->setObjectName(QStringLiteral("cardTitle"));
    layout->addWidget(title);

    auto *taskGrid = new QGridLayout;
    taskGrid->setHorizontalSpacing(22);
    taskGrid->setVerticalSpacing(3);
    taskGrid->addWidget(new QLabel(QStringLiteral("SKU"), card), 0, 0);
    taskGrid->addWidget(new QLabel(QStringLiteral("Expected Quantity"), card), 0, 1);
    taskGrid->addWidget(new QLabel(QStringLiteral("Picked Quantity"), card), 0, 2);
    skuValue = new QLabel(card);
    skuValue->setObjectName(QStringLiteral("taskValue"));
    expectedValue = new QLabel(card);
    expectedValue->setObjectName(QStringLiteral("taskValue"));
    pickedValue = new QLabel(card);
    pickedValue->setObjectName(QStringLiteral("taskValue"));
    taskGrid->addWidget(skuValue, 1, 0);
    taskGrid->addWidget(expectedValue, 1, 1);
    taskGrid->addWidget(pickedValue, 1, 2);
    layout->addLayout(taskGrid);

    auto *progressLabel = new QLabel(QStringLiteral("Pick Progress"), card);
    progressLabel->setObjectName(QStringLiteral("sectionLabel"));
    layout->addWidget(progressLabel);
    progressBar = new QProgressBar(card);
    progressBar->setObjectName(QStringLiteral("pickProgress"));
    progressBar->setRange(0, 100);
    progressBar->setTextVisible(true);
    layout->addWidget(progressBar);

    auto *statusRow = new QHBoxLayout;
    auto *statusLabel = new QLabel(QStringLiteral("Task Status"), card);
    statusLabel->setObjectName(QStringLiteral("sectionLabel"));
    statusRow->addWidget(statusLabel);
    taskStatusValue = new QLabel(card);
    taskStatusValue->setObjectName(QStringLiteral("taskStatus"));
    taskStatusValue->setAlignment(Qt::AlignRight);
    statusRow->addWidget(taskStatusValue);
    layout->addLayout(statusRow);
    layout->addStretch();

    auto *buttonLayout = new QHBoxLayout;
    buttonLayout->setSpacing(8);
    auto *confirmButton = new QPushButton(QStringLiteral("Confirm Pick"), card);
    confirmButton->setObjectName(QStringLiteral("primaryButton"));
    auto *nextButton = new QPushButton(QStringLiteral("Next Task"), card);
    nextButton->setObjectName(QStringLiteral("secondaryButton"));
    confirmButton->setCursor(Qt::PointingHandCursor);
    nextButton->setCursor(Qt::PointingHandCursor);
    buttonLayout->addWidget(confirmButton);
    buttonLayout->addWidget(nextButton);
    layout->addLayout(buttonLayout);

    connect(confirmButton, &QPushButton::clicked, this, [this] { confirmPick(); });
    connect(nextButton, &QPushButton::clicked, this, [this] { nextTask(); });
    return card;
}

QFrame *PickingPage::createExceptionsCard()
{
    auto *card = new QFrame(this);
    card->setObjectName(QStringLiteral("contentCard"));
    auto *layout = new QVBoxLayout(card);
    layout->setContentsMargins(20, 20, 20, 20);
    layout->setSpacing(7);

    auto *title = new QLabel(QStringLiteral("Picking Exceptions"), card);
    title->setObjectName(QStringLiteral("cardTitle"));
    layout->addWidget(title);

    const QStringList exceptions{QStringLiteral("Wrong SKU"), QStringLiteral("Missing Items"),
                                 QStringLiteral("Damaged Items"), QStringLiteral("Order Carrier Full"),
                                 QStringLiteral("Zero Cross Check")};
    const QStringList descriptions{QStringLiteral("Scanned item does not match"), QStringLiteral("Quantity is unavailable"),
                                   QStringLiteral("Item requires inspection"), QStringLiteral("Carrier has reached capacity"),
                                   QStringLiteral("Verification is required")};
    const QStringList themes{QStringLiteral("error"), QStringLiteral("warning"), QStringLiteral("error"),
                             QStringLiteral("warning"), QStringLiteral("warning")};

    for (int index = 0; index < exceptions.size(); ++index) {
        auto *exceptionRow = new QWidget(card);
        auto *rowLayout = new QHBoxLayout(exceptionRow);
        rowLayout->setContentsMargins(0, 8, 0, 8);
        rowLayout->setSpacing(11);
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

QFrame *PickingPage::createTaskTableCard()
{
    auto *card = new QFrame(this);
    card->setObjectName(QStringLiteral("contentCard"));
    auto *layout = new QVBoxLayout(card);
    layout->setContentsMargins(20, 16, 20, 16);
    layout->setSpacing(8);

    auto *title = new QLabel(QStringLiteral("Pick Task Table"), card);
    title->setObjectName(QStringLiteral("cardTitle"));
    layout->addWidget(title);

    taskTable = new QTableWidget(5, 4, card);
    taskTable->setObjectName(QStringLiteral("taskTable"));
    taskTable->setHorizontalHeaderLabels({QStringLiteral("SKU"), QStringLiteral("Expected"),
                                          QStringLiteral("Picked"), QStringLiteral("Status")});
    taskTable->verticalHeader()->setVisible(false);
    taskTable->horizontalHeader()->setStretchLastSection(true);
    taskTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    taskTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    taskTable->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    taskTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    taskTable->setSelectionMode(QAbstractItemView::SingleSelection);
    taskTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    taskTable->setAlternatingRowColors(true);
    taskTable->setShowGrid(false);

    const QList<QStringList> tasks{
        {QStringLiteral("SKU-44108"), QStringLiteral("12"), QStringLiteral("0"), QStringLiteral("Ready")},
        {QStringLiteral("SKU-77214"), QStringLiteral("8"), QStringLiteral("3"), QStringLiteral("In progress")},
        {QStringLiteral("SKU-19032"), QStringLiteral("4"), QStringLiteral("4"), QStringLiteral("Completed")},
        {QStringLiteral("SKU-55367"), QStringLiteral("16"), QStringLiteral("0"), QStringLiteral("Ready")},
        {QStringLiteral("SKU-62891"), QStringLiteral("6"), QStringLiteral("0"), QStringLiteral("Ready")}};
    for (int row = 0; row < tasks.size(); ++row) {
        for (int column = 0; column < tasks.at(row).size(); ++column) {
            taskTable->setItem(row, column, createTableItem(tasks.at(row).at(column)));
        }
    }
    layout->addWidget(taskTable, 1);
    return card;
}

void PickingPage::updateTask(int row)
{
    if (row < 0 || row >= taskTable->rowCount()) {
        return;
    }

    skuValue->setText(taskTable->item(row, 0)->text());
    expectedValue->setText(taskTable->item(row, 1)->text());
    pickedValue->setText(taskTable->item(row, 2)->text());
    const QString status = taskTable->item(row, 3)->text();
    setTaskStatus(status, status == QStringLiteral("Completed") ? QStringLiteral("completed")
                                                                  : status == QStringLiteral("In progress") ? QStringLiteral("progress") : QStringLiteral("ready"));
    updateProgress();
}

void PickingPage::updateProgress()
{
    const int expected = expectedValue->text().toInt();
    const int picked = pickedValue->text().toInt();
    progressBar->setValue(expected > 0 ? (picked * 100) / expected : 0);
}

void PickingPage::confirmPick()
{
    const int row = taskTable->currentRow();
    if (row < 0) {
        return;
    }

    taskTable->item(row, 2)->setText(taskTable->item(row, 1)->text());
    taskTable->item(row, 3)->setText(QStringLiteral("Completed"));
    updateTask(row);
}

void PickingPage::nextTask()
{
    const int nextRow = (taskTable->currentRow() + 1) % taskTable->rowCount();
    taskTable->selectRow(nextRow);
}

void PickingPage::setTaskStatus(const QString &status, const QString &theme)
{
    taskStatusValue->setObjectName(QStringLiteral("taskStatus"));
    taskStatusValue->setProperty("theme", theme);
    taskStatusValue->setText(status);
    taskStatusValue->style()->unpolish(taskStatusValue);
    taskStatusValue->style()->polish(taskStatusValue);
}
