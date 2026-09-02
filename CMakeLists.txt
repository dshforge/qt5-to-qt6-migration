cmake_minimum_required(VERSION 3.21)

# Phase 3: replaces QSimpleScada.pro and com_indeema_QSimpleScada.pri.
#
# The .pro file carried version, target name and install rules; the .pri
# carried the file lists, include path and resources. Both are folded in
# here, because splitting a project this size across two CMake files
# reproduces the qmake layout without gaining anything from it.

project(QSimpleScada VERSION 0.9.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)          # Qt 6 requires C++17
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)               # replaces qmake FORMS
set(CMAKE_AUTORCC ON)               # replaces qmake RESOURCES

find_package(Qt6 6.5 REQUIRED COMPONENTS
    Core Gui Widgets Network Qml Quick QuickWidgets)

qt_standard_project_setup()

# Mirrors SOURCES / HEADERS / FORMS / RESOURCES from the .pri, in the
# same order, so the two can be diffed during review.
qt_add_library(QSimpleScada SHARED
    QScadaObject/qscadaobject.cpp
    QScadaObject/qscadaobject.h
    QScadaObject/qscadaobjectinfo.cpp
    QScadaObject/qscadaobjectinfo.h
    QScadaObject/qscadaobjectinfodialog.cpp
    QScadaObject/qscadaobjectinfodialog.h
    QScadaObject/qscadaobjectqml.cpp
    QScadaObject/qscadaobjectqml.h
    QScadaBoard/qscadaboard.cpp
    QScadaBoard/qscadaboard.h
    QScadaBoard/qscadaboardcontroller.cpp
    QScadaBoard/qscadaboardcontroller.h
    QScadaBoard/qscadaboardmanager.cpp
    QScadaBoard/qscadaboardmanager.h
    QScadaBoard/qscadaboardinfo.h
    QScadaEntity/qscadaconnecteddeviceinfo.cpp
    QScadaEntity/qscadaconnecteddeviceinfo.h
    QScadaEntity/qscadabaseprefentity.cpp
    QScadaEntity/qscadabaseprefentity.h
    QScadaDevice/qscadadeviceinfo.cpp
    QScadaDevice/qscadadeviceinfo.h
    qscadaconfig.h

    QScadaObject/qscadaobjectinfodialog.ui
    QScadaObject/qaxiswidget.ui

    com_indeema_QSimpleScada.qrc
)

# qmake's INCLUDEPATH += $$PWD, made PUBLIC so consumers of the library
# get the same headers without repeating the path.
target_include_directories(QSimpleScada PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/QScadaObject
    ${CMAKE_CURRENT_SOURCE_DIR}/QScadaBoard
    ${CMAKE_CURRENT_SOURCE_DIR}/QScadaEntity
    ${CMAKE_CURRENT_SOURCE_DIR}/QScadaDevice
)

target_link_libraries(QSimpleScada PUBLIC
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::Network
    Qt6::Qml
    Qt6::Quick
    Qt6::QuickWidgets
)

set_target_properties(QSimpleScada PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
    WINDOWS_EXPORT_ALL_SYMBOLS ON   # the .pro relied on qmake's DLL handling
)

include(GNUInstallDirs)
install(TARGETS QSimpleScada
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)
