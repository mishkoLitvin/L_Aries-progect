#include "generalsettingdialog.h"
#include "ui_generalsettingdialog.h"

GeneralSettingDialog::GeneralSettingDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GeneralSettingDialog)
{
    ui->setupUi(this);
}

GeneralSettingDialog::~GeneralSettingDialog()
{
    delete ui;
}
