#include "donewdialog.h"
#include "ui_donewdialog.h"
#include <QColorDialog>

DoNewDialog::DoNewDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DoNewDialog)
{
    ui->setupUi(this);

    backColor = Qt::white;
}

DoNewDialog::~DoNewDialog()
{
    delete ui;
}

//用于返回画布的宽
int DoNewDialog::getWidth()
{
    return ui->widthSpinBox->text().toInt();
}

//用于返回画布的高
int DoNewDialog::getHeight()
{
    return ui->heightSpinBox->text().toInt();
}

//用于返回画布的背景色
QColor DoNewDialog::getBackColor()
{
    return backColor;
}

//改变背景色按钮
void DoNewDialog::on_toolButton_clicked()
{
    QColor newColor = QColorDialog::getColor();     //从颜色对话框获得颜色
    if (newColor.isValid())                                         //如果得到的是可用的颜色
    {
        backColor = newColor;

        QPalette palette = ui->textBrowser->palette();     //显示这个颜色
        palette.setColor(QPalette::Base,backColor);
        ui->textBrowser->setPalette(palette);
        update();
    }
}
