#include "serialport.h"

SerialPort::SerialPort(QObject *parent):QObject(parent)
{

}

void SerialPort::connectToPort()
{

}

void SerialPort::sendData(QByteArray data)
{
    qDebug()<<"console: "<<data.toHex();
}
