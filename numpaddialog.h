#ifndef NUMPADDIALOG_H
#define NUMPADDIALOG_H

#include <QDialog>
#include <QPushButton>

class NumpadButton : public QPushButton{
    Q_OBJECT
public:
    NumpadButton(unsigned int inputNumber, QString name);
signals:
    void clicked(int index);
private:
    unsigned int index;
private slots:
    void thisClicked()
    {
        emit this->clicked(this->index);
    }
};

namespace Ui {
class NumpadDialog;
}

class NumpadDialog : public QDialog
{
    Q_OBJECT

signals:
    void valueSubmited(int value);

public:
    explicit NumpadDialog(QWidget *parent = 0);
    ~NumpadDialog();

private slots:
    void appendToLineEdit(int number);
    void submitValue();

private:
    Ui::NumpadDialog *ui;
    NumpadButton *buttons[12];
};

#endif // NUMPADDIALOG_H
