#ifndef MAILSENDER_H
#define MAILSENDER_H

#include <QObject>
#include <QDebug>
#include <QMessageBox>

#include "mailSrc/SmtpMime"

class MailSender : public QObject
{
    Q_OBJECT
public:
    explicit MailSender(QObject *parent = 0);

    int sendMessage(QString message, QString subject);
    int sendMessage(QString message);

private:
    SmtpClient *smtp;
    MimeMessage message;
    EmailAddress *senderAddr;
    EmailAddress *recipientAddr;
    MimeText messageText;
    QString messageSubject;


signals:

public slots:
    void setRecipientMailAdress(QString mail = "mishko.litvin@gmail.com");
    void setEmailSubject(QString subject = "LiQt Machine interface autosend message");
    void setRecipientName(QString name = "Customer");
    void setSenderMailAdress(QString mail = "Liqt.autosend@gmail.com");
    void setSenderPassword(QString pswd = "D7C5cqAz");
    void setSenderName(QString name = "LiQt Interface");
};

#endif // MAILSENDER_H
