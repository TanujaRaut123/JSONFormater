#pragma once

#include <QWidget>

class QFrame;
class QLabel;
class QProgressBar;
class QTableWidget;
class QString;

class PickingPage : public QWidget
{
public:
    explicit PickingPage(QWidget *parent = nullptr);

private:
    QWidget *createInfoRow(const QString &label, QLabel *&valueLabel);
    QFrame *createOrderCard();
    QFrame *createTaskCard();
    QFrame *createExceptionsCard();
    QFrame *createTaskTableCard();
    void updateTask(int row);
    void updateProgress();
    void confirmPick();
    void nextTask();
    void setTaskStatus(const QString &status, const QString &theme);

    QLabel *orderIdValue = nullptr;
    QLabel *packGroupValue = nullptr;
    QLabel *workstationValue = nullptr;
    QLabel *skuValue = nullptr;
    QLabel *expectedValue = nullptr;
    QLabel *pickedValue = nullptr;
    QLabel *taskStatusValue = nullptr;
    QProgressBar *progressBar = nullptr;
    QTableWidget *taskTable = nullptr;
};
