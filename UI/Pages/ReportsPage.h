#pragma once

#include <QWidget>

class QFrame;
class QLabel;
class QTableWidget;
class QString;

class ReportsPage : public QWidget
{
public:
    explicit ReportsPage(QWidget *parent = nullptr);

private:
    QFrame *createReportCard(const QString &title, const QString &summary, const QString &theme);
    QFrame *createChartCard(const QString &title);
    QFrame *createExportTableCard();
    void addInboundTrendChart(QFrame *card);
    void addStorageUtilizationChart(QFrame *card);
    void addPickingPerformanceChart(QFrame *card);
    void exportReport(const QString &format);
    void viewReport();

    QTableWidget *reportTable = nullptr;
    QLabel *messageLabel = nullptr;
};
