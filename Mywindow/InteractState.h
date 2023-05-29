#pragma once
#include<qpoint.h>
#include<QGraphicsSceneMouseEvent>
#include<QStyleOptionGraphicsItem>
class InteractState
{
public:
    virtual ~InteractState() {}
    virtual void mousePressEvent(QGraphicsSceneMouseEvent* event) = 0;
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent* event) = 0;
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) = 0;
    virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option,
        QWidget* widget) = 0;
    virtual void setStyle(const Qt::PenStyle&) = 0;
    virtual void setColor(const QColor&) = 0;
};

