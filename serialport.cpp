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

//    this->openSerialPort();
}

void SerialPort::setRegisterPointer(Register *regPtr)
{
    this->registers = regPtr;
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
    this->dataToSendBuff.clear();
    this->replyCnt = 0;
    this->initApp = false;

}

void SerialPort::openSerialPort(ComSettings cSett)
{
    this->closeSerialPort();

    settingsComDialog->setSettings(cSett);
    this->dataToSendBuff.clear();
    this->replyCnt = 0;
    this->initApp = false;

    this->openSerialPort();
}

void SerialPort::closeSerialPort()
{
    if (serial->isOpen())
    {
        serial->close();
        qDebug()<<"Serial disconnected.";
    }
    settingsComDialog->setEnabled(true);

}

void SerialPort::sendProgram(QByteArray programArr)
{
    uint32_t i;
    for(i = 0;i<programArr.length();)
    {
        this->sendData(programArr.mid(i, 16), true, true);
        i+= 16;
        if(i&0x80)
            emit this->proramProgres(i);
    }

    if(i<programArr.length())
    {
        QByteArray last;
        last.clear();
        last.append(programArr.mid(i, programArr.length()-1-i));
        for(;last.length()<16;)
        {
            last.append((char)0xFF);
            i++;
        }
        this->sendData(last, true, true);
    }
    emit this->proramProgres(i);
}

void SerialPort::readData()
{
    static QByteArray data = 0;
    static bool firstByte = false;
    data.append(serial->readAll());

    while(data.length()>5)
    {
        bool ok;
        modData8.all = data.mid(0,6).toHex().toLong(&ok, 16);
        if((CrcCalc::CalculateCRC16(data.mid(0,4)) != modData8.fileds.crc16Val))
        {
            qDebug()<<"CRC Err!!!";
            data.remove(0,1);
        }
        else
        {
            qDebug()<<"Got  data:"<<data.mid(0,6).toHex().toUpper()<<initApp<<replyCnt;
            emit this->working();
            if(modData8.fileds.rwBit)
            {
//                qDebug()<<"write:"
//                       <<modData8.fileds.adress
//                      <<modData8.fileds.registerNo
//                     <<modData8.fileds.data;
                this->sendData(data.mid(0,6), true);
                registers->writeReg(modData8.fileds.adress,
                                    modData8.fileds.registerNo,
                                    modData8.fileds.data);
                if(!((modData8.fileds.adress == 0)&(modData8.fileds.registerNo == 2)))
                    emit this->dataReady(modData8);
                if((modData8.fileds.adress == 0)
                        &(modData8.fileds.registerNo == 34)
                        &(modData8.fileds.data == 65535))
                {
                    dataToSendBuff.clear();
                    initApp = true;
                }
            }
            else
            {
                if((modData8.fileds.adress == 0)
                        &(modData8.fileds.registerNo == 2)
                        /*&(modData8.fileds.data == 0)*/)
                {
                    if(modData8.fileds.data != 0)
                    {

                        dataToSendBuff.remove(0,6);
                        if(initApp)
                            replyCnt++;
                    }
                    else
                        if((initApp)&(replyCnt != 0))
                            replyCnt++;
                    //=================================================================================
                    //this part of code is mustwrited becose machine will not start without this data
                    //but master board dont ask that.
                    if((initApp)&(modData8.fileds.data == 0))
                    {
                        QByteArray bArr;
                        int data;
                        qDebug()<<"\t\t\t\t"<<initApp<<replyCnt;
                        switch(replyCnt){
                        case 0:
                            bArr.clear();
                            data = registers->readReg(MachineSettings::MasterDevice, Register::masterReg_DAT);
                            bArr.append(MachineSettings::MasterDevice);
                            bArr.append(Register::masterReg_DAT);
                            bArr.append((char)(data>>8));
                            bArr.append((char)(data&0x00FF));
                            data = CrcCalc::CalculateCRC16(bArr);
                            bArr.append((char)(data>>8));
                            bArr.append((char)(data&0x00FF));
                            this->sendData(bArr);
                            break;
                        case 3:
                            bArr.clear();
                            bArr.append((char)MachineSettings::MasterDevice);
                            bArr.append((char)MachineSettings::MasterMachineType);
                            data = MachineSettings::getHeadMaxRange();
                            bArr.append((char)(data>>8));
                            bArr.append((char)(data&0x00FF));
                            data = CrcCalc::CalculateCRC16(bArr);
                            bArr.append((char)(data>>8));
                            bArr.append((char)(data&0x00FF));
                            this->sendData(bArr);
                            break;
                        case 6:
                            bArr.clear();
                            bArr.append((char)MachineSettings::MasterDevice);
                            bArr.append((char)MachineSettings::MasterMachineType);
                            data = MachineSettings::getHeadType();
                            bArr.append((char)(data>>8));
                            bArr.append((char)(data&0x00FF));
                            data = CrcCalc::CalculateCRC16(bArr);
                            bArr.append((char)(data>>8));
                            bArr.append((char)(data&0x00FF));
                            this->sendData(bArr);
                            break;
                        case 9:
                            bArr.clear();
                            bArr.append((char)MachineSettings::MasterDevice);
                            bArr.append((char)MachineSettings::MasterMachineType);
                            data = MachineSettings::getIndexLiftType();
                            bArr.append((char)(data>>8));
                            bArr.append((char)(data&0x00FF));
                            data = CrcCalc::CalculateCRC16(bArr);
                            bArr.append((char)(data>>8));
                            bArr.append((char)(data&0x00FF));
                            this->sendData(bArr);
                            break;
                        case 12:
                            bArr.clear();
                            bArr.append((char)IndexerLiftSettings::IndexerDevice);
                            bArr.append((char)Register::indexerReg_SKOK_SR);
                            data = registers->readReg(IndexerLiftSettings::IndexerDevice, Register::indexerReg_SKOK_SR);
                            bArr.append((char)(data>>8));
                            bArr.append((char)(data&0x00FF));
                            data = CrcCalc::CalculateCRC16(bArr);
                            bArr.append((char)(data>>8));
                            bArr.append((char)(data&0x00FF));
                            this->sendData(bArr);
                            break;
                        case 15:
                            bArr.clear();
                            bArr.append((char)IndexerLiftSettings::LiftDevice);
                            bArr.append((char)Register::liftReg_DIST_PULSE_L);
                            data = registers->readReg(IndexerLiftSettings::LiftDevice, Register::liftReg_DIST_PULSE_L);
                            bArr.append((char)(data>>8));
                            bArr.append((char)(data&0x00FF));
                            data = CrcCalc::CalculateCRC16(bArr);
                            bArr.append((char)(data>>8));
                            bArr.append((char)(data&0x00FF));
                            this->sendData(bArr);
                            break;
                        case 18:
                            bArr.clear();
                            bArr.append((char)IndexerLiftSettings::LiftDevice);
                            bArr.append((char)Register::liftReg_DIST_PULSE_H);
                            data = registers->readReg(IndexerLiftSettings::LiftDevice, Register::liftReg_DIST_PULSE_H);
                            bArr.append((char)(data>>8));
                            bArr.append((char)(data&0x00FF));
                            data = CrcCalc::CalculateCRC16(bArr);
                            bArr.append((char)(data>>8));
                            bArr.append((char)(data&0x00FF));
                            this->sendData(bArr);
                            this->initApp = false;
                            break;
                        }
                    }
                    //end of mustwrited code.
                    //=================================================================================


                    if((dataToSendBuff.length()>5))
                    {
                        qDebug()<<"read request:"<<modData8.bits.adress<<modData8.bits.registerNo<<modData8.bits.data
                            <<"\t\trequest send:"<<0<< 2<< (uint16_t)dataToSendBuff[0]<< (uint16_t)dataToSendBuff[1];
                        this->sendModData(0, 2, ((uint16_t)(((uint16_t)(dataToSendBuff[0]<<8))|dataToSendBuff[1])));
                    }
                    else
                        this->sendModData(0, 2, 0);
//                        this->sendData(data.mid(0,6), true);

                }
                else
                {
//                    qDebug()<<"read reg:"<<modData8.fileds.adress<<modData8.fileds.registerNo<<modData8.fileds.data
//                        <<"\t\treg send:"<<modData8.fileds.adress<<modData8.fileds.registerNo<<registers->readReg(modData8.fileds.adress,
//                                                                                                                  modData8.fileds.registerNo);
                    this->sendModData(modData8.bits.adress, modData8.bits.registerNo, registers->readReg(modData8.fileds.adress,
                                                                                                         modData8.fileds.registerNo));
                }
            }
            data.remove(0,6);
        }
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
    qDebug()<<"Status message:"<<message;
}

void SerialPort::getSerialSetting(ComSettings setting)
{
    emit this->serialSettingAccepted(setting);
}

void SerialPort::sendData(QByteArray data, bool send, bool halfByte)
{
    bool ok;
    ModData mData;
    mData.all = data.toHex().toLong(&ok, 16);
    registers->writeReg(mData.fileds.adress,
                        mData.fileds.registerNo,
                        mData.fileds.data);

    if(halfByte)
    {
        QByteArray dataToSend;
        int i;
        for(i = 0; i<data.length(); i++)
        {
            dataToSend.append((data[i]>>4)&0x0F);
            dataToSend.append(data[i]&0x0F);
        }
        serial->write(dataToSend);
        serial->waitForBytesWritten(-1);
    }
    else
    {
        if(send)
        {

            serial->write(data);
            serial->waitForBytesWritten(-1);
//            qDebug()<<"Send data:"<<data.toHex().toUpper();
        }
        else
        {
            dataToSendBuff.append(data);
            qDebug()<<"buff:"<<dataToSendBuff.length()<<dataToSendBuff.toHex();
        }
    }
//
}

void SerialPort::sendModData(uint8_t dev, uint8_t place, uint16_t data)
{
    QByteArray dataArr;
    dataArr.append(dev);
    dataArr.append(place);
    dataArr.append((char)((data>>8)&0x00FF));
    dataArr.append((char)(data&0x00FF));
    uint16_t crc;
    crc = CrcCalc::CalculateCRC16(dataArr);
    dataArr.append((char)((crc>>8)&0x00FF));
    dataArr.append((char)(crc&0x00FF));
    serial->write(dataArr);
    serial->waitForBytesWritten(-1);
//    qDebug()<<"send mod data:"<<dataArr.toHex().toUpper();
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
