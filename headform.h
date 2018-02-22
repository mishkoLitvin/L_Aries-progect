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

    typedef enum PixmapState_ {pixmapShow, pixmapHide, pixmapAnimate} PixmapState;

    void setIndex(int i);
    void setPixmap(PixmapState state, QString stStr = "background-color: rgb(0,255,150);");

    QSize getLabelSize();

signals:
    void settingButtonCliced(int index);

private:
    Ui::HeadForm *ui;
    QPushButton *pButtonSets;
    QLabel *labelIndex;
    QImage pixShirtShow, pixShirtHide, pixShirtAnimate;

    int index;

private slots:
    void settingPButtonClicedSlot();
};



#endif // HEADFORM_H
