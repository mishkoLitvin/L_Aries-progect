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
    explicit SerialPort(SerialSettingsDialog::Settings settings, QObject *parent = 0);

private:
    SerialSettingsDialog *settingsComDialog;
    QSerialPort *serial;



public slots:
    void openSerialPort();
    void closeSerialPort();

    void sendData(QByteArray data);
    void setupPort();


private slots:
    void readData();
    void handleError(QSerialPort::SerialPortError error);
    void showStatusMessage(const QString &message);
    void getSerialSetting(SerialSettingsDialog::Settings setting);



signals:
    void dataReady(QByteArray data);
    void serialSettingAccepted(SerialSettingsDialog::Settings seittngs);
};

#endif // SERIALPORT_H
