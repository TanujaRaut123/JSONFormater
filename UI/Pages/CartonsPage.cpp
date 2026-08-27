#include "CartonsPage.h"

#include <QAbstractItemView>
#include <QComboBox>
#include <QDateTime>
#include <QFrame>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QLineEdit>
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

CartonsPage::CartonsPage(QWidget *parent)
    : QWidget(parent)
{
    setObjectName(QStringLiteral("cartonsPage"));
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    auto *contentLayout = new QVBoxLayout(this);
    contentLayout->setContentsMargins(30, 28, 30, 30);
    contentLayout->setSpacing(20);

    auto *headerLayout = new QHBoxLayout;
    auto *titleLayout = new QVBoxLayout;
    titleLayout->setSpacing(3);

    auto *title = new QLabel(QStringLiteral("Cartons"), this);
    title->setObjectName(QStringLiteral("pageTitle"));
    titleLayout->addWidget(title);

    auto *subtitle = new QLabel(QStringLiteral("Search, review, and manage all cartons in the facility"), this);
    subtitle->setObjectName(QStringLiteral("pageSubtitle"));
    titleLayout->addWidget(subtitle);
    headerLayout->addLayout(titleLayout);
    headerLayout->addStretch();

    auto *countLabel = new QLabel(QStringLiteral("12 CARTONS"), this);
    countLabel->setObjectName(QStringLiteral("countLabel"));
    headerLayout->addWidget(countLabel, 0, Qt::AlignTop);
    contentLayout->addLayout(headerLayout);

    auto *toolbar = new QHBoxLayout;
    toolbar->setSpacing(10);
    searchInput = new QLineEdit(this);
    searchInput->setObjectName(QStringLiteral("searchInput"));
    searchInput->setPlaceholderText(QStringLiteral("Search by SSCC, location, or status"));
    searchInput->setClearButtonEnabled(true);
    toolbar->addWidget(searchInput, 1);

    auto *filterLabel = new QLabel(QStringLiteral("Status"), this);
    filterLabel->setObjectName(QStringLiteral("toolbarLabel"));
    toolbar->addWidget(filterLabel);

    statusFilter = new QComboBox(this);
    statusFilter->setObjectName(QStringLiteral("statusFilter"));
    statusFilter->addItems({QStringLiteral("All"), QStringLiteral("Created"), QStringLiteral("Pending"),
                            QStringLiteral("Decanting"), QStringLiteral("Completed")});
    statusFilter->setMinimumWidth(150);
    toolbar->addWidget(statusFilter);
    contentLayout->addLayout(toolbar);

    auto *mainLayout = new QHBoxLayout;
    mainLayout->setSpacing(18);

    auto *tableCard = new QFrame(this);
    tableCard->setObjectName(QStringLiteral("contentCard"));
    auto *tableLayout = new QVBoxLayout(tableCard);
    tableLayout->setContentsMargins(18, 18, 18, 18);
    tableLayout->setSpacing(10);

    auto *tableTitle = new QLabel(QStringLiteral("All Cartons"), tableCard);
    tableTitle->setObjectName(QStringLiteral("cardTitle"));
    tableLayout->addWidget(tableTitle);

    cartonTable = new QTableWidget(6, 7, tableCard);
    cartonTable->setObjectName(QStringLiteral("cartonTable"));
    cartonTable->setHorizontalHeaderLabels({QStringLiteral("SSCC"), QStringLiteral("First Seen Time"),
                                             QStringLiteral("Length"), QStringLiteral("Width"),
                                             QStringLiteral("Height"), QStringLiteral("Weight"),
                                             QStringLiteral("Status")});
    cartonTable->verticalHeader()->setVisible(false);
    cartonTable->horizontalHeader()->setStretchLastSection(true);
    cartonTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    cartonTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    cartonTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    cartonTable->setSelectionMode(QAbstractItemView::SingleSelection);
    cartonTable->setEditTriggers(QAbstractItemView::DoubleClicked | QAbstractItemView::EditKeyPressed);
    cartonTable->setAlternatingRowColors(true);
    cartonTable->setShowGrid(false);

    const QList<QStringList> cartons{
        {QStringLiteral("003456789012345678"), QStringLiteral("08:42:16"), QStringLiteral("400 mm"), QStringLiteral("300 mm"), QStringLiteral("250 mm"), QStringLiteral("8.4 kg"), QStringLiteral("Completed")},
        {QStringLiteral("003456789012345661"), QStringLiteral("08:37:04"), QStringLiteral("600 mm"), QStringLiteral("400 mm"), QStringLiteral("300 mm"), QStringLiteral("12.1 kg"), QStringLiteral("Pending")},
        {QStringLiteral("003456789012345654"), QStringLiteral("08:25:51"), QStringLiteral("300 mm"), QStringLiteral("200 mm"), QStringLiteral("180 mm"), QStringLiteral("4.6 kg"), QStringLiteral("Decanting")},
        {QStringLiteral("003456789012345647"), QStringLiteral("08:18:29"), QStringLiteral("500 mm"), QStringLiteral("350 mm"), QStringLiteral("280 mm"), QStringLiteral("9.8 kg"), QStringLiteral("Created")},
        {QStringLiteral("003456789012345630"), QStringLiteral("08:03:12"), QStringLiteral("450 mm"), QStringLiteral("320 mm"), QStringLiteral("220 mm"), QStringLiteral("7.2 kg"), QStringLiteral("Completed")},
        {QStringLiteral("003456789012345623"), QStringLiteral("07:56:48"), QStringLiteral("250 mm"), QStringLiteral("180 mm"), QStringLiteral("160 mm"), QStringLiteral("3.1 kg"), QStringLiteral("Pending")}};

    for (int row = 0; row < cartons.size(); ++row) {
        for (int column = 0; column < cartons.at(row).size(); ++column) {
            cartonTable->setItem(row, column, createTableItem(cartons.at(row).at(column)));
        }
    }
    tableLayout->addWidget(cartonTable, 1);
    mainLayout->addWidget(tableCard, 4);
    mainLayout->addWidget(createDetailsCard(), 2);
    contentLayout->addLayout(mainLayout, 1);

    connect(searchInput, &QLineEdit::textChanged, this, [this] { applyFilters(); });
    connect(statusFilter, &QComboBox::currentTextChanged, this, [this] { applyFilters(); });
    connect(cartonTable, &QTableWidget::currentCellChanged, this,
            [this](int currentRow, int, int, int) { updateDetails(currentRow); });

    cartonTable->selectRow(0);
}

QWidget *CartonsPage::createDetailsRow(const QString &label, QLabel *&valueLabel)
{
    auto *row = new QWidget(this);
    auto *rowLayout = new QHBoxLayout(row);
    rowLayout->setContentsMargins(0, 0, 0, 0);

    auto *labelWidget = new QLabel(label, row);
    labelWidget->setObjectName(QStringLiteral("detailLabel"));
    rowLayout->addWidget(labelWidget);

    valueLabel = new QLabel(QStringLiteral("-"), row);
    valueLabel->setObjectName(QStringLiteral("detailValue"));
    valueLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    rowLayout->addWidget(valueLabel);
    return row;
}

QFrame *CartonsPage::createDetailsCard()
{
    auto *card = new QFrame(this);
    card->setObjectName(QStringLiteral("contentCard"));
    auto *layout = new QVBoxLayout(card);
    layout->setContentsMargins(22, 20, 22, 20);
    layout->setSpacing(6);

    auto *title = new QLabel(QStringLiteral("Carton Details"), card);
    title->setObjectName(QStringLiteral("cardTitle"));
    layout->addWidget(title);

    layout->addWidget(createDetailsRow(QStringLiteral("SSCC"), detailSsccValue));
    layout->addWidget(createDetailsRow(QStringLiteral("Current Location"), detailLocationValue));
    layout->addWidget(createDetailsRow(QStringLiteral("Status"), detailStatusValue));
    layout->addWidget(createDetailsRow(QStringLiteral("Created Time"), detailCreatedValue));
    layout->addWidget(createDetailsRow(QStringLiteral("Last Updated"), detailUpdatedValue));
    layout->addStretch();

    auto *buttonLayout = new QHBoxLayout;
    buttonLayout->setSpacing(8);
    auto *viewButton = new QPushButton(QStringLiteral("View"), card);
    viewButton->setObjectName(QStringLiteral("primaryButton"));
    auto *editButton = new QPushButton(QStringLiteral("Edit"), card);
    editButton->setObjectName(QStringLiteral("secondaryButton"));
    auto *deleteButton = new QPushButton(QStringLiteral("Delete"), card);
    deleteButton->setObjectName(QStringLiteral("dangerButton"));
    viewButton->setCursor(Qt::PointingHandCursor);
    editButton->setCursor(Qt::PointingHandCursor);
    deleteButton->setCursor(Qt::PointingHandCursor);
    buttonLayout->addWidget(viewButton);
    buttonLayout->addWidget(editButton);
    buttonLayout->addWidget(deleteButton);
    layout->addLayout(buttonLayout);

    connect(viewButton, &QPushButton::clicked, this, [this] { viewSelectedCarton(); });
    connect(editButton, &QPushButton::clicked, this, [this] { editSelectedCarton(); });
    connect(deleteButton, &QPushButton::clicked, this, [this] { deleteSelectedCarton(); });
    return card;
}

void CartonsPage::applyFilters()
{
    const QString searchText = searchInput->text().trimmed();
    const QString selectedStatus = statusFilter->currentText();

    for (int row = 0; row < cartonTable->rowCount(); ++row) {
        bool matchesSearch = searchText.isEmpty();
        for (int column = 0; column < cartonTable->columnCount() && !matchesSearch; ++column) {
            matchesSearch = cartonTable->item(row, column)->text().contains(searchText, Qt::CaseInsensitive);
        }
        const bool matchesStatus = selectedStatus == QStringLiteral("All")
                                   || cartonTable->item(row, 6)->text() == selectedStatus;
        cartonTable->setRowHidden(row, !(matchesSearch && matchesStatus));
    }
}

void CartonsPage::updateDetails(int row)
{
    if (row < 0 || cartonTable->isRowHidden(row)) {
        clearDetails();
        return;
    }

    detailSsccValue->setText(cartonTable->item(row, 0)->text());
    detailLocationValue->setText(row % 2 == 0 ? QStringLiteral("Inbound Dock 01") : QStringLiteral("Decant Zone B"));
    detailStatusValue->setText(cartonTable->item(row, 6)->text());
    detailStatusValue->setObjectName(QStringLiteral("detailStatus"));
    detailStatusValue->setProperty("theme", cartonTable->item(row, 6)->text().toLower());
    detailCreatedValue->setText(QStringLiteral("27 Aug 2026, %1").arg(cartonTable->item(row, 1)->text()));
    detailUpdatedValue->setText(QDateTime::currentDateTime().toString(QStringLiteral("dd MMM yyyy, hh:mm:ss")));
    detailStatusValue->style()->unpolish(detailStatusValue);
    detailStatusValue->style()->polish(detailStatusValue);
}

void CartonsPage::clearDetails()
{
    detailSsccValue->setText(QStringLiteral("-") );
    detailLocationValue->setText(QStringLiteral("-") );
    detailStatusValue->setText(QStringLiteral("No carton selected"));
    detailCreatedValue->setText(QStringLiteral("-") );
    detailUpdatedValue->setText(QStringLiteral("-") );
}

void CartonsPage::viewSelectedCarton()
{
    if (cartonTable->currentRow() >= 0) {
        updateDetails(cartonTable->currentRow());
    }
}

void CartonsPage::editSelectedCarton()
{
    if (cartonTable->currentRow() >= 0) {
        cartonTable->editItem(cartonTable->item(cartonTable->currentRow(), 6));
    }
}

void CartonsPage::deleteSelectedCarton()
{
    const int row = cartonTable->currentRow();
    if (row >= 0) {
        cartonTable->removeRow(row);
        clearDetails();
    }
}
