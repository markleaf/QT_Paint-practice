﻿#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "donewdialog.h"
#include "paintarea.h"
#include <QMessageBox>      //消息框头文件
#include <QFileDialog>          //文件对话框头文件
#include <QPainter>             //绘图头文件
#include <QString>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)      //构造函数
{
    ui->setupUi(this);

    resize(700,500);                                                 //主窗口大小设为700*500
    area = new PaintArea;
    scrollArea = new QScrollArea;
    scrollArea->setBackgroundRole(QPalette::Dark);   //scrollArea对象的背景色设为Dark
    scrollArea->setWidget(area);                                //将画布添加到scrollArea中
    scrollArea->widget()->setMinimumSize(800,600);  //scrollArea初始化大小设为800*600

    setCentralWidget(scrollArea);    //将scrollArea加入到主窗口的中心区

    isSaved = false;                        //设置为未保存
    curFile = tr("未命名.png");        //初始化文件名

    creatColorComboBox(ui->penColorComboBox);       //画笔颜色组合框
    creatColorComboBox(ui->brushColorComboBox);    //填充颜色组合框
}

MainWindow::~MainWindow()       //析构函数
{
    delete ui;
}

void MainWindow::doOpen()       //打开文件
{
    if (maybeSave())
    {
         QString fileName = QFileDialog::getOpenFileName(this,
                                    tr("打开文件"), QDir::currentPath());
         if (!fileName.isEmpty())
         {
             area->openImage(fileName);
             scrollArea->widget()->resize(area->getImageSize());
             //获得图片的大小，然后更改scrollArea的大小
             isSaved = true;
             curFile = fileName;
         }
    }
}

void MainWindow::doNew()        //新建文件
{
    if(maybeSave())
    {
        DoNewDialog dlg;
        if(dlg.exec() == QDialog::Accepted)
        {
            int width = dlg.getWidth();
            int height = dlg.getHeight();
            area->setImageSize(width,height);
            scrollArea->widget()->resize(width,height);
            area->setImageColor(dlg.getBackColor());
            isSaved = false;
        }
    }
}

bool MainWindow::maybeSave()        //询问保存
{
    if(area->isModified())
    {
        QMessageBox::StandardButton box;
        box = QMessageBox::warning(this,tr("保存文件"),tr("图片已经改变，是否保存？"),
                                   QMessageBox::Yes|QMessageBox::No|QMessageBox::Cancel);
        if(box == QMessageBox::Yes)
        {
            return doFileSave();
        }
        else if(box == QMessageBox::Cancel)
        {
            return false;
        }
    }
    return true;
}

bool MainWindow::doFileSave()       //文件保存还是另存为
{
    if(isSaved)
    {
        return saveFile(curFile);
    }
    else  return doFileSaveAs();
}

bool MainWindow::saveFile(QString fileName)     //文件保存
{
    if(area->saveImage(fileName,"png"))
    {
        isSaved = true;
        return true;
    }
    else return false;
}

bool MainWindow::doFileSaveAs()     //文件另存为
{
    QString fileName = QFileDialog::getSaveFileName(this,tr("另存为"),curFile);
    if(fileName.isEmpty())
    {
        return false;
    }
    else return saveFile(fileName);
}

void MainWindow::on_action_N_triggered()  //新建菜单
{
   doNew();
}

void MainWindow::on_action_O_triggered()  //打开菜单
{
    doOpen();
}

void MainWindow::on_action_S_triggered()  //保存菜单
{
    doFileSave();
}

void MainWindow::on_action_A_triggered()  //另存为菜单
{
    doFileSaveAs();
}

void MainWindow::on_action_X_triggered()   //退出菜单
{
    if(maybeSave())
        qApp->quit();
}

void MainWindow::closeEvent(QCloseEvent *event)   //关闭事件
{
    if(maybeSave())
        qApp->quit();
    else event->ignore();
}

void MainWindow::on_action_P_triggered()  //打印菜单
{
    area->doPrint();
}

void MainWindow::on_action_3_triggered()    //放大菜单
{
    area->zoomIn();
    scrollArea->widget()->resize(area->getImageSize());
     //获得图片的大小，然后更改 scrollArea 的大小
}

void MainWindow::on_action_4_triggered()    //缩小菜单
{
    area->zoomOut();
}

void MainWindow::on_action_5_triggered()    //还原菜单
{
    area->zoom_1();
}

void MainWindow::on_action_6_triggered()    //旋转菜单
{
    area->doRotate();
}

void MainWindow::on_action_7_triggered()    //拉伸菜单
{
    area->doShear();
}

void MainWindow::on_action_8_triggered()    //清空菜单
{
    area->doClear();
}

void MainWindow::on_action_9_triggered()    //绘图工具栏菜单
{
    ui->dockWidget->show();
}

void MainWindow::creatColorComboBox(QComboBox *comboBox)    //构建颜色组合框
{
    QPixmap pix(16,16);
    QPainter painter(&pix);

    painter.fillRect(0,0,16,16,Qt::black);   //先绘制一个16*16的小图片，然后给其涂色
    comboBox->addItem(QIcon(pix),tr("黑色"),QColor(Qt::black));   //再用该图片作为组合框条目的图标
    painter.fillRect(0,0,16,16,Qt::white);
    comboBox->addItem(QIcon(pix),tr("白色"),QColor(Qt::white));
    painter.fillRect(0,0,16,16,Qt::red);
    comboBox->addItem(QIcon(pix),tr("红色"),QColor(Qt::red));
    painter.fillRect(0,0,16,16,Qt::green);
    comboBox->addItem(QIcon(pix),tr("绿色"),QColor(Qt::green));
    painter.fillRect(0,0,16,16,Qt::blue);
    comboBox->addItem(QIcon(pix),tr("蓝色"),QColor(Qt::blue));
    painter.fillRect(0,0,16,16,Qt::yellow);
    comboBox->addItem(QIcon(pix),tr("黄色"),QColor(Qt::yellow));

    comboBox->addItem(tr("无颜色"),QColor(Qt::transparent));  //即透明
}

void MainWindow::on_shapeComboBox_currentIndexChanged(const QString &arg1)      //选择图形组合框
{
    if(&arg1 == tr("无"))
        area->setShape(PaintArea::None);   //利用PaintArea类中的枚举变量
    else if(&arg1 == tr("矩形"))
        area->setShape(PaintArea::Rectangle);
    else if(&arg1 == tr("直线"))
        area->setShape(PaintArea::Line);
    else if(&arg1 == tr("椭圆"))
        area->setShape(PaintArea::Ellipse);
}

void MainWindow::on_penStyleComboBox_currentIndexChanged(const QString &arg1)   //画笔风格组合框
{
    if(&arg1 == tr("实线"))
    {
        area->setPenStyle(Qt::SolidLine);
    }
    else if(&arg1 == tr("点线"))
    {
        area->setPenStyle(Qt::DotLine);
    }
}

void MainWindow::on_penWidthSpinBox_valueChanged(int arg1)   //画笔线宽组合框
{
    area->setPenWidth(arg1);
}

void MainWindow::on_penColorComboBox_currentIndexChanged(int index)   //画笔颜色组合框
{
    QColor color = ui->penColorComboBox->itemData(index,Qt::UserRole).value<QColor>();
    area->setPenColor(color);
}

void MainWindow::on_brushColorComboBox_currentIndexChanged(int index)   //填充颜色组合框
{
    QColor color = ui->brushColorComboBox->itemData(index,Qt::UserRole).value<QColor>();
    area->setBrushColor(color);
}
