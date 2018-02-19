#-------------------------------------------------
#
# Project created by QtCreator 2018-01-26T14:49:52
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = test0
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    headform.cpp \
    settingdialog.cpp \
    headsetting.cpp \
    indexerwidget.cpp \
    passworddialog.cpp
    indexersettingdialog.cpp

HEADERS  += mainwindow.h \
    headform.h \
    settingdialog.h \
    headsetting.h \
    indexerwidget.h \
    crc16.h \
    passworddialog.h
    indexersettingdialog.h

FORMS    += mainwindow.ui \
    headform.ui \
    settingdialog.ui \
    indexerwidget.ui \
    passworddialog.ui
    indexersettingdialog.ui

RESOURCES += \
    icons.qrc
