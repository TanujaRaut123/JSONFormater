#pragma once

#include <QWidget>

class QLabel;
class QFrame;
class QLineEdit;
class QString;
class QTableWidget;

class InboundPage : public QWidget
{
public:
    explicit InboundPage(QWidget *parent = nullptr);

private:
    QWidget *createResultRow(const QString &label, QLabel *&valueLabel);
    QFrame *createStatisticsCard();
    QFrame *createHistoryCard();
    void scanBarcode();
    void clearScan();
    void validateCarton();
    void createCarton();
    void addHistoryEntry(const QString &barcode, const QString &status);
    void setResultStatus(const QString &status, const QString &theme);

    QLineEdit *barcodeInput = nullptr;
    QLabel *barcodeStatusValue = nullptr;
    QLabel *firstSeenValue = nullptr;
    QLabel *lengthValue = nullptr;
    QLabel *widthValue = nullptr;
    QLabel *heightValue = nullptr;
    QLabel *weightValue = nullptr;
    QLabel *cartonStatusValue = nullptr;
    QTableWidget *historyTable = nullptr;

    QLabel *totalScannedValue = nullptr;
    QLabel *successfulValue = nullptr;
    QLabel *failedValue = nullptr;
    QLabel *waitingValue = nullptr;

    int totalScanned = 2;
    int successful = 1;
    int failed = 0;
    int waiting = 1;
};
