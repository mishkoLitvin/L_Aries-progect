#-------------------------------------------------
#
# Project created by QtCreator 2018-01-26T14:49:52
#
#-------------------------------------------------

QT       += core network gui serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = test0
TEMPLATE = app

#OPENSSL_LIBS='-L/usr/bin -lssl -lcrypto'

#PKGCONFIG += openssl
#win32:LIBS+=C:\Users\T\Dropbox\ANATOL_SharedFolder\Windows_version\SMTPEmail.dll

SOURCES += main.cpp\
        mainwindow.cpp \
    headform.cpp \
    headsettingdialog.cpp \
    settings.cpp \
    indexerwidget.cpp \
    indexersettingdialog.cpp \
    numpaddialog.cpp \
    serialport.cpp \
    serialsettingsdialog.cpp \
    mailsender.cpp\
    generalsettingdialog.cpp

unix:SOURCES += mailSrc/emailaddress.cpp \
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
    mailSrc/smtpclient.cpp \

HEADERS  += mainwindow.h \
    headform.h \
    headsettingdialog.h \
    settings.h \
    indexerwidget.h \
    crc16.h \
    indexersettingdialog.h \
    numpaddialog.h \
    serialport.h \
    serialsettingsdialog.h \
    mailsender.h \
    generalsettingdialog.h

unix:HEADERS  += mailSrc/emailaddress.h \
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
    mailSrc/SmtpMime \

FORMS    += mainwindow.ui \
    headform.ui \
    headsettingdialog.ui \
    indexerwidget.ui \
    indexersettingdialog.ui \
    numpaddialog.ui \
    serialsettingsdialog.ui \
    generalsettingdialog.ui

RESOURCES += \
    icons.qrc

SMTP_LIBRARY_LOCATION = C:\Users\T\Dropbox\SharedProgects\NewProjects\buildsWin\build-SMTPEmailRelease
win32:CONFIG(release, debug|release): LIBS += -L$$SMTP_LIBRARY_LOCATION/release/ -lSMTPEmail
INCLUDEPATH += $$SMTP_LIBRARY_LOCATION
DEPENDPATH += $$SMTP_LIBRARY_LOCATION
