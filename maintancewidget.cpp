#include "maintancewidget.h"
#include "ui_maintancewidget.h"

MaintanceWidget::MaintanceWidget(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MaintanceWidget)
{
    ui->setupUi(this);
    connect(ui->pButtonSolved, SIGNAL(clicked(bool)), this, SLOT(solved()));
    connect(ui->pButtonClose, SIGNAL(clicked(bool)), this, SLOT(closeWindow()));
    connect(ui->pButtonTutorial, SIGNAL(clicked(bool)), this, SLOT(callTutorial()));
}

MaintanceWidget::~MaintanceWidget()
{
    delete ui;
}

void MaintanceWidget::clearText()
{
    ui->listWidget->clear();
}

void MaintanceWidget::setElemets(QList<MaintanceElement> elemList)
{
    maintanceList.clear();
    maintanceList.append(elemList);
    this->clearText();
    int i;
    for(i = 0; i<elemList.length(); i++)
    {
        ui->listWidget->addItem(elemList[i].troubleName+"\n"+elemList[i].troubleInfo);
    }
}

void MaintanceWidget::solved()
{
    int trIndex = ui->listWidget->currentRow();
    emit this->troubleSolved(trIndex);
    maintanceList.removeAt(trIndex);
    ui->listWidget->takeItem(trIndex);
}

void MaintanceWidget::closeWindow()
{
    emit this->maintanceClosed(this->maintanceList);
    this->hide();
}

void MaintanceWidget::callTutorial()
{
    int trIndex = ui->listWidget->currentRow();
    emit this->tutorialRequest(trIndex);
}
