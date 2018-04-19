#ifndef SERIALPORT_H
#define SERIALPORT_H

#include <QObject>
#include <QByteArray>
#include <QDebug>
#include <QtSerialPort/QSerialPort>
#include <QMessageBox>

#include "serialsettingsdialog.h"
#include "settings.h"
#include "crc16.h"


class SerialPort : public QObject
{
    Q_OBJECT

public:
    explicit SerialPort(QObject *parent = 0);
    explicit SerialPort(ComSettings settings, QObject *parent = 0);

private:
    SerialSettingsDialog *settingsComDialog;
    QSerialPort *serial;

    QByteArray dataTransform(QByteArray data);

    typedef union ModData_{
        struct{
            uint8_t crc16ValLo:8;
            uint8_t crc16ValHi:8;
            uint8_t dataLo:8;
            uint8_t dataHi:8;
            uint8_t registerNoLo:8;
            uint8_t registerNoHi:8;
            uint8_t adressLo:8;
            uint8_t adressHi:8;

        }bits;
        struct{
            uint16_t crc16Val:16;
            uint16_t data:16;
            uint16_t registerNo:16;
            uint16_t adress:16;
        }fileds;
        u_int64_t all;
    }ModData;

    ModData modData8;
    QByteArray data8;



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
    void dataReady(ModData modData);
    void serialSettingAccepted(ComSettings seittngs);
};

#endif // SERIALPORT_H
