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


typedef union ModData_{
    struct{
        uint8_t crc16ValLo:8;
        uint8_t crc16ValHi:8;
        uint8_t dataLo:8;
        uint8_t dataHi:8;
        uint8_t registerNoLo:8;
//            uint8_t registerNoHi:8;
        uint8_t adressLo:7;
        uint8_t rwBit:1;
//            uint8_t adressHi:8;

    }bits;
    struct{
        uint16_t crc16Val:16;
        uint16_t data:16;
        uint8_t registerNo:8;
        uint8_t adress:7;
        uint8_t rwBit:1;
    }fileds;
    u_int64_t all:48;
}ModData;

class SerialPort : public QObject
{
    Q_OBJECT

public:
    explicit SerialPort(QObject *parent = 0);
    explicit SerialPort(ComSettings settings, QObject *parent = 0);

    void setRegisterPointer(Register* regPtr);
private:
    SerialSettingsDialog *settingsComDialog;
    QSerialPort *serial;

    QByteArray dataTransform(QByteArray data);

    Register *registers;

    ModData modData8;
    QByteArray data8;

public slots:
    void openSerialPort();
    void openSerialPort(ComSettings cSett);
    void closeSerialPort();

    void sendData(QByteArray data, bool halfByte = false);
    void sendData(uint8_t dev, uint8_t place, uint16_t data);
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
