QT += core
QT -= gui
CONFIG += console c++17
CONFIG -= app_bundle
TEMPLATE = app
TARGET = probe5utf8
SOURCES += encoding_probe.cpp
# The only difference from probe5.pro: tell MSVC the source is UTF-8,
# which Qt 6's qt_standard_project_setup() does automatically.
win32-msvc: QMAKE_CXXFLAGS += /utf-8
DESTDIR = utf8build
OBJECTS_DIR = utf8build/obj
MOC_DIR = utf8build/moc
