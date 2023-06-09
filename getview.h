#pragma once

#include<QColor>
#include<QColorDialog>
#include<QInputDialog>
#include<QPainter>
#include<QMessageBox>
#include<cmath>
#include<QDebug>

#include "basic.h"
#include "geometry.h"

extern QWidget* MainWindow;

class View
{
protected:
    int activePoint;
    Point translateOriginPoint;
    Point rotateOriginPoint;
    Point scaleOriginPoint;
public:
    View(){}
    virtual BBox get_box() = 0;
    virtual QVector<Point*>& get_p() = 0;
    virtual QColor& get_borderColor() = 0;
    virtual int& get_borderStyle() = 0;
    virtual QColor& get_fillColor() = 0;
    virtual int& get_fillStyle() = 0;
    virtual double& get_borderWidth() = 0;
    virtual void show(QPainter& painter) = 0;
    virtual void createBegin(double x, double y) = 0;
    virtual void creating(double x, double y) = 0;
    virtual void createEnd(double x, double y) = 0;
    virtual bool isInside(double xmin, double ymin, double xmax, double ymax) = 0;
    void translateBegin(double x, double y)
    {
        translateOriginPoint.x = x;
        translateOriginPoint.y = y;
    }
    virtual void translateEnd(double x, double y,double windowscaletime)
    {
        for (int i = 0; i < get_p().size(); i++)
        {
            get_p()[i]->x += (x - translateOriginPoint.x)/ windowscaletime;
            get_p()[i]->y += (y - translateOriginPoint.y)/ windowscaletime;
        }
        translateOriginPoint.x = x;
        translateOriginPoint.y = y;
    }

    void rotateBegin(double x, double y)
    {
        rotateOriginPoint.x = x;  // 记录旋转起始点坐标
        rotateOriginPoint.y = y;
    }

    void rotateEnd(double x, double y,Point center)
    {
        double dx1 = rotateOriginPoint.x - center.x;
        double dy1 = rotateOriginPoint.y - center.y;
        double dx2 = x - center.x;   // 计算旋转终点相对于旋转中心的偏移量
        double dy2 = y - center.y;

        double theta = atan2(dy2, dx2) - atan2(dy1, dx1);

        for (int i = 0; i < get_p().size(); i++)
        {
            get_p()[i]->x = cos(theta) * (get_p()[i]->x - center.x) - sin(theta) * (get_p()[i]->y - center.y) + center.x;
            get_p()[i]->y = sin(theta) * (get_p()[i]->x - center.x) + cos(theta) * (get_p()[i]->y - center.y) + center.y;
        }
        rotateOriginPoint.x = x; // 更新旋转起始点坐标
        rotateOriginPoint.y = y;
    }

    void scale(BBox bbox,bool e)
    {
        if (e)
        {
            for (int i = 0; i < get_p().size(); i++)
            {
                get_p()[i]->x = (get_p()[i]->x - bbox.center.x) * 1.1 + bbox.center.x;
                get_p()[i]->y = (get_p()[i]->y - bbox.center.y) * 1.1 + bbox.center.y;
            }
        }
        else
        {
            for (int i = 0; i < get_p().size(); i++)
            {
                get_p()[i]->x = (get_p()[i]->x - bbox.center.x) * 0.9 + bbox.center.x;
                get_p()[i]->y = (get_p()[i]->y - bbox.center.y) * 0.9 + bbox.center.y;
            }
        }
    }
};

extern View* view;

extern QVector<int> selectViewSet;

extern QVector<View*> selectSet;

class Layer;

extern Layer* currentLayer;

class Layer
{
protected:
    QString LayerFileName;
    QString LayerName;
    QVector<View*> pv;
    int step;
    double xbegin, ybegin, xmin, xmax, ymin, ymax;
public:
    Layer()
    {
    }
    Layer(QString str) : step(0)
    {
        LayerFileName = str;
    }
    QString getName()
    {
        return LayerName;
    }
    void setName(QString name)
    {
        LayerName=name;
    }
    QVector<View*>& getVector()
    {
        return pv;
    }
    void boxSelectBegin(double x, double y)
    {
        selectSet.clear();
        xbegin = x; xmin = x; xmax = x;
        ybegin = y; ymin = y; ymax = y;
    }

    void boxSelecting(double x, double y)
    {
        selectSet.clear();
        xmin = (x < xbegin) ? x : xbegin;
        xmax = (x > xbegin) ? x : xbegin;
        ymin = (y < ybegin) ? y : ybegin;
        ymax = (y > ybegin) ? y : ybegin;
        for (int i = 0; i < pv.size(); i++)
        {
            int flag = 1;
            if (pv[i]->isInside(xmin, ymin, xmax, ymax))
            {
                selectSet.push_back(currentLayer->getVector()[i]);
            }                
        }
    }

    void boxSelectEnd(double x, double y)
    {
        boxSelecting(x, y);          
    }
    void boxDisplay(QPainter& painter)
    {
        QPen pen(Qt::DashLine);

        pen.setColor(Qt::black);
        pen.setWidth(1);

        painter.setPen(pen);
        painter.setBrush(QBrush());

        QRect rect(xmin, ymin, xmax - xmin, ymax - ymin);
        painter.drawRect(rect);
    }
};

extern QVector<Layer*> LayerList;

class PointView :public CPoint, public View
{
public:
    PointView(QVector<Point*>& geom, QString bordercolor = "#000000", int borderstyle =1)
        :CPoint(geom,bordercolor.toStdString(), borderstyle)
    {

    }
    PointView(CPoint& cp) :CPoint(cp) { }

    QVector<Point*>& get_p()
    {
        return getP();
    }
    QColor& get_borderColor() { return BorderColor; }
    int& get_borderStyle() { return BorderStyle; }
    QColor& get_fillColor() { return FillColor; }
    int& get_fillStyle() { return FillStyle; }
    double& get_borderWidth() { return BorderWidth; }
    void show(QPainter& painter)
    {
        if (!selectSet.isEmpty())
        {
            int flag = 0;
            for (int i = 0; i < selectSet.size(); i++)
            {
                if (this == selectSet[i])
                    flag = 1;
            }
            if (flag == 1)
                painter.setPen(QPen(QColor(0, 255, 255), 3, Qt::SolidLine));
            else
                painter.setPen(QPen(QColor(BorderColor), BorderWidth, Qt::SolidLine));
        }
        else
            painter.setPen(QPen(QColor(BorderColor), BorderWidth, Qt::SolidLine));
        painter.setBrush(QBrush(QColor(FillColor), Qt::SolidPattern));

        painter.drawEllipse((*p[0]).x-4, (*p[0]).y-4, 8, 8);
    }
    void createBegin(double x, double y)
    {
        Point* temp = new Point;
        temp->x = x; temp->y = y;
        currentLayer->getVector().first()->get_p().append(temp);
    }
    void creating(double x, double y)
    {
        //Bat SeoiJiu SatJing
    }
    void createEnd(double x, double y)
    {
        //Bat SeoiJiu SatJing
    }
    BBox get_box()
    {
        return GetBBox();
    }
    bool isInside(double xmin, double ymin, double xmax, double ymax)
    {
        for (auto p : get_p())
        {
            if (p->x>xmin && p->x<xmax && p->y>ymin && p->y<ymax)
                return true;
        }
        return false;
    }
};


class LineView :public CLine, public View
{
public:
    LineView(QVector<Point*>& geom, double borderwidth=2,QString bordercolor="#000000", int borderstyle=1)
        :CLine(geom, borderwidth,bordercolor.toStdString(), BorderStyle) {}
    LineView(const CLine& cp) : CLine(cp) { }
    QColor& get_borderColor() { return BorderColor; }
    int& get_borderStyle() { return BorderStyle; }
    QColor& get_fillColor() { return FillColor; }
    int& get_fillStyle() { return FillStyle; }
    double& get_borderWidth() { return BorderWidth; }
    QVector<Point*>& get_p()
    {
        return getP();
    }

    void show(QPainter& painter)
    {
        if (!selectSet.isEmpty())
        {
            int flag = 0;
            for (int i = 0; i < selectSet.size(); i++)
            {
                if (this == selectSet[i])
                    flag = 1;
            }
            if (flag == 1)
                painter.setPen(QPen(QColor("#00FFFF"), BorderWidth, Qt::SolidLine));
            else
                painter.setPen(QPen(QColor(BorderColor), BorderWidth, Qt::SolidLine));
        }
        else
            painter.setPen(QPen(QColor(BorderColor), BorderWidth, Qt::SolidLine));
        painter.setBrush(QBrush(QColor(FillColor), Qt::SolidPattern));

       // painter.drawEllipse((*p[0]).x - 2, (*p[0]).y - 2, 4, 4);
        for (int i = 1; i < p.size(); i++)
        {
			//painter.drawEllipse((*p[i]).x-2, (*p[i]).y-2, 4, 4);
            painter.drawLine((*p[i-1]).x, (*p[i-1]).y, (*p[i]).x, (*p[i]).y);
		}
    }
    void createBegin(double x, double y)
    {
        Point* temp = new Point;
        temp->x = x; temp->y = y;
        currentLayer->getVector().first()->get_p().append(temp);
      //  currentLayer->getVector().first()->get_p().append(temp);
    }
    void creating(double x, double y)
    {
       /* Point* temp = new Point;
        temp->x = x; temp->y = y;
        currentLayer->getVector().first()->get_p().last()=temp;*/
    }
    void createEnd(double x, double y)
    {
        creating(x, y);
    }
    BBox get_box()
    {
        return GetBBox();
    }
    bool isInside(double xmin, double ymin, double xmax, double ymax)
    {
        for (auto p : get_p())
        {
            if(p->x<xmin ||p->x>xmax||p->y<ymin||p->y>ymax)
				return false;
        }
        return true;
    }
};

class PolygonView :public CPolygon, public View
{
public:
    PolygonView(QVector<Point*>& geom, double borderwidth = 3, QString bordercolor = "#000000", int borderstyle = 1,QString fillcolor="#00000000",int fillstyle =1)
        :CPolygon(geom, borderwidth, bordercolor.toStdString(), borderstyle, fillcolor.toStdString(), fillstyle)
    { }
    PolygonView(const CPolygon& cp) :CPolygon(cp) { }
    QColor& get_borderColor() { return BorderColor; }
    int& get_borderStyle() { return BorderStyle; }
    QColor& get_fillColor() { return FillColor; }
    int& get_fillStyle() { return FillStyle; }
    double& get_borderWidth() { return BorderWidth; }
    QVector<Point*>& get_p()
    {
        return getP();
    }
    void show(QPainter& painter)
    {
        if (!selectSet.isEmpty())
        {
            int flag = 0;
            for (int i = 0; i < selectSet.size(); i++)
            {
                if (this == selectSet[i])
                    flag = 1;
            }
            if (flag == 1)
                painter.setPen(QPen(QColor("#00FFFF"), BorderWidth, Qt::SolidLine));
            else
                painter.setPen(QPen(QColor(BorderColor), BorderWidth, Qt::SolidLine));
        }
        else
            painter.setPen(QPen(QColor(BorderColor), BorderWidth, Qt::SolidLine));
        painter.setBrush(QBrush(QColor(FillColor), Qt::SolidPattern));

        for (int i = 0; i < p.size(); i++)
        {
            painter.drawEllipse((*p[i]).x-2, (*p[i]).y-2, 4, 4);
        }

        if (p.size() == 2)
        {
            painter.drawLine((*p[0]).x, (*p[0]).y, (*p[1]).x, (*p[1]).y);
        }
        else if (p.size() > 2)
        {
            QPolygon polygon;
            for (int i = 0; i < p.size(); i++)
            {
				polygon << QPoint((*p[i]).x, (*p[i]).y);
			}          
            painter.drawPolygon(polygon);
        }
    }
    void createBegin(double x, double y)
    {
        Point* temp = new Point;
        temp->x = x; temp->y = y;
        currentLayer->getVector().first()->get_p().append(temp);
        currentLayer->getVector().first()->get_p().append(temp);
    }
    void creating(double x, double y)
    {
        Point* temp = new Point;
        temp->x = x; temp->y = y;
        currentLayer->getVector().first()->get_p().last() = temp;
    }
    void createEnd(double x, double y)
    {
        creating(x, y);
    }
    BBox get_box()
    {
        return GetBBox();
    }
    bool isInside(double xmin, double ymin, double xmax, double ymax)
    {
        for (auto p : get_p())
        {
            if (p->x<xmin || p->x>xmax || p->y<ymin || p->y>ymax)
                return false;
        }
        return true;
    }
};

class RectangleView :public CRectangle, public View
{
public:
    RectangleView(QVector<Point*>& geom, double borderwidth=3,QString bordercolor="#000000", int borderstyle=1, QString fillcolor="#00000000",int fillstyle =1)
        :CRectangle(geom, borderwidth, bordercolor.toStdString(), borderstyle, fillcolor.toStdString(), fillstyle) {}
    RectangleView(const CRectangle& cp) : CRectangle(cp) { }
    QVector<Point*>& get_p()
    {
        return getP();
    }
    QColor& get_borderColor() { return BorderColor; }
    int& get_borderStyle() { return BorderStyle; }
    QColor& get_fillColor() { return FillColor; }
    int& get_fillStyle() { return FillStyle; }
    double& get_borderWidth() { return BorderWidth; }
    void show(QPainter& painter)
    {
        if (!selectSet.isEmpty())
        {
            int flag = 0;
            for (int i = 0; i < selectSet.size(); i++)
            {
                if (this == selectSet[i])
                    flag = 1;
            }
            if (flag == 1)
                painter.setPen(QPen(QColor("#00FFFF"), BorderWidth, Qt::SolidLine));
            else
                painter.setPen(QPen(QColor(BorderColor), BorderWidth, Qt::SolidLine));
        }
        else
            painter.setPen(QPen(QColor(BorderColor), BorderWidth, Qt::SolidLine));
        painter.setBrush(QBrush(QColor(FillColor), Qt::SolidPattern));

        for (int i = 0; i < p.size(); i++)
        {
            painter.drawEllipse((*p[i]).x - 2, (*p[i]).y - 2, 4, 4);
            painter.drawEllipse((*p[i]).x - 2, (*p[1 - i]).y - 2, 4, 4);
        }

        if (p.size() == 2)
        {
            painter.drawRect((*p[0]).x, (*p[0]).y, (*p[1]).x - (*p[0]).x, (*p[1]).y - (*p[0]).y);
        }
    }
    void createBegin(double x, double y)
    {
        Point* temp = new Point;
        temp->x = x; temp->y = y;
        currentLayer->getVector().first()->get_p().append(temp);
        currentLayer->getVector().first()->get_p().append(temp);
    }
    void creating(double x, double y)
    {
        Point* temp = new Point;
        temp->x = x; temp->y = y;
        currentLayer->getVector().first()->get_p().last() = temp;
    }
    void createEnd(double x, double y)
    {
        creating(x, y);
    }
    BBox get_box()
    {
        return GetBBox();
    }
    bool isInside(double xmin, double ymin, double xmax, double ymax)
    {
        if (xmin <= GetBBox().xmin && xmax >= GetBBox().xmin && ymin <= GetBBox().ymin && ymax >= GetBBox().ymax)
            return true;
        return false;
    }
};

class CircleView :public CCircle, public View
{
public:
    CircleView(QVector<Point*>& geom, double radius=0, double borderwidth = 3, QString bordercolor = "#000000", int borderstyle = 1, QString fillcolor = "#00000000", int fillstyle = 1)
        :CCircle(geom, radius, borderwidth, bordercolor.toStdString(),borderstyle, fillcolor.toStdString(), fillstyle) {}
    CircleView(const CCircle& cp) : CCircle(cp) { }
    QVector<Point*>& get_p()
    {
        return getP();
    }
    QColor& get_borderColor() { return BorderColor; }
    int& get_borderStyle() { return BorderStyle; }
    QColor& get_fillColor() { return FillColor; }
    int& get_fillStyle() { return FillStyle; }
    double& get_borderWidth() { return BorderWidth; }
    void show(QPainter& painter)
    {
        if (!selectSet.isEmpty())
        {
            int flag = 0;
            for (int i = 0; i < selectSet.size(); i++)
            {
                if (this == selectSet[i])
                    flag = 1;
            }
            if (flag == 1)
                painter.setPen(QPen(QColor("#00FFFF"), BorderWidth, Qt::SolidLine));
            else
                painter.setPen(QPen(QColor(BorderColor), BorderWidth, Qt::SolidLine));
        }
        else
            painter.setPen(QPen(QColor(BorderColor), BorderWidth, Qt::SolidLine));
        painter.setBrush(QBrush(QColor(FillColor), Qt::SolidPattern));
        painter.drawEllipse((*p[0]).x - Radius, (*p[0]).y - Radius, 2 * Radius, 2 * Radius);
    }
    void createBegin(double x, double y)
    {
        Point* temp = new Point;
        temp->x = x; temp->y = y;
        currentLayer->getVector().first()->get_p().append(temp);
    }
    void creating(double x, double y)
    {
        this->Radius=sqrt(pow((currentLayer->getVector().first()->get_p().last()->x-x),2)+pow((currentLayer->getVector().first()->get_p().last()->y - y),2));
    }
    void createEnd(double x, double y)
    {
        creating(x, y);
    }
    BBox get_box()
    {
        return GetBBox();
    }
    bool isInside(double xmin, double ymin, double xmax, double ymax)
	{
        if (xmin <= GetBBox().xmin && xmax>= GetBBox().xmax && ymin<= GetBBox().ymin && ymax>=GetBBox().ymax)
            return true;
        return false;
	}
};

class SectorView :public CSector, public View
{
public:
    SectorView(QVector<Point*>& geom, double radius = 0, double borderwidth = 3, QString bordercolor = "#000000", int borderstyle = 1, QString fillcolor = "#00000000", int fillstyle = 1)
        :CSector(geom, radius = 0,borderwidth = 3, bordercolor.toStdString(), borderstyle, fillcolor.toStdString(), fillstyle) {}
    SectorView(const CSector& cp) : CSector(cp) { }
    QVector<Point*>& get_p()
    {
        return getP();
    }
    QColor& get_borderColor() { return BorderColor; }
    int& get_borderStyle() { return BorderStyle; }
    QColor& get_fillColor() { return FillColor; }
    int& get_fillStyle() { return FillStyle; }
    double& get_borderWidth() { return BorderWidth; }
    void show(QPainter& painter)
    {
       
        if (get_p().count() <=2) return;
        QPointF p0(qreal(p[0]->x), qreal(p[0]->y));
        QPointF p1(qreal((p[1])->x), qreal((p[1])->y));
        QPointF p2(qreal(p[2]->x), qreal(p[2]->y));

        QLineF line1(p0,p1);
        QLineF line2(p0, p2);
        double beginAngle =line1.angle();
        double endAngle = line2.angle();
        this->Radius = sqrt(pow(p[1]->x - p[0]->x, 2) + pow(p[1]->y - p[0]->y, 2));
        double spanAngle = endAngle - beginAngle;
        if (spanAngle < 0) spanAngle += 360.0;

        if (!selectSet.isEmpty())
        {
            int flag = 0;
            for (int i = 0; i < selectSet.size(); i++)
            {
                if (this == selectSet[i])
                    flag = 1;
            }
            if (flag == 1)
                painter.setPen(QPen(QColor("#00FFFF"), BorderWidth, Qt::SolidLine));
            else
                painter.setPen(QPen(QColor(BorderColor), BorderWidth, Qt::SolidLine));
        }
        else
            painter.setPen(QPen(QColor(BorderColor), BorderWidth, Qt::SolidLine));
        painter.setBrush(QBrush(QColor(FillColor), Qt::SolidPattern));
        painter.drawPie(QRectF((*p[0]).x - Radius, (*p[0]).y - Radius, 2 * Radius, 2 * Radius), beginAngle * 16, spanAngle * 16);
       /* double beginAngle;
        if (!selectSet.isEmpty())
        {
            int flag = 0;
            for (int i = 0; i < selectSet.size(); i++)
            {
                if (this == selectSet[i])
                    flag = 1;
            }
            if (flag == 1)
                painter.setPen(QPen(QColor("#00FFFF"), BorderWidth, Qt::SolidLine));
            else
                painter.setPen(QPen(QColor(BorderColor), BorderWidth, Qt::SolidLine));
        }
        else
            painter.setPen(QPen(QColor(BorderColor), BorderWidth, Qt::SolidLine));
        painter.setBrush(QBrush(QColor(FillColor), Qt::SolidPattern));
        painter.drawPie(QRectF((*p[0]).x-Radius, (*p[0]).y-Radius,2*Radius,2*Radius), (*p[1]).x * 16, ((*p[1]).y - (*p[1]).x) * 16);*/
    }
    void createBegin(double x, double y)
    {
        Point* temp = new Point;
        temp->x = x; temp->y = y;
        currentLayer->getVector().first()->get_p().append(temp);
       // currentLayer->getVector().first()->get_p().append(temp);
    }
    void creating(double x, double y)
    {
      /*  Point* temp = new Point;
        temp->x = x; temp->y = y;
        currentLayer->getVector().first()->get_p().append(temp);*/
        
    }
    void createEnd(double x, double y)
    {
        Point* temp = new Point;
        temp->x = x; temp->y = y;
        currentLayer->getVector().first()->get_p().append(temp);
       
    }
   /* void translateEnd(double x, double y, double windowscaletime)
    {
        get_p()[0]->x += (x - translateOriginPoint.x) / windowscaletime;
        get_p()[0]->y += (y - translateOriginPoint.y) / windowscaletime;
        translateOriginPoint.x = x;
        translateOriginPoint.y = y;
    }*/
    BBox get_box()
    {
        return GetBBox();
    }
    bool isInside(double xmin, double ymin, double xmax, double ymax)
    {
        if (xmin <= get_box().xmin && xmax >= get_box().xmax && ymin <= get_box().ymin && ymax >= get_box().ymax)
            return true;
        return false;
    }
};









