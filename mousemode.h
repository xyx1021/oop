#pragma once
#include <QMouseEvent>
#include <QCursor>
#include <QApplication>
#include <limits>

#include "getview.h"

extern QToolBar* toolbar, * addbar, * editbar, * stylebar;
extern Layer* currentLayer;
class MouseMode
{
public:
    virtual ~MouseMode() {}
    virtual void handleMouseMoveEvent(QMouseEvent* event, Point& windowTranslateBegin, Point& windowTranslateEnd, Point& windowTranslateAccumalation,double windowScaleTime) = 0;
    virtual bool handleMousePress1Event(QMouseEvent* event, Point& windowTranslateBegin, Point& windowTranslateAccumalation, double windowScaleTime) = 0;
    virtual bool handleMousePress2Event(QMouseEvent* event, Point& windowTranslateBegin, Point& windowTranslateAccumalation, double windowScaleTime) = 0;
    virtual void handleInterupt() = 0;
    virtual void handleWheelEvent(bool e,double& windowScaleTime) = 0;
    virtual void displaySelectBox(QPainter& painter) = 0;
};
 

class BrowseMode : public MouseMode
{
public:

    void handleMouseMoveEvent(QMouseEvent* event, Point& windowTranslateBegin, Point& windowTranslateEnd, Point& windowTranslateAccumalation, double windowScaleTime)
    {
        QApplication::setOverrideCursor(Qt::ClosedHandCursor);

        windowTranslateEnd.x = event->x();
        windowTranslateEnd.y = event->y();
        windowTranslateAccumalation.x += (windowTranslateEnd.x - windowTranslateBegin.x)/windowScaleTime;
        windowTranslateAccumalation.y += (windowTranslateEnd.y - windowTranslateBegin.y)/windowScaleTime;
        windowTranslateBegin.x = event->x();
        windowTranslateBegin.y = event->y();
	}


    bool handleMousePress1Event(QMouseEvent* event, Point& windowTranslateBegin, Point& windowTranslateAccumalation, double windowScaleTime) override
    {
        QApplication::setOverrideCursor(Qt::ClosedHandCursor);
        windowTranslateBegin.x = event->x();
        windowTranslateBegin.y = event->y();
        return true;
    }
    bool handleMousePress2Event(QMouseEvent* event, Point& windowTranslateBegin, Point& windowTranslateAccumalation, double windowScaleTime) override
    {
        QApplication::setOverrideCursor(Qt::OpenHandCursor);
        return false;
    }
    void handleInterupt()
    {

    }
    void displaySelectBox(QPainter& painter)
    {

    }
    void handleWheelEvent(bool e, double& windowScaleTime)
    {
        if (e)windowScaleTime += 0.1;
		else windowScaleTime -= 0.1;
    }
};


class ChooseMode : public MouseMode
{
public:

    void handleMouseMoveEvent(QMouseEvent* event, Point& windowTranslateBegin, Point& windowTranslateEnd, Point& windowTranslateAccumalation,double windowScaleTime)
    {
        currentLayer->boxSelecting((event->x()) / windowScaleTime - windowTranslateAccumalation.x, event->y() / windowScaleTime - windowTranslateAccumalation.y);
    }

    bool handleMousePress1Event(QMouseEvent* event, Point& windowTranslateBegin, Point& windowTranslateAccumalation, double windowScaleTime) override
    {
        currentLayer->boxSelectBegin((event->x()) / windowScaleTime - windowTranslateAccumalation.x, event->y() / windowScaleTime - windowTranslateAccumalation.y);
        return true;
    }
    bool handleMousePress2Event(QMouseEvent* event, Point& windowTranslateBegin, Point& windowTranslateAccumalation, double windowScaleTime) override
    {
        currentLayer->boxSelectEnd((event->x()) / windowScaleTime - windowTranslateAccumalation.x, event->y() / windowScaleTime - windowTranslateAccumalation.y);
        style();
        return false;
    }
    void handleInterupt()
    {

    }
    void displaySelectBox(QPainter& painter)
    {
        currentLayer->boxDisplay(painter);
    }
    static void style()
    {
        bool bordercolor = true;        bool fillcolor = true;
        if (!selectSet.isEmpty())
        {
            for (auto p : selectSet)
            {
                if (p->get_borderColor().name() != selectSet[0]->get_borderColor().name())
                {
                    bordercolor = false;
                }
                if (p->get_fillColor().name() != selectSet[0]->get_fillColor().name())
                {
                    fillcolor = false;
                }
            }

            if (bordercolor)
            {
                QPixmap pixmap(32, 32);

                QPainter painter(&pixmap);
                painter.setBrush(selectSet[0]->get_borderColor());
                painter.drawRect(0.0, 0.0, 32.0, 32.0);
                painter.eraseRect(6.0, 6.0, 20.0, 20.0);

                stylebar->actions().at(0)->setIcon(pixmap);
            }
            else
                stylebar->actions().at(0)->setIcon(QIcon());

            if (fillcolor)
            {
                QPixmap pixmap(32, 32);

                QPainter painter(&pixmap);
                painter.setBrush(selectSet[0]->get_fillColor());
                painter.drawRect(QRectF(5.0, 5.0, 20.0, 20.0));

                stylebar->actions().at(2)->setIcon(pixmap);
            }
            else
                stylebar->actions().at(2)->setIcon(QIcon());
        }
    }
    void handleWheelEvent(bool e, double& windowScaleTime)
    {
    }
};

class DrawMode : public MouseMode
{
public:

    void handleMouseMoveEvent(QMouseEvent* event, Point& windowTranslateBegin, Point& windowTranslateEnd, Point& windowTranslateAccumalation, double windowScaleTime)
    {

    }



    bool handleMousePress1Event(QMouseEvent* event, Point& windowTranslateBegin, Point& windowTranslateAccumalation, double windowScaleTime) override
    {
        return false;
    }
    bool handleMousePress2Event(QMouseEvent* event, Point& windowTranslateBegin, Point& windowTranslateAccumalation, double windowScaleTime) override
    {
        return false;
    }
    void handleInterupt()
    {

    }
    void displaySelectBox(QPainter& painter)
    {

    }
    void handleWheelEvent(bool e, double& windowScaleTime)
    {

    }
};

class DrawPointMode : public DrawMode
{
public:

    void handleMouseMoveEvent(QMouseEvent* event, Point& windowTranslateBegin, Point& windowTranslateEnd, Point& windowTranslateAccumalation, double windowScaleTime)
    {

    }



    bool handleMousePress1Event(QMouseEvent* event, Point& windowTranslateBegin, Point& windowTranslateAccumalation, double windowScaleTime) override
    {
        QApplication::setOverrideCursor(Qt::CrossCursor);
        QVector<Point*> temp;
        PointView* add = new PointView(temp);
        currentLayer->getVector().prepend(add);
        currentLayer->getVector().first()->createBegin((event->x()) / windowScaleTime - windowTranslateAccumalation.x, event->y() / windowScaleTime - windowTranslateAccumalation.y);
        return false;
    }
    bool handleMousePress2Event(QMouseEvent* event, Point& windowTranslateBegin, Point& windowTranslateAccumalation, double windowScaleTime) override
    {       
        return handleMousePress1Event(event, windowTranslateBegin, windowTranslateAccumalation, windowScaleTime);
    }
    void handleInterupt()
    {

    }
};

class DrawLineMode : public DrawMode
{
public:


    void handleMouseMoveEvent(QMouseEvent* event, Point& windowTranslateBegin, Point& windowTranslateEnd, Point& windowTranslateAccumalation, double windowScaleTime)
    {
        currentLayer->getVector().first()->creating((event->x()) / windowScaleTime - windowTranslateAccumalation.x, event->y() / windowScaleTime - windowTranslateAccumalation.y);
    }

    bool handleMousePress1Event(QMouseEvent* event, Point& windowTranslateBegin, Point& windowTranslateAccumalation, double windowScaleTime) override
    {
        QApplication::setOverrideCursor(Qt::CrossCursor);
        QVector<Point*> temp;
        LineView* add = new LineView(temp);
        currentLayer->getVector().prepend(add);
        currentLayer->getVector().first()->createBegin((event->x()) / windowScaleTime - windowTranslateAccumalation.x, event->y() / windowScaleTime - windowTranslateAccumalation.y);
        LayerList[0]->getVector().prepend(currentLayer->getVector().first());           
        return true;
    }
    bool handleMousePress2Event(QMouseEvent* event, Point& windowTranslateBegin, Point& windowTranslateAccumalation, double windowScaleTime) override
    {
        currentLayer->getVector().first()->createBegin((event->x()) / windowScaleTime - windowTranslateAccumalation.x, event->y() / windowScaleTime - windowTranslateAccumalation.y);
        return false;
    }
    void handleInterupt()
    {
        currentLayer->getVector().first()->get_p().pop_back();
        if (currentLayer->getVector().first()->get_p().count()== 1)
        {
            currentLayer->getVector().pop_front();
        }
    }
};

class DrawPolylineMode : public DrawMode
{
public:


    void handleMouseMoveEvent(QMouseEvent* event, Point& windowTranslateBegin, Point& windowTranslateEnd, Point& windowTranslateAccumalation, double windowScaleTime)
    {
        currentLayer->getVector().first()->creating((event->x()) / windowScaleTime - windowTranslateAccumalation.x, event->y() / windowScaleTime - windowTranslateAccumalation.y);
    }

    bool handleMousePress1Event(QMouseEvent* event, Point& windowTranslateBegin, Point& windowTranslateAccumalation, double windowScaleTime) override
    {
        QApplication::setOverrideCursor(Qt::CrossCursor);
        QVector<Point*> temp;
        LineView* add = new LineView(temp);
        currentLayer->getVector().prepend(add);
        currentLayer->getVector().first()->createBegin((event->x()) / windowScaleTime - windowTranslateAccumalation.x, event->y() / windowScaleTime - windowTranslateAccumalation.y);
        return true;
    }
    bool handleMousePress2Event(QMouseEvent* event, Point& windowTranslateBegin, Point& windowTranslateAccumalation, double windowScaleTime) override
    {
        currentLayer->getVector().first()->createBegin((event->x()) / windowScaleTime - windowTranslateAccumalation.x, event->y() / windowScaleTime - windowTranslateAccumalation.y);
        return true;
    }
    void handleInterupt()
    {
        currentLayer->getVector().first()->get_p().pop_back();
        if (currentLayer->getVector().first()->get_p().count() == 1)
        {
            currentLayer->getVector().pop_front();
        }
    }
};


class DrawRectangleMode : public DrawMode
{
public:

    void handleMouseMoveEvent(QMouseEvent* event, Point& windowTranslateBegin, Point& windowTranslateEnd, Point& windowTranslateAccumalation, double windowScaleTime)
    {
        currentLayer->getVector().first()->creating((event->x()) / windowScaleTime - windowTranslateAccumalation.x, event->y() / windowScaleTime - windowTranslateAccumalation.y);
    }


    bool handleMousePress1Event(QMouseEvent* event, Point& windowTranslateBegin, Point& windowTranslateAccumalation, double windowScaleTime) override
    {
        QApplication::setOverrideCursor(Qt::CrossCursor);
        QVector<Point*> temp;
        RectangleView* add = new RectangleView(temp);
        currentLayer->getVector().prepend(add);
        currentLayer->getVector().first()->createBegin((event->x()) / windowScaleTime - windowTranslateAccumalation.x, event->y() / windowScaleTime - windowTranslateAccumalation.y);
        return true;
    }
    bool handleMousePress2Event(QMouseEvent* event, Point& windowTranslateBegin, Point& windowTranslateAccumalation, double windowScaleTime) override
    {
        currentLayer->getVector().first()->createEnd((event->x()) / windowScaleTime - windowTranslateAccumalation.x, event->y() / windowScaleTime - windowTranslateAccumalation.y);
        return false;
    }
    void handleInterupt()
    {
        currentLayer->getVector().first()->get_p().pop_back();
        if (currentLayer->getVector().first()->get_p().count() == 1)
        {
            currentLayer->getVector().pop_front();
        }
    }
};

class DrawPolygonMode : public DrawMode
{
public:

    void handleMouseMoveEvent(QMouseEvent* event, Point& windowTranslateBegin, Point& windowTranslateEnd, Point& windowTranslateAccumalation, double windowScaleTime)
    {
        currentLayer->getVector().first()->creating((event->x()) / windowScaleTime - windowTranslateAccumalation.x, event->y() / windowScaleTime - windowTranslateAccumalation.y);
    }


    bool handleMousePress1Event(QMouseEvent* event, Point& windowTranslateBegin, Point& windowTranslateAccumalation, double windowScaleTime) override
    {
        QApplication::setOverrideCursor(Qt::CrossCursor);
        QVector<Point*> temp;
        PolygonView* add = new PolygonView(temp);
        currentLayer->getVector().prepend(add);
        currentLayer->getVector().first()->createBegin((event->x()) / windowScaleTime - windowTranslateAccumalation.x, event->y() / windowScaleTime - windowTranslateAccumalation.y);
        return true;
    }
    bool handleMousePress2Event(QMouseEvent* event, Point& windowTranslateBegin, Point& windowTranslateAccumalation, double windowScaleTime) override
    {
        currentLayer->getVector().first()->createBegin((event->x()) / windowScaleTime - windowTranslateAccumalation.x, event->y() / windowScaleTime - windowTranslateAccumalation.y);
        return true;
    }
    void handleInterupt()
    {
        currentLayer->getVector().first()->get_p().pop_back();
        if (currentLayer->getVector().first()->get_p().count() == 1)
        {
            currentLayer->getVector().pop_front();
        }
    }
};

class DrawCircleMode : public DrawMode
{
public:

    void handleMouseMoveEvent(QMouseEvent* event, Point& windowTranslateBegin, Point& windowTranslateEnd, Point& windowTranslateAccumalation, double windowScaleTime)
    {
        currentLayer->getVector().first()->creating((event->x()) / windowScaleTime - windowTranslateAccumalation.x, event->y() / windowScaleTime - windowTranslateAccumalation.y);
    }


    bool handleMousePress1Event(QMouseEvent* event, Point& windowTranslateBegin, Point& windowTranslateAccumalation, double windowScaleTime) override
    {
        QApplication::setOverrideCursor(Qt::CrossCursor);
        QVector<Point*> temp;
        CircleView* add = new CircleView(temp);
        currentLayer->getVector().prepend(add);
        currentLayer->getVector().first()->createBegin((event->x()) / windowScaleTime - windowTranslateAccumalation.x, event->y() / windowScaleTime - windowTranslateAccumalation.y);
        return true;
    }
    bool handleMousePress2Event(QMouseEvent* event, Point& windowTranslateBegin, Point& windowTranslateAccumalation, double windowScaleTime) override
    {
        currentLayer->getVector().first()->createBegin((event->x()) / windowScaleTime - windowTranslateAccumalation.x, event->y() / windowScaleTime - windowTranslateAccumalation.y);
        return false;
    }
    void handleInterupt()
    {
        currentLayer->getVector().first()->get_p().pop_back();
        if (currentLayer->getVector().first()->get_p().count() == 1)
        {
            currentLayer->getVector().pop_front();
        }
    }
};

class DrawSectorMode :public DrawMode {
public:
    void handleMouseMoveEvent(QMouseEvent* event, Point& windowTranslateBegin, Point& windowTranslateEnd, Point& windowTranslateAccumalation, double windowScaleTime) 
    {

      //  currentLayer->getVector().first()->creating((event->x()) / windowScaleTime - windowTranslateAccumalation.x, event->y() / windowScaleTime - windowTranslateAccumalation.y);
    }
    bool handleMousePress1Event(QMouseEvent* event, Point& windowTranslateBegin, Point& windowTranslateAccumalation, double windowScaleTime) override {
        QApplication::setOverrideCursor(Qt::CrossCursor);
        QVector<Point*> temp;
        SectorView* add = new SectorView(temp);
        currentLayer->getVector().prepend(add);
        currentLayer->getVector().first()->createBegin((event->x()) / windowScaleTime - windowTranslateAccumalation.x, event->y() / windowScaleTime - windowTranslateAccumalation.y);
        return true;
    }
    bool handleMousePress2Event(QMouseEvent* event, Point& windowTranslateBegin, Point& windowTranslateAccumalation, double windowScaleTime) override {
        if (currentLayer->getVector()[0]->get_p().count()>=3) return false;
        currentLayer->getVector().first()->createEnd((event->x()) / windowScaleTime - windowTranslateAccumalation.x, event->y() / windowScaleTime - windowTranslateAccumalation.y);
        return true;
    }
    void handleInterupt()
    {
       /* currentLayer->getVector().first()->get_p().pop_back();
        if (currentLayer->getVector().first()->get_p().count() == 1)
        {
            currentLayer->getVector().pop_front();
        }*/
    }
};





class EditMode : public MouseMode
{
public:
    EditMode() { editing = nullptr; }
    void handleMouseMoveEvent(QMouseEvent* event, Point& windowTranslateBegin, Point& windowTranslateEnd, Point& windowTranslateAccumalation, double windowScaleTime)
    {
        if (editing != nullptr)
        {
            editing->x = event->x() / windowScaleTime - windowTranslateAccumalation.x;
            editing->y = event->y() / windowScaleTime - windowTranslateAccumalation.y;
        }
    }


    bool handleMousePress1Event(QMouseEvent* event, Point& windowTranslateBegin, Point& windowTranslateAccumalation, double windowScaleTime) override
    {
        for (auto p : selectSet)
        {
            for (auto q : p->get_p() )
            {
                if(event->x()/windowScaleTime-windowTranslateAccumalation.x - q->x < 3 && event->y()/windowScaleTime-windowTranslateAccumalation.y - q->y < 3)
				{
                    editing = q;
                    break;
				}
            }
        }
        return true;
    }

    bool handleMousePress2Event(QMouseEvent* event, Point& windowTranslateBegin, Point& windowTranslateAccumalation, double windowScaleTime) override
    {        
        handleMouseMoveEvent(event, windowTranslateBegin, windowTranslateBegin, windowTranslateAccumalation, windowScaleTime);
        editing = nullptr;
        return false;
    }

    void handleInterupt()
    {

    }
    void displaySelectBox(QPainter& painter)
    {

    }
    void handleWheelEvent(bool e, double& windowScaleTime)
    {
    }
private:
    Point* editing;
};

class TranslateMode : public MouseMode
{
public:

    void handleMouseMoveEvent(QMouseEvent* event, Point& windowTranslateBegin, Point& windowTranslateEnd, Point& windowTranslateAccumalation, double windowScaleTime)
    {
        for(auto p : selectSet)
		{
			p->translateEnd(event->x(), event->y(), windowScaleTime);
		}        
    }


    bool handleMousePress1Event(QMouseEvent* event, Point& windowTranslateBegin, Point& windowTranslateAccumalation, double windowScaleTime) override
    {
        for (auto p : selectSet)
        {
            p->translateBegin(event->x(), event->y());
        }
        return true;
    }
    bool handleMousePress2Event(QMouseEvent* event, Point& windowTranslateBegin, Point& windowTranslateAccumalation, double windowScaleTime) override
    {
        for (auto p : selectSet)
        {
            p->translateEnd(event->x(), event->y(), windowScaleTime);
        }
        return false;
    }

    void handleInterupt()
    {

    }
    void displaySelectBox(QPainter& painter)
    {

    }
    void handleWheelEvent(bool e, double& windowScaleTime)
    {
    }
};

class RotateMode : public MouseMode
{
public:
    RotateMode() { BBoxSet.xmax = -std::numeric_limits<double>::infinity(); BBoxSet.xmin = std::numeric_limits<double>::infinity(); BBoxSet.ymax = -std::numeric_limits<double>::infinity(); BBoxSet.ymin = std::numeric_limits<double>::infinity(); }
    void handleMouseMoveEvent(QMouseEvent* event, Point& windowTranslateBegin, Point& windowTranslateEnd, Point& windowTranslateAccumalation, double windowScaleTime)
    {
        updateBbox();
        for (int i=0;i<selectSet.size();i++)
        {
            selectSet[i]->rotateEnd(event->x(), event->y(), BBoxSet.center);
        }
    }


    bool handleMousePress1Event(QMouseEvent* event, Point& windowTranslateBegin, Point& windowTranslateAccumalation, double windowScaleTime) override
    {
        updateBbox();
        for (auto p : selectSet)
        {
            p->rotateBegin(event->x(), event->y());
        }
        return true;
    }
    bool handleMousePress2Event(QMouseEvent* event, Point& windowTranslateBegin, Point& windowTranslateAccumalation, double windowScaleTime) override
    {
       /* updateBbox();
        for (auto p : selectSet)
        {
            p->rotateEnd(event->x(), event->y(), BBoxSet.center);
        }*/
        return false;
    }

    void handleInterupt()
    {

    }
    void displaySelectBox(QPainter& painter)
    {

    }
    void updateBbox()
    {        
	    for (auto p : selectSet)
		{
            if (p->get_box().xmax > BBoxSet.xmax)BBoxSet.xmax = p->get_box().xmax;
            if (p->get_box().xmin < BBoxSet.xmin)BBoxSet.xmin = p->get_box().xmin;
            if (p->get_box().ymax > BBoxSet.ymax)BBoxSet.ymax = p->get_box().ymax;
            if (p->get_box().ymin < BBoxSet.ymin)BBoxSet.ymin = p->get_box().ymin;
            BBoxSet.center=Point((BBoxSet.xmax + BBoxSet.xmin) / 2, (BBoxSet.ymax + BBoxSet.ymin) / 2);
		}
    }
    void handleWheelEvent(bool e, double& windowScaleTime)
    {

    }

private:
    BBox BBoxSet;
};


class ScaleMode : public MouseMode
{
public:
    ScaleMode() { BBoxSet.xmax = -std::numeric_limits<double>::infinity(); BBoxSet.xmin = std::numeric_limits<double>::infinity(); BBoxSet.ymax = -std::numeric_limits<double>::infinity(); BBoxSet.ymin = std::numeric_limits<double>::infinity(); }
    void handleMouseMoveEvent(QMouseEvent* event, Point& windowTranslateBegin, Point& windowTranslateEnd, Point& windowTranslateAccumalation, double windowScaleTime)
    {

    }


    bool handleMousePress1Event(QMouseEvent* event, Point& windowTranslateBegin, Point& windowTranslateAccumalation, double windowScaleTime) override
    {
        return false;
    }
    bool handleMousePress2Event(QMouseEvent* event, Point& windowTranslateBegin, Point& windowTranslateAccumalation, double windowScaleTime) override
    {
        return false;
    }

    void handleInterupt()
    {

    }
    void displaySelectBox(QPainter& painter)
    {

    }
    void updateBbox()
    {
        for (auto p : selectSet)
        {
            if (p->get_box().xmax > BBoxSet.xmax)BBoxSet.xmax = p->get_box().xmax;
            if (p->get_box().xmin < BBoxSet.xmin)BBoxSet.xmin = p->get_box().xmin;
            if (p->get_box().ymax > BBoxSet.ymax)BBoxSet.ymax = p->get_box().ymax;
            if (p->get_box().ymin < BBoxSet.ymin)BBoxSet.ymin = p->get_box().ymin;
            BBoxSet.center = Point((BBoxSet.xmax + BBoxSet.xmin) / 2, (BBoxSet.ymax + BBoxSet.ymin) / 2);
        }
    }
    void handleWheelEvent(bool e, double& windowScaleTime)
    {
        updateBbox();
        for (auto p : selectSet)
        {
            p->scale(BBoxSet,e);
        }
    }
private:
    BBox BBoxSet;
};