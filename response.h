//#pragma once
//
//#include <QWidget>
//#include <QApplication>
//
//#include "getview.h"
//
//extern QWidget* MainWindow;
//
//class Handle
//{
//public:
//    Handle() { }
//    virtual ~Handle()  { }
//    virtual const char* getName() = 0;
//    virtual void mousePressEvent(double x, double y) = 0;
//    virtual void mouseMoveEvent(double x, double y) = 0;
//    virtual void mouseReleaseEvent(double x, double y) = 0;
//};
//
////extern Handle* ppt;
//extern Layer* currentLayer;
//extern int selectView;
//
//class SelectHandle : public Handle
//{
//public:
//    SelectHandle() { MainWindow->setCursor(Qt::CrossCursor); }
//    ~SelectHandle() { }
//    const char* getName()
//    {
//        return "SelectHandle";
//    }
//    void mousePressEvent(double x, double y)
//    {
//        currentLayer->select(x, y);
//    }
//    void mouseMoveEvent(double x, double y)
//    {
//
//    }
//    void mouseReleaseEvent(double x, double y)
//    {
//
//    }
//};
//
//class BoxSelectHandle : public Handle
//{
//public:
//    BoxSelectHandle() { QApplication::setOverrideCursor(Qt::CrossCursor);}
//    ~BoxSelectHandle() { }
//    const char* getName()
//    {
//        return "BoxSelectHandle";
//    }
//    /*void mousePressEvent(double x, double y)
//    {
//        currentLayer->boxSelect(x, y,);
//    }*/
//    void mouseMoveEvent(double x, double y)
//    {
//
//    }
//    void mouseReleaseEvent(double x, double y)
//    {
//        currentLayer->boxSelect(x, y);
//    }
//};
//
//class CreateHandle : public Handle
//{
//public:
//    CreateHandle() { MainWindow->setCursor(Qt::CrossCursor); }
//    ~CreateHandle() { }
//    const char* getName()
//    {
//        return "CreateHandle";
//    }
//    void mousePressEvent(double x, double y)
//    {
//        currentLayer->getVector().first()->create(x, y);
//    }
//    void mouseMoveEvent(double x, double y)
//    {
//
//    }
//    void mouseReleaseEvent(double x, double y)
//    {
//
//    }
//};
//
//class EditHandle : public Handle
//{
//public:
//    EditHandle() { MainWindow->setCursor(Qt::SizeAllCursor); }
//    ~EditHandle() { }
//    const char* getName()
//    {
//        return "EditHandle";
//    }
//    void mousePressEvent(double x, double y)
//    {
//        if (selectView != -1)
//        {
//            currentLayer->getVector()[selectView]->edit(x, y);
//        }
//    }
//    void mouseMoveEvent(double x, double y)
//    {
//        if (selectView != -1)
//            currentLayer->getVector()[selectView]->editMoveAuxiliary(x, y);
//    }
//    void mouseReleaseEvent(double x, double y)
//    {
//        if (selectView != -1)
//        {
//            currentLayer->getVector()[selectView]->editReleaseAuxiliary();
//        }
//    }
//};
//
//class TranslateHandle : public Handle
//{
//public:
//    TranslateHandle() { MainWindow->setCursor(Qt::SizeAllCursor); }
//    ~TranslateHandle() { }
//    const char* getName()
//    {
//        return "TranslateHandle";
//    }
//    void mousePressEvent(double x, double y)
//    {
//        if (selectView != -1)
//        {
//            currentLayer->getVector()[selectView]->translate(x, y);
//        }
//    }
//    void mouseMoveEvent(double x, double y)
//    {
//        if (selectView != -1)
//            currentLayer->getVector()[selectView]->translate(x, y);
//    }
//    void mouseReleaseEvent(double x, double y)
//    {
//
//    }
//};
//
//class RotateHandle : public Handle
//{
//public:
//    RotateHandle() { MainWindow->setCursor(Qt::SizeAllCursor); }
//    ~RotateHandle() { }
//    const char* getName()
//    {
//        return "RotateHandle";
//    }
//    void mousePressEvent(double x, double y)
//    {
//        if (selectView != -1)
//        {
//            currentLayer->getVector()[selectView]->rotate(x, y);
//        }
//    }
//    void mouseMoveEvent(double x, double y)
//    {
//        if (selectView != -1)
//            currentLayer->getVector()[selectView]->rotate(x, y);
//    }
//    void mouseReleaseEvent(double x, double y)
//    {
//
//    }
//};
//
//class ScaleHandle : public Handle
//{
//public:
//    ScaleHandle() { MainWindow->setCursor(Qt::SizeAllCursor); }
//    ~ScaleHandle() { }
//    const char* getName()
//    {
//        return "ScaleHandle";
//    }
//    void mousePressEvent(double x, double y)
//    {
//        if (selectView != -1)
//        {
//            currentLayer->getVector()[selectView]->scale(x, y);
//        }
//    }
//    void mouseMoveEvent(double x, double y)
//    {
//        if (selectView != -1)
//            currentLayer->getVector()[selectView]->scale(x, y);
//    }
//    void mouseReleaseEvent(double x, double y)
//    {
//
//    }
//};
//
//class DeleteHandle : public Handle
//{
//public:
//    DeleteHandle()
//    {
//        if (selectView != -1)
//        {
//            currentLayer->del();
//        }
//    }
//    ~DeleteHandle() { }
//    const char* getName()
//    {
//        return "DeleteHandle";
//    }
//    void mousePressEvent(double x, double y)
//    {
//
//    }
//    void mouseMoveEvent(double x, double y)
//    {
//
//    }
//    void mouseReleaseEvent(double x, double y)
//    {
//
//    }
//};
//
//class ModifyBorderColorHandle : public Handle
//{
//public:
//    ModifyBorderColorHandle()
//    {
//        if (selectView != -1)
//        {
//            currentLayer->getVector()[selectView]->modifyBorderColor();
//        }
//    }
//    ~ModifyBorderColorHandle() { }
//    const char* getName()
//    {
//        return "ModifyBorderColorHandle";
//    }
//    void mousePressEvent(double x, double y)
//    {
//
//    }
//    void mouseMoveEvent(double x, double y)
//    {
//
//    }
//    void mouseReleaseEvent(double x, double y)
//    {
//
//    }
//};
//
//class ModifyFillColorHandle : public Handle
//{
//public:
//    ModifyFillColorHandle()
//    {
//        if (selectView != -1)
//        {
//            currentLayer->getVector()[selectView]->modifyFillColor();
//        }
//    }
//    ~ModifyFillColorHandle() {}
//    const char* getName()
//    {
//        return "ModifyFillColorHandle";
//    }
//    void mousePressEvent(double x, double y)
//    {
//
//    }
//    void mouseMoveEvent(double x, double y)
//    {
//
//    }
//    void mouseReleaseEvent(double x, double y)
//    {
//
//    }
//};
//
//class ModifyBorderWidthHandle : public Handle
//{
//public:
//    ModifyBorderWidthHandle()
//    {
//        if (selectView != -1)
//        {
//            currentLayer->getVector()[selectView]->modifyBorderWidth();
//        }
//    }
//    ~ModifyBorderWidthHandle() { }
//    const char* getName()
//    {
//        return "ModifyBorderWidthHandle";
//    }
//    void mousePressEvent(double x, double y)
//    {
//
//    }
//    void mouseMoveEvent(double x, double y)
//    {
//
//    }
//    void mouseReleaseEvent(double x, double y)
//    {
//
//    }
//};
