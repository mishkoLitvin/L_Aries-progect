#include "usersetting.h"
#include "ui_usersetting.h"

UserSetting::UserSetting(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UserSetting)
{
    ui->setupUi(this);

    ui->tableWidget->setColumnWidth(0, 25);
    ui->tableWidget->setColumnWidth(1, (ui->tableWidget->width()-40)/2);
    ui->tableWidget->setColumnWidth(2, (ui->tableWidget->width()-40)/2);

    int i;
    for(i = 0; i<ui->tableWidget->rowCount(); i++)
    {
        ui->tableWidget->setRowHeight(i, 40);
        ui->tableWidget->setCellWidget(i, 0, new QCheckBox("", this));
        ui->tableWidget->cellWidget(i,0)->setMinimumSize(30,30);
    }

    connect(ui->tableWidget, SIGNAL(cellClicked(int,int)), this, SLOT(tableCellActivated(int,int)));
    connect(ui->pButtonAdd, SIGNAL(clicked(bool)), this, SLOT(addUser()));
    connect(ui->pButtonRemove, SIGNAL(clicked(bool)), this, SLOT(removeUsers()));
    connect(ui->pButtonRemoveAll, SIGNAL(clicked(bool)), this, SLOT(removeAllUsers()));


}

UserSetting::~UserSetting()
{
    delete ui;
}

void UserSetting::tableCellActivated(int row, int col)
{
    qDebug()<<row<<col;
    tableRowSelected = row;
    tableColnumSelected = col;
}

void UserSetting::addUser()
{
    ui->tableWidget->setRowCount(ui->tableWidget->rowCount()+1);
    ui->tableWidget->setRowHeight(ui->tableWidget->rowCount()-1, 40);
    ui->tableWidget->setCellWidget(ui->tableWidget->rowCount()-1, 0, new QCheckBox("", this));
    ui->tableWidget->cellWidget(ui->tableWidget->rowCount()-1,0)->setMinimumSize(30,30);
}

void UserSetting::removeUsers()
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
}

void UserSetting::removeAllUsers()
{
    for(;ui->tableWidget->rowCount();)
        ui->tableWidget->removeRow(0);
}

void UserSetting::showEvent(QShowEvent *ev)
{
    ui->tableWidget->setColumnWidth(0, 35);
    ui->tableWidget->setColumnWidth(1, (ui->tableWidget->width()-40)/2);
    ui->tableWidget->setColumnWidth(2, (ui->tableWidget->width()-40)/2);

}
