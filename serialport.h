#ifndef SERIALPORT_H
#define SERIALPORT_H

#include <QObject>
#include <QByteArray>
#include <QDebug>

class SerialPort : public QObject
{
    Q_OBJECT

public:
    explicit SerialPort(QObject *parent = 0);

public slots:
    void connectToPort();
    void sendData(QByteArray data);

signals:
    void dataReady(QByteArray data);
};

#endif // SERIALPORT_H
