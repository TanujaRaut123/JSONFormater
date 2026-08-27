#pragma once

#include <QWidget>

class QComboBox;
class QFrame;
class QLabel;
class QLineEdit;
class QTableWidget;
class QString;

class ActivityLogPage : public QWidget
{
public:
    explicit ActivityLogPage(QWidget *parent = nullptr);

private:
    QWidget *createDetailRow(const QString &label, QLabel *&valueLabel);
    QFrame *createFilterCard();
    QFrame *createDetailsCard();
    QFrame *createEventsCard();
    void applyFilters();
    void updateDetails(int row);
    void refreshEvents();
    void setStatusBadge(QTableWidgetItem *item, const QString &status);

    QComboBox *dateRangeFilter = nullptr;
    QComboBox *eventTypeFilter = nullptr;
    QComboBox *entityTypeFilter = nullptr;
    QLineEdit *searchInput = nullptr;
    QTableWidget *eventTable = nullptr;

    QLabel *eventsTodayValue = nullptr;
    QLabel *warningsValue = nullptr;
    QLabel *errorsValue = nullptr;
    QLabel *completedValue = nullptr;

    QLabel *eventIdValue = nullptr;
    QLabel *timestampValue = nullptr;
    QLabel *entityValue = nullptr;
    QLabel *locationValue = nullptr;
    QLabel *resultValue = nullptr;
    QLabel *descriptionValue = nullptr;
};
