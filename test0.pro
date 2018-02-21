#-------------------------------------------------
#
# Project created by QtCreator 2018-01-26T14:49:52
#
#-------------------------------------------------

QT       += core gui serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = test0
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    headform.cpp \
    settingdialog.cpp \
    headsetting.cpp \
    indexerwidget.cpp \
    indexersettingdialog.cpp \
    numpaddialog.cpp \
    serialport.cpp

HEADERS  += mainwindow.h \
    headform.h \
    settingdialog.h \
    headsetting.h \
    indexerwidget.h \
    crc16.h \
    indexersettingdialog.h \
    numpaddialog.h \
    serialport.h

FORMS    += mainwindow.ui \
    headform.ui \
    settingdialog.ui \
    indexerwidget.ui \
    indexersettingdialog.ui \
    numpaddialog.ui

RESOURCES += \
    icons.qrc
