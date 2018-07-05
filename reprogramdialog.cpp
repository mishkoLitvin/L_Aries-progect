#include "reprogramdialog.h"
#include "ui_reprogramdialog.h"

ReprogramDialog::ReprogramDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ReprogramDialog)
{
    ui->setupUi(this);
}

ReprogramDialog::~ReprogramDialog()
{
    delete ui;
}

void ReprogramDialog::setProgress(int progress)
{
    qDebug()<<progress<<programArr.length();
    ui->progressBar->setValue(progress/programArr.length()*100+1);
    if(progress >= programArr.length())
    {
        QMessageBox msgBox;
//        msgBox.setParent(this);
        msgBox.setText("Reprogram complete.");
        msgBox.addButton(QMessageBox::Ok);
        msgBox.exec();
    }
}

void ReprogramDialog::on_pushButtonOpenFile_clicked()
{
    inFileName = QFileDialog::getOpenFileName(this, "Open HEX", QDir::homePath(), "HEX files (*.hex)");
    inFile.setFileName(inFileName);

    ui->lineEditFName->setText(inFileName);
}

void ReprogramDialog::on_pushButtonErase_clicked()
{
    int i;
    programArr.clear();
    for(i = 0; i<0xFFFF; i++)
    {
        programArr.append((char)0x00);
    }
    emit this->programArrReady(programArr);
}

void ReprogramDialog::on_pushButtonWrite_clicked()
{
    inFile.open(QIODevice::ReadOnly);
    QString tempReadLine, tempAppendLine;
    int addr, len, recType;
    bool ok;
    QByteArray tempBArr;
    programArr.clear();

    bool errFlag = false;

    for(;!inFile.atEnd();)
    {
        tempReadLine.clear();
        tempAppendLine.clear();
        tempReadLine = inFile.readLine();
        qDebug()<<tempReadLine.mid(0, 1);
        if((tempReadLine.mid(0, 1) != ":")|(tempReadLine.length()>100))
        {
            qDebug()<<"RRRRRRRRRRRR";
            errFlag = true;
            break;
        }
        len = tempReadLine.mid(1,2).toInt(&ok, 16);
        addr = tempReadLine.mid(3,4).toInt(&ok, 16);
        recType = tempReadLine.mid(7,2).toInt(&ok, 16);
        tempBArr = QByteArray::fromHex(tempReadLine.mid(9,len*2).toLatin1());
        programArr.append(tempBArr);
    }
    inFile.close();

    if(errFlag)
    {
        QMessageBox msgBox;
//        msgBox.setParent(this);
        msgBox.setText("Wrong HEX file format!\n Reprogram is not complete.");
        msgBox.addButton(QMessageBox::Ok);
        msgBox.exec();
    }
    else
        emit this->programArrReady(programArr);
}

