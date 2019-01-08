#-------------------------------------------------
#
# Project created by QtCreator 2018-12-17T22:23:27
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
CONFIG +=c++11
CONFIG += qt
TARGET = LAB3_2
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    Identifier.cpp \
    hashcounter.cpp


HEADERS  += mainwindow.h \
    Identifier.h \
    hashcounter.h

FORMS    += mainwindow.ui

