#pragma once

#include <QWidget>

class QComboBox;
class QFrame;
class QLabel;
class QLineEdit;
class QString;

class StoragePage : public QWidget
{
public:
    explicit StoragePage(QWidget *parent = nullptr);

private:
    QWidget *createOrderRow(const QString &label, QLabel *&valueLabel);
    QFrame *createOrderCard();
    QFrame *createExceptionsCard();
    QFrame *createKpiCard(const QString &title, const QString &value, const QString &theme, QLabel *&valueLabel);
    void findLocation();
    void storeCarrier();
    void retrieveCarrier();
    void setMessage(const QString &message, const QString &theme);

    QLineEdit *carrierInput = nullptr;
    QLineEdit *subIndexInput = nullptr;
    QLabel *assignedBlockValue = nullptr;
    QLabel *assignedBinValue = nullptr;
    QLabel *messageLabel = nullptr;

    QLabel *storedTodayValue = nullptr;
    QLabel *retrievedTodayValue = nullptr;
    QLabel *utilizationValue = nullptr;
    QLabel *activeOrdersValue = nullptr;

    int storedToday = 248;
    int retrievedToday = 186;
    int activeOrders = 12;
};
