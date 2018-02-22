#include "serialport.h"



SerialPort::SerialPort(QObject *parent):QObject(parent)
{
    serial = new QSerialPort(this);
    settingsComDialog = new SettingsDialog;

    connect(serial, SIGNAL(error(QSerialPort::SerialPortError)), this, SLOT(handleError(QSerialPort::SerialPortError)));
    connect(serial, SIGNAL(readyRead()), this, SLOT(readData()));

    this->openSerialPort();
}

void SerialPort::openSerialPort()
{
    SettingsDialog::Settings p = settingsComDialog->settings();
    serial->setPortName(p.name);
    serial->setBaudRate(p.baudRate);
    serial->setDataBits(p.dataBits);
    serial->setParity(p.parity);
    serial->setStopBits(p.stopBits);
    serial->setFlowControl(p.flowControl);
    if (serial->open(QIODevice::ReadWrite)) {
        settingsComDialog->setEnabled(false);
        showStatusMessage(tr("Connected to %1 : %2, %3, %4, %5, %6")
                          .arg(p.name).arg(p.stringBaudRate).arg(p.stringDataBits)
                          .arg(p.stringParity).arg(p.stringStopBits).arg(p.stringFlowControl));
    } else {
        QMessageBox::critical(0, tr("Error"), serial->errorString());

        showStatusMessage(tr("Open error"));
    }
}

void SerialPort::closeSerialPort()
{
    if (serial->isOpen())
        serial->close();
    settingsComDialog->setEnabled(true);
}

void SerialPort::readData()
{
    QByteArray data = serial->readAll();
    qDebug()<<data.toHex();
    emit this->dataReady(data);
}

void SerialPort::handleError(QSerialPort::SerialPortError error)
{
    if (error == QSerialPort::ResourceError) {
        QMessageBox::critical(0, tr("Critical Error"), serial->errorString());
        closeSerialPort();
    }
}

void SerialPort::showStatusMessage(const QString &message)
{
    qDebug()<<message;
}

void SerialPort::sendData(QByteArray data)
{
    serial->write(data);
    qDebug()<<"console: "<<data.toHex();
}

void SerialPort::setupPort()
{
    this->closeSerialPort();
    settingsComDialog->exec();
    qDebug()<<settingsComDialog->settings().baudRate;
    this->openSerialPort();
}
