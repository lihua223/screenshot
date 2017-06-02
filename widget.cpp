#include "widget.h"
#include "ui_widget.h"
#include <QDebug>
#include <QDesktopWidget>
#include <QImage>
#include "form.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    setMouseTracking(true);

    is_drawing = false;
    form = new Form();
    fullScreenPix = QPixmap::grabWindow(QApplication::desktop()->winId());
//    QPainter painter(this);
//    painter.setPen(QPen(Qt::blue, 1, Qt::SolidLine, Qt::FlatCap));//设置画笔
//    painter.drawPixmap(0, 0, QApplication::desktop()->width()
//                       , QApplication::desktop()->height(), fullScreenPix);
    pix = fullScreenPix;
//    qDebug() << QApplication::desktop()->height();
//    qDebug() << QApplication::desktop()->availableGeometry().height();
    init();
}

Widget::~Widget()
{
    delete ui;
}

void Widget::init()
{
    startPoint = QPoint(0, 0);
    endPoint = QPoint(0, 0);
    startMovePoint = QPoint(0, 0);
    endMovePoint = QPoint(0, 0);
    currentShotStatus = initshot;
    controlStatus = left;
}

QRect Widget::getShotRect(const QPoint &startPoint, const QPoint &endPoint)
{
    int x, y, width, height;
    width = qAbs(startPoint.x() - endPoint.x());
    height = qAbs(startPoint.y() - endPoint.y());
    x = startPoint.x() > endPoint.x() ? endPoint.x() : startPoint.x();
    y = startPoint.y() > endPoint.y() ? endPoint.y() : startPoint.y();

    return QRect(x, y, width, height);
}

void Widget::drawShotPixmap()
{
    painter.drawRect(shotRect);
    tempPix = pix.copy(shotRect);

    if (shotRect.width() > 0 && shotRect.height())
    {
        painter.drawPixmap(shotRect.topLeft(), tempPix);
    }

    drawControlSpot(shotRect);
}

void Widget::drawControlSpot(const QRect &rect)
{
    int x, y;
    QColor color(0, 0, 255);
    QPoint leftspot = rect.topLeft();
    QPoint rightspot = rect.topRight();
    QPoint leftbottomspot = rect.bottomLeft();
    QPoint rightbottomspot = rect.bottomRight();

    x = (leftspot.x()+rightspot.x())/2;
    y = leftspot.y();
    QPoint topcenterspot = QPoint(x, y);

    x = leftspot.x();
    y = (leftspot.y()+leftbottomspot.y())/2;
    QPoint leftcenterspot = QPoint(x, y);

    x = rightspot.x();
    y = (rightspot.y()+rightbottomspot.y())/2;
    QPoint rightcenterspot = QPoint(x, y);

    x = (leftbottomspot.x()+rightbottomspot.x())/2;
    y = leftbottomspot.y();
    QPoint bottomcenterspot = QPoint(x, y);

    spotLeft = QRect(leftspot.x()-2, leftspot.y()-2, 6, 6);
    spotRight = QRect(rightspot.x()-2, rightspot.y()-2, 6, 6);
    spotTopCenter = QRect(topcenterspot.x()-2, topcenterspot.y()-2, 6, 6);
    spotLeftCenter = QRect(leftcenterspot.x()-2, leftcenterspot.y()-2, 6, 6);
    spotRightCenter = QRect(rightcenterspot.x()-2, rightcenterspot.y()-2, 6, 6);
    spotLeftBottom = QRect(leftbottomspot.x()-2, leftbottomspot.y()-2, 6, 6);
    spotRightBottom = QRect(rightbottomspot.x()-2, rightbottomspot.y()-2, 6, 6);
    spotBottomCenter = QRect(bottomcenterspot.x()-2, bottomcenterspot.y()-2, 6, 6);

    painter.fillRect(spotLeft, color);
    painter.fillRect(spotRight, color);
    painter.fillRect(spotTopCenter, color);
    painter.fillRect(spotLeftCenter, color);
    painter.fillRect(spotRightCenter, color);
    painter.fillRect(spotLeftBottom, color);
    painter.fillRect(spotRightBottom, color);
    painter.fillRect(spotBottomCenter, color);
}

bool Widget::is_shotRect(const QPoint &point)
{
    if (shotRect.contains(point))
    {
        return true;
    }

    return false;
}

QRect Widget::getMoveShotRect()
{
//    int MoveX, MoveY;
//    checkMoveEndPoint();

    QRect result;
    QPoint tmpBeginPoint,tmpEndPoint;
    int moveX,moveY;
    checkMoveEndPoint(); //对移动选区进行判断，当移动的选区超出边界，则停止移动
    moveX = endMovePoint.x() - startMovePoint.x();
    moveY = endMovePoint.y() - startMovePoint.y();
    tmpBeginPoint.setX(startPoint.x() + moveX);
    tmpBeginPoint.setY(startPoint.y() + moveY);
    tmpEndPoint.setX(endPoint.x() + moveX);
    tmpEndPoint.setY(endPoint.y() + moveY);

    result = getShotRect(tmpBeginPoint, tmpEndPoint);
    qDebug() << result;
    return result;
}

void Widget::checkMoveEndPoint()
{
    int x, y;
    QRect rect = QRect(startPoint, endPoint);
    QPoint bottomright = rect.bottomRight();
    x = endMovePoint.x() - startMovePoint.x();
    y = endMovePoint.y() - startMovePoint.y();

    if (x+rect.x() < 0)
    {
        qDebug() << "22  " << startMovePoint;
        endMovePoint.setX(qAbs(rect.x() - startMovePoint.x()));
    }


}

void Widget::mousePressEvent(QMouseEvent *event)
{
#if 0
    if (event->button() == Qt::LeftButton)
    {
        is_drawing = true;
        startPoint = event->pos();
        status = 1;
    }
#endif

    if (event->button() == Qt::LeftButton && currentShotStatus == initshot)
    {
        currentShotStatus = beginshot;
        startPoint = event->pos();
    }

    if (event->button() == Qt::LeftButton && is_shotRect(event->pos())
            && controlStatus == left)
    {
        qDebug() << "2";
        currentShotStatus = beginshot;
        startMovePoint = event->pos();
        qDebug() << "11  " << startMovePoint;
    }
}

void Widget::mouseMoveEvent(QMouseEvent *event)
{
#if 0
    QString pos = QString("%1 %2").arg(event->pos().x()).arg(event->pos().y());
    QToolTip::showText(event->globalPos(), pos, this);
//    QImage image = tempPix.toImage();
//    qDebug() << QColor(image.pixel(1, 0));
//    qDebug() << image.format();

    if (is_drawing)
    {
        tempPix = fullScreenPix;
        endPoint = event->pos();
        paint(tempPix);
    }
#endif

    QString pos = QString("%1 %2").arg(event->pos().x()).arg(event->pos().y());
    QToolTip::showText(event->globalPos(), pos, this);
    if (currentShotStatus == beginshot)
    {
        endPoint = event->pos();
        update();
    }

    if (currentShotStatus == beginmove)
    {
        endMovePoint = event->pos();
        update();
    }

    if(currentShotStatus == beginmove || currentShotStatus == finishshot)
    {
        setCursor(Qt::SizeAllCursor);
        //setCursor(Qt::OpenHandCursor);
    }

    setMouseTracking(true);
}

void Widget::mouseReleaseEvent(QMouseEvent *event)
{
#if 0
    if (event->button() == Qt::LeftButton)
    {
        is_drawing = false;
        endPoint = event->pos();
        //shotRect = QRect(startPoint, endPoint);
        paint(tempPix);
        //savePixMap();
    }
#endif

    if (event->button() == Qt::LeftButton && currentShotStatus == beginshot)
    {
        currentShotStatus = finishshot;
        endPoint = event->pos();
        update();
    }

    if (event->button() == Qt::LeftButton && currentShotStatus == begincontrol)
    {
        currentShotStatus = finishcontrol;
        endMovePoint = event->pos();
        update();
    }

//    if(form)
//     {

//        form->move(event->pos());
//        form->show();
//        form->activateWindow();
//     }
}

void Widget::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape)
    {
        qApp->exit(0);
    }
}

void Widget::showEvent(QShowEvent *event)
{
    showFullScreen();
    setCursor(Qt::CrossCursor);
}

void Widget::paintEvent(QPaintEvent *event)
{
#if 0
//    QPainter painter(this);
//    painter.drawPixmap(0, 0, tempPix);
//    QWidget::paintEvent(event);

    QColor shadowColor;
    QPainter painter;
    shadowColor= QColor(0,0,0,100); //阴影颜色设置
    painter.begin(this); //进行重绘
    painter.setPen(QPen(Qt::blue, 1, Qt::SolidLine, Qt::FlatCap));//设置画笔
    painter.drawPixmap(0, 0, tempPix);

    if (status == 0)
    {
        painter.drawRect(QRect(startPoint, endPoint));
        painter.end();
    }

    if (status == 1)
    {
        painter.fillRect(0, 0, QApplication::desktop()->width(), QApplication::desktop()->height()
                           , QBrush(shadowColor));
        painter.drawRect(QRect(startPoint, endPoint));
        //painter.setCompositionMode( QPainter::CompositionMode_Clear);
        painter.fillRect(QRect(startPoint, endPoint), QBrush(QColor(0, 0, 0, 1)));
        painter.fillRect(QRect(startPoint.x()-2, endPoint.y()-2, 5, 5), QColor(0, 0, 255));
        painter.end();
    }

    //painter.setPen(QPen(Qt::blue, 1, Qt::SolidLine, Qt::FlatCap));//设置画笔
    //painter.drawPixmap(screenx,screeny,loadPixmap); //将背景图片画到窗体上
    //painter.drawPixmap(0, 0, tempPix);
    //paintRer.fillRect(QRect(startPoint, endPoint), QBrush(shadowColor)); //画影罩效果
//    painter.fillRect(0, 0, QApplication::desktop()->width(), QApplication::desktop()->height()
//                     , QBrush(shadowColor));
    QWidget::paintEvent(event);
#endif

    QColor shadowColor;
    shadowColor= QColor(0,0,0,100); //阴影颜色设置
    painter.begin(this); //进行重绘
    painter.setPen(QPen(Qt::blue, 2, Qt::SolidLine, Qt::FlatCap));//设置画笔
    painter.drawPixmap(0, 0, pix);

    switch (currentShotStatus)
    {
    case initshot:
        break;
    case beginshot:
    case finishshot:
        //qDebug() << "1";
        painter.fillRect(0, 0, QApplication::desktop()->width(), QApplication::desktop()->height()
                       , QBrush(shadowColor));
        shotRect = getShotRect(startPoint, endPoint);
        drawShotPixmap();
        break;
    case beginmove:
    case finishmove:
        shotRect = getMoveShotRect();
        drawShotPixmap();
    }

    painter.end();

//    if (currentShotStatus == finishmove)
//    {
//        startPoint = shotRect.topLeft();
//        endPoint = shotRect.bottomRight();

//        startMovePoint = QPoint(0,0);
//        endMovePoint = QPoint(0,0);
//    }
}

void Widget::paint(QPixmap &pix)
{
    QPainter painter(&pix);
    painter.drawRect(QRect(startPoint, endPoint));
    update();
}

void Widget::savePixmap()
{
//    QWidget::accept();
//    QPixmap shotPix = QPixmap::grabWidget(this, shotRect);
//    ScreenShotSave *savePix = new ScreenShotSave(shotPix);
//    savePix->exec();
//    delete savePix;
//    savePix = NULL;
}
