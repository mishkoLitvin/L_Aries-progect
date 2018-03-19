#ifndef INFOWIDGET_H
#define INFOWIDGET_H

#include <QFrame>
#include <QImage>
#include <QBitmap>

namespace Ui {
class InfoWidget;
}

class InfoWidget : public QFrame
{
    Q_OBJECT

public:
    explicit InfoWidget(QWidget *parent = 0);
    ~InfoWidget();
    void setPrinted(int val);
    void setTotal(int val);

private:
    Ui::InfoWidget *ui;
    QImage imageHome, imageLock, imageUp, imageArrows, imageEmerg, imageWarning, imageStopHand;
};

#endif // INFOWIDGET_H
