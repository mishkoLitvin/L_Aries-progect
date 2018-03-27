#include "logodialog.h"
#include "ui_logodialog.h"

LogoDialog::LogoDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LogoDialog)
{
    ui->setupUi(this);

    this->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
//    this->setWindowState(Qt::WindowMaximized);

    timer = new QTimer(this);
    timer->setInterval(30);
    movie = new QMovie(":/new/icons/icons/AAAAAAAA.gif");

    ui->label->setMovie(movie);
    connect(timer, SIGNAL(timeout()), this, SLOT(timerTimeOut()));
}

LogoDialog::~LogoDialog()
{
    delete ui;
}

void LogoDialog::timerTimeOut()
{
    static int timerCnt = 0;
    timerCnt++;
//    ui->progressBar->setValue(timerCnt);
    if(timerCnt>100)
        this->close();
}

void LogoDialog::showEvent(QShowEvent *ev)
{
    movie->start();
    timer->start();
    ev->accept();
}
