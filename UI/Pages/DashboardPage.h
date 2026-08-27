#pragma once

#include <QFrame>
#include <QStyle>
#include <QString>
#include <QWidget>

class QLabel;
class QTableWidget;

namespace QtCharts
{
class QBarSet;
class QChartView;
}

class DashboardPage : public QWidget
{
public:
    explicit DashboardPage(QWidget *parent = nullptr);

    void setKpiValues(int inboundToday, int decanted, int pending, int errors);

private:
    QFrame *createKpiCard(const QString &title,
                          const QString &theme,
                          QStyle::StandardPixmap icon,
                          QLabel *&valueLabel);
    QFrame *createChartSection();
    QFrame *createActivitySection();
    void updateChart();

    QLabel *inboundValueLabel = nullptr;
    QLabel *decantedValueLabel = nullptr;
    QLabel *pendingValueLabel = nullptr;
    QLabel *errorsValueLabel = nullptr;

    QtCharts::QBarSet *inboundBar = nullptr;
    QtCharts::QBarSet *decantedBar = nullptr;
    QtCharts::QBarSet *pendingBar = nullptr;
    QtCharts::QBarSet *errorsBar = nullptr;
    QtCharts::QChartView *chartView = nullptr;

    int inboundToday = 100;
    int decanted = 70;
    int pending = 20;
    int errors = 10;
};
