#-------------------------------------------------
#
# Project created by QtCreator 2018-02-27T16:49:30
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = untitled1
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    polynomial.cpp

HEADERS  += mainwindow.h \
    polynomial.h

FORMS    += mainwindow.ui

QMAKE_CXXFLAGS += -std=c++11
