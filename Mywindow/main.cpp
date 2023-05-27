#include "Mywindow.h"
#include <QtWidgets/QApplication>
#include <QtWidgets>
//
//class CustomWindow : public QWidget
//{
//public:
//    CustomWindow(QWidget* parent = nullptr)
//        : QWidget(parent), isDragging(false)
//    {
//        setFixedSize(400, 300);
//        setStyleSheet("background-color: white;");
//        setWindowTitle("Custom Window");
//    }
//
//protected:
//    void mousePressEvent(QMouseEvent* event) override
//    {
//        if (event->button() == Qt::LeftButton)
//        {
//            isDragging = true;
//            lastPos = event->pos();
//        }
//        QWidget::mousePressEvent(event);
//    }
//
//    void mouseMoveEvent(QMouseEvent* event) override
//    {
//        if (isDragging)
//        {
//            QPoint delta = event->pos() - lastPos;
//            move(pos() + delta);
//        }
//        QWidget::mouseMoveEvent(event);
//    }
//
//    void mouseReleaseEvent(QMouseEvent* event) override
//    {
//        if (event->button() == Qt::LeftButton)
//        {
//            isDragging = false;
//        }
//        QWidget::mouseReleaseEvent(event);
//    }
//
//private:
//    bool isDragging;
//    QPoint lastPos;
//};
//
//int main(int argc, char** argv)
//{
//    QApplication app(argc, argv);
//
//    CustomWindow window;
//    window.show();
//
//    return app.exec();
//}


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Mywindow w;
    w.show();
    //QGraphicsScene scene;
    //scene.setSceneRect(0, 0, 500, 500);

    //QGraphicsRectItem* rect = scene.addRect(QRectF(0, 0, 100, 100), QPen(Qt::black), QBrush(Qt::red));
    //rect->setFlag(QGraphicsItem::ItemIsMovable);

    //QGraphicsView view(&scene);
    //view.setFixedSize(400, 300);
    //view.setRenderHint(QPainter::Antialiasing);
    //view.setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    //view.setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    //view.setAlignment(Qt::AlignLeft | Qt::AlignTop);

    //view.show();

    //// 平移视图到指定的场景坐标点
    //QPointF targetPoint(200, 200);
    //view.centerOn(targetPoint);
    return a.exec();
}
