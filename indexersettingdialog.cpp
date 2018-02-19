#include "indexersettingdialog.h"
#include "ui_indexersettingdialog.h"

indexerSettingDialog::indexerSettingDialog(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::indexerSettingDialog)
{
    ui->setupUi(this);
}

indexerSettingDialog::~indexerSettingDialog()
{
    delete ui;
}
