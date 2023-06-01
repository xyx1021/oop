#pragma once
#include"InteractState.h"
#include<QPen>
#include<QColordialog>
#include<QGraphicsScene>
// CreateState class inheriting from InteractState, an abstract class for creating different geometric shapes
class CreateState : public InteractState {
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
// CreatePointState class inheriting from CreateState, for creating points
class CreatePointState : public CreateState {
public:
    CreatePointState(QGraphicsView* view, QGraphicsScene* scene)
        : CreateState(view, scene) {}

    void mousePressEvent(QGraphicsSceneMouseEvent* event) override {
        if (event->button() == Qt::LeftButton) {
            startPoint = event->scenePos();
            shape_ = new Point(startPoint, pen_);
            scene_->addItem(static_cast<QGraphicsItem*>(shape_));
        }
    }

    void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override {
        // No need to implement this for point creation
    }

    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override {
        if (event->button() == Qt::LeftButton && shape_) {
            auto state = new EditState(view_, scene_, shape_);
            scene_->setSceneState(state);
            shape_ = nullptr;
        }
    }
};
// CreateLineState class inheriting from CreateState, for creating lines
class CreateLineState : public CreateState {
public:
    CreateLineState(QGraphicsView* view, QGraphicsScene* scene)
        : CreateState(view, scene) {}

    void mousePressEvent(QGraphicsSceneMouseEvent* event) override {
        if (event->button() == Qt::LeftButton) {
            startPoint = event->scenePos();
            shape_ = new Line(startPoint, startPoint, pen_);
            scene_->addItem(static_cast<QGraphicsItem*>(shape_));
        }
    }

    void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override {
        if (event->buttons() & Qt::LeftButton && shape_) {
            auto endPoint = event->scenePos();
            static_cast<Line*>(shape_)->setLine(QLineF(startPoint, endPoint));
        }
    }

    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override {
        if (event->button() == Qt::LeftButton && shape_) {
            auto state = new EditState(view_, scene_, shape_);
            scene_->setSceneState(state);   
            shape_ = nullptr;
        }
    }
};          
// CreateRectangleState class inheriting from CreateState, for creating rectangles
class CreateRectangleState : public CreateState {
public:
    CreateRectangleState(QGraphicsView* view, QGraphicsScene* scene)
        : CreateState(view, scene) {}

    void mousePressEvent(QGraphicsSceneMouseEvent* event) override {
        if (event->button() == Qt::LeftButton) {
            startPoint = event->scenePos();
            shape_ = new Rectangle(QRectF(startPoint, QSizeF(0, 0)), pen_);
            scene_->addItem(static_cast<QGraphicsItem*>(shape_));
        }
    }

    void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override {
        if (event->buttons() & Qt::LeftButton && shape_) {
            auto endPoint = event->scenePos();
            static_cast<Rectangle*>(shape_)->setRect(QRectF(startPoint, endPoint).normalized());
        }
    }

    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override {
        if (event->button() == Qt::LeftButton && shape_) {
            auto state = new EditState(view_, scene_, shape_);
            scene_->setSceneState(state);
            shape_ = nullptr;
        }
    }
};

// CreateCircleState class inheriting from CreateState, for creating circles
class CreateCircleState : public CreateState {
public:
    CreateCircleState(QGraphicsView* view, QGraphicsScene* scene)
        : CreateState(view, scene) {}

    void mousePressEvent(QGraphicsSceneMouseEvent* event) override {
        if (event->button() == Qt::LeftButton) {
            startPoint = event->scenePos();
            shape_ = new Circle(QRectF(startPoint, QSizeF(0, 0)), pen_);
            scene_->addItem(static_cast<QGraphicsItem*>(shape_));
        }
    }

    void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override {
        if (event->buttons() & Qt::LeftButton && shape_) {
            auto endPoint = event->scenePos();
            qreal radius = QLineF(startPoint, endPoint).length();
            QRectF circleRect(startPoint - QPointF(radius, radius), QSizeF(radius * 2, radius * 2));
            static_cast<Circle*>(shape_)->setRect(circleRect);
        }
    }

    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override {
        if (event->button() == Qt::LeftButton && shape_) {
            auto state = new EditState(view_, scene_, shape_);
            scene_->setSceneState(state);
            shape_ = nullptr;
        }
    }
};
 
// CreatePolygonState class inheriting from CreateState, for creating polygons
class CreatePolygonState : public CreateState {
public:
    CreatePolygonState(QGraphicsView* view, QGraphicsScene* scene)
        : CreateState(view, scene) {}

    void mousePressEvent(QGraphicsSceneMouseEvent* event) override {
        if (event->button() == Qt::LeftButton) {
            if (!shape_) {
                startPoint = event->scenePos();
                shape_ = new Polygon(QPolygonF() << startPoint, pen_);
                scene_->addItem(static_cast<QGraphicsItem*>(shape_));
            }
            else {
                auto polygon = static_cast<Polygon*>(shape_);
                polygon->addPoint(event->scenePos());
            }
        }
    }

    void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override {
        if (event->buttons() & Qt::LeftButton && shape_) {
            auto polygon = static_cast<Polygon*>(shape_);
            polygon->setLastPoint(event->scenePos());
        }
    }

    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override {
        if (event->button() == Qt::LeftButton && shape_) {
            auto state = new EditState(view_, scene_, shape_);
            scene_->setSceneState(state);
            shape_ = nullptr;
        }
    }
};

// CreateSectorState class inheriting from CreateState, for creating sectors
class CreateSectorState : public CreateState {
public:
    CreateSectorState(QGraphicsView* view, QGraphicsScene* scene)
        : CreateState(view, scene) {}

    void mousePressEvent(QGraphicsSceneMouseEvent* event) override {
        if (event->button() == Qt::LeftButton) {
            startPoint = event->scenePos();
            shape_ = new Sector(QRectF(startPoint, QSizeF(0, 0)), 0, 0, pen_);
            scene_->addItem(static_cast<QGraphicsItem*>(shape_));
        }
    }

    void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override {
        if (event->buttons() & Qt::LeftButton && shape_) {
            auto endPoint = event->scenePos();
            qreal radius = QLineF(startPoint, endPoint).length();
            QRectF sectorRect(startPoint - QPointF(radius, radius), QSizeF(radius * 2, radius * 2));
            qreal startAngle = QLineF(sectorRect.center(), startPoint).angle();
            qreal spanAngle = QLineF(sectorRect.center(), endPoint).angle() - startAngle;
            static_cast<Sector*>(shape_)->setRect(sectorRect);
            static_cast<Sector*>(shape_)->setStartAngle(startAngle);
            static_cast<Sector*>(shape_)->setSpanAngle(spanAngle);
        }
    }

    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override {
        if (event->button() == Qt::LeftButton && shape_) {
            auto state = new EditState(view_, scene_, shape_);
            scene_->setSceneState(state);
            shape_ = nullptr;
        }
    }
};


 
class TranslateState :public InteractState {
public:
    TranslateState(QGraphicsView* view, QGraphicsScene* scene)
        : view_(view)
        , scene_(scene)
        , shape_(nullptr)
        , startPoint()
        , pen_(Qt::black) {}

    void mousePressEvent(QGraphicsSceneMouseEvent* event) override {
        if (event->button() == Qt::LeftButton) {
            auto item = scene_->itemAt(event->scenePos(), QTransform());
            if (item && item->type() == Shape::Type) {
                shape_ = static_cast<Shape*>(item);
                startPoint = event->scenePos();
            }
        }
    }

    void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override {
        if (event->buttons() & Qt::LeftButton && shape_) {
            auto delta = event->scenePos() - startPoint;
            shape_->moveBy(delta.x(), delta.y());
            startPoint = event->scenePos();
        }
    }

    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override {
        Q_UNUSED(event);
        shape_ = nullptr;
    }

protected:
    QGraphicsView* view_;
    QGraphicsScene* scene_;
    Shape* shape_;
    QPointF startPoint;
    QColorDialog* colorDialog_;
    QPen pen_;
};

class TranslatePointState : public TranslateState {
public:
    TranslatePointState(QGraphicsView* view, QGraphicsScene* scene)
        : TranslateState(view, scene) {}

    void mousePressEvent(QGraphicsSceneMouseEvent* event) override {
        if (event->button() == Qt::LeftButton) {
            auto item = scene_->itemAt(event->scenePos(), QTransform());
            if (item && item->type() == Shape::Type && item->shape() == Shape::Point) {
                shape_ = static_cast<Shape*>(item);
                startPoint = event->scenePos();
            }
        }
    }
};
 
class TranslateLineState :public TranslateState {
public:
    TranslateLineState(QGraphicsView* view, QGraphicsScene* scene)
        : TranslateState(view, scene) {}

    void mousePressEvent(QGraphicsSceneMouseEvent* event) override {
        if (event->button() == Qt::LeftButton) {
            auto item = scene_->itemAt(event->scenePos(), QTransform());
            if (item && item->type() == Shape::Type && item->shape() == Shape::Line) {
                shape_ = static_cast<Shape*>(item);
                startPoint = event->scenePos();
            }
        }
    }
};
 
class TranslateRectState :public TranslateState {
public:
    TranslateRectState(QGraphicsView* view, QGraphicsScene* scene)
        : TranslateState(view, scene) {}

    void mousePressEvent(QGraphicsSceneMouseEvent* event) override {
        if (event->button() == Qt::LeftButton) {
            auto item = scene_->itemAt(event->scenePos(), QTransform());
            if (item && item->type() == Shape::Type && item->shape() == Shape::Rect) {
                shape_ = static_cast<Shape*>(item);
                startPoint = event->scenePos();
            }
        }
    }
};

class TranslateCircleState :public TranslateState {
public:
    TranslateCircleState(QGraphicsView* view, QGraphicsScene* scene)
        : TranslateState(view, scene) {}

    void mousePressEvent(QGraphicsSceneMouseEvent* event) override {
        if (event->button() == Qt::LeftButton) {
            auto item = scene_->itemAt(event->scenePos(), QTransform());
            if (item && item->type() == Shape::Type && item->shape() == Shape::Circle) {
                shape_ = static_cast<Shape*>(item);
                startPoint = event->scenePos();
            }
        }

    }
};
 
class TranslatePolygonState :public TranslateState {
public:
    TranslatePolygonState(QGraphicsView* view, QGraphicsScene* scene)
        : TranslateState(view, scene) {}

    void mousePressEvent(QGraphicsSceneMouseEvent* event) override {
        if (event->button() == Qt::LeftButton) {
            auto item = scene_->itemAt(event->scenePos(), QTransform());
            if (item && item->type() == Shape::Type && item->shape() == Shape::Polygon) {
                shape_ = static_cast<Shape*>(item);
                startPoint = event->scenePos();
            }
        }
    }

};
class TranslateSectorState :public TranslateState{
    
public:
    TranslateSectorState(QGraphicsView* view, QGraphicsScene* scene)
        : TranslateState(view, scene) {}

    void mousePressEvent(QGraphicsSceneMouseEvent* event) override {
        if (event->button() == Qt::LeftButton) {
            auto item = scene_->itemAt(event->scenePos(), QTransform());
            if (item && item->type() == Shape::Type && item->shape() == Shape::Sector) {
                shape_ = static_cast<Shape*>(item);
                startPoint = event->scenePos();
            }
        }
    }
};

 
class RotateState : public InteractState {
public:
    RotateState(QGraphicsView* view, QGraphicsScene* scene)
        : view_(view)
        , scene_(scene)
        , shape_(nullptr)
        , startPoint()
        , pen_(Qt::black) {}

    void mousePressEvent(QGraphicsSceneMouseEvent* event) override {
        if (event->button() == Qt::LeftButton) {
            auto item = scene_->itemAt(event->scenePos(), QTransform());
            if (item && item->type() == Shape::Type) {
                shape_ = static_cast<Shape*>(item);
                startPoint = event->scenePos();
            }
        }
    }

    void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override {
        if (event->buttons() & Qt::LeftButton && shape_) {
            auto delta = event->scenePos() - startPoint;
            qreal angle = QLineF(shape_->pos(), event->scenePos()).angle() - QLineF(shape_->pos(), startPoint).angle();
            shape_->setRotation(shape_->rotation() + angle);
            startPoint = event->scenePos();
        }
    }

    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override {
        Q_UNUSED(event);
        shape_ = nullptr;
    }
    
protected:
    QGraphicsView* view_;
    QGraphicsScene* scene_;
    Shape* shape_;
    QPointF startPoint;
    QColorDialog* colorDialog_;
    QPen pen_;
};
 
class RotatePointState : public RotateState {
public:
    RotatePointState(QGraphicsView* view, QGraphicsScene* scene)
        : RotateState(view, scene) {}

    void mousePressEvent(QGraphicsSceneMouseEvent* event) override {
        if (event->button() == Qt::LeftButton) {
            auto item = scene_->itemAt(event->scenePos(), QTransform());
            if (item && item->type() == Shape::Type && item->shape() == Shape::Point) {
                shape_ = static_cast<Shape*>(item);
                startPoint = event->scenePos();
            }
        }
    }
};

 
class RotateLineState : public RotateState {
public:
    RotateLineState(QGraphicsView* view, QGraphicsScene* scene)
        : RotateState(view, scene) {}

    void mousePressEvent(QGraphicsSceneMouseEvent* event) override {
        if (event->button() == Qt::LeftButton) {
            auto item = scene_->itemAt(event->scenePos(), QTransform());
            if (item && item->type() == Shape::Type && item->shape() == Shape::Line) {
                shape_ = static_cast<Shape*>(item);
                startPoint = event->scenePos();
            }
        }
    }
};
class RotateRectState :public RotateState {
    RotateRectState(QGraphicsView* view, QGraphicsScene* scene)
        : RotateState(view, scene) {}

    void mousePressEvent(QGraphicsSceneMouseEvent* event) override {
        if (event->button() == Qt::LeftButton) {
            auto item = scene_->itemAt(event->scenePos(), QTransform());
            if (item && item->type() == Shape::Type && item->shape() == Shape::Rect) {
                shape_ = static_cast<Shape*>(item);
                startPoint = event->scenePos();
            }
        }
    }

};



    








//
//// ����״̬��
//class CreateState : public InteractState {
//public:
//    CreateState(QGraphicsView* view, QGraphicsScene* scene)
//        : view_(view)
//        , scene_(scene)
//        , pen_(Qt::black)
//        , colorDialog_(nullptr) {}
//
//    void mousePressEvent(QGraphicsSceneMouseEvent* event) override {
//        Q_UNUSED(event);
//    }
//
//    void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override {
//        Q_UNUSED(event);
//    }
//
//    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override {
//        Q_UNUSED(event);
//    }
//
//    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option,
//        QWidget* widget) override {
//        Q_UNUSED(painter);
//        Q_UNUSED(option);
//        Q_UNUSED(widget);
//    }
//
//    void setStyle(const Qt::PenStyle& style) override {
//        pen_.setStyle(style);
//    }
//
//    void setColor(const QColor& color) override {
//        Q_UNUSED(color);
//    }
//
//protected:
//    QGraphicsView* view_;
//    QGraphicsScene* scene_;
//    Shape* shape_;
//    QColorDialog* colorDialog_;
//    QPen pen_;
//};
