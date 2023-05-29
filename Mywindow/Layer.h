#pragma once

#include <QVector>
#include <QGraphicsItem>
#include <QGraphicsView>


class Layer
{
private:
	QVector<Shape*> shapes;
	QString savingPath;//图层名或存储路径
	BBox box;
	DataReader* dataReader;//图层的数据读取器
	bool isSaved;

public:
    Layer();
    void createFromShapefile(); //由shapefile创建Layer
    void createFromTextFile(); //由文本文件创建Layer
    void createFromDBTable();//由数据库表创建Layer
    void saveLayerInTextFile();//将图层保存到文本文件中
    bool deleteShapeFromLayer(Shape* shape);//从图层中删除图形
    void addShapeToLayer(Shape* shape);
    QGraphicsItemGroup* transformToItemGroup(Shape* shape);

    QVector<Shape*> getShapes() const;
    QString getSavingPath() const;
    void setSavingPath(const QString& value);
    void mapToScene(QGraphicsView* view);

};

