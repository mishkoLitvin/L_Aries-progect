#include "mailsender.h"

MailSender::MailSender(const QString &from, const QString &to, const QString &subject, const QString &body, QObject *parent) : QObject(parent)
{
    socket = new QTcpSocket( this );

        connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()));
        connect(socket, SIGNAL(connected()), this, SLOT(connected()));
        connect(socket, SIGNAL(stateChanged(QTcpSocket::SocketState)), this,
            SLOT(stateChanged(QTcpSocket::SocketState)));
        connect(socket, SIGNAL(disconnectedFromHost()), this,
            SLOT(disconnected()));;

        message = "To: " + to + "\n";
        message.append("From: " + from + "\n");
        message.append("Subject: " + subject + "\n");
        message.append(body);
        message.replace(QString::fromLatin1( "\n" ), QString::fromLatin1( "\r\n" ) );
        message.replace(QString::fromLatin1( "\r\n.\r\n" ),
        QString::fromLatin1("\r\n..\r\n") );
        this->from = from;
        rcpt = to;
        state = Init;
        socket->connectToHost("MailSender.yourserver.com", 25);
        if(socket->waitForConnected (30000)) 	{qDebug("connected"); 	}

        textStream = new QTextStream(socket);
}

MailSender::~MailSender()
{
    delete textStream;
    delete socket;
}

void MailSender::stateChanged(QTcpSocket::SocketState socketState)
{
    qDebug() <<"stateChanged " << socketState;
}

void MailSender::errorReceived(QAbstractSocket::SocketError socketError)
{

}

void MailSender::disconnected()
{
    qDebug() <<"disconneted";
    qDebug() << "error "  << socket->errorString();
}

void MailSender::readyRead()
{

     qDebug() <<"readyRead";

    QString responseLine;
    do
    {
        responseLine = socket->readLine();
        response += responseLine;
    }
    while ( socket->canReadLine() && responseLine[3] != ' ' );

    responseLine.truncate( 3 );


    if ( state == Init && responseLine[0] == '2' )
    {
        // banner was okay, let's go on

        *textStream << "HELO there\r\n";
        textStream->flush();

        state = Mail;
    }
    else if ( state == Mail && responseLine[0] == '2' )
    {
        // HELO response was okay (well, it has to be)

        *textStream << "MAIL FROM: " << from << "\r\n";
        textStream->flush();
        state = Rcpt;
    }
    else if ( state == Rcpt && responseLine[0] == '2' )
    {

        *textStream << "RCPT TO: " << rcpt << "\r\n"; //r
        textStream->flush();
        state = Data;
    }
    else if ( state == Data && responseLine[0] == '2' )
    {

        *textStream << "DATA\r\n";
        textStream->flush();
        state = Body;
    }
    else if ( state == Body && responseLine[0] == '3' )
    {

        *textStream << message << "\r\n.\r\n";
        textStream->flush();
        state = Quit;
    }
    else if ( state == Quit && responseLine[0] == '2' )
    {

        *textStream << "QUIT\r\n";
        textStream->flush();
        // here, we just close.
        state = Close;
        emit status( tr( "Message sent" ) );
    }
    else if ( state == Close )
    {
        deleteLater();
        return;
    }
    else
    {
        // something broke.
        QMessageBox::warning( 0, tr( "Qt Mail Example" ), tr( "Unexpected reply from MailSender server:\n\n" ) + response );
        state = Close;
    }
    response = "";
}
