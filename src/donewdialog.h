#ifndef DONEWDIALOG_H
#define DONEWDIALOG_H

#include <QDialog>

namespace Ui {
class DoNewDialog;
}

class DoNewDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DoNewDialog(QWidget *parent = 0);
    ~DoNewDialog();

    int getWidth();                 //用于返回画布的宽
    int getHeight();                 //用于返回画布的高
    QColor getBackColor();   //用于返回画布的背景色

private slots:
    void on_toolButton_clicked();

private:
    Ui::DoNewDialog *ui;

    QColor backColor;           //用于保存画布的背景色
};

#endif // DONEWDIALOG_H
