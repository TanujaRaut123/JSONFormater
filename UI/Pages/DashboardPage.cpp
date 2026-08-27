#include "DashboardPage.h"

#include "../Components/MetricCard.h"

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
#include <QPainter>
#include <QPushButton>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QValueAxis>
#include <QVBoxLayout>
#include <QtCharts/QBarSeries>
#include <QStyle>

namespace
{
const QStringList chartCategories{
    QStringLiteral("Inbound"),
    QStringLiteral("Decanted"),
    QStringLiteral("Pending"),
    QStringLiteral("Errors")};

QTableWidgetItem *createTableItem(const QString &text)
{
    auto *item = new QTableWidgetItem(text);
    item->setFlags(item->flags() & ~Qt::ItemIsEditable);
    return item;
}
}

DashboardPage::DashboardPage(QWidget *parent)
    : QWidget(parent)
{
    setObjectName(QStringLiteral("dashboardPage"));
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    auto *contentLayout = new QVBoxLayout(this);
    contentLayout->setContentsMargins(30, 28, 30, 30);
    contentLayout->setSpacing(22);

    auto *headerLayout = new QHBoxLayout;
    auto *titleLayout = new QVBoxLayout;
    titleLayout->setSpacing(3);

    auto *title = new QLabel(QStringLiteral("Dashboard"), this);
    title->setObjectName(QStringLiteral("dashboardTitle"));
    titleLayout->addWidget(title);

    auto *subtitle = new QLabel(QStringLiteral("Live overview of warehouse operations"), this);
    subtitle->setObjectName(QStringLiteral("dashboardSubtitle"));
    titleLayout->addWidget(subtitle);
    headerLayout->addLayout(titleLayout);
    headerLayout->addStretch();

    auto *dateLabel = new QLabel(QDateTime::currentDateTime().toString(QStringLiteral("dddd, dd MMMM yyyy")), this);
    dateLabel->setObjectName(QStringLiteral("dateLabel"));
    headerLayout->addWidget(dateLabel, 0, Qt::AlignTop);
    contentLayout->addLayout(headerLayout);

    auto *kpiLayout = new QGridLayout;
    kpiLayout->setContentsMargins(0, 0, 0, 0);
    kpiLayout->setHorizontalSpacing(16);
    kpiLayout->setVerticalSpacing(16);

    kpiLayout->addWidget(createKpiCard(QStringLiteral("Inbound Today"), QStringLiteral("inbound"),
                                       QStyle::SP_ArrowDown, inboundValueLabel), 0, 0);
    kpiLayout->addWidget(createKpiCard(QStringLiteral("Decanted"), QStringLiteral("decanted"),
                                       QStyle::SP_DialogApplyButton, decantedValueLabel), 0, 1);
    kpiLayout->addWidget(createKpiCard(QStringLiteral("Pending"), QStringLiteral("pending"),
                                       QStyle::SP_BrowserReload, pendingValueLabel), 0, 2);
    kpiLayout->addWidget(createKpiCard(QStringLiteral("Errors"), QStringLiteral("errors"),
                                       QStyle::SP_MessageBoxCritical, errorsValueLabel), 0, 3);
    contentLayout->addLayout(kpiLayout);

    contentLayout->addWidget(createChartSection(), 1);
    contentLayout->addWidget(createActivitySection(), 1);

    setKpiValues(inboundToday, decanted, pending, errors);
}

QFrame *DashboardPage::createKpiCard(const QString &title,
                                     const QString &theme,
                                     QStyle::StandardPixmap icon,
                                     QLabel *&valueLabel)
{
    auto *card = new MetricCard(title, QString(), theme, this, icon);
    valueLabel = card->valueLabel();
    return card;
}

QFrame *DashboardPage::createChartSection()
{
    auto *section = new QFrame(this);
    section->setObjectName(QStringLiteral("contentSection"));

    auto *sectionLayout = new QVBoxLayout(section);
    sectionLayout->setContentsMargins(20, 16, 20, 12);
    sectionLayout->setSpacing(8);

    auto *title = new QLabel(QStringLiteral("Warehouse Activity"), section);
    title->setObjectName(QStringLiteral("sectionTitle"));
    sectionLayout->addWidget(title);

    auto *series = new QtCharts::QBarSeries(section);
    inboundBar = new QBarSet(QStringLiteral("Inbound"), series);
    decantedBar = new QBarSet(QStringLiteral("Decanted"), series);
    pendingBar = new QBarSet(QStringLiteral("Pending"), series);
    errorsBar = new QBarSet(QStringLiteral("Errors"), series);
    inboundBar->setColor(QColor(QStringLiteral("#2d86c5")));
    decantedBar->setColor(QColor(QStringLiteral("#2e9d78")));
    pendingBar->setColor(QColor(QStringLiteral("#e0a43a")));
    errorsBar->setColor(QColor(QStringLiteral("#d25b64")));
    series->append(inboundBar);
    series->append(decantedBar);
    series->append(pendingBar);
    series->append(errorsBar);

    chartView = new QtCharts::QChartView(new QtCharts::QChart, section);
    chartView->setObjectName(QStringLiteral("activityChart"));
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setMinimumHeight(190);
    chartView->chart()->addSeries(series);
    chartView->chart()->setAnimationOptions(QtCharts::QChart::SeriesAnimations);
    chartView->chart()->legend()->setVisible(false);
    chartView->chart()->setBackgroundVisible(false);
    chartView->chart()->setMargins(QMargins(0, 0, 0, 0));

    auto *categoryAxis = new QtCharts::QBarCategoryAxis(chartView->chart());
    categoryAxis->append(chartCategories);
    categoryAxis->setLabelsColor(QColor(QStringLiteral("#73869a")));
    categoryAxis->setGridLineVisible(false);
    chartView->chart()->addAxis(categoryAxis, Qt::AlignBottom);
    series->attachAxis(categoryAxis);

    auto *valueAxis = new QtCharts::QValueAxis(chartView->chart());
    valueAxis->setRange(0, 120);
    valueAxis->setTickCount(7);
    valueAxis->setLabelsColor(QColor(QStringLiteral("#73869a")));
    valueAxis->setGridLineColor(QColor(QStringLiteral("#e5ebf1")));
    chartView->chart()->addAxis(valueAxis, Qt::AlignLeft);
    series->attachAxis(valueAxis);

    sectionLayout->addWidget(chartView, 1);
    return section;
}

QFrame *DashboardPage::createActivitySection()
{
    auto *section = new QFrame(this);
    section->setObjectName(QStringLiteral("contentSection"));

    auto *sectionLayout = new QVBoxLayout(section);
    sectionLayout->setContentsMargins(20, 16, 20, 14);
    sectionLayout->setSpacing(8);

    auto *title = new QLabel(QStringLiteral("Recent Activity"), section);
    title->setObjectName(QStringLiteral("sectionTitle"));
    sectionLayout->addWidget(title);

    auto *table = new QTableWidget(4, 3, section);
    table->setObjectName(QStringLiteral("recentActivityTable"));
    table->setHorizontalHeaderLabels({QStringLiteral("Timestamp"), QStringLiteral("Event"), QStringLiteral("Status")});
    table->setVerticalHeaderLabels({});
    table->verticalHeader()->setVisible(false);
    table->horizontalHeader()->setStretchLastSection(true);
    table->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    table->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    table->setSelectionMode(QAbstractItemView::NoSelection);
    table->setFocusPolicy(Qt::NoFocus);
    table->setShowGrid(false);

    const QStringList timestamps{
        QStringLiteral("08:42:16"), QStringLiteral("08:37:04"), QStringLiteral("08:25:51"), QStringLiteral("08:18:29")};
    const QStringList events{
        QStringLiteral("Inbound shipment received"), QStringLiteral("Decanting wave completed"),
        QStringLiteral("Carton assigned to storage"), QStringLiteral("Scanner connection restored")};
    const QStringList statuses{
        QStringLiteral("Completed"), QStringLiteral("Completed"), QStringLiteral("In progress"), QStringLiteral("Resolved")};

    for (int row = 0; row < timestamps.size(); ++row) {
        table->setItem(row, 0, createTableItem(timestamps.at(row)));
        table->setItem(row, 1, createTableItem(events.at(row)));
        table->setItem(row, 2, createTableItem(statuses.at(row)));
    }

    table->setMinimumHeight(150);
    sectionLayout->addWidget(table, 1);
    return section;
}

void DashboardPage::setKpiValues(int inboundTodayValue, int decantedValue, int pendingValue, int errorsValue)
{
    inboundToday = inboundTodayValue;
    decanted = decantedValue;
    pending = pendingValue;
    errors = errorsValue;

    if (inboundValueLabel == nullptr) {
        return;
    }

    inboundValueLabel->setText(QString::number(inboundToday));
    decantedValueLabel->setText(QString::number(decanted));
    pendingValueLabel->setText(QString::number(pending));
    errorsValueLabel->setText(QString::number(errors));
    updateChart();
}

void DashboardPage::updateChart()
{
    if (inboundBar == nullptr) {
        return;
    }

    const QList<QPair<QtCharts::QBarSet *, int>> values{
        {inboundBar, inboundToday}, {decantedBar, decanted},
        {pendingBar, pending}, {errorsBar, errors}};

    for (int categoryIndex = 0; categoryIndex < values.size(); ++categoryIndex) {
        auto *bar = values.at(categoryIndex).first;
        const auto value = values.at(categoryIndex).second;
        if (bar->count() == 0) {
            bar->append(0);
            bar->append(0);
            bar->append(0);
            bar->append(0);
        }
        bar->replace(categoryIndex, value);
    }
}
