#pragma once
#include"InteractState.h"
#include<QPen>
#include<QColordialog>
#include<QGraphicsScene>

class CreateState :public InteractState {
public:
    CreateState(QGraphicsView* view, QGraphicsScene* scene)
        : view_(view)
        , scene_(scene)
        , shape_(nullptr)
        , pen_(Qt::black)
        , colorDialog_(nullptr) {}
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override {
        if (event->button() == Qt::LeftButton) {
            startPoint = event->scenePos();
        }
    }
    void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override {
        if (event->buttons() & Qt::LeftButton && !shape_) {
            auto endPoint = event->scenePos();
        }
    }
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) {
        if (event->button() == Qt::LeftButton && shape_) {
            auto state = new EditState(view_, scene_, shape_);
            scene_->setSceneState(state);
            shape_ = nullptr;
        }
    }

protected:
    QGraphicsView* view_;
    QGraphicsScene* scene_;
    Shape* shape_;
    QPointF startPoint;
    QColorDialog* colorDialog_;
    QPen pen_;
};




// 创建状态类
class CreateState : public InteractState {
public:
    CreateState(QGraphicsView* view, QGraphicsScene* scene)
        : view_(view)
        , scene_(scene)
        , pen_(Qt::black)
        , colorDialog_(nullptr) {}

    void mousePressEvent(QGraphicsSceneMouseEvent* event) override {
        Q_UNUSED(event);
    }

    void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override {
        Q_UNUSED(event);
    }

    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override {
        Q_UNUSED(event);
    }

    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option,
        QWidget* widget) override {
        Q_UNUSED(painter);
        Q_UNUSED(option);
        Q_UNUSED(widget);
    }

    void setStyle(const Qt::PenStyle& style) override {
        pen_.setStyle(style);
    }

    void setColor(const QColor& color) override {
        Q_UNUSED(color);
    }

protected:
    QGraphicsView* view_;
    QGraphicsScene* scene_;
    Shape* shape_;
    QColorDialog* colorDialog_;
    QPen pen_;
};

// 创建矩形状态类
class RectCreateState : public CreateState {
public:
    RectCreateState(QGraphicsView* view, QGraphicsScene* scene)
        : CreateState(view, scene)
        , startPoint_(0, 0) {}

    void mousePressEvent(QGraphicsSceneMouseEvent* event) override {
        startPoint_ = event->scenePos();
        shape_ = new Rectangle(QRectF(startPoint_, QSizeF(0, 0)), Qt::white, Qt::SolidLine);
        shape_->setPen(pen_);
        scene_->addItem(static_cast<QGraphicsItem*>(shape_));
    }

    void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override {
        QRectF rect(startPoint_, event->scenePos());
        shape_->setRect(rect.normalized());
    }

    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override {
        Q_UNUSED(event);
        scene_->removeItem(static_cast<QGraphicsItem*>(shape_));
        delete shape_;
        shape_ = nullptr;
    }

    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option,
        QWidget* widget) override {
        Q_UNUSED(painter);
        Q_UNUSED(option);
        Q_UNUSED



    };


class TranslateState :public InteractState {
public:
    TranslateState(QGraphicsView* view, QGraphicsScene* scene)
        : view_(view)
        , scene_(scene)
        , shape_(nullptr)
        , startPoint()
        , pen_(Qt::black) {}






protected:
    QGraphicsView* view_;
    QGraphicsScene* scene_;
    Shape* shape_;
    QPointF startPoint;
    QColorDialog* colorDialog_;
    QPen pen_;
};