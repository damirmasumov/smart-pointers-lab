QT += core gui charts
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += console c++17

TARGET = SmartPointers
TEMPLATE = app

SOURCES += main.cpp \
           MainWindow.cpp \
           tests.cpp

HEADERS += MainWindow.h \
           UnqPtr.h \
           ShrdPtr.h \
           DynamicArray.h \
           tests.h