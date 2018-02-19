#ifndef HEADFORM_H
#define HEADFORM_H

#include <QWidget>
#include <QPixmap>
#include <QPushButton>
#include <QLabel>

namespace Ui {
class HeadForm;
}

class HeadForm : public QWidget
{
    Q_OBJECT

public:
    HeadForm(QWidget *parent = 0);
    ~HeadForm();
    void setIndex(int i);
    void setPixmap(QPixmap pix, QString stStr = "background-color: rgb(0,255,150);");

    QSize getLabelSize();

signals:
    void settingButtonCliced(int index);

private:
    Ui::HeadForm *ui;
    QPushButton *pButtonSets;
    QLabel *labelIndex;

    int index;

private slots:
    void settingPButtonClicedSlot();
};



#endif // HEADFORM_H
