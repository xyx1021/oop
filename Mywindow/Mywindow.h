#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_Mywindow.h"
#include<QGraphicsView>
#include <QMouseEvent>
#include<QVBoxLayout>
#include<QGraphicsRectItem>
class Mywindow : public QMainWindow
{
    Q_OBJECT

public:
    Mywindow(QWidget *parent = nullptr);
    ~Mywindow();



protected:
    virtual void mousePressEvent(QMouseEvent* event);
    virtual void mouseMoveEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
    virtual void wheelEvent(QWheelEvent* event);
    virtual void mouseDoubleClickEvent(QMouseEvent* event);


private:
    Ui::MywindowClass ui;
    QGraphicsView* m_view;
    qreal m_scaleFactor;
    QPointF m_lastPos;
    bool isDragging;
};
