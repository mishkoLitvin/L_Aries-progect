#ifndef UDPSOCKET_H
#define UDPSOCKET_H

#include <QUdpSocket>
#include <QByteArray>
#include <QDebug>
#include <QWidget>
#include <QNetworkInterface>
#include <QThread>
#include <QMessageBox>

class UdpSocket : public QWidget
{
    Q_OBJECT

public:
    explicit UdpSocket(QWidget *parent = 0);

    void startUdp(bool isMaster = true);
    void stopUdp();
    void sendData(QByteArray bArr);

signals:
    void exitReq();
    void dataReady(QByteArray data);

private:
    QUdpSocket *udp;
    QHostAddress udpAdr;
    quint16 udpPort;
    bool search;
    bool isMaster;

    QHostAddress myIP;


private slots:
    void connectToSocket();
    void disconnectFromSocket();

    void startTransmit(QByteArray data);
    void reciveData();
};

#endif // UDPSOCKET_H
