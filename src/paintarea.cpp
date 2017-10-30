#include "paintarea.h"
#include <QPainter>     //绘图头文件
#include <QtPrintSupport/QPrintDialog>      //打印对话框头文件
#include <QtPrintSupport/QPrinter>             //打印头文件

//构造函数
PaintArea::PaintArea(QWidget *parent) : QWidget(parent)
{
    image = QImage(400,300,QImage::Format_RGB32);       // 画 布 的 初 始 化 大 小 设 为 400*300，使用 32 位颜色
    backColor = qRgb(255,255,255);                                   //画布初始化背景色使用白色
    image.fill(backColor);

    modified = false;                                                           //设置画布内容为未改过

    scale = 1;      //缩放量初始化
    angle = 0;      //角度初始化
    shear = 0;      //拉伸量初始化

    penStyle = Qt::SolidLine;   //画笔类型初始化
    penColor = Qt::black;        //画笔颜色初始化
    penWidth = 1;                   //画笔宽度初始化
    brushColor = Qt::black;      //填充颜色初始化
    curShape = None;             //当前图形初始化

    isDrawing = false;
}

void PaintArea::paint(QImage &theImage)     //绘图事件
{
    QPainter pp(&theImage);                         //在theImage上绘图
    QPen pen = QPen();
    pen.setColor(penColor);
    pen.setStyle(penStyle);
    pen.setWidth(penWidth);
    QBrush brush = QBrush(brushColor);
    pp.setPen(pen);
    pp.setBrush(brush);

    int x,y,w,h;
     x = lastPoint.x()/scale;
     y = lastPoint.y()/scale;
     w = endPoint.x()/scale - x;
     h = endPoint.y()/scale - y;

    switch(curShape)
    {
    case None:
        {
            pp.drawLine(lastPoint/scale,endPoint/scale);   //由起始坐标和终止坐标绘制直线
            lastPoint = endPoint;                    //让终止坐标变为起始坐标
            break;
        }
    case Line:
        {
            pp.drawLine(lastPoint/scale,endPoint/scale);
            break;
        }
    case Rectangle:
        {
            pp.drawRect(x,y,w,h);
            break;
        }
    case Ellipse:
        {
            pp.drawEllipse(x,y,w,h);
            break;
        }
    }
    update();                   //进行更新界面显示，可引起窗口重绘事件，重绘窗口
    modified = true;        //设置画布内容为改过
}

void PaintArea::paintEvent(QPaintEvent *)   //重绘事件
{
    QPainter painter(this);
     painter.scale(scale,scale);
     if(isDrawing)                                          //如果正在绘制特殊图形，则显示临时绘图区上的内容
     {
         painter.drawImage(0,0,tempImage);
     }
     else
     {
         if(angle)
         {
             QImage copyImage = image;            //新建临时的copyImage，利用它进行旋转操作
             QPainter pp(&copyImage);
             QPointF center(copyImage.width()/2.0,copyImage.height()/2.0);
             pp.translate(center);
             pp.rotate(angle);
             pp.translate(-center);
             pp.drawImage(0,0,image);
             image = copyImage;                       //只会复制图片上的内容，不会复制坐标系统
             angle = 0;                                      //完成旋转后将角度值重新设为0
         }
         if(shear)
         {
             QImage copyImage = image;
             QPainter pp(&copyImage);
             pp.shear(shear,shear);
             pp.drawImage(0,0,image);
             image = copyImage;
             shear = 0;
         }
         painter.drawImage(0,0,image);
     }
}

void PaintArea::mousePressEvent(QMouseEvent *event)     //鼠标按下事件
{
    if(event->button() == Qt::LeftButton)                    //当鼠标左键按下
    {
        lastPoint = event->pos();                                //获得鼠标指针的当前坐标作为起始坐标
        isDrawing = true;
    }
}

void PaintArea::mouseMoveEvent(QMouseEvent *event)      //鼠标移动事件
{
    if(event->buttons()&Qt::LeftButton)   //如果鼠标左键按着的同时移动鼠标
     {
         endPoint = event->pos();              //获得鼠标指针的当前坐标作为终止坐标
         if(curShape == None)                  //如果不进行特殊图形绘制，则直接在image上绘制
         {
             isDrawing = false;
             paint(image);
         }
         else                                           //如果绘制特殊图形，则在临时绘图区tempImage上绘制
         {
             tempImage = image;              //每次绘制tempImage前用上一次image中的图片对其进行填充
             paint(tempImage);
         }
     }
}

void PaintArea::mouseReleaseEvent(QMouseEvent *event)       //鼠标释放事件
{
    if(event->button() == Qt::LeftButton)                     //如果鼠标左键释放
    {
        endPoint = event->pos();
        isDrawing = false;
        paint(image);
    }
}

void PaintArea::setImageSize(int width, int height)      //设置画布尺寸
{
    QImage newImage(width,height,QImage::Format_RGB32);
    image = newImage;
    update();
}

void PaintArea::setImageColor(QColor color)     //设置画布颜色
{
    backColor = color.rgb();                                //因为image的背景色要用QRgb类型的颜色，所以这里进行了一下转换
    image.fill(backColor);
    update();
}

bool PaintArea::saveImage(const QString &fileName, const char *fileFormat)      //保存图片
{
    QImage visibleImage = image;

    if (visibleImage.save(fileName, fileFormat))   //实现了文件存储
    {
        modified = false;
        return true;
    }
    else
    {
        return false;
    }
}

bool PaintArea::openImage(const QString &fileName)      //打开图片
{
    QImage loadedImage;
    if (!loadedImage.load(fileName))
        return false;

    QSize newSize = loadedImage.size();
    setImageSize(newSize.width(),newSize.height());
    image = loadedImage;

    modified = false;
    update();
    return true;
}

QSize PaintArea::getImageSize() //获得画布尺寸
{
    return image.size()*scale;
}

void PaintArea::doPrint()   //打印
 {
    QPrinter printer(QPrinter::HighResolution);

    QPrintDialog *printDialog = new QPrintDialog(&printer, this);
    if (printDialog->exec() == QDialog::Accepted)
    {
        QPainter painter(&printer);
        QRect rect = painter.viewport();
        QSize size = image.size();
        size.scale(rect.size(), Qt::KeepAspectRatio);
        painter.setViewport(rect.x(), rect.y(), size.width(), size.height());
        painter.setWindow(image.rect());
        painter.drawImage(0, 0, image);
    }
 }

void PaintArea::zoomIn()    //放大
{
    scale*=1.2;
    update();
}

void PaintArea::zoomOut()    //缩小
{
    scale/=1.2;
    update();
}

void PaintArea::zoom_1()    //还原
{
    scale = 1;
    update();
}

void PaintArea::doRotate()    //旋转
{
    angle +=90;
    update();
}

void PaintArea::doShear()    //拉伸
{
    shear = 0.2;
    update();
}

void PaintArea::doClear()    //清除
{
    image.fill(backColor);    //用现在的画布背景色进行填充
    update();
}

void PaintArea::setPenStyle(Qt::PenStyle style)    //设置画笔类型
{
   penStyle = style;
}
void PaintArea::setPenWidth(int width)    //设置画笔宽度
{
    penWidth = width;
}
void PaintArea::setPenColor(QColor color)    //设置画笔颜色
{
    penColor = color;
}

void PaintArea::setBrushColor(QColor color)    //设置填充颜色
{
    brushColor = color;
}

void PaintArea::setShape(ShapeType shape)    //设置要绘制的图形
{
    curShape = shape;
}
