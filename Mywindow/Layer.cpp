#include "Layer.h"


Layer::Layer()
{
    isSaved = false;
    savingPath = "";
}

QVector<Shape*> Layer::getShapes() const
{
    return shapes;
}

QString Layer::getSavingPath() const
{
    return savingPath;
}

void Layer::setSavingPath(const QString& value)
{
    savingPath = value;
}

void Layer::createFromShapefile() {
    dataReader = new Shapefile;
    dataReader->open(savingPath);
    ((Shapefile*)dataReader)->GetBoundingBox(box);
    shapes = dataReader->read();
}
void Layer::createFromTextFile() {
    dataReader = new FileOperator;
    dataReader->open(savingPath);
    shapes = dataReader->read();
}
void Layer::createFromDBTable() {
    dataReader = new DBManager;
    ((DBManager*)dataReader)->open(savingPath);
    shapes = dataReader->read();
}
void Layer::saveLayerInTextFile() {
    dataReader = new FileOperator;
    for (auto item : shapes)
    {
        dataReader->open(savingPath);
        dataReader->write(item);
    }
    ((FileOperator*)dataReader)->close();
    isSaved = true;
}

bool Layer::deleteShapeFromLayer(Shape* shape) {
    for (auto it = shapes.begin(); it != shapes.end(); it++) {
        if (*it == shape)
        {
            shapes.erase(it);
            return true;
        }
    }
    return false;
}

void Layer::addShapeToLayer(Shape* shape) {
    shapes.push_back(shape);
}
QGraphicsItemGroup* Layer::transformToItemGroup(Shape* shape) {


}

void Layer::mapToScene(QGraphicsView* view) {
    for (auto item : shapes)
    {
        for (auto it = item->begin(); it != item->end(); it++)
        {
            Point pt = *it;
            QPointF ptf = view->mapToScene(QPoint(pt.x, pt.y));
            *it = { ptf.x(), ptf.y() };
        }
    }
}














