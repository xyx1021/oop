#pragma once

#include <QVector>
#include <sstream>
#include <string>
#include <iomanip>
#include <QColor>
#include <QString>

#include "basic.h"

#define PI 3.1416

class Geometry
{
protected:
    QVector<Point*> p;
    QColor BorderColor;
    int BorderStyle = 1;
    QColor FillColor;
    int FillStyle = 1;
    double BorderWidth = 0;
    double Radius= -1;
public:
    virtual ~Geometry() { }
    QVector<Point*>& getP() { return p; }//获取图形顶点容器
    QColor& getBorderColor() { return BorderColor; }
    QColor& getFillColor() { return FillColor; }
    double& getBorderWidth() { return BorderWidth; }
    void setP(QVector<Point*> geo) { p = geo; }
    void setBorderColor(QColor c) { BorderColor = c; }
    void setFillColor(QColor c) { FillColor = c; }
    void setBorderWidth(double BW) { BorderWidth = BW; }
    virtual void Build(char* buf, int size) { }
    virtual const char* getname() = 0;
    virtual BBox GetBBox() const = 0; //获取最小外包矩形
    virtual double getArea() = 0;
    virtual double getLength() = 0;
    virtual QString wkt() = 0;           //兼容自定义数据格式转wkt格式
    virtual QString gisk() = 0;
};

class CPoint :public Geometry
{
public:
    CPoint()
    {
        BorderColor= QColor("#000000");
        FillColor = QColor("#000000");
        BorderWidth = 3; BorderStyle = 1; FillStyle = 1;
    }
    CPoint(QVector<Point*> geom,std::string bordercolor,int borderstyle)
    {
        p = geom;
        BorderColor = QColor(QString::fromStdString(bordercolor));
        FillColor = QColor(QString::fromStdString(bordercolor));
        BorderStyle = borderstyle;
        FillStyle = borderstyle;
    }
    const char* getname()
    {
        return "POINT";
    }
    double getArea()
    {
        return 0;
    }
    double getLength()
    {
        return 0;
    }
    BBox GetBBox() const
    {
        BBox BoundingBox;
        BoundingBox.center = p[0][0];
        BoundingBox.xmin = p[0][0].x - 10;
        BoundingBox.ymin = p[0][0].y - 10;
        BoundingBox.xmax = p[0][0].x + 10;
        BoundingBox.ymax = p[0][0].y + 10;
        return BoundingBox;
    }
    QString wkt()
    {
        std::ostringstream ofs;
        ofs << "POINT (";
        //POINT (3 4)
        ofs << p[0][0].x << " " << p[0][0].y;
        ofs << ")";
        return QString::fromStdString(ofs.str());
    }
    QString gisk()
    {
        std::ostringstream ofs;
        ofs << "POINT (";
        //POINT (3 4)
        ofs << p[0][0].x << " " << p[0][0].y;
        ofs << ")";
        ofs <<' '<< BorderColor.name().toStdString() << ' ' << BorderStyle;
        return QString::fromStdString(ofs.str());
    }
    void Build(char* buf, int size)
    {
        int numPoints = *(int*)(buf + 40);
        int* Parts = (int*)(buf + 44);
        Point* pt = (Point*)(buf + 48);
        p.push_back(&pt[0]);
    }
};

class CLine :public Geometry
{
public:
    CLine()
    {
        BorderColor = QColor("#000000");
        FillColor = QColor("#00000000");
        BorderWidth = 3; BorderStyle = 1; FillStyle = 1;
    }
    CLine(QVector<Point*>& geom, double width, std::string bordercolor, int borderstyle)
    {
        p = geom;
        BorderWidth = width;
        BorderColor = QColor(QString::fromStdString(bordercolor));
        FillColor = BorderColor;
        BorderStyle = borderstyle;
        FillStyle = BorderStyle;
    }
    const char* getname()
    {
        return "LINESTRING";
    }
    double getArea()
    {
        return 0;
    }
    double getLength()
    {
        double len = (double)(sqrt(pow((p[1]->x - p[0]->x), 2) + pow((p[1]->y - p[0]->y), 2)));
        return len;
    }
    BBox GetBBox() const
    {
        BBox BoundingBox;
        BoundingBox.xmin = p[0]->x; BoundingBox.ymin = p[0]->y;
        BoundingBox.xmax = p[0]->x; BoundingBox.ymax = p[0]->y;
        for (int i = 0; i < p.size(); i++) {
            if (p[i]->x > BoundingBox.xmax)BoundingBox.xmax = p[i]->x;
            if (p[i]->y > BoundingBox.ymax)BoundingBox.ymax = p[i]->y;
            if (p[i]->x < BoundingBox.xmin)BoundingBox.xmin = p[i]->x;
            if (p[i]->y < BoundingBox.ymin)BoundingBox.ymin = p[i]->y;
            if (BoundingBox.xmax == BoundingBox.xmin) { BoundingBox.xmax += 1; BoundingBox.xmin -= 1; }
            if (BoundingBox.ymax == BoundingBox.ymin) { BoundingBox.ymax += 1; BoundingBox.ymin -= 1; }
        }
        BoundingBox.center.x = (BoundingBox.xmax + BoundingBox.xmin) / 2;
        BoundingBox.center.y = (BoundingBox.ymax + BoundingBox.ymin) / 2;
        return BoundingBox;
    }
    QString wkt()
    {
        std::ostringstream ofs;
        ofs << "LINESTRING (";
        //LINESTRING(3 4, 10 50, 20 25)
        for (int i = 0; i < p.size(); i++)
        {
            ofs << p[i]->x << " " << p[i]->y;
            if (i < p.size() - 1)
                ofs << ",";
            else
                ofs << ")";
        }
        return QString::fromStdString(ofs.str());
    }

    QString gisk()
    {
        std::ostringstream ofs;
        ofs << "LINESTRING (";
        //LINESTRING (3 4, 5 6)
        for (int i = 0; i < p.size(); i++)
        {
            ofs << p[i]->x << " " << p[i]->y;
            if (i < p.size() - 1)
                ofs << ",";
            else
                ofs << ")";
        }
        ofs << ' ' << BorderWidth <<' '<< BorderColor.name().toStdString()<< ' '<< BorderStyle;
        return QString::fromStdString(ofs.str());
    }
    void Build(char* buf, int size)
    {
        int numPoints = *(int*)(buf + 40);
        int* Parts = (int*)(buf + 44);
        Point* pt = (Point*)(buf + 48);

        for (int i = 0; i < numPoints; i++)
        {
            p.push_back(&pt[i]);
        }
    }
};

class CPolygon :public Geometry
{
public:
    CPolygon()
    {
        BorderColor = QColor("#000000");
        FillColor = QColor("#00000000");
        BorderWidth = 3; BorderStyle = 1; FillStyle = 1;
    }

    CPolygon(QVector<Point*>& geom, double width, std::string bordercolor, int borderstyle, std::string fillcolor, int fillstyle)
    {
        p = geom;
        BorderWidth = width;
        BorderColor = QColor(QString::fromStdString(bordercolor));
        FillColor = QColor(QString::fromStdString(fillcolor));
        BorderStyle = borderstyle;
        FillStyle = fillstyle;
    }

    const char* getname()
    {
        return"POLYGON";
    }
    double getArea()
    {
        //返回多边形面积
        double area = 0;
        for (int i = 1; i < p.size(); i++) {
            area += (double)(p[i]->x * p[i - 1]->y - p[i]->y * p[i - 1]->x) / 2.0;
        }
        area += (double)(p[0]->x * p[p.size() - 1]->y - p[0]->y * p[p.size() - 1]->x) / 2.0;//第1和第n个点单独处理
        if (area < 0.0) area = area;//顺时针与逆时针输入结果互为相反数
        return area;
    }
    double getLength()
    {
        //返回多边形周长
        double len = 0;
        for (int i = 1; i < p.size(); i++) {
            len += (double)(sqrt(pow((p[i]->x - p[i - 1]->x), 2) + pow((p[i]->y - p[i - 1]->y), 2)));
        }
        len += (double)(sqrt(pow((p[0]->x - p[p.size() - 1]->x), 2) + pow((p[0]->y - p[p.size() - 1]->y), 2)));//第1和第n个点单独处理
        return len;
    }
    BBox GetBBox() const
    {
        BBox BoundingBox;
        BoundingBox.xmin = p[0]->x; BoundingBox.ymin = p[0]->y;
        BoundingBox.xmax = p[0]->x; BoundingBox.ymax = p[0]->y;
        for (int i = 0; i < p.size(); i++) {
            if (p[i]->x > BoundingBox.xmax)BoundingBox.xmax = p[i]->x;
            if (p[i]->y > BoundingBox.ymax)BoundingBox.ymax = p[i]->y;
            if (p[i]->x < BoundingBox.xmin)BoundingBox.xmin = p[i]->x;
            if (p[i]->y < BoundingBox.ymin)BoundingBox.ymin = p[i]->y;
        }
        BoundingBox.center.x = (BoundingBox.xmax + BoundingBox.xmin) / 2;
        BoundingBox.center.y = (BoundingBox.ymax + BoundingBox.ymin) / 2;
        return BoundingBox;
    }
    QString wkt()
    {
        //POLYGON((1 1,5 1,5 5,1 5,1 1))
        std::ostringstream ofs;
        //ofs<<setiosflags(ios_base::scientific);
        ofs << std::setiosflags(std::ios_base::fixed);
        ofs << "POLYGON ((";
        for (int i = 0; i < p.size(); i++) {
            ofs << std::setprecision(10) << p[i]->x << " " << std::setprecision(10) << p[i]->y;
            if (i < p.size() - 1)
                ofs << ",";
            else
                ofs << "))";
        }
        return QString::fromStdString(ofs.str());
    }

    QString gisk()
    {
        //POLYGON((1 1,5 1,5 5,1 5,1 1))
        std::ostringstream ofs;
        //ofs<<setiosflags(ios_base::scientific);
        ofs << std::setiosflags(std::ios_base::fixed);
        ofs << "POLYGON ((";
        for (int i = 0; i < p.size(); i++) {
            ofs << std::setprecision(10) << p[i]->x << " " << std::setprecision(10) << p[i]->y;
            if (i < p.size() - 1)
                ofs << ",";
            else
                ofs << "))";
        }
        ofs << ' ' << BorderWidth << ' ' << BorderColor.name().toStdString() << ' ' << BorderStyle << ' ' << FillColor.name(QColor::HexArgb).toStdString() << ' ' << FillStyle;
        return QString::fromStdString(ofs.str());
    }

    void Build(char* buf, int size)
    {
        int numPoints = *(int*)(buf + 40);
        int* Parts = (int*)(buf + 44);
        Point* pt = (Point*)(buf + 48);

        for (int i = 0; i < numPoints; i++)
        {
            p.push_back(&pt[i]);
        }
    }
};

class CRectangle : public Geometry
{
public:
    CRectangle()
    {
        BorderColor = QColor("#000000");
        FillColor = QColor("#00000000");
        BorderWidth = 3; BorderStyle = 1; FillStyle = 1;
    }

    CRectangle(QVector<Point*>& geom, double width, std::string bordercolor, int borderstyle, std::string fillcolor, int fillstyle)
    {
        p = geom;
        BorderWidth = width;
        BorderColor = QColor(QString::fromStdString(bordercolor));
        FillColor = QColor(QString::fromStdString(fillcolor));
        BorderStyle = borderstyle;
        FillStyle = fillstyle;
    }
    const char* getname()
    {
        return"RECTANGLE";
    }
    double getArea()
    {
        // A=ab
        double edge1 = getEdge1();
        double edge2 = getEdge2();
        return edge1 * edge2;
    }
    double getLength()
    {
        // C=2(a+b)
        double edge1 = getEdge1();
        double edge2 = getEdge2();
        return 2 * (edge1 + edge2);
    }
    double getEdge1()
    {
        double edge1 = abs(p[1]->x - p[0]->x);
        return edge1;
    }
    double getEdge2()
    {
        double edge2 = abs(p[1]->y - p[0]->y);
        return edge2;
    }
    BBox GetBBox() const
    {
        BBox BoundingBox;
        if (p[0]->x > p[1]->x) { BoundingBox.xmin = p[1]->x; BoundingBox.xmax = p[0]->x; }
        else { BoundingBox.xmin = p[0]->x; BoundingBox.xmax = p[1]->x; }
        if (p[0]->y > p[1]->y) { BoundingBox.ymin = p[1]->y; BoundingBox.ymax = p[0]->y; }
        else { BoundingBox.ymin = p[0]->y; BoundingBox.ymax = p[1]->y; }
        BoundingBox.center.x = (BoundingBox.xmax + BoundingBox.xmin) / 2;
        BoundingBox.center.y = (BoundingBox.ymax + BoundingBox.ymin) / 2;
        return BoundingBox;
    }
    QString wkt()
    {
        BBox box = GetBBox();

        std::ostringstream ofs;
        //ofs<<setiosflags(ios_base::scientific);
        ofs << std::setiosflags(std::ios_base::fixed);
        ofs << "POLYGON ((";
        ofs << std::setprecision(10) << box.xmin << " " << std::setprecision(10) << box.ymin << ",";  //第一个点
        ofs << std::setprecision(10) << box.xmax << " " << std::setprecision(10) << box.ymin << ",";//第二个点
        ofs << std::setprecision(10) << box.xmax << " " << std::setprecision(10) << box.ymax << ",";//第三个点
        ofs << std::setprecision(10) << box.xmin << " " << std::setprecision(10) << box.ymax << "))"; //第四个点
        ofs << std::setprecision(10) << box.xmin << " " << std::setprecision(10) << box.ymin << ",";  //第一个点
        return QString::fromStdString(ofs.str());
    }

    QString gisk()
    {
        BBox box = GetBBox();

        std::ostringstream ofs;
        //ofs<<setiosflags(ios_base::scientific);
        ofs << std::setiosflags(std::ios_base::fixed);
        ofs << "RECTANGLE (";
        ofs << std::setprecision(10) << box.xmin << " " << std::setprecision(10) << box.ymin << ",";  //第一个点
        ofs << std::setprecision(10) << box.xmax << " " << std::setprecision(10) << box.ymax << ")"; 
        ofs << ' ' << BorderWidth << ' ' << BorderColor.name().toStdString() << ' ' << BorderStyle << ' ' << FillColor.name(QColor::HexArgb).toStdString() << ' ' << FillStyle;
        return QString::fromStdString(ofs.str());
    }

};

class CCircle :public Geometry
{
public:
    CCircle()
    {
        BorderColor = QColor("#000000");
        FillColor = QColor("#00000000");
        BorderWidth = 3; BorderStyle = 1; FillStyle = 1;
        Radius = 0;
    }
    CCircle(QVector<Point*>& geom, double r,double width, std::string bordercolor, int borderstyle, std::string fillcolor, int fillstyle)
    {
        p = geom;
        BorderWidth = width;
        BorderColor = QColor(QString::fromStdString(bordercolor));
        FillColor = QColor(QString::fromStdString(fillcolor));
        BorderStyle = borderstyle;
        FillStyle = fillstyle;
        Radius = r;
    }
    const char* getname()
    {
        return"CIRCLE";
    }
    double getArea()
    {
        return PI * Radius * Radius;
    }
    double getLength()
    {
        return 2 * PI * Radius;
    }
    BBox GetBBox() const
    {
        BBox BoundingBox;
        BoundingBox.center = *p[0];
        BoundingBox.xmin = p[0]->x - Radius;
        BoundingBox.ymin = p[0]->y - Radius;
        BoundingBox.xmax = p[0]->x + Radius;
        BoundingBox.ymax = p[0]->y + Radius;
        return BoundingBox;
    }
    QString wkt()
    {
        BBox box = GetBBox();
        double p = box.xmax - Radius;
        double q = box.center.y;
        std::ostringstream ofs;
        //ofs<<setiosflags(ios_base::scientific);
        ofs << std::setiosflags(std::ios_base::fixed);
        ofs << "POLYGON ((";
        Point temp;
        for (int i = 0; i <= 100; i++)
        {
            temp.x = p + Radius * cos(2 * PI * i / 100);
            temp.y = q + Radius * sin(2 * PI * i / 100);
            ofs << std::setprecision(10) << temp.x << " " << std::setprecision(10) << temp.y << ",";
        }
        temp.x = p + Radius * cos(0);
        temp.y = q + Radius * sin(0);
        ofs << std::setprecision(10) << temp.x << " " << std::setprecision(10) << temp.y << "))";
        return QString::fromStdString(ofs.str());
    }
    QString gisk()
    {
        std::ostringstream ofs;
        //ofs<<setiosflags(ios_base::scientific);
        ofs << std::setiosflags(std::ios_base::fixed);
        ofs << "CIRCLE (";
        ofs << std::setprecision(10) << p[0]->x << " " << std::setprecision(10) << p[0]->y << ")";  //第一个点
        ofs << ' ' << Radius << ' ' << BorderWidth << ' ' << BorderColor.name().toStdString() << ' ' << BorderStyle << ' ' << FillColor.name(QColor::HexArgb).toStdString() << ' ' << FillStyle;
        return QString::fromStdString(ofs.str());
    }
};

class CSector :public Geometry
{
public:
    CSector()
    {
        BorderColor = QColor("#000000");
        FillColor = QColor("#00000000");
        BorderWidth = 3; BorderStyle = 1; FillStyle = 1;
        Radius = 0;
    }
    CSector(QVector<Point*>& geom, double r, double width, std::string bordercolor, int borderstyle, std::string fillcolor, int fillstyle)
    {
        p = geom;
        BorderWidth = width;
        BorderColor = QColor(QString::fromStdString(bordercolor));
        FillColor = QColor(QString::fromStdString(fillcolor));
        BorderStyle = borderstyle;
        FillStyle = fillstyle;
        Radius = r;
    }
    const char* getname()
    {
        return"SECTOR";
    }
    double getArea()
    {
        return PI * Radius * Radius;
    }
    double getLength()
    {
        return 2 * PI * Radius;
    }
    BBox GetBBox() const
    {
        BBox BoundingBox;
        if (p[1]->x < 90)
        {
            if (p[1]->y < 90)
            {
                BoundingBox.xmin = p[0]->x;
                BoundingBox.ymin = p[0]->y - Radius * sin(p[1]->y * PI/180);
                BoundingBox.xmax = p[0]->x + Radius * cos(p[1]->x * PI/180);
                BoundingBox.ymax = p[0]->y;
            }
            else if (p[1]->y < 180)
            {
                BoundingBox.xmin = p[0]->x + Radius * cos(p[1]->y * PI/180);
                BoundingBox.ymin = p[0]->y - Radius;
                BoundingBox.xmax = p[0]->x + Radius * cos(p[1]->x * PI/180);
                BoundingBox.ymax = p[0]->y;
            }
            else if (p[1]->y < 270)
            {
                BoundingBox.xmin = p[0]->x - Radius;
                BoundingBox.ymin = p[0]->y - Radius;
                BoundingBox.xmax = p[0]->x + Radius;
                BoundingBox.ymax = p[0]->y - Radius * sin(p[1]->y * PI/180);
            }
            else
            {
                BoundingBox.xmin = p[0]->x - Radius;
                BoundingBox.ymin = p[0]->y - Radius;
                BoundingBox.xmax = (p[0]->x + Radius * sin(p[1]->y * PI / 180))> (p[0]->x + Radius * sin(p[0]->y * PI / 180))?(p[0]->x + Radius * sin(p[1]->y * PI / 180)):p[0]->x + Radius * sin(p[0]->y * PI / 180);
                BoundingBox.ymax = p[0]->y - Radius;
            }
        }
        else if (p[1]->x < 180)
        {
            if (p[1]->y < 180)
            {
                BoundingBox.xmin = p[0]->x + Radius * cos(p[1]->y * PI/180);
                BoundingBox.ymin = p[0]->y - Radius * sin(p[1]->x * PI/180);
                BoundingBox.xmax = p[0]->x ;
                BoundingBox.ymax = p[0]->y;
            }
            else if (p[1]->y < 270)
            {
                BoundingBox.xmin = p[0]->x - Radius;
                BoundingBox.ymin = p[0]->y - Radius * sin(p[1]->x * PI/180);
                BoundingBox.xmax = p[0]->x;
                BoundingBox.ymax = p[0]->y - Radius * sin(p[1]->y * PI/180);
            }
            else
            {
                BoundingBox.xmin = p[0]->x - Radius;
                BoundingBox.ymin = p[0]->y - Radius * sin(p[1]->x * PI/180);
                BoundingBox.xmax = p[0]->x + Radius * cos(p[1]->y * PI/180);
                BoundingBox.ymax = p[0]->y - Radius;
            }
        }
		else if (p[1]->x < 270)
		{
            if (p[1]->y < 270)
            {
                BoundingBox.xmin = p[0]->x + Radius * cos(p[1]->x * PI/180);
                BoundingBox.ymin = p[0]->y;
                BoundingBox.xmax = p[0]->x;
                BoundingBox.ymax = p[0]->y - Radius * sin(p[1]->y * PI/180);
            }
            else
            {
                BoundingBox.xmin = p[0]->x + Radius * cos(p[1]->x * PI/180);
                BoundingBox.ymin = p[0]->y ;
                BoundingBox.xmax = p[0]->x + Radius * cos(p[1]->y * PI/180);
                BoundingBox.ymax = p[0]->y + Radius;
            }
		}
		else
		{
            BoundingBox.xmin = p[0]->x;
            BoundingBox.ymin = p[0]->y;
            BoundingBox.xmax = p[0]->x + Radius * cos(p[1]->y * PI/180);
            BoundingBox.ymax = p[0]->y - Radius * sin(p[1]->x * PI/180);
		}
        BoundingBox.center.x=(BoundingBox.xmax+ BoundingBox.xmin)/2;
        BoundingBox.center.y=(BoundingBox.ymax+ BoundingBox.ymin)/2;
        return BoundingBox;
    }

    QString wkt()
    {
        BBox box = GetBBox();
        double x0 = box.xmax - Radius;
        double y0 = box.center.y;
        std::ostringstream ofs;
        //ofs<<setiosflags(ios_base::scientific);
        ofs << std::setiosflags(std::ios_base::fixed);
        ofs << "POLYGON ((";
        Point temp;
        ofs << std::setprecision(10) << p[0]->x << " " << std::setprecision(10) << p[0]->y << ",";
        for (double i = p[1]->x; i <= p[1]->y; i += 1)
        {
            temp.x = x0 + Radius * cos(PI * i / 180);
            temp.y = y0 + Radius * sin(PI * i / 180);
            ofs << std::setprecision(10) << temp.x << " " << std::setprecision(10) << temp.y << ",";
        }
        ofs << std::setprecision(10) << p[0]->x << " " << std::setprecision(10) << p[0]->y << "))";
        return QString::fromStdString(ofs.str());
    }

    QString gisk()
    {
        std::ostringstream ofs;
        //ofs<<setiosflags(ios_base::scientific);
        ofs << std::setiosflags(std::ios_base::fixed);
        ofs << "SECTOR (";
        ofs << std::setprecision(10) << p[0]->x << " " << std::setprecision(10) << p[0]->y << ", ";  //第一个点
        ofs << std::setprecision(10) << p[1]->x << " " << std::setprecision(10) << p[1]->y << ")";  //第一个点
        ofs << ' ' << Radius << ' ' << BorderWidth << ' ' << BorderColor.name().toStdString() << ' ' << BorderStyle << ' ' << FillColor.name(QColor::HexArgb).toStdString() << ' ' << FillStyle;
        return QString::fromStdString(ofs.str());
    }
};
