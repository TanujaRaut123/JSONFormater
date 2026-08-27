cmake_minimum_required(VERSION 3.21)

project(WarehouseOperations LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets Charts)

add_executable(WarehouseOperations
    main.cpp
    UI/MainWindow.h
    UI/MainWindow.cpp
    UI/AppTheme.h
    UI/AppTheme.cpp
    UI/Components/MetricCard.h
    UI/Components/MetricCard.cpp
    UI/Components/SectionCard.h
    UI/Components/SectionCard.cpp
    UI/Pages/ActivityLogPage.h
    UI/Pages/ActivityLogPage.cpp
    UI/Pages/CartonsPage.h
    UI/Pages/CartonsPage.cpp
    UI/Pages/DashboardPage.h
    UI/Pages/DashboardPage.cpp
    UI/Pages/DecantingPage.h
    UI/Pages/DecantingPage.cpp
    UI/Pages/InboundPage.h
    UI/Pages/InboundPage.cpp
    UI/Pages/LogoutPage.h
    UI/Pages/LogoutPage.cpp
    UI/Pages/PickingPage.h
    UI/Pages/PickingPage.cpp
    UI/Pages/ReportsPage.h
    UI/Pages/ReportsPage.cpp
    UI/Pages/StoragePage.h
    UI/Pages/StoragePage.cpp
)

target_link_libraries(WarehouseOperations PRIVATE Qt6::Widgets Qt6::Charts)

target_include_directories(WarehouseOperations PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/UI
    ${CMAKE_CURRENT_SOURCE_DIR}/UI/Components
    ${CMAKE_CURRENT_SOURCE_DIR}/UI/Pages
)
