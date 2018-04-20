#include "serialport.h"



SerialPort::SerialPort(QObject *parent):QObject(parent)
{
    serial = new QSerialPort(this);
    settingsComDialog = new SerialSettingsDialog();

    connect(serial, SIGNAL(error(QSerialPort::SerialPortError)), this, SLOT(handleError(QSerialPort::SerialPortError)));
    connect(serial, SIGNAL(readyRead()), this, SLOT(readData()));

    connect(settingsComDialog, SIGNAL(serialSettingAccepted(ComSettings)), this, SLOT(getSerialSetting(ComSettings)));

    this->openSerialPort();
}

SerialPort::SerialPort(ComSettings settings, QObject *parent):QObject(parent)
{
    serial = new QSerialPort(this);
    settingsComDialog = new SerialSettingsDialog(settings);

    connect(serial, SIGNAL(error(QSerialPort::SerialPortError)), this, SLOT(handleError(QSerialPort::SerialPortError)));
    connect(serial, SIGNAL(readyRead()), this, SLOT(readData()));

    connect(settingsComDialog, SIGNAL(serialSettingAccepted(ComSettings)), this, SLOT(getSerialSetting(ComSettings)));

    this->openSerialPort();
}

QByteArray SerialPort::dataTransform(QByteArray data)
{
    QByteArray trData;
    trData.resize(data.length()/2);
    int i;
    for(i = 0; i<trData.length(); i++)
    {
        trData[i] = ((data[i*2]<<4)&0xF0)|(data[i*2+1]&0x0F);
    }
    return trData;
}

void SerialPort::openSerialPort()
{
    ComSettings p = settingsComDialog->settings();
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
        QMessageBox::critical(0, tr("Error"), QString("Could not connect to serial port \n"
                                                      "%1 : %2, %3, %4, %5, %6\n\n"+serial->errorString())
                              .arg(p.name).arg(p.stringBaudRate).arg(p.stringDataBits)
                              .arg(p.stringParity).arg(p.stringStopBits).arg(p.stringFlowControl));

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
    static QByteArray data;
    data += serial->readAll();
    while(data.length()>11)
    {
        data8 = dataTransform(data.mid(0,12));
        bool ok;
        modData8.all = data8.toHex().toLong(&ok, 16);
        if(CrcCalc::CalculateCRC16(0xFFFF, data8.mid(0,4)) == modData8.fileds.crc16Val)
        {
            if(modData8.fileds.registerNo&0x80)
            {
                serial->write(data.mid(0,12));
                emit this->dataReady(data8);
                emit this->dataReady(modData8);
                qDebug()<<"all"<<data8.toHex()<<modData8.fileds.adress<<modData8.fileds.registerNo<<modData8.fileds.data;
                data.remove(0,12);
            }
            else
                emit this->dataReady(modData8);

        }
        else
            data.clear();
    }
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

void SerialPort::getSerialSetting(ComSettings setting)
{
    emit this->serialSettingAccepted(setting);
}

void SerialPort::sendData(QByteArray data)
{
    QByteArray dataToSend;
    int i;
    for(i = 0; i<data.length(); i++)
    {
        dataToSend.append((data[i]>>4)&0x0F);
        dataToSend.append(data[i]&0x0F);
    }
    serial->write(dataToSend);
    qDebug()<<"console: "<<data.toHex()<<" 4:"<<dataToSend.toHex();
    serial->waitForBytesWritten(-1);
}

void SerialPort::setupPort()
{
    this->closeSerialPort();

    settingsComDialog->exec();

    this->openSerialPort();
}

void SerialPort::setComParams(ComSettings sett)
{
    this->closeSerialPort();


    serial->setPortName(sett.name);
    serial->setBaudRate(sett.baudRate);
    serial->setDataBits(static_cast<QSerialPort::DataBits>(sett.stringDataBits.toInt()));
    serial->setParity(static_cast<QSerialPort::Parity>(sett.stringParity.toInt()));
    serial->setStopBits(static_cast<QSerialPort::StopBits>(sett.stringStopBits.toInt()));
    serial->setFlowControl(static_cast<QSerialPort::FlowControl>(sett.stringFlowControl.toInt()));
    if (serial->open(QIODevice::ReadWrite)) {
        settingsComDialog->setEnabled(false);
        showStatusMessage(tr("Connected to %1 : %2, %3, %4, %5, %6")
                          .arg(sett.name).arg(sett.stringBaudRate).arg(sett.stringDataBits)
                          .arg(sett.stringParity).arg(sett.stringStopBits).arg(sett.stringFlowControl));
    } else {
        QMessageBox::critical(0, tr("Error"), "Could not connect to serial port"/*serial->errorString()*/);

        showStatusMessage(tr("Open error"));
    }

}

void SerialPort::setStyleSheet(QString stSheet)
{
    settingsComDialog->setStyleSheet(stSheet);
}
