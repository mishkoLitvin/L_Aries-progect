#include "usersetting.h"
#include "ui_usersetting.h"

UserSettingDialog::UserSettingDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UserSettingDialog)
{
    ui->setupUi(this);

    usersData = new QSettings("./users.ini", QSettings::IniFormat);

    ui->checkBoxLoginDialogEn->setChecked(usersData->value("LOGIN_DIALOG_EN", false).toBool());

    ui->tableWidget->setRowCount(usersData->value("USER_COUNT", 0).toInt());
    if(ui->tableWidget->rowCount() == 0)
        ui->checkBoxLoginDialogEn->setChecked(false);

    ui->tableWidget->setColumnWidth(0, 60);
    ui->tableWidget->setColumnWidth(1, (ui->tableWidget->width()-80)/2);
    ui->tableWidget->setColumnWidth(2, (ui->tableWidget->width()-80)/2);

    int i;
    this->setStyleSheet("QCheckBox::indicator { width:32px; height: 32px;}");
    for(i = 0; i<ui->tableWidget->rowCount(); i++)
    {
        ui->tableWidget->setRowHeight(i, 45);
        ui->tableWidget->setCellWidget(i, 0, new QCheckBox("", this));
        ui->tableWidget->cellWidget(i,0)->setMinimumSize(45,45);
        static_cast<QCheckBox*>(ui->tableWidget->cellWidget(i, 0))->setStyleSheet("QCheckBox::indicator { width:40px; height: 40px;}");

        ui->tableWidget->setItem(i, 1, new QTableWidgetItem());
        ui->tableWidget->setItem(i, 2, new QTableWidgetItem());

        ui->tableWidget->item(i, 1)->setText(usersData->value("USER_"+QString::number(i)+"_NAME").toString());
        ui->tableWidget->item(i, 2)->setText(usersData->value("USER_"+QString::number(i)+"_PASSWORD").toString());
    }

    connect(ui->tableWidget, SIGNAL(cellClicked(int,int)), this, SLOT(tableCellActivated(int,int)));
    connect(ui->pButtonAdd, SIGNAL(clicked(bool)), this, SLOT(addUser()));
    connect(ui->pButtonRemove, SIGNAL(clicked(bool)), this, SLOT(removeUsers()));
    connect(ui->pButtonRemoveAll, SIGNAL(clicked(bool)), this, SLOT(removeAllUsers()));
    connect(ui->pButtonOK, SIGNAL(clicked(bool)), this, SLOT(acceptSlot()));
    connect(ui->pButtonCancel, SIGNAL(clicked(bool)), this, SLOT(rejectSlot()));
    connect(ui->checkBoxLoginDialogEn, SIGNAL(clicked(bool)), this, SLOT(loginDialogEnable()));


}

UserSettingDialog::~UserSettingDialog()
{
    delete ui;
}

bool UserSettingDialog::isUser(QString userName, QString userPassw)
{
    bool userFlag = false;
    int i;
    for(i = 0; i<ui->tableWidget->rowCount(); i++)
    {
        if((ui->tableWidget->item(i,1)->text() == userName)&(ui->tableWidget->item(i,2)->text() == userPassw))
        {
            userFlag = true;
            break;
        }
    }
    return userFlag;
}

QStringList UserSettingDialog::getUserNames()
{
    QStringList names;
    int i;
    for(i = 0; i<ui->tableWidget->rowCount(); i++)
        names.append(ui->tableWidget->item(i,1)->text());
    return names;
}

bool UserSettingDialog::getLoginWindowEnable()
{
    return ui->checkBoxLoginDialogEn->isChecked();
}

void UserSettingDialog::tableCellActivated(int row, int col)
{
    tableRowSelected = row;
    tableColnumSelected = col;
    if(col != 0)
    {
        QString itemText = KeyboardDialog::getText(this);
        if(itemText != "")
            ui->tableWidget->item(row, col)->setText(itemText);
    }
}

void UserSettingDialog::addUser()
{
    ui->tableWidget->setRowCount(ui->tableWidget->rowCount()+1);
    ui->tableWidget->setRowHeight(ui->tableWidget->rowCount()-1, 40);
    ui->tableWidget->setCellWidget(ui->tableWidget->rowCount()-1, 0, new QCheckBox("", this));
    ui->tableWidget->cellWidget(ui->tableWidget->rowCount()-1,0)->setMinimumSize(30,30);
    ui->tableWidget->setItem(ui->tableWidget->rowCount()-1, 1, new QTableWidgetItem());
    ui->tableWidget->setItem(ui->tableWidget->rowCount()-1, 2, new QTableWidgetItem());
    usersData->setValue("USER_COUNT", ui->tableWidget->rowCount());
}

void UserSettingDialog::removeUsers()
{
    int i;
    for(i = 0; i<ui->tableWidget->rowCount(); i++)
    {
        if(static_cast<QCheckBox*>(ui->tableWidget->cellWidget(i,0))->isChecked())
        {
            ui->tableWidget->removeRow(i);
            i--;
        }
    }
    usersData->setValue("USER_COUNT", ui->tableWidget->rowCount());
}

void UserSettingDialog::removeAllUsers()
{
    for(;ui->tableWidget->rowCount();)
        ui->tableWidget->removeRow(0);
    usersData->setValue("USER_COUNT", ui->tableWidget->rowCount());
}

void UserSettingDialog::acceptSlot()
{
    int i;
    usersData->setValue("USER_COUNT", ui->tableWidget->rowCount());

    for(i = 0; i<ui->tableWidget->rowCount(); i++)
    {
        usersData->setValue("USER_"+QString::number(i)+"_NAME", ui->tableWidget->item(i,1)->text());
        usersData->setValue("USER_"+QString::number(i)+"_PASSWORD", ui->tableWidget->item(i,2)->text());
    }
    usersData->sync();
    this->hide();
}

void UserSettingDialog::rejectSlot()
{
    this->hide();
}

void UserSettingDialog::loginDialogEnable()
{
    usersData->setValue("LOGIN_DIALOG_EN", ui->checkBoxLoginDialogEn->isChecked());
}

void UserSettingDialog::showEvent(QShowEvent *ev)
{
    ui->tableWidget->setColumnWidth(0, 45);
    ui->tableWidget->setColumnWidth(1, (ui->tableWidget->width()-80)/2);
    ui->tableWidget->setColumnWidth(2, (ui->tableWidget->width()-80)/2);

    ev->accept();
}

void UserSettingDialog::changeEvent(QEvent* event)
{
    if(event->type() == QEvent::LanguageChange)
    {
        ui->retranslateUi(this);
    }
}
