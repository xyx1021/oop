#include "Mywindow.h"

Mywindow::Mywindow(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    resize(1000, 800);
   /* QVBoxLayout* layout = new QVBoxLayout(this);
    
    layout->addWidget(m_view);*/
  m_view = new QGraphicsView(this);
   QGraphicsScene* scene = new QGraphicsScene();
    QGraphicsItem* rect = new QGraphicsRectItem(0, 0, 100, 100);
    scene->addItem(rect);
    m_view->setScene(scene);

    m_view->setFixedSize(800, 600);
    m_view->setDragMode(QGraphicsView::ScrollHandDrag);
    m_view->setRenderHint(QPainter::Antialiasing);
    m_view->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    m_view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_view->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    m_view->setStyleSheet("background-color: white;");
    m_scaleFactor = 1.0;
   
 
}
void Mywindow::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        m_lastPos = event->pos();
    }
    QWidget::mousePressEvent(event);

}
void Mywindow::mouseMoveEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton)
    {
        isDragging = true;
        m_lastPos = event->pos();
    }
    QWidget::mousePressEvent(event);
   /* if (event->buttons() & Qt::LeftButton) {
        QPointF delta = (event->pos() - m_lastPos) / m_scaleFactor;
        m_view->centerOn(m_view->mapToScene(m_view->viewport()->rect().center() - delta.toPoint()));
        m_lastPos = event->pos();
    }
    QWidget::mouseMoveEvent(event);*/
}
void Mywindow::mouseReleaseEvent(QMouseEvent* event) {
   /* if (event->button() == Qt::LeftButton)
    {
        isDragging = false;
    }
    QWidget::mouseReleaseEvent(event);*/
    if (isDragging)
    {
        QPointF delta = event->pos() - m_lastPos;
        move((pos() + delta).toPoint());
    }
    QWidget::mouseMoveEvent(event);

}
void Mywindow::wheelEvent(QWheelEvent* event) {
    QPointF mousePos = m_view->mapToScene(event->pos());
    qreal scaleFactor = (event->delta() > 0) ? 1.1 : 0.9;
    m_view->scale(scaleFactor, scaleFactor);
    m_scaleFactor *= scaleFactor;
    QPointF newMousePos = m_view->mapFromScene(mousePos);
    QPointF delta = (event->pos() - newMousePos) / m_scaleFactor;
    m_view->centerOn(m_view->mapToScene(m_view->viewport()->rect().center() - delta.toPoint()));
}
void Mywindow::mouseDoubleClickEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        m_view->resetMatrix();
        m_view->centerOn(0.0, 0.0);
        m_scaleFactor = 1.0;
    }
    QWidget::mouseDoubleClickEvent(event);

}

Mywindow::~Mywindow()
{}
