#include "generalsettingdialog.h"
#include "ui_generalsettingdialog.h"

GeneralSettingDialog::GeneralSettingDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GeneralSettingDialog)
{
    ui->setupUi(this);
    acceptOnDeactilationEn = true;
    ui->emailSettingWidget->setDisabled(true);

    setStyleSheet(QString(("* {color: #ABEFF6;"
                           "background-color: qlineargradient(x1: 0, y1: 0, x2: 1, y2: 1, stop: 0 #0080F0, stop: 0.8 #0050A0,stop: 1.0 #003070);"
                           "selection-color: yellow;"
                           "selection-background-color: blue;"
                           "font: 14px bold italic large \"Times New Roman\"}"
                           "QTabBar::tab:selected, QTabBar::tab:hover {background: qlineargradient(x1: 0, y1: 0, x2: 1, y2: 1, stop: 0 #0080F0, stop: 0.8 #0050A0,stop: 1.0 #003070);}"
                           "QTabBar::tab:!selected {background: qlineargradient(x1: 0, y1: 0, x2: 1, y2: 1, stop: 0 #8080A0, stop: 0.8 #606070,stop: 1.0 #202030);}")));

    connect(ui->pButtonLockUnlockEmail, SIGNAL(clicked(bool)), this, SLOT(lockUnlockEmail()));
}

GeneralSettingDialog::~GeneralSettingDialog()
{
    delete ui;
}

void GeneralSettingDialog::accept()
{
    this->hide();
}

void GeneralSettingDialog::reject()
{
    this->hide();
}

void GeneralSettingDialog::lockUnlockEmail()
{
    if(ui->pButtonLockUnlockEmail->isChecked()){
        ui->emailSettingWidget->setEnabled(true);
        ui->pButtonLockUnlockEmail->setText("Lock");
//        QByteArray passwordBArr;
//        passwordBArr.append(QInputDialog::getText(this, "Password", "Entet password:", QLineEdit::Normal));
//        if((CalculateCRC16(0xFFFF, passwordBArr) == ->value("PASSWORD_EMAIL"))){

//        }
    }
    else{
        ui->emailSettingWidget->setDisabled(true);
        ui->pButtonLockUnlockEmail->setText("Unlock");
    }
}

bool GeneralSettingDialog::event(QEvent *e)
{
    if(e->type()==QEvent::WindowDeactivate)
    {
        if(acceptOnDeactilationEn)
            this->accept();
    }
    return QWidget::event(e);
}
