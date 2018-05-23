#include "udpsocket.h"

UdpSocket::UdpSocket(QWidget *parent):
    QWidget(parent)
{

}

void UdpSocket::startUdp(bool isMaster)
{
    this->isMaster = isMaster;

    this->connectToSocket();

    foreach (const QHostAddress &address, QNetworkInterface::allAddresses()) {
        if (address.protocol() == QAbstractSocket::IPv4Protocol && address != QHostAddress(QHostAddress::LocalHost))
             this->myIP = address;
    }

    this->search = true;
    int cnt = 0;
    bool finded = true;
    quint8 i, j;

    if(!this->isMaster)
    {
        for(j = 1; j<0xFF; j++)
        {
            if(!this->search)
            {
                break;
            }
            cnt++;
            for(i = 1; i<0xFF; i++)
            {
                if(!this->search)
                {
                    break;
                }
                if((i != (myIP.toIPv4Address()&0xFF))&this->search)
                {
                    this->udpAdr.setAddress(((myIP.toIPv4Address())&((quint32)0xFFFFFF00))|(i));
                    this->startTransmit("UUUUUU");
                    QThread::usleep(1600);
                    this->reciveData();
                }
            }
            if(cnt>100)
            {
                finded = false;
                break;
            }
        }
        if(!finded)
        {
            QMessageBox msgBox;
            msgBox.setText("Cant connect");
            msgBox.setInformativeText("Please restart a program\n"
                                      "Press OK to try again");
            msgBox.setWindowTitle("Info");
            msgBox.setIcon(QMessageBox::Warning);
            msgBox.setStandardButtons(QMessageBox::Ok);
            msgBox.exec();
            qDebug()<<"Exit";
            this->startUdp();
        }
    }
}

void UdpSocket::stopUdp()
{
    this->disconnectFromSocket();
}

void UdpSocket::sendData(QByteArray bArr)
{
    this->startTransmit(bArr);
}

void UdpSocket::connectToSocket()
{
    udpPort = 1111;

    udp = new QUdpSocket(this);
    udp->bind(udpPort);
    udp->open(QIODevice::ReadWrite);

    connect(udp, SIGNAL(readyRead()), this, SLOT(reciveData()));

}

void UdpSocket::disconnectFromSocket()
{
    {
        udp->disconnectFromHost();
        udp->close();
    }
    delete udp;
}

void UdpSocket::startTransmit(QByteArray data)
{
    udp->writeDatagram(data,udpAdr,udpPort);
    udp->waitForBytesWritten();
}

void UdpSocket::reciveData()
{
    static QByteArray inData;
    QByteArray tempData;
    QHostAddress senderIP;
    quint16 senderPort;
    while (udp->hasPendingDatagrams())
    {
        tempData.resize(udp->pendingDatagramSize());
        udp->readDatagram(tempData.data(), (qint64)tempData.size(), &senderIP, &senderPort);
        inData.append(tempData);
    }

    udpAdr = senderIP;
    if(inData.length()>0)
    {
        if((inData == QByteArray("UUUUUU"))&this->isMaster)
        {
            this->startTransmit(inData);
            this->search = false;
        }
        else
        {
            emit this->dataReady(inData);
        }
        inData.clear();
    }

}
