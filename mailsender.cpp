#include "mailsender.h"
#include <QDebug>

MailSender::MailSender(QObject *parent) : QObject(parent)
{
//    smtp = new SmtpClient("smtp.gmail.com", 587, SmtpClient::TlsConnection);
    smtp = new SmtpClient("smtp.gmail.com", 465, SmtpClient::SslConnection);

    senderAddr = new EmailAddress();
    recipientAddr = new EmailAddress();
    this->message.setSender(this->senderAddr);
    this->message.addRecipient(this->recipientAddr);

    this->setRecipientMailAdress();
    this->setRecipientName();
    this->setSenderMailAdress();
    this->setSenderName();
    this->setSenderPassword();
}

void MailSender::setRecipientMailAdress(QString mail)
{
    this->recipientAddr->setAddress(mail);
}

void MailSender::setRecipientName(QString name)
{
    this->recipientAddr->setName(name);
}

void MailSender::setSenderMailAdress(QString mail)
{
    this->senderAddr->setAddress(mail);
    smtp->setUser(mail);
}

void MailSender::setSenderPassword(QString pswd)
{
    smtp->setPassword(pswd);
}

void MailSender::setSenderName(QString name)
{
    this->senderAddr->setName(name);
}

int MailSender::sendMessage(QString message, QString subject)
{
    this->messageText.setText(message);
    this->message.setSubject(subject);
    this->message.addPart(&(this->messageText));

    if (!smtp->connectToHost()) {
        qDebug() << "Failed to connect to host!" << endl;
        return -1;
    }

    if (!smtp->login()) {
        qDebug() << "Failed to login!" << endl;
        return -2;
    }

    if (!smtp->sendMail(this->message)) {
        qDebug() << "Failed to send mail!" << endl;
        return -3;
    }

    smtp->quit();
    return 0;
}
