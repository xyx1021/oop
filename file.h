#pragma once

#include <fstream>
#include <string>
#include <QDebug>
#include <iostream>


#include "basic.h"
#include "geometry.h"


class Shapefile
{
    std::ifstream ifs;
public:
    bool Open(std::string fname)
    {
        ifs.open(fname, std::ios::binary);
        if (!ifs)
            return false;

        ifs.seekg(0);
        if (GetBigInt() != 9994)
            return false;

        ifs.seekg(24);
        int size = GetBigInt() * 2;

        ifs.seekg(0, std::ios::end);
        int size0 = ifs.tellg();
        if (size != size0)
            return false;

        return true;
    }
    int  GetShapeType()
    {
        ifs.seekg(32);
        return GetLittleInt();
    }
    std::string GetShapeTypeString() { return ""; }
    bool GetBoundingBox(BBox& box)
    {
        ifs.seekg(36);
        box.xmin = GetDouble();
        ifs.seekg(44);
        box.ymin = GetDouble();

        ifs.seekg(52);
        box.xmax = GetDouble();
        ifs.seekg(60);
        box.ymax = GetDouble();
        box.center.x = (box.xmin + box.xmax) / 2;
        box.center.y = (box.ymin + box.ymax) / 2;
        return true;
    }

    class iterator {
        std::ifstream& ifs;
        int      off;
    public:
        iterator(std::ifstream& ifs, int off) :ifs(ifs), off(off) {};
        void operator++(int)
        {
            ifs.seekg(off + 4);
            int size = GetBigInt() * 2;
            off += 8;
            off += size;
        }
        Geometry* operator*()
        {
            ifs.seekg(off + 8);
            int type = GetLittleInt();
            Geometry* pt = NULL;
            if (type == 1)
                pt = new CPoint;
            else if (type == 3)
                pt = new CLine;
            else if (type == 5)
                pt = new CPolygon;
            ifs.seekg(off + 4);
            int size = GetBigInt() * 2;
            char* ptBuf = new char[size];
            ifs.seekg(off + 8);
            ifs.read(ptBuf, size);
            pt->Build(ptBuf, size);
            return pt;
        }
        bool operator!=(iterator oth)
        {
            return off != oth.off;
        }
    private:
        int GetBigInt()
        {
            //cc
            //return (i >> 24 & 0xff) | (i >> 8 & 0xff00) | (i << 8 & 0xff0000) | (i << 24);
            char tmp[4], x[4];
            ifs.read(tmp, sizeof(int));
            x[0] = tmp[3];
            x[1] = tmp[2];
            x[2] = tmp[1];
            x[3] = tmp[0];
            return *(int*)x;
        }
        int GetLittleInt()
        {
            int i;
            ifs.read((char*)&i, sizeof(int));
            return i;
        }
        double GetDouble()
        {
            double d;
            ifs.read((char*)&d, sizeof(double));
            return d;
        }
    };

    iterator begin()
    {
        return iterator(ifs, 100);
    }
    iterator end()
    {
        ifs.seekg(0, std::ios::end);
        int size0 = ifs.tellg();
        return iterator(ifs, size0);
    }

private:
    int GetBigInt()
    {
        char tmp[4], x[4];
        ifs.read(tmp, sizeof(int));
        x[0] = tmp[3];
        x[1] = tmp[2];
        x[2] = tmp[1];
        x[3] = tmp[0];
        return *(int*)x;
    }
    int GetLittleInt()
    {
        int i;
        ifs.read((char*)&i, sizeof(int));
        return i;
    }
    double GetDouble()
    {
        double d;
        ifs.read((char*)&d, sizeof(double));
        return d;
    }
};


class FileData
{
    //QVector<Geometry*>geometry;
    std::fstream file;
public:
    bool getGisk(QString address, QVector<Geometry*>& array)
    {
        Geometry* ptr = NULL;
        file.open(address.toStdString());
        if (!file)
        {
            qDebug() << "The file open defeat!" << endl; return false;
        }
        qDebug() << "The file: " << address << "open successfully!" << endl;//文件打开成功，参数初始化定义

        //QString type;
        std::string typestring;
        std::string bordercolor, fillcolor;
        int borderstyle, fillstyle;
        double width = 1;
        std::string linestring;//按行读取文件
        while (std::getline(file, linestring, '\n'))
        {
            QVector<Point*> p;
            std::stringstream os;
            os << linestring;
            os >> typestring;//读取图形类别
            Point temp;
            char bit;
            os >> bit;

            if (typestring == "POINT")
            {
                os >> temp.x >> temp.y >> bit >> bordercolor >> borderstyle >> fillcolor >> fillstyle;//读取(x,y)
                p.push_back(new Point(temp.x, temp.y));
                ptr = new CPoint(p, bordercolor,borderstyle); //创建点对象
            }
            else if (typestring == "LINESTRING")
            {
				do
				{
					os >> temp.x >> temp.y >> bit;//读取(x,y)
					p.push_back(new Point(temp.x, temp.y));
				} while (bit == ',');
                os >> width >> bordercolor >> borderstyle;//读取(x,y)
				ptr = new CLine(p, width, bordercolor, borderstyle);
			}
			else if (typestring == "POLYGON")
			{
                os >> bit;
                do
                {
                    os >> temp.x >> temp.y >> bit;//读取(x,y)
                    p.push_back(new Point(temp.x, temp.y));
                } while (bit == ',');
                os >> bit >> width >> bordercolor >> borderstyle >> fillcolor >> fillstyle;//读取(x,y)
                ptr = new CPolygon (p, width, bordercolor, borderstyle, fillcolor, fillstyle);
			}
			else if (typestring == "RECTANGLE")
			{
				do
				{
					os >> temp.x >> temp.y >> bit;//读取(x,y)
					p.push_back(new Point(temp.x, temp.y));
				} while (bit == ',');
                os >> width >> bordercolor >> borderstyle >> fillcolor >> fillstyle;//读取(x,y)
				ptr = new CRectangle(p, width, bordercolor, borderstyle, fillcolor, fillstyle);
			}
			else if (typestring == "CIRCLE")
			{
                double r;
                os >> temp.x >> temp.y >> bit >> r >> width;//读取(x,y)
				p.push_back(new Point(temp.x, temp.y));				
                os >> bordercolor >> borderstyle >> fillcolor >> fillstyle;//读取(x,y)
				ptr = new CCircle(p, r, width, bordercolor, borderstyle, fillcolor, fillstyle);
			}
            else if (typestring == "SECTOR")
            {
                double r;
                do
                {
                    os >> temp.x >> temp.y >> bit;//读取(x,y)
                    p.push_back(new Point(temp.x, temp.y));
                } while (bit == ',');
                os >> r >> width >> bordercolor >> borderstyle >> fillcolor >> fillstyle;//读取(x,y)
                ptr = new CSector(p, r, width, bordercolor, borderstyle, fillcolor, fillstyle);
            }
            array.push_back(ptr);
            ptr = NULL;
        }
        file.close();
        delete ptr;
        return true;
    }

    bool getShp(QString fname, QVector<Geometry*>& array)
    {
        //将shp格式转换为自定义格式
        Shapefile shp;
        if (!shp.Open(fname.toStdString()))
        {           
            return false;
        }       
        BBox box;
        shp.GetBoundingBox(box);

        for (Shapefile::iterator it = shp.begin(); it != shp.end(); it++)
        {
            array.push_back(*it);
        }
        return true;
    }

    bool saveData(QString address, QVector<Geometry*> array)
    {
        QFile file(address);
        QTextStream out(&file);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        {            
            return false;
        }       

        for (int i = 0; i < array.size(); i++)
        {
            //存储图形类别
            out << array[i]->gisk()<<'\n';
        }
        file.close();
        return true;
    }

};

