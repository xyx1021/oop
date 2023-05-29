#pragma once

#include <QVector>
#include <QGraphicsItem>
#include <QGraphicsView>


class Layer
{
private:
	QVector<Shape*> shapes;
	QString savingPath;//ͼ������洢·��
	BBox box;
	DataReader* dataReader;//ͼ������ݶ�ȡ��
	bool isSaved;

public:
    Layer();
    void createFromShapefile(); //��shapefile����Layer
    void createFromTextFile(); //���ı��ļ�����Layer
    void createFromDBTable();//�����ݿ����Layer
    void saveLayerInTextFile();//��ͼ�㱣�浽�ı��ļ���
    bool deleteShapeFromLayer(Shape* shape);//��ͼ����ɾ��ͼ��
    void addShapeToLayer(Shape* shape);
    QGraphicsItemGroup* transformToItemGroup(Shape* shape);

    QVector<Shape*> getShapes() const;
    QString getSavingPath() const;
    void setSavingPath(const QString& value);
    void mapToScene(QGraphicsView* view);

};

