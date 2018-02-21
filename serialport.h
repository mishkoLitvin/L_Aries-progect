#ifndef SERIALPORT_H
#define SERIALPORT_H

#include <QObject>
#include <QByteArray>
#include <QDebug>
#include <QtSerialPort/QSerialPort>
#include <QMessageBox>

#include "serialsettingsdialog.h"


class SerialPort : public QObject
{
    Q_OBJECT

public:
    explicit SerialPort(QObject *parent = 0);

private:
    SettingsDialog *settingsComDialog;
    QSerialPort *serial;



public slots:
    void openSerialPort();
    void closeSerialPort();

    void sendData(QByteArray data);

private slots:
    void readData();
    void handleError(QSerialPort::SerialPortError error);
    void showStatusMessage(const QString &message);



signals:
    void dataReady(QByteArray data);
};

#endif // SERIALPORT_H
