QT -= gui
QT += core sql

CONFIG += c++14 console
CONFIG -= app_bundle

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
        main.cpp \
    employee.cpp \
    database_manager.cpp \
    utils.cpp

HEADERS += \
    employee.h \
    database_manager.h \
    utils.h
