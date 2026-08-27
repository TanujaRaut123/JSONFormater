#pragma once

#include <QWidget>

class QComboBox;
class QFrame;
class QLabel;
class QLineEdit;
class QTableWidget;
class QString;

class CartonsPage : public QWidget
{
public:
    explicit CartonsPage(QWidget *parent = nullptr);

private:
    QWidget *createDetailsRow(const QString &label, QLabel *&valueLabel);
    QFrame *createDetailsCard();
    void applyFilters();
    void updateDetails(int row);
    void clearDetails();
    void viewSelectedCarton();
    void editSelectedCarton();
    void deleteSelectedCarton();

    QLineEdit *searchInput = nullptr;
    QComboBox *statusFilter = nullptr;
    QTableWidget *cartonTable = nullptr;

    QLabel *detailSsccValue = nullptr;
    QLabel *detailLocationValue = nullptr;
    QLabel *detailStatusValue = nullptr;
    QLabel *detailCreatedValue = nullptr;
    QLabel *detailUpdatedValue = nullptr;
};
