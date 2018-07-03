#include "logindialog.h"
#include "ui_logindialog.h"

LoginDialog::LoginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDialog)
{
    ui->setupUi(this);

    connect(ui->pButtonLogin, SIGNAL(clicked(bool)), this, SLOT(loginAccepted()));

    ui->lineEditPassword->installEventFilter(this);
}

LoginDialog::~LoginDialog()
{
    delete ui;
}

void LoginDialog::setUserNames(QStringList names)
{
    ui->listWidget->addItems(names);
}

void LoginDialog::loginAccepted()
{
    this->userName = ui->listWidget->item(ui->listWidget->currentRow())->text();
    this->userPassword = ui->lineEditPassword->text();
    this->accept();
}

bool LoginDialog::eventFilter(QObject *watched, QEvent *event)
{
    if((event->type()==QEvent::MouseButtonDblClick)
            |((QApplication::platformName() == "eglfs")&(event->type()==QEvent::MouseButtonRelease))
            |((QApplication::platformName() == "linuxfb")&(event->type()==QEvent::MouseButtonRelease)))
    {
        ui->lineEditPassword->setText(KeyboardDialog::getText(this, "User password"));
        ui->lineEditPassword->clearFocus();
    }
    return false;
}

void LoginDialog::changeEvent(QEvent* event)
{
    if(event->type() == QEvent::LanguageChange)
    {
        ui->retranslateUi(this);
    }
}
