#ifndef REPROGRAMDIALOG_H
#define REPROGRAMDIALOG_H

#include <QDialog>
#include <QFile>
#include <QFileDialog>
#include <QByteArray>
#include <QMessageBox>

#include <QDebug>

namespace Ui {
class ReprogramDialog;
}

class ReprogramDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ReprogramDialog(QWidget *parent = 0);
    ~ReprogramDialog();

    typedef enum BoardType_{
        Master_ATXmega,
        Heads_ATXmega,
        Master_ATmega,
        Heads_ATmega
    }BoardType;

signals:
    void programArrReady(ReprogramDialog::BoardType type, QByteArray progArr);

public slots:
    void setProgress(int progress);

private slots:
    void on_pushButtonOpenFile_clicked();
    void on_pushButtonErase_clicked();
    void on_pushButtonWrite_clicked();

private:
    Ui::ReprogramDialog *ui;

    QFile inFile;
    QString inFileName;
    QByteArray programArr;

};

#endif // REPROGRAMDIALOG_H
