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
    explicit SerialPort(ComSettings settings, QObject *parent = 0);

private:
    SerialSettingsDialog *settingsComDialog;
    QSerialPort *serial;



public slots:
    void openSerialPort();
    void closeSerialPort();

    void sendData(QByteArray data);
    void setupPort();
    void setComParams(ComSettings sett);
    void setStyleSheet(QString stSheet);


private slots:
    void readData();
    void handleError(QSerialPort::SerialPortError error);
    void showStatusMessage(const QString &message);
    void getSerialSetting(ComSettings setting);




signals:
    void dataReady(QByteArray data);
    void serialSettingAccepted(ComSettings seittngs);
};

#endif // SERIALPORT_H
