#pragma once

#include <QMainWindow>

#include "Pages/ActivityLogPage.h"
#include "Pages/CartonsPage.h"
#include "Pages/DashboardPage.h"
#include "Pages/DecantingPage.h"
#include "Pages/InboundPage.h"
#include "Pages/LogoutPage.h"
#include "Pages/PickingPage.h"
#include "Pages/ReportsPage.h"
#include "Pages/StoragePage.h"

class QButtonGroup;
class QStackedWidget;
class QVBoxLayout;
class QWidget;

class MainWindow : public QMainWindow
{
public:
    explicit MainWindow(QWidget *parent = nullptr);

private:
    QWidget *createSidebar();
    void createPages();
    void connectNavigation();

    QButtonGroup *navigationGroup = nullptr;
    QStackedWidget *pageStack = nullptr;
};
