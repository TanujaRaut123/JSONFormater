#include "AppTheme.h"

namespace AppTheme
{
QString styleSheet()
{
    return QStringLiteral(R"(
        QMainWindow, QWidget#pageStack, QWidget#dashboardPage, QWidget#inboundPage,
        QWidget#cartonsPage, QWidget#storagePage, QWidget#pickingPage, QWidget#reportsPage,
        QWidget#decantingPage, QWidget#activityLogPage, QWidget#logoutPage {
            background-color: #f4f7fb;
        }
        QWidget#sidebar {
            background-color: #102a43;
            border-right: 1px solid #1f4668;
        }
        QLabel#brandTitle { color: #ffffff; font-size: 20px; font-weight: 700; }
        QLabel#brandSubtitle { color: #9fbad0; font-size: 11px; }
        QLabel#sectionLabel { color: #7698b5; font-size: 10px; font-weight: 700; padding: 12px 12px 6px 12px; }
        QPushButton#navigationButton, QPushButton#logoutButton {
            color: #c9d9e8; background-color: transparent; border: 0; border-radius: 6px;
            padding: 12px 14px; text-align: left; font-size: 13px; font-weight: 600;
        }
        QPushButton#navigationButton:hover { color: #ffffff; background-color: #1b4264; }
        QPushButton#navigationButton:checked { color: #ffffff; background-color: #2374a8; }
        QPushButton#logoutButton { color: #f1b4b4; }
        QPushButton#logoutButton:hover { color: #ffffff; background-color: #7d3545; }
        QPushButton#logoutButton:checked { color: #ffffff; background-color: #a84450; }
        QLabel#pageTitle { color: #19324d; font-size: 26px; font-weight: 700; }
        QLabel#pageSubtitle, QLabel#dashboardSubtitle, QLabel#dateLabel, QLabel#toolbarLabel { color: #73869a; font-size: 12px; }
        QLabel#dateLabel, QLabel#countLabel, QLabel#stationLabel, QLabel#systemLabel {
            border-radius: 6px; padding: 9px 13px; font-size: 11px; font-weight: 700;
        }
        QLabel#liveLabel { color: #2e9d78; background-color: #e6f5ef; border-radius: 6px; padding: 9px 13px; font-size: 11px; font-weight: 700; }
        QFrame#filterCard { background-color: #ffffff; border: 1px solid #e2e9f0; border-radius: 10px; }
        QLabel#filterLabel { color: #73869a; font-size: 11px; font-weight: 600; }
        QComboBox#activityFilter, QLineEdit#activitySearch { min-height: 18px; }
        QTableWidget#activityTable { alternate-background-color: #f8fafc; }
        QTableWidget#activityTable::item:hover { background-color: #eef5f9; }
        QLabel#activityDetailValue { color: #19324d; font-size: 12px; font-weight: 600; }
        QLabel#dateLabel { color: #2374a8; background-color: #ffffff; border: 1px solid #dfe7ef; }
        QLabel#countLabel, QLabel#systemLabel { color: #2374a8; background-color: #e5f1f8; }
        QLabel#stationLabel { color: #2e9d78; background-color: #e6f5ef; }
        QFrame#metricCard, QFrame#sectionCard, QFrame#contentCard, QFrame#contentSection, QFrame#kpiCard, QFrame#reportCard {
            background-color: #ffffff; border: 1px solid #e2e9f0; border-radius: 10px;
        }
        QFrame#metricCard[theme="inbound"], QFrame#kpiCard[theme="inbound"], QFrame#reportCard[theme="inbound"] { border-top: 4px solid #2d86c5; }
        QFrame#metricCard[theme="decanted"], QFrame#kpiCard[theme="decanted"], QFrame#reportCard[theme="decanting"] { border-top: 4px solid #2e9d78; }
        QFrame#metricCard[theme="pending"], QFrame#kpiCard[theme="pending"] { border-top: 4px solid #e0a43a; }
        QFrame#metricCard[theme="errors"], QFrame#kpiCard[theme="errors"] { border-top: 4px solid #d25b64; }
        QFrame#metricCard[theme="stored"] { border-top: 4px solid #2d86c5; }
        QFrame#metricCard[theme="retrieved"] { border-top: 4px solid #2e9d78; }
        QFrame#metricCard[theme="utilization"], QFrame#reportCard[theme="storage"] { border-top: 4px solid #e0a43a; }
        QFrame#metricCard[theme="orders"], QFrame#reportCard[theme="picking"] { border-top: 4px solid #7b78b9; }
        QFrame#reportCard[theme="decanting"] { border-top: 4px solid #2e9d78; }
        QLabel#metricTitle, QLabel#kpiTitle { color: #6d8194; font-size: 12px; font-weight: 600; }
        QLabel#metricValue, QLabel#kpiValue { color: #19324d; font-size: 28px; font-weight: 700; }
        QLabel#metricIcon, QLabel#kpiIcon { background-color: #2374a8; border-radius: 8px; }
        QLabel#cardTitle, QLabel#sectionCardTitle, QLabel#sectionTitle { color: #19324d; font-size: 15px; font-weight: 700; }
        QLabel#resultLabel, QLabel#detailLabel, QLabel#orderLabel, QLabel#sectionLabel { color: #73869a; font-size: 12px; }
        QLabel#resultValue, QLabel#detailValue, QLabel#orderValue, QLabel#infoValue { color: #19324d; padding: 8px 0; font-size: 12px; font-weight: 600; }
        QLabel#taskValue { color: #19324d; font-size: 22px; font-weight: 700; }
        QLabel#reportTitle { color: #19324d; font-size: 14px; font-weight: 700; }
        QLabel#reportSummary, QLabel#exceptionDescription { color: #73869a; font-size: 11px; }
        QLabel#reportArrow { color: #2374a8; font-size: 18px; font-weight: 700; }
        QLabel#exceptionTitle { color: #19324d; font-size: 12px; font-weight: 700; }
        QLabel#messageLabel { color: #73869a; background-color: #f8fafc; border: 1px solid #e5ebf1; border-radius: 6px; padding: 10px 12px; font-size: 11px; }
        QLabel#statusValue[theme="success"], QLabel#detailStatus[theme="completed"], QLabel#taskStatus[theme="completed"] { color: #2e9d78; }
        QLabel#statusValue[theme="warning"], QLabel#detailStatus[theme="pending"], QLabel#taskStatus[theme="progress"] { color: #c7881e; }
        QLabel#detailStatus[theme="decanting"], QLabel#taskStatus[theme="progress"] { color: #2374a8; }
        QPushButton#primaryButton, QPushButton#secondaryButton, QPushButton#dangerButton {
            border-radius: 6px; padding: 10px 14px; font-size: 12px; font-weight: 700;
        }
        QPushButton#primaryButton { color: #ffffff; background-color: #2374a8; border: 1px solid #2374a8; }
        QPushButton#primaryButton:hover { background-color: #1d638f; }
        QPushButton#secondaryButton { color: #426078; background-color: #ffffff; border: 1px solid #cbd9e5; }
        QPushButton#secondaryButton:hover { background-color: #eef5f9; }
        QPushButton#dangerButton { color: #c34e59; background-color: #ffffff; border: 1px solid #e4b9bd; }
        QPushButton#dangerButton:hover { background-color: #fff2f3; }
        QLineEdit, QComboBox {
            color: #19324d; background-color: #ffffff; border: 1px solid #ccd9e5; border-radius: 6px; padding: 10px 12px; font-size: 12px;
        }
        QLineEdit:focus, QComboBox:focus { border: 1px solid #2d86c5; }
        QTableWidget {
            color: #31495f; background-color: #ffffff; alternate-background-color: #f8fafc; border: 0;
            gridline-color: #edf1f5; selection-background-color: #e5f1f8; selection-color: #19324d;
        }
        QHeaderView::section {
            color: #72869a; background-color: #f8fafc; border: 0; border-bottom: 1px solid #e2e9f0;
            padding: 9px 10px; font-size: 11px; font-weight: 700;
        }
        QProgressBar { color: #ffffff; background-color: #e7edf2; border: 0; border-radius: 6px; text-align: center; min-height: 14px; max-height: 14px; }
        QProgressBar::chunk { background-color: #2374a8; border-radius: 6px; }
    )");
}
}
