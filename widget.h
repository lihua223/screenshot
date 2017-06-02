#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QMouseEvent>
#include <QToolTip>
#include <QDebug>
#include <QPixmap>
#include <QDesktopWidget>
#include <QPainter>
#include <QPaintEvent>

namespace Ui {
class Widget;
}

class Form;
class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

    enum shotStatus{initshot, beginshot, finishshot, beginmove, finishmove
                   , begincontrol, finishcontrol};

    enum controlPoint{left, topCenter, right, leftCenter, rightCenter, leftBottom
                     , bottomCenter, rightBottom};

    void init();

    QRect getShotRect(const QPoint &startPoint, const QPoint &endPoint);

    void drawShotPixmap();

    void drawControlSpot(const QRect &rect);

public:
    bool is_shotRect(const QPoint &point);

    QRect getMoveShotRect();

    void checkMoveEndPoint();

protected:
    void mousePressEvent(QMouseEvent *event);

    void mouseMoveEvent(QMouseEvent *event);

    void mouseReleaseEvent(QMouseEvent *event);

    void keyPressEvent(QKeyEvent *event);

    void showEvent(QShowEvent *event);

    void paintEvent(QPaintEvent *event);

    void paint(QPixmap &pix);

    void savePixmap();

private:
    Ui::Widget *ui;
    Form *form;
    bool is_drawing;
    QPixmap fullScreenPix;
    QPixmap tempPix;
    QPixmap pix;
    QPoint startPoint, startMovePoint;
    QPoint endPoint, endMovePoint;
    QRect shotRect;
    shotStatus currentShotStatus;
    controlPoint controlStatus;
    QPainter painter;

private:
    QRect spotLeft;
    QRect spotRight;
    QRect spotTopCenter;
    QRect spotLeftCenter;
    QRect spotRightCenter;
    QRect spotLeftBottom;
    QRect spotRightBottom;
    QRect spotBottomCenter;
};

#endif // WIDGET_H
