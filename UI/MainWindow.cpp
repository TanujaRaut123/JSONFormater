#include "MainWindow.h"

#include "AppTheme.h"

#include <QButtonGroup>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QStackedWidget>
#include <QStringList>
#include <QVBoxLayout>
#include <QWidget>

namespace
{
constexpr int SidebarWidth = 248;

const QStringList navigationLabels{
    QStringLiteral("Dashboard"),
    QStringLiteral("Inbound"),
    QStringLiteral("Cartons"),
    QStringLiteral("Decanting"),
    QStringLiteral("Storage"),
    QStringLiteral("GTP Picking"),
    QStringLiteral("Reports"),
    QStringLiteral("Activity Logs"),
    QStringLiteral("Logout")};
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle(QStringLiteral("Warehouse Operations"));
    setMinimumSize(1100, 700);
    resize(1280, 800);

    auto *workspace = new QWidget(this);
    auto *workspaceLayout = new QHBoxLayout(workspace);
    workspaceLayout->setContentsMargins(0, 0, 0, 0);
    workspaceLayout->setSpacing(0);

    workspaceLayout->addWidget(createSidebar());

    pageStack = new QStackedWidget(workspace);
    pageStack->setObjectName(QStringLiteral("pageStack"));
    workspaceLayout->addWidget(pageStack, 1);

    setCentralWidget(workspace);

    setStyleSheet(AppTheme::styleSheet());

    createPages();
    connectNavigation();
}

QWidget *MainWindow::createSidebar()
{
    auto *sidebar = new QWidget(this);
    sidebar->setObjectName(QStringLiteral("sidebar"));
    sidebar->setFixedWidth(SidebarWidth);

    auto *sidebarLayout = new QVBoxLayout(sidebar);
    sidebarLayout->setContentsMargins(20, 26, 20, 20);
    sidebarLayout->setSpacing(6);

    auto *brandTitle = new QLabel(QStringLiteral("WAREHOUSE"), sidebar);
    brandTitle->setObjectName(QStringLiteral("brandTitle"));
    sidebarLayout->addWidget(brandTitle);

    auto *brandSubtitle = new QLabel(QStringLiteral("OPERATIONS CONTROL"), sidebar);
    brandSubtitle->setObjectName(QStringLiteral("brandSubtitle"));
    sidebarLayout->addWidget(brandSubtitle);

    auto *sectionLabel = new QLabel(QStringLiteral("WORKSPACE"), sidebar);
    sectionLabel->setObjectName(QStringLiteral("sectionLabel"));
    sidebarLayout->addWidget(sectionLabel);

    navigationGroup = new QButtonGroup(sidebar);
    navigationGroup->setExclusive(true);

    for (int index = 0; index < navigationLabels.size(); ++index) {
        auto *button = new QPushButton(navigationLabels.at(index), sidebar);
        button->setObjectName(index == navigationLabels.size() - 1
                                  ? QStringLiteral("logoutButton")
                                  : QStringLiteral("navigationButton"));
        button->setCheckable(true);
        button->setCursor(Qt::PointingHandCursor);
        navigationGroup->addButton(button, index);
        sidebarLayout->addWidget(button);

        if (index == 0) {
            button->setChecked(true);
        }
    }

    sidebarLayout->addStretch();

    auto *footer = new QLabel(QStringLiteral("SYSTEM READY\nFacility 01  |  Shift A"), sidebar);
    footer->setObjectName(QStringLiteral("brandSubtitle"));
    sidebarLayout->addWidget(footer);

    return sidebar;
}

void MainWindow::createPages()
{
    pageStack->addWidget(new DashboardPage(pageStack));
    pageStack->addWidget(new InboundPage(pageStack));
    pageStack->addWidget(new CartonsPage(pageStack));
    pageStack->addWidget(new DecantingPage(pageStack));
    pageStack->addWidget(new StoragePage(pageStack));
    pageStack->addWidget(new PickingPage(pageStack));
    pageStack->addWidget(new ReportsPage(pageStack));
    pageStack->addWidget(new ActivityLogPage(pageStack));
    pageStack->addWidget(new LogoutPage(pageStack));
}

void MainWindow::connectNavigation()
{
    connect(navigationGroup, &QButtonGroup::idClicked, this, [this](int pageIndex) {
        pageStack->setCurrentIndex(pageIndex);
    });
}
