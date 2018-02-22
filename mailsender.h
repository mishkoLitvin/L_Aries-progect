#ifndef MAILSENDER_H
#define MAILSENDER_H


#include <QObject>
#include <QTcpSocket>
#include <QString>
#include <QTextStream>
#include <QDebug>
#include <QMessageBox>

class MailSender : public QObject
{
    Q_OBJECT
public:
    MailSender(const QString &from, const QString &to, const QString &subject, const QString &body, QObject *parent = 0);
    ~MailSender();

signals:
    void status( const QString &);

private slots:
    void stateChanged(QTcpSocket::SocketState socketState);
    void errorReceived(QTcpSocket::SocketError socketError);
    void disconnected();
    void readyRead();

private:
    QString message;
    QTextStream *textStream;
    QTcpSocket *socket;
    QString from;
    QString rcpt;
    QString response;
    enum states{Rcpt,Mail,Data,Init,Body,Quit,Close};
    int state;
};

#endif // MAILSENDER_H
