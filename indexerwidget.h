#ifndef INDEXERWIDGET_H
#define INDEXERWIDGET_H

#include <QWidget>

namespace Ui {
class IndexerWidget;
}

class IndexerWidget : public QWidget
{
    Q_OBJECT

public:
    explicit IndexerWidget(QWidget *parent = 0);
    ~IndexerWidget();

private slots:

    void on_pButtonLock_clicked();
    void on_pButtonMove_clicked();
    void on_pButtonAuto_clicked();
    void on_pButtonPrint_clicked();
    void on_pButtonMoveLeft_clicked();
    void on_pButtonMoveUp_clicked();
    void on_pButtonRight_clicked();

private:
    Ui::IndexerWidget *ui;
};

#endif // INDEXERWIDGET_H
