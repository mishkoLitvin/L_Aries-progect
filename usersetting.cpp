#include "usersetting.h"
#include "ui_usersetting.h"

UserSettingDialog::UserSettingDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UserSettingDialog)
{
    ui->setupUi(this);

    usersData = new QSettings("./users.ini", QSettings::IniFormat);

    ui->tableWidget->setRowCount(usersData->value("USER_COUNT", 0).toInt());

    ui->tableWidget->setColumnWidth(0, 25);
    ui->tableWidget->setColumnWidth(1, (ui->tableWidget->width()-40)/2);
    ui->tableWidget->setColumnWidth(2, (ui->tableWidget->width()-40)/2);

    int i;
    for(i = 0; i<ui->tableWidget->rowCount(); i++)
    {
        ui->tableWidget->setRowHeight(i, 40);
        ui->tableWidget->setCellWidget(i, 0, new QCheckBox("", this));
        ui->tableWidget->cellWidget(i,0)->setMinimumSize(30,30);

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

void UserSettingDialog::tableCellActivated(int row, int col)
{
    qDebug()<<row<<col;
    tableRowSelected = row;
    tableColnumSelected = col;
}

void UserSettingDialog::addUser()
{
    ui->tableWidget->setRowCount(ui->tableWidget->rowCount()+1);
    ui->tableWidget->setRowHeight(ui->tableWidget->rowCount()-1, 40);
    ui->tableWidget->setCellWidget(ui->tableWidget->rowCount()-1, 0, new QCheckBox("", this));
    ui->tableWidget->cellWidget(ui->tableWidget->rowCount()-1,0)->setMinimumSize(30,30);
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
    this->hide();
}

void UserSettingDialog::rejectSlot()
{
    this->hide();
}

void UserSettingDialog::showEvent(QShowEvent *ev)
{
    ui->tableWidget->setColumnWidth(0, 35);
    ui->tableWidget->setColumnWidth(1, (ui->tableWidget->width()-40)/2);
    ui->tableWidget->setColumnWidth(2, (ui->tableWidget->width()-40)/2);
}
