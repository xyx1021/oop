#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_GISLite.h"

#include <QMainWindow>
#include <QMenuBar>
#include <QAction>
#include <QFile>
#include <QFileDialog>
#include <QVector>
#include <sstream>
#include <QString>
#include <QListWidgetItem>
#include <QMouseEvent>
#include <QListWidget> 
#include <QToolBar>
#include <QLinearGradient>
#include <QPainter>

#include "basic.h"
#include "geometry.h"
#include "file.h"
#include "getview.h"
#include "mousemode.h"
#include "Command.h"
//#i??nclude "Header.h"

#define PI 3.1416

extern int selectSetIndex;

namespace Ui { class GISLite; }

class GISLite : public QMainWindow
{
    Q_OBJECT

public:
    GISLite(QWidget *parent = nullptr);
    ~GISLite();
    void getView(QVector<Geometry*>, int);
    void resizeEvent(QResizeEvent* event)
    {
        listWidget->setGeometry(this->width() - listWidget->width(), 20+toolbar->height()+menuBar->height(), 200, this->height() - 40 - toolbar->height()-menuBar->height());
    }
    void setState(MouseMode* state)
    {
        if (currentMode)
        {
            delete currentMode;
        }
        currentMode = state;
    }

protected:
    void paintEvent(QPaintEvent*)
    {
        QPainter painter(this);
        painter.scale(windowScaletime, windowScaletime);
        painter.translate(windowTranslateAccumalation.x , windowTranslateAccumalation.y);
        if (LayerList.count() > 1)
        {
            int start = selectSetIndex == 0 ? 1 : selectSetIndex;
            int end = selectSetIndex == 0 ? LayerList.count() - 1 : selectSetIndex;
            for (int i = start; i <= end; i++)
            {
                for (int j = 0; j < LayerList[i]->getVector().size(); j++)
                {
                    (LayerList[i]->getVector())[j]->show(painter);
                }
            }
        }
           
        //if (currentLayer != nullptr)
        //{

        //}
        //if (!selectSet.empty())
        //{
        //    for (int i = 0; i < selectSet.size(); i++)
        //    {
        //        selectSet[i]->show(painter);
        //    }
        //}
        if (waiting == 1)
        {
            currentMode->displaySelectBox(painter);
        }
    }
    void mousePressEvent(QMouseEvent* event)
    {
        if (event->button() == Qt::LeftButton )
        {    
            if(waiting==0)
			{                
				waiting = currentMode->handleMousePress1Event(event, windowTranslateBegin, windowTranslateAccumalation, windowScaletime);
                setMouseTracking(waiting);                
			}
			else if(waiting==1)
			{
                waiting = currentMode->handleMousePress2Event(event, windowTranslateBegin, windowTranslateAccumalation, windowScaletime);
                setMouseTracking(waiting);
			}
        }
        else if (event->button() == Qt::RightButton)
        {
            setMouseTracking(false);
            if (waiting == 1)
            {
                currentMode->handleInterupt();
                waiting = 0;
            }
        }
        repaint();
    }
    void mouseMoveEvent(QMouseEvent* event)
    {
        currentMode->handleMouseMoveEvent(event, windowTranslateBegin, windowTranslateEnd, windowTranslateAccumalation, windowScaletime);
        repaint();
    }
    void mouseReleaseEvent(QMouseEvent* event)
    {
 
    }

    void wheelEvent(QWheelEvent* event)
    {
        currentMode->handleWheelEvent(event->delta()>0,windowScaletime);
        repaint();
    }

    void keyPressEvent(QKeyEvent* event) override
    {
        if (event->key() == Qt::Key_Delete)
        {
            int res = QMessageBox::warning(MainWindow, "Delete geometry", "Are you sure you want to permanently delete this geometry?",
                QMessageBox::Ok |
                QMessageBox::Cancel,
                QMessageBox::Cancel);
            if (res == QMessageBox::Ok)
            {
                if (LayerList.count() > 1)
                {
                    int start = selectSetIndex == 0 ? 1 : selectSetIndex;
                    int end = selectSetIndex == 0 ? LayerList.count() - 1 : selectSetIndex;
                    for (int i = start; i <= end; i++)
                    {
                        for (QVector<View*>::iterator  it=LayerList[i]->getVector().begin();it!=LayerList[i]->getVector().end();) 
                        {
                            for (int m = 0; m < selectSet.size(); m++) {
                                if (*it == selectSet[m]) {
                                    it = LayerList[i]->getVector().erase(it);
                                }
                                else { ++it; }
                          }
                        }
                    }
                }
                repaint();
            }
        }
    }

private slots:
    void importFile();
    void openFile();
    void saveFile();
    void onItemClicked(QListWidgetItem* item);
    void onRepositionClicked();
    void on_listWidget_customContextMenuRequested(const QPoint& pos);

private:
    Ui::GISLiteClass* ui;
    QMenuBar* menuBar;
    QListWidget* listWidget;
    double windowScaletime;
    Point windowTranslateBegin=Point(0,0);
    Point windowTranslateEnd=Point(0,0);
    Point windowTranslateAccumalation=Point (0,0);
    int waiting = 0;
};
