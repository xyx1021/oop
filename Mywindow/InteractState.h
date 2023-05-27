#pragma once
#include<qpoint.h>
#include<QGraphicsSceneMouseEvent>
class InteractState
{
public:
    virtual ~InteractState() {}
    virtual void createShape(QPointF position) = 0;
    virtual void adjustShape(QPointF position) = 0;
    virtual void translateShape(QPointF delta) = 0;
    virtual void rotateShape(double angle) = 0;
    virtual void scaleShape(double factor) = 0;
    virtual void deleteShape() = 0;
    virtual void selectShape(QPointF position) = 0;
    virtual void mousePressEvent(QGraphicsSceneMouseEvent* event) = 0;
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent* event) = 0;
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) = 0;
};

