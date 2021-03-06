#ifndef SERIALPORT_H
#define SERIALPORT_H

#include <QObject>
#include <QByteArray>
#include <QDebug>
#include <QtSerialPort/QSerialPort>
#include <QMessageBox>
#include <QThread>
#include <QProgressDialog>

#include "serialsettingsdialog.h"
#include "settings.h"
#include "crc16.h"
#include "reprogramdialog.h"


typedef union ModData_{
    struct{
        uint16_t crc16Val:16;
        uint16_t data:16;
        uint8_t registerNo:8;
        uint8_t adress:8;
    }bits;
    struct{
        uint16_t crc16Val:16;
        uint16_t data:16;
        uint8_t registerNo:7;
        uint8_t rwBit:1;
        uint8_t adress:7;
        uint8_t rwBit_:1;
    }fileds;
    uint64_t all:48;
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
    QByteArray dataToSendBuff;
    int replyCnt;
    bool initApp;



public slots:
    void openSerialPort();
    void openSerialPort(ComSettings cSett);
    void closeSerialPort();

    void sendProgram(ReprogramDialog::BoardType type, QByteArray programArr);

    void sendData(QByteArray data, bool send = false, bool halfByte = false);
    void sendModData(uint8_t dev, uint8_t place, uint16_t data);
    void sendReg(uint8_t dev, uint8_t place);
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
    void working();
    void initFinihed();
    void proramProgres(int progres);
};

#endif // SERIALPORT_H
