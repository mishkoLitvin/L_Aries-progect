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

void SerialPort::sendProgram(ReprogramDialog::BoardType type, QByteArray programArr)
{
    int32_t i;
    QByteArray cmdArr;
    if((type == ReprogramDialog::Master_ATXmega)|(type == ReprogramDialog::Master_ATmega))
    {
        cmdArr.clear();
        cmdArr.append(0x75);// "u"
        cmdArr.append(0x64);// "d"
        this->sendData(cmdArr, true, true);
        QThread::msleep(500);
        cmdArr.clear();
        cmdArr.append(0x75);// "u"
        cmdArr.append(0x77);// "w"
        this->sendData(cmdArr, true, true);
    }
    else
    {
        cmdArr.clear();
        cmdArr.append(0x75);// "u"
        cmdArr.append(0x78);// "x"
        cmdArr.append(0x75);// "u"
        cmdArr.append(0x64);// "d"
        this->sendData(cmdArr, true, true);
        QThread::msleep(500);
        cmdArr.clear();
        cmdArr.append(0x75);// "u"
        cmdArr.append(0x78);// "x"
        cmdArr.append(0x75);// "u"
        cmdArr.append(0x77);// "w"
        this->sendData(cmdArr, true, true);
    }

//    cmdArr.clear();
//    cmdArr.append(0x23);
//    this->sendData(cmdArr, true, true);

    for(i = 0;i<programArr.length();)
    {
        if((type == ReprogramDialog::Master_ATXmega)|(type == ReprogramDialog::Heads_ATXmega))
        {
            cmdArr.clear();
            cmdArr.append(0x23);
            cmdArr.append(programArr.mid(i, 512));
            this->sendData(cmdArr, true, true);
            i+= 512; //correct with processor type
        }
        else
        {
            this->sendData(cmdArr, true, true);
            this->sendData(programArr.mid(i, 256), true, true);
            i+= 256; //correct with processor type
        }
//        qDebug()<<i<<cmdArr.toHex();
        QThread::msleep(500);
    }
    if(i<programArr.length())
    {
        QByteArray last;
        last.clear();
        last.append(programArr.mid(i, programArr.length()-1-i));
        if((type == ReprogramDialog::Master_ATXmega)|(type == ReprogramDialog::Heads_ATXmega))
            for(;last.length()<512;)
            {
                last.append(static_cast<char>(0xFF));
                i++;
            }
        else
            for(;last.length()<256;)
            {
                last.append(static_cast<char>(0xFF));
                i++;
            }
        cmdArr.clear();
        cmdArr.append(0x23);
        cmdArr.append(last);
        this->sendData(cmdArr, true, true);
    }
    cmdArr.clear();
    QThread::msleep(500);
    cmdArr.append(static_cast<char>(0xFF));
    cmdArr.append(static_cast<char>(0xFF));
    this->sendData(cmdArr, true, true);

    emit this->proramProgres(i);
}

void SerialPort::readData()
{
    static QByteArray data = 0;
    static bool firstByte = false;
    data.append(serial->readAll());
//    qDebug()<<"Got w check"<<data.toHex().toUpper();
    while(data.length()>5)
    {
        bool ok;
        modData8.all = data.mid(0,6).toHex().toLong(&ok, 16);
        if((CrcCalc::CalculateCRC16(data.mid(0,4)) != modData8.fileds.crc16Val))
        {
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
                            bArr.append(static_cast<char>(MachineSettings::MasterDevice));
                            bArr.append(static_cast<char>(Register::masterReg_DAT));
                            bArr.append(static_cast<char>(data>>8));
                            bArr.append(static_cast<char>(data&0x00FF));
                            data = CrcCalc::CalculateCRC16(bArr);
                            bArr.append(static_cast<char>(data>>8));
                            bArr.append(static_cast<char>(data&0x00FF));
                            this->sendData(bArr);
                            break;
                        case 3:
                            bArr.clear();
                            bArr.append(static_cast<char>(MachineSettings::MasterDevice));
                            bArr.append(static_cast<char>(MachineSettings::MasterMachineType));
                            data = MachineSettings::getHeadMaxRange();
                            bArr.append(static_cast<char>(data>>8));
                            bArr.append(static_cast<char>(data&0x00FF));
                            data = CrcCalc::CalculateCRC16(bArr);
                            bArr.append(static_cast<char>(data>>8));
                            bArr.append(static_cast<char>(data&0x00FF));
                            this->sendData(bArr);
                            break;
                        case 6:
                            bArr.clear();
                            bArr.append(static_cast<char>(MachineSettings::MasterDevice));
                            bArr.append(static_cast<char>(MachineSettings::MasterMachineType));
                            data = MachineSettings::getHeadType();
                            bArr.append(static_cast<char>(data>>8));
                            bArr.append(static_cast<char>(data&0x00FF));
                            data = CrcCalc::CalculateCRC16(bArr);
                            bArr.append(static_cast<char>(data>>8));
                            bArr.append(static_cast<char>(data&0x00FF));
                            this->sendData(bArr);
                            break;
                        case 9:
                            bArr.clear();
                            bArr.append(static_cast<char>(MachineSettings::MasterDevice));
                            bArr.append(static_cast<char>(MachineSettings::MasterMachineType));
                            data = MachineSettings::getIndexLiftType();
                            bArr.append(static_cast<char>(data>>8));
                            bArr.append(static_cast<char>(data&0x00FF));
                            data = CrcCalc::CalculateCRC16(bArr);
                            bArr.append(static_cast<char>(data>>8));
                            bArr.append(static_cast<char>(data&0x00FF));
                            this->sendData(bArr);
                            break;
                        case 12:
                            bArr.clear();
                            bArr.append(static_cast<char>(IndexerLiftSettings::IndexerDevice));
                            bArr.append(static_cast<char>(Register::indexerReg_SKOK_SR));
                            data = registers->readReg(IndexerLiftSettings::IndexerDevice, Register::indexerReg_SKOK_SR);
                            bArr.append(static_cast<char>(data>>8));
                            bArr.append(static_cast<char>(data&0x00FF));
                            data = CrcCalc::CalculateCRC16(bArr);
                            bArr.append(static_cast<char>(data>>8));
                            bArr.append(static_cast<char>(data&0x00FF));
                            this->sendData(bArr);
                            break;
                        case 15:
                            bArr.clear();
                            bArr.append(static_cast<char>(IndexerLiftSettings::LiftDevice));
                            bArr.append(static_cast<char>(Register::liftReg_DIST_PULSE_L));
                            data = registers->readReg(IndexerLiftSettings::LiftDevice, Register::liftReg_DIST_PULSE_L);
                            bArr.append(static_cast<char>(data>>8));
                            bArr.append(static_cast<char>(data&0x00FF));
                            data = CrcCalc::CalculateCRC16(bArr);
                            bArr.append(static_cast<char>(data>>8));
                            bArr.append(static_cast<char>(data&0x00FF));
                            this->sendData(bArr);
                            break;
                        case 18:
                            bArr.clear();
                            bArr.append(static_cast<char>(IndexerLiftSettings::LiftDevice));
                            bArr.append(static_cast<char>(Register::liftReg_DIST_PULSE_H));
                            data = registers->readReg(IndexerLiftSettings::LiftDevice, Register::liftReg_DIST_PULSE_H);
                            bArr.append(static_cast<char>(data>>8));
                            bArr.append(static_cast<char>(data&0x00FF));
                            data = CrcCalc::CalculateCRC16(bArr);
                            bArr.append(static_cast<char>(data>>8));
                            bArr.append(static_cast<char>(data&0x00FF));
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
                    this->sendModData(modData8.bits.adress,
                                      modData8.bits.registerNo,
                                      registers->readReg(modData8.fileds.adress,
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
        qDebug()<<"send half "<<dataToSend.length();
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
    dataArr.append(static_cast<char>(dev));
    dataArr.append(static_cast<char>(place));
    dataArr.append(static_cast<char>((data>>8)&0x00FF));
    dataArr.append(static_cast<char>(data&0x00FF));
    uint16_t crc;
    crc = CrcCalc::CalculateCRC16(dataArr);
    dataArr.append(static_cast<char>((crc>>8)&0x00FF));
    dataArr.append(static_cast<char>(crc&0x00FF));
    serial->write(dataArr);
    serial->waitForBytesWritten(-1);
    //    qDebug()<<"send mod data:"<<dataArr.toHex().toUpper();
}

void SerialPort::sendReg(uint8_t dev, uint8_t place)
{
    uint16_t data;
    data = registers->readReg(dev, place);
    QByteArray dataArr;
    dataArr.append(static_cast<char>(dev));
    dataArr.append(static_cast<char>(place));
    dataArr.append(static_cast<char>((data>>8)&0x00FF));
    dataArr.append(static_cast<char>(data&0x00FF));
    data = CrcCalc::CalculateCRC16(dataArr);
    dataArr.append(static_cast<char>((data>>8)&0x00FF));
    dataArr.append(static_cast<char>(data&0x00FF));
    this->sendData(dataArr);
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
