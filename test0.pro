#-------------------------------------------------
#
# Project created by QtCreator 2018-01-26T14:49:52
#
#-------------------------------------------------

QT       += core gui serialport network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = test0
TEMPLATE = app


#SMTP_LIBRARY_LOCATION = .
#LIBS += -L$$SMTP_LIBRARY_LOCATION -lSMTPEmail

SOURCES += main.cpp\
        mainwindow.cpp \
    headform.cpp \
    settingdialog.cpp \
    headsetting.cpp \
    indexerwidget.cpp \
    indexersettingdialog.cpp \
    numpaddialog.cpp \
    serialport.cpp \
    serialsettingsdialog.cpp \
    mailsender.cpp \
    mailSrc/emailaddress.cpp \
    mailSrc/mimeattachment.cpp \
    mailSrc/mimecontentformatter.cpp \
    mailSrc/mimefile.cpp \
    mailSrc/mimehtml.cpp \
    mailSrc/mimeinlinefile.cpp \
    mailSrc/mimemessage.cpp \
    mailSrc/mimemultipart.cpp \
    mailSrc/mimepart.cpp \
    mailSrc/mimetext.cpp \
    mailSrc/quotedprintable.cpp \
    mailSrc/smtpclient.cpp

HEADERS  += mainwindow.h \
    headform.h \
    settingdialog.h \
    headsetting.h \
    indexerwidget.h \
    crc16.h \
    indexersettingdialog.h \
    numpaddialog.h \
    serialport.h \
    serialsettingsdialog.h \
    mailsender.h \
    mailSrc/emailaddress.h \
    mailSrc/mimeattachment.h \
    mailSrc/mimecontentformatter.h \
    mailSrc/mimefile.h \
    mailSrc/mimehtml.h \
    mailSrc/mimeinlinefile.h \
    mailSrc/mimemessage.h \
    mailSrc/mimemultipart.h \
    mailSrc/mimepart.h \
    mailSrc/mimetext.h \
    mailSrc/quotedprintable.h \
    mailSrc/smtpclient.h \
    mailSrc/smtpexports.h \
    mailSrc/SmtpMime

FORMS    += mainwindow.ui \
    headform.ui \
    settingdialog.ui \
    indexerwidget.ui \
    indexersettingdialog.ui \
    numpaddialog.ui \
    serialsettingsdialog.ui

RESOURCES += \
    icons.qrc
