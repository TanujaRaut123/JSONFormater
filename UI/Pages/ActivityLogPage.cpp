#include "ActivityLogPage.h"

#include "../Components/MetricCard.h"
#include "../Components/SectionCard.h"

#include <QAbstractItemView>
#include <QComboBox>
#include <QDateTime>
#include <QFrame>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QStyle>
#include <QStringList>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QVBoxLayout>

namespace
{
const QStringList eventTypes{
    QStringLiteral("All Events"), QStringLiteral("Carton Created"), QStringLiteral("Transport Order Created"),
    QStringLiteral("Decant Started"), QStringLiteral("Inventory Created"), QStringLiteral("Storage Order Created"),
    QStringLiteral("Storage Completed"), QStringLiteral("Retrieval Completed"), QStringLiteral("Picking Started"),
    QStringLiteral("Order Shipped")};

const QStringList entityTypes{
    QStringLiteral("All Entities"), QStringLiteral("Carton"), QStringLiteral("Transport Order"),
    QStringLiteral("Inventory"), QStringLiteral("Storage Order"), QStringLiteral("Pick Order")};

QTableWidgetItem *createTableItem(const QString &text)
{
    auto *item = new QTableWidgetItem(text);
    item->setFlags(item->flags() & ~Qt::ItemIsEditable);
    return item;
}
}

ActivityLogPage::ActivityLogPage(QWidget *parent)
    : QWidget(parent)
{
    setObjectName(QStringLiteral("activityLogPage"));
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    auto *contentLayout = new QVBoxLayout(this);
    contentLayout->setContentsMargins(30, 28, 30, 30);
    contentLayout->setSpacing(18);

    auto *headerLayout = new QHBoxLayout;
    auto *titleLayout = new QVBoxLayout;
    titleLayout->setSpacing(3);
    auto *title = new QLabel(QStringLiteral("Activity Logs"), this);
    title->setObjectName(QStringLiteral("pageTitle"));
    titleLayout->addWidget(title);
    auto *subtitle = new QLabel(QStringLiteral("Monitor warehouse events across WCS and WMS operations"), this);
    subtitle->setObjectName(QStringLiteral("pageSubtitle"));
    titleLayout->addWidget(subtitle);
    headerLayout->addLayout(titleLayout);
    headerLayout->addStretch();
    auto *liveLabel = new QLabel(QStringLiteral("LIVE MONITORING"), this);
    liveLabel->setObjectName(QStringLiteral("liveLabel"));
    headerLayout->addWidget(liveLabel, 0, Qt::AlignTop);
    contentLayout->addLayout(headerLayout);

    contentLayout->addWidget(createFilterCard());

    auto *kpiGrid = new QGridLayout;
    kpiGrid->setContentsMargins(0, 0, 0, 0);
    kpiGrid->setHorizontalSpacing(16);
    kpiGrid->addWidget(new MetricCard(QStringLiteral("Events Today"), QStringLiteral("1,284"), QStringLiteral("inbound"), this, QStyle::SP_FileDialogDetailedView), 0, 0);
    kpiGrid->addWidget(new MetricCard(QStringLiteral("Warnings"), QStringLiteral("18"), QStringLiteral("pending"), this, QStyle::SP_MessageBoxWarning), 0, 1);
    kpiGrid->addWidget(new MetricCard(QStringLiteral("Errors"), QStringLiteral("4"), QStringLiteral("errors"), this, QStyle::SP_MessageBoxCritical), 0, 2);
    kpiGrid->addWidget(new MetricCard(QStringLiteral("Completed Operations"), QStringLiteral("936"), QStringLiteral("decanted"), this, QStyle::SP_DialogApplyButton), 0, 3);
    contentLayout->addLayout(kpiGrid);

    auto *mainLayout = new QHBoxLayout;
    mainLayout->setSpacing(18);
    mainLayout->addWidget(createEventsCard(), 4);
    mainLayout->addWidget(createDetailsCard(), 2);
    contentLayout->addLayout(mainLayout, 1);

    connect(searchInput, &QLineEdit::textChanged, this, [this] { applyFilters(); });
    connect(dateRangeFilter, &QComboBox::currentTextChanged, this, [this] { applyFilters(); });
    connect(eventTypeFilter, &QComboBox::currentTextChanged, this, [this] { applyFilters(); });
    connect(entityTypeFilter, &QComboBox::currentTextChanged, this, [this] { applyFilters(); });
    connect(eventTable, &QTableWidget::currentCellChanged, this,
            [this](int currentRow, int, int, int) { updateDetails(currentRow); });

    eventTable->selectRow(0);
}

QFrame *ActivityLogPage::createFilterCard()
{
    auto *card = new QFrame(this);
    card->setObjectName(QStringLiteral("filterCard"));
    auto *layout = new QHBoxLayout(card);
    layout->setContentsMargins(16, 12, 16, 12);
    layout->setSpacing(10);

    auto *dateLabel = new QLabel(QStringLiteral("Date Range"), card);
    dateLabel->setObjectName(QStringLiteral("filterLabel"));
    layout->addWidget(dateLabel);
    dateRangeFilter = new QComboBox(card);
    dateRangeFilter->setObjectName(QStringLiteral("activityFilter"));
    dateRangeFilter->addItems({QStringLiteral("Today"), QStringLiteral("Last 7 Days"), QStringLiteral("Last 30 Days")});
    layout->addWidget(dateRangeFilter);

    auto *eventLabel = new QLabel(QStringLiteral("Event Type"), card);
    eventLabel->setObjectName(QStringLiteral("filterLabel"));
    layout->addWidget(eventLabel);
    eventTypeFilter = new QComboBox(card);
    eventTypeFilter->setObjectName(QStringLiteral("activityFilter"));
    eventTypeFilter->addItems(eventTypes);
    layout->addWidget(eventTypeFilter, 1);

    auto *entityLabel = new QLabel(QStringLiteral("Entity Type"), card);
    entityLabel->setObjectName(QStringLiteral("filterLabel"));
    layout->addWidget(entityLabel);
    entityTypeFilter = new QComboBox(card);
    entityTypeFilter->setObjectName(QStringLiteral("activityFilter"));
    entityTypeFilter->addItems(entityTypes);
    layout->addWidget(entityTypeFilter);

    searchInput = new QLineEdit(card);
    searchInput->setObjectName(QStringLiteral("activitySearch"));
    searchInput->setPlaceholderText(QStringLiteral("Search events or entity IDs"));
    searchInput->setClearButtonEnabled(true);
    layout->addWidget(searchInput, 1);

    auto *refreshButton = new QPushButton(QStringLiteral("Refresh"), card);
    refreshButton->setObjectName(QStringLiteral("secondaryButton"));
    refreshButton->setIcon(style()->standardIcon(QStyle::SP_BrowserReload));
    refreshButton->setCursor(Qt::PointingHandCursor);
    layout->addWidget(refreshButton);
    connect(refreshButton, &QPushButton::clicked, this, [this] { refreshEvents(); });
    return card;
}

QFrame *ActivityLogPage::createDetailsCard()
{
    auto *card = new SectionCard(QStringLiteral("Event Details"), this);
    auto *layout = card->contentLayout();
    layout->addWidget(createDetailRow(QStringLiteral("Event ID"), eventIdValue));
    layout->addWidget(createDetailRow(QStringLiteral("Timestamp"), timestampValue));
    layout->addWidget(createDetailRow(QStringLiteral("Entity"), entityValue));
    layout->addWidget(createDetailRow(QStringLiteral("Location"), locationValue));
    layout->addWidget(createDetailRow(QStringLiteral("Result"), resultValue));
    layout->addWidget(createDetailRow(QStringLiteral("Description"), descriptionValue));
    layout->addStretch();
    return card;
}

QFrame *ActivityLogPage::createEventsCard()
{
    auto *card = new SectionCard(QStringLiteral("Warehouse Events"), this);
    auto *layout = card->contentLayout();
    eventTable = new QTableWidget(9, 6, card);
    eventTable->setObjectName(QStringLiteral("activityTable"));
    eventTable->setHorizontalHeaderLabels({QStringLiteral("Timestamp"), QStringLiteral("Event Type"), QStringLiteral("Entity Type"), QStringLiteral("Entity ID"), QStringLiteral("Description"), QStringLiteral("Status")});
    eventTable->verticalHeader()->setVisible(false);
    eventTable->horizontalHeader()->setStretchLastSection(true);
    eventTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    eventTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    eventTable->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    eventTable->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);
    eventTable->horizontalHeader()->setSectionResizeMode(4, QHeaderView::Stretch);
    eventTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    eventTable->setSelectionMode(QAbstractItemView::SingleSelection);
    eventTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    eventTable->setAlternatingRowColors(true);
    eventTable->setShowGrid(false);

    const QList<QStringList> events{
        {QStringLiteral("08:42:16"), QStringLiteral("Carton Created"), QStringLiteral("Carton"), QStringLiteral("CTN-10482"), QStringLiteral("Inbound carton registered"), QStringLiteral("Completed")},
        {QStringLiteral("08:40:03"), QStringLiteral("Transport Order Created"), QStringLiteral("Transport Order"), QStringLiteral("TO-88241"), QStringLiteral("Transport order queued"), QStringLiteral("Completed")},
        {QStringLiteral("08:37:04"), QStringLiteral("Decant Started"), QStringLiteral("Carton"), QStringLiteral("CTN-10479"), QStringLiteral("Decant station assigned"), QStringLiteral("In Progress")},
        {QStringLiteral("08:32:48"), QStringLiteral("Inventory Created"), QStringLiteral("Inventory"), QStringLiteral("INV-55281"), QStringLiteral("Inventory record created"), QStringLiteral("Completed")},
        {QStringLiteral("08:25:51"), QStringLiteral("Storage Order Created"), QStringLiteral("Storage Order"), QStringLiteral("SO-30198"), QStringLiteral("Carrier location requested"), QStringLiteral("Pending")},
        {QStringLiteral("08:18:29"), QStringLiteral("Storage Completed"), QStringLiteral("Storage Order"), QStringLiteral("SO-30194"), QStringLiteral("Carrier stored in B-04-018"), QStringLiteral("Completed")},
        {QStringLiteral("08:03:12"), QStringLiteral("Retrieval Completed"), QStringLiteral("Storage Order"), QStringLiteral("SO-30189"), QStringLiteral("Carrier delivered to GTP-04"), QStringLiteral("Completed")},
        {QStringLiteral("07:56:48"), QStringLiteral("Picking Started"), QStringLiteral("Pick Order"), QStringLiteral("PO-77420"), QStringLiteral("Picking task released"), QStringLiteral("In Progress")},
        {QStringLiteral("07:41:22"), QStringLiteral("Order Shipped"), QStringLiteral("Pick Order"), QStringLiteral("PO-77411"), QStringLiteral("Order dispatched from facility"), QStringLiteral("Completed")}};

    for (int row = 0; row < events.size(); ++row) {
        for (int column = 0; column < events.at(row).size(); ++column) {
            auto *item = createTableItem(events.at(row).at(column));
            if (column == 5) {
                setStatusBadge(item, events.at(row).at(column));
            }
            eventTable->setItem(row, column, item);
        }
    }
    layout->addWidget(eventTable, 1);
    return card;
}

QWidget *ActivityLogPage::createDetailRow(const QString &label, QLabel *&valueLabel)
{
    auto *row = new QWidget(this);
    auto *layout = new QVBoxLayout(row);
    layout->setContentsMargins(0, 3, 0, 3);
    layout->setSpacing(2);
    auto *labelWidget = new QLabel(label, row);
    labelWidget->setObjectName(QStringLiteral("detailLabel"));
    layout->addWidget(labelWidget);
    valueLabel = new QLabel(QStringLiteral("-"), row);
    valueLabel->setObjectName(QStringLiteral("activityDetailValue"));
    valueLabel->setWordWrap(true);
    layout->addWidget(valueLabel);
    return row;
}

void ActivityLogPage::applyFilters()
{
    const QString search = searchInput->text().trimmed();
    const QString eventType = eventTypeFilter->currentText();
    const QString entityType = entityTypeFilter->currentText();
    for (int row = 0; row < eventTable->rowCount(); ++row) {
        const bool eventMatches = eventType == QStringLiteral("All Events") || eventTable->item(row, 1)->text() == eventType;
        const bool entityMatches = entityType == QStringLiteral("All Entities") || eventTable->item(row, 2)->text() == entityType;
        bool searchMatches = search.isEmpty();
        for (int column = 0; column < eventTable->columnCount() && !searchMatches; ++column) {
            searchMatches = eventTable->item(row, column)->text().contains(search, Qt::CaseInsensitive);
        }
        eventTable->setRowHidden(row, !(eventMatches && entityMatches && searchMatches));
    }
}

void ActivityLogPage::updateDetails(int row)
{
    if (row < 0 || eventTable->isRowHidden(row)) {
        eventIdValue->setText(QStringLiteral("-"));
        timestampValue->setText(QStringLiteral("-"));
        entityValue->setText(QStringLiteral("-"));
        locationValue->setText(QStringLiteral("-"));
        resultValue->setText(QStringLiteral("-"));
        descriptionValue->setText(QStringLiteral("Select an event to view details"));
        return;
    }
    eventIdValue->setText(QStringLiteral("EVT-%1").arg(1284 - row));
    timestampValue->setText(eventTable->item(row, 0)->text());
    entityValue->setText(QStringLiteral("%1  |  %2").arg(eventTable->item(row, 2)->text(), eventTable->item(row, 3)->text()));
    locationValue->setText(row % 2 == 0 ? QStringLiteral("Inbound Dock 01") : QStringLiteral("GTP Workstation 04"));
    resultValue->setText(eventTable->item(row, 5)->text());
    descriptionValue->setText(eventTable->item(row, 4)->text());
}

void ActivityLogPage::refreshEvents()
{
    searchInput->clear();
    dateRangeFilter->setCurrentIndex(0);
    eventTypeFilter->setCurrentIndex(0);
    entityTypeFilter->setCurrentIndex(0);
    for (int row = 0; row < eventTable->rowCount(); ++row) {
        eventTable->setRowHidden(row, false);
    }
    eventTable->selectRow(0);
}

void ActivityLogPage::setStatusBadge(QTableWidgetItem *item, const QString &status)
{
    item->setTextAlignment(Qt::AlignCenter);
    if (status == QStringLiteral("Completed")) {
        item->setBackground(QColor(QStringLiteral("#e7f5ef")));
        item->setForeground(QColor(QStringLiteral("#267c61")));
    } else if (status == QStringLiteral("Pending")) {
        item->setBackground(QColor(QStringLiteral("#fff5dc")));
        item->setForeground(QColor(QStringLiteral("#a97013")));
    } else {
        item->setBackground(QColor(QStringLiteral("#e5f1f8")));
        item->setForeground(QColor(QStringLiteral("#23678f")));
    }
}
