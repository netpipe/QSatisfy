#-------------------------------------------------
#
# Project created by QtCreator 2019-01-27T07:49:36
#
#-------------------------------------------------

QT       += core gui
QT       += charts
QT += multimedia
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QSatisfy
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    settings.cpp \
    stats.cpp

HEADERS  += mainwindow.h \
    settings.h \
    stats.h

FORMS    += mainwindow.ui \
    settings.ui \
    stats.ui

DISTFILES +=
