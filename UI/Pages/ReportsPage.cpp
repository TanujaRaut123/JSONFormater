#include "ReportsPage.h"

#include <QBarCategoryAxis>
#include <QBarSet>
#include <QChart>
#include <QChartView>
#include <QDateTime>
#include <QFrame>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QLineSeries>
#include <QPushButton>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QValueAxis>
#include <QVBoxLayout>
#include <QtCharts/QBarSeries>
#include <QAbstractItemView>
#include <QPainter>
#include <QStringList>

namespace
{
QTableWidgetItem *createTableItem(const QString &text)
{
    auto *item = new QTableWidgetItem(text);
    item->setFlags(item->flags() & ~Qt::ItemIsEditable);
    return item;
}

void styleAxis(QtCharts::QValueAxis *axis)
{
    axis->setLabelsColor(QColor(QStringLiteral("#73869a")));
    axis->setGridLineColor(QColor(QStringLiteral("#e5ebf1")));
}
}

ReportsPage::ReportsPage(QWidget *parent)
    : QWidget(parent)
{
    setObjectName(QStringLiteral("reportsPage"));
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    auto *contentLayout = new QVBoxLayout(this);
    contentLayout->setContentsMargins(30, 28, 30, 30);
    contentLayout->setSpacing(20);

    auto *headerLayout = new QHBoxLayout;
    auto *titleLayout = new QVBoxLayout;
    titleLayout->setSpacing(3);
    auto *title = new QLabel(QStringLiteral("Reports"), this);
    title->setObjectName(QStringLiteral("pageTitle"));
    titleLayout->addWidget(title);
    auto *subtitle = new QLabel(QStringLiteral("Monitor operational performance and export warehouse reports"), this);
    subtitle->setObjectName(QStringLiteral("pageSubtitle"));
    titleLayout->addWidget(subtitle);
    headerLayout->addLayout(titleLayout);
    headerLayout->addStretch();
    auto *dateLabel = new QLabel(QDateTime::currentDateTime().toString(QStringLiteral("dd MMM yyyy")), this);
    dateLabel->setObjectName(QStringLiteral("dateLabel"));
    headerLayout->addWidget(dateLabel, 0, Qt::AlignTop);
    contentLayout->addLayout(headerLayout);

    auto *reportGrid = new QGridLayout;
    reportGrid->setContentsMargins(0, 0, 0, 0);
    reportGrid->setHorizontalSpacing(16);
    reportGrid->setVerticalSpacing(16);
    reportGrid->addWidget(createReportCard(QStringLiteral("Inbound Report"), QStringLiteral("1,248 cartons received"), QStringLiteral("inbound")), 0, 0);
    reportGrid->addWidget(createReportCard(QStringLiteral("Decanting Report"), QStringLiteral("892 cartons processed"), QStringLiteral("decanting")), 0, 1);
    reportGrid->addWidget(createReportCard(QStringLiteral("Storage Report"), QStringLiteral("78% block utilization"), QStringLiteral("storage")), 0, 2);
    reportGrid->addWidget(createReportCard(QStringLiteral("Picking Report"), QStringLiteral("94.2% task accuracy"), QStringLiteral("picking")), 0, 3);
    contentLayout->addLayout(reportGrid);

    auto *chartGrid = new QGridLayout;
    chartGrid->setContentsMargins(0, 0, 0, 0);
    chartGrid->setHorizontalSpacing(16);
    auto *inboundChartCard = createChartCard(QStringLiteral("Inbound Trend"));
    auto *storageChartCard = createChartCard(QStringLiteral("Storage Utilization"));
    auto *pickingChartCard = createChartCard(QStringLiteral("Picking Performance"));
    addInboundTrendChart(inboundChartCard);
    addStorageUtilizationChart(storageChartCard);
    addPickingPerformanceChart(pickingChartCard);
    chartGrid->addWidget(inboundChartCard, 0, 0);
    chartGrid->addWidget(storageChartCard, 0, 1);
    chartGrid->addWidget(pickingChartCard, 0, 2);
    contentLayout->addLayout(chartGrid, 2);
    contentLayout->addWidget(createExportTableCard(), 2);
}

QFrame *ReportsPage::createReportCard(const QString &title, const QString &summary, const QString &theme)
{
    auto *card = new QFrame(this);
    card->setObjectName(QStringLiteral("reportCard"));
    card->setProperty("theme", theme);
    card->setMinimumHeight(92);
    auto *layout = new QHBoxLayout(card);
    layout->setContentsMargins(18, 14, 14, 14);
    auto *textLayout = new QVBoxLayout;
    textLayout->setSpacing(6);
    auto *titleLabel = new QLabel(title, card);
    titleLabel->setObjectName(QStringLiteral("reportTitle"));
    textLayout->addWidget(titleLabel);
    auto *summaryLabel = new QLabel(summary, card);
    summaryLabel->setObjectName(QStringLiteral("reportSummary"));
    textLayout->addWidget(summaryLabel);
    layout->addLayout(textLayout, 1);
    auto *arrow = new QLabel(QStringLiteral(">"), card);
    arrow->setObjectName(QStringLiteral("reportArrow"));
    layout->addWidget(arrow, 0, Qt::AlignVCenter);
    return card;
}

QFrame *ReportsPage::createChartCard(const QString &title)
{
    auto *card = new QFrame(this);
    card->setObjectName(QStringLiteral("contentCard"));
    auto *layout = new QVBoxLayout(card);
    layout->setContentsMargins(18, 14, 18, 10);
    layout->setSpacing(6);
    auto *titleLabel = new QLabel(title, card);
    titleLabel->setObjectName(QStringLiteral("sectionTitle"));
    layout->addWidget(titleLabel);
    return card;
}

void ReportsPage::addInboundTrendChart(QFrame *card)
{
    auto *layout = qobject_cast<QVBoxLayout *>(card->layout());
    auto *series = new QtCharts::QLineSeries(card);
    series->append(0, 72); series->append(1, 88); series->append(2, 80); series->append(3, 108); series->append(4, 100); series->append(5, 124);
    series->setColor(QColor(QStringLiteral("#2d86c5")));
    auto *chart = new QtCharts::QChart;
    chart->addSeries(series);
    chart->setBackgroundVisible(false);
    chart->legend()->setVisible(false);
    chart->setMargins(QMargins(0, 0, 0, 0));
    auto *axisX = new QtCharts::QValueAxis(chart); axisX->setRange(0, 5); axisX->setLabelsVisible(false); axisX->setGridLineVisible(false);
    auto *axisY = new QtCharts::QValueAxis(chart); axisY->setRange(0, 140); axisY->setTickCount(4); styleAxis(axisY);
    chart->addAxis(axisX, Qt::AlignBottom); chart->addAxis(axisY, Qt::AlignLeft); series->attachAxis(axisX); series->attachAxis(axisY);
    auto *view = new QtCharts::QChartView(chart, card); view->setRenderHint(QPainter::Antialiasing); view->setMinimumHeight(145);
    layout->addWidget(view, 1);
}

void ReportsPage::addStorageUtilizationChart(QFrame *card)
{
    auto *layout = qobject_cast<QVBoxLayout *>(card->layout());
    auto *series = new QtCharts::QLineSeries(card);
    series->append(0, 61); series->append(1, 65); series->append(2, 69); series->append(3, 73); series->append(4, 78); series->append(5, 78);
    series->setColor(QColor(QStringLiteral("#e0a43a")));
    auto *chart = new QtCharts::QChart; chart->addSeries(series); chart->setBackgroundVisible(false); chart->legend()->setVisible(false); chart->setMargins(QMargins(0, 0, 0, 0));
    auto *axisX = new QtCharts::QValueAxis(chart); axisX->setRange(0, 5); axisX->setLabelsVisible(false); axisX->setGridLineVisible(false);
    auto *axisY = new QtCharts::QValueAxis(chart); axisY->setRange(0, 100); axisY->setLabelFormat(QStringLiteral("%d%%")); axisY->setTickCount(3); styleAxis(axisY);
    chart->addAxis(axisX, Qt::AlignBottom); chart->addAxis(axisY, Qt::AlignLeft); series->attachAxis(axisX); series->attachAxis(axisY);
    auto *view = new QtCharts::QChartView(chart, card); view->setRenderHint(QPainter::Antialiasing); view->setMinimumHeight(145); layout->addWidget(view, 1);
}

void ReportsPage::addPickingPerformanceChart(QFrame *card)
{
    auto *layout = qobject_cast<QVBoxLayout *>(card->layout());
    auto *set = new QtCharts::QBarSet(QStringLiteral("Accuracy"), card); set->append({91, 96, 88, 94}); set->setColor(QColor(QStringLiteral("#7b78b9")));
    auto *series = new QtCharts::QBarSeries(card); series->append(set);
    auto *chart = new QtCharts::QChart; chart->addSeries(series); chart->setBackgroundVisible(false); chart->legend()->setVisible(false); chart->setMargins(QMargins(0, 0, 0, 0));
    auto *axisX = new QtCharts::QBarCategoryAxis(chart); axisX->append({QStringLiteral("Mon"), QStringLiteral("Tue"), QStringLiteral("Wed"), QStringLiteral("Thu")}); axisX->setLabelsColor(QColor(QStringLiteral("#73869a"))); axisX->setGridLineVisible(false);
    auto *axisY = new QtCharts::QValueAxis(chart); axisY->setRange(0, 100); axisY->setLabelFormat(QStringLiteral("%d%%")); axisY->setTickCount(3); styleAxis(axisY);
    chart->addAxis(axisX, Qt::AlignBottom); chart->addAxis(axisY, Qt::AlignLeft); series->attachAxis(axisX); series->attachAxis(axisY);
    auto *view = new QtCharts::QChartView(chart, card); view->setRenderHint(QPainter::Antialiasing); view->setMinimumHeight(145); layout->addWidget(view, 1);
}

QFrame *ReportsPage::createExportTableCard()
{
    auto *card = new QFrame(this); card->setObjectName(QStringLiteral("contentCard"));
    auto *layout = new QVBoxLayout(card); layout->setContentsMargins(18, 14, 18, 14); layout->setSpacing(8);
    auto *topRow = new QHBoxLayout;
    auto *title = new QLabel(QStringLiteral("Report Export Table"), card); title->setObjectName(QStringLiteral("cardTitle")); topRow->addWidget(title); topRow->addStretch();
    messageLabel = new QLabel(QStringLiteral("Ready"), card); messageLabel->setObjectName(QStringLiteral("messageLabel")); topRow->addWidget(messageLabel); layout->addLayout(topRow);
    reportTable = new QTableWidget(4, 3, card); reportTable->setObjectName(QStringLiteral("reportTable"));
    reportTable->setHorizontalHeaderLabels({QStringLiteral("Report Name"), QStringLiteral("Generated Date"), QStringLiteral("Status")});
    reportTable->verticalHeader()->setVisible(false); reportTable->horizontalHeader()->setStretchLastSection(true); reportTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch); reportTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents); reportTable->setSelectionBehavior(QAbstractItemView::SelectRows); reportTable->setSelectionMode(QAbstractItemView::SingleSelection); reportTable->setEditTriggers(QAbstractItemView::NoEditTriggers); reportTable->setAlternatingRowColors(true); reportTable->setShowGrid(false);
    const QStringList names{QStringLiteral("Inbound Daily Report"), QStringLiteral("Storage Utilization Report"), QStringLiteral("Picking Performance Report"), QStringLiteral("Decanting Summary Report")};
    const QStringList dates{QStringLiteral("27 Aug 2026"), QStringLiteral("27 Aug 2026"), QStringLiteral("26 Aug 2026"), QStringLiteral("26 Aug 2026")};
    const QStringList statuses{QStringLiteral("Ready"), QStringLiteral("Ready"), QStringLiteral("Ready"), QStringLiteral("Processing")};
    for (int row = 0; row < names.size(); ++row) { reportTable->setItem(row, 0, createTableItem(names.at(row))); reportTable->setItem(row, 1, createTableItem(dates.at(row))); reportTable->setItem(row, 2, createTableItem(statuses.at(row))); }
    layout->addWidget(reportTable, 1);
    auto *buttonLayout = new QHBoxLayout; buttonLayout->addStretch();
    auto *pdfButton = new QPushButton(QStringLiteral("Export PDF"), card); pdfButton->setObjectName(QStringLiteral("secondaryButton"));
    auto *csvButton = new QPushButton(QStringLiteral("Export CSV"), card); csvButton->setObjectName(QStringLiteral("secondaryButton"));
    auto *viewButton = new QPushButton(QStringLiteral("View Report"), card); viewButton->setObjectName(QStringLiteral("primaryButton"));
    buttonLayout->addWidget(pdfButton); buttonLayout->addWidget(csvButton); buttonLayout->addWidget(viewButton); layout->addLayout(buttonLayout);
    connect(pdfButton, &QPushButton::clicked, this, [this] { exportReport(QStringLiteral("PDF")); }); connect(csvButton, &QPushButton::clicked, this, [this] { exportReport(QStringLiteral("CSV")); }); connect(viewButton, &QPushButton::clicked, this, [this] { viewReport(); });
    reportTable->selectRow(0);
    return card;
}

void ReportsPage::exportReport(const QString &format)
{
    const int row = reportTable->currentRow();
    if (row >= 0) { messageLabel->setText(QStringLiteral("%1 exported: %2").arg(format, reportTable->item(row, 0)->text())); }
}

void ReportsPage::viewReport()
{
    const int row = reportTable->currentRow();
    if (row >= 0) { messageLabel->setText(QStringLiteral("Viewing: %1").arg(reportTable->item(row, 0)->text())); }
}
