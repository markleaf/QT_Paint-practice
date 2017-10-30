#ifndef PAINTAREA_H
#define PAINTAREA_H

#include <QWidget>
#include <QMouseEvent>                           //鼠标事件头文件
#include <QPoint>                                     //坐标头文件

class PaintArea : public QWidget
{
    Q_OBJECT
public:
    explicit PaintArea(QWidget *parent = 0);

    void setImageSize(int width,int height);            //设置画布尺寸
    void setImageColor(QColor color);                  //设置画布颜色
           QSize getImageSize();                              //获得画布尺寸

    bool isModified() const { return modified; }                                     //判断画布内容是否被更改过
    bool saveImage(const QString &fileName, const char *fileFormat);    //保存图片
    bool openImage(const QString &fileName);                                     //打开图片
    void doPrint();                                                                                 //打印

    void zoomIn();    //放大
    void zoomOut();   //缩小
    void zoom_1();    //还原
    void doRotate();   //旋转
    void doShear();    //拉伸
    void doClear();    //清空

    enum ShapeType   //枚举变量，几个图形的选择
    {
        None,            //没有图形
        Line,             //直线
        Rectangle,     //矩形
        Ellipse           //椭圆
    };

    void setPenStyle(Qt::PenStyle style);     //设置画笔类型
    void setPenWidth(int width);                 //设置画笔宽度
    void setPenColor(QColor color);           //设置画笔颜色
    void setBrushColor(QColor color);        //设置填充颜色
    void setShape(ShapeType shape);          //设置要绘制的图形

protected:
    void paint(QImage& theImage);                      //绘图事件
    void paintEvent(QPaintEvent *);                     //重绘事件

    void mousePressEvent(QMouseEvent *);         //鼠标按下事件
    void mouseMoveEvent(QMouseEvent *);         //鼠标移动事件
    void mouseReleaseEvent(QMouseEvent *);      //鼠标释放事件

private:
    QImage image;                                               //QImage 类对象，用于在其上绘图
    QRgb backColor;                                           //QRgb 颜色对象，存储 image 的背景色

    QPoint lastPoint,endPoint;                              //定义两个坐标对象存放鼠标指针的前后两个坐标
    bool modified;                                               //标志画布是否被更改过

    qreal scale;   //缩放量
    int angle;     //角度
    qreal shear;   //拉伸量

    QColor penColor;             //画笔颜色
    QColor brushColor;          //填充颜色
    int penWidth;                   //画笔宽度
    Qt::PenStyle penStyle;      //画笔风格

    ShapeType curShape;       //当前图形

    QImage tempImage;       //临时绘图区
    bool isDrawing;              //是否在绘制特殊图形

signals:

public slots:
};

#endif // PAINTAREA_H
