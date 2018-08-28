#include "mailsender.h"
#include <QDebug>

MailSender::MailSender(QObject *parent) : QObject(parent)
{
//    smtp = new SmtpClient("smtp.gmail.com", 587, SmtpClient::TlsConnection);
    smtp = new SmtpClient("smtp.gmail.com", 465, SmtpClient::SslConnection);

    senderAddr = new EmailAddress();
    recipientAddr = new EmailAddress();

    messageText = new MimeText();
    message = new MimeMessage();
    this->message->setContent(this->messageText);

    this->message->setSender(this->senderAddr);
    this->message->addRecipient(this->recipientAddr);

    this->setRecipientMailAdress();
    this->setRecipientName();
    this->setSenderMailAdress();
    this->setSenderName();
    this->setSenderPassword();
    this->setEmailSubject();
}

void MailSender::setRecipientMailAdress(QString mail)
{
    this->recipientAddr->setAddress(mail);
}

void MailSender::setEmailSubject(QString subject)
{
    this->messageSubject = subject;
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

int MailSender::sendMessage(QString messageStr, QString subject, bool clear)
{
    if(clear)
    {
        delete  messageText;
        messageText = new MimeText(messageStr);
    }

    this->messageText->setText(messageStr);
    this->message->setContent(this->messageText);

    this->message->setSubject(subject);
//    this->message.addPart(&(this->messageText));
    if (!smtp->connectToHost()) {
        QMessageBox msgBox;
        msgBox.setText(tr("E-mail error.\n"
                          "Failed connect to host!"));
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.exec();
        return -1;
    }

    if (!smtp->login()) {
        QMessageBox msgBox;
        msgBox.setText(tr("E-mail error.\n"
                          "Failed to login."));
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.exec();
        return -2;
    }

    if (!smtp->sendMail(*(this->message))) {
        QMessageBox msgBox;
        msgBox.setText(tr("E-mail error.\n"
                          "Failed to send mail."));
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.exec();
        return -3;
    }
    smtp->quit();
    return 0;
}

int MailSender::sendMessage(QString message, bool clear)
{
    return this->sendMessage(message, this->messageSubject, clear);
}
