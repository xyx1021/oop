#pragma once
#include <QObject>
#include <QToolBar>

#include "mousemode.h"

extern QToolBar* toolbar, * addbar, * editbar, *stylebar;
extern MouseMode* currentMode;
class Command : public QObject
{
public:
    Command(){}
    virtual void Invoke()= 0;
};

class CommandBrowse : public Command
{
    Q_OBJECT
public:
    CommandBrowse(){}
public slots:
    void Invoke()
    {
        QApplication::setOverrideCursor(Qt::OpenHandCursor);
        addbar->setVisible(false);
        editbar->setVisible(false);
        stylebar->setVisible(false);
        toolbar->actions().at(0)->setIcon(QIcon(":/GISLite/icons/blue-handcursor-32.png"));
        toolbar->actions().at(1)->setIcon(QIcon(":/GISLite/icons/cursor-32.png"));
        toolbar->actions().at(2)->setIcon(QIcon(":/GISLite/icons/plus-32.png"));
        if (currentMode!= nullptr)
        {
            delete currentMode;
        }
        currentMode = new BrowseMode;
    }
};

class CommandSelect : public Command
{
    Q_OBJECT
public:
    CommandSelect(){}
public slots:
    void Invoke()
    {
        QApplication::setOverrideCursor(Qt::ArrowCursor);
        addbar->setVisible(false);
        editbar->setVisible(true);
        stylebar->setVisible(true);
        toolbar->actions().at(0)->setIcon(QIcon(":/GISLite/icons/handcursor-32.png"));
        toolbar->actions().at(1)->setIcon(QIcon(":/GISLite/icons/red-cursor-32.png"));
        toolbar->actions().at(2)->setIcon(QIcon(":/GISLite/icons/plus-32.png"));
   
        if (currentMode != nullptr)
        {
            delete currentMode;
        }
        currentMode = new ChooseMode;
    }
};

class CommandAdd:public Command
{
    Q_OBJECT
public:
    CommandAdd(){}
public slots:
    void Invoke()
    {
        QApplication::setOverrideCursor(Qt::ArrowCursor);
        addbar->setVisible(true);
        editbar->setVisible(false);
        stylebar->setVisible(false);
        toolbar->actions().at(0)->setIcon(QIcon(":/GISLite/icons/handcursor-32.png"));
        toolbar->actions().at(1)->setIcon(QIcon(":/GISLite/icons/cursor-32.png"));
        toolbar->actions().at(2)->setIcon(QIcon(":/GISLite/icons/green-plus-32.png"));
        if (currentMode != nullptr)
        {
            delete currentMode;
        }
        currentMode = new DrawMode;
    }
};

class CommandAddPoint:public Command
{
    Q_OBJECT
public:
    CommandAddPoint(){}
public slots:
    void Invoke()
    {
        if (currentLayer == LayerList[0])
        {
            QMessageBox::information(nullptr, "Notice", "Select a layer to add in!");
            return;
        }
        QApplication::setOverrideCursor(Qt::CrossCursor);
        if (currentMode != nullptr)
        {
            delete currentMode;
        }
        currentMode = new DrawPointMode;
    }
};

class CommandAddLine :public Command
{
    Q_OBJECT
public:
    CommandAddLine(){}
public slots:
    void Invoke()
    {
        if (currentLayer == LayerList[0])
        {
            QMessageBox::information(nullptr, "Notice", "Select a layer to add in!");
            return;
        }
        QApplication::setOverrideCursor(Qt::CrossCursor);
        if (currentMode != nullptr)
        {
            delete currentMode;
        }
        currentMode = new DrawLineMode;
    }
};

class CommandAddPolyline :public Command
{
    Q_OBJECT
public:
    CommandAddPolyline() {}
public slots:
    void Invoke()
    {
        if (currentLayer == LayerList[0])
        {
            QMessageBox::information(nullptr, "Notice", "Select a layer to add in!");
            return;
        }
        QApplication::setOverrideCursor(Qt::CrossCursor);
        if (currentMode != nullptr)
        {
            delete currentMode;
        }
        currentMode = new DrawPolylineMode;
    }
};

class CommandAddRectangle :public Command
{
    Q_OBJECT
public:
    CommandAddRectangle(){}
public slots:
    void Invoke()
    {
        if (currentLayer == LayerList[0])
        {
            QMessageBox::information(nullptr, "Notice", "Select a layer to add in!");
            return;
        }
        QApplication::setOverrideCursor(Qt::CrossCursor);
        if (currentMode != nullptr)
        {
            delete currentMode;
        }
        currentMode = new DrawRectangleMode;
    }
};
 
class CommandAddPolygon :public Command
{
    Q_OBJECT
public:
    CommandAddPolygon(){}
public slots:
    void Invoke()
    {
        if (currentLayer == LayerList[0])
        {
            QMessageBox::information(nullptr, "Notice", "Select a layer to add in!");
            return;
        }
        QApplication::setOverrideCursor(Qt::CrossCursor);
        if (currentMode != nullptr)
        {
            delete currentMode;
        }
        currentMode = new DrawPolygonMode;
    }
};

class CommandAddCircle :public Command
{
    Q_OBJECT
public:
    CommandAddCircle(){}
public slots:
    void Invoke()
    {
        if (currentLayer == LayerList[0])
        {
            QMessageBox::information(nullptr, "Notice", "Select a layer to add in!");
            return;
        }
        QApplication::setOverrideCursor(Qt::CrossCursor);
        if (currentMode != nullptr)
        {
            delete currentMode;
        }
        currentMode = new DrawCircleMode;
    }
};
class CommandAddSector :public Command {
    Q_OBJECT
public:
    CommandAddSector(){}
public slots:
    void Invoke()
    {
        if (currentLayer == LayerList[0])
        {
            QMessageBox::information(nullptr, "Notice", "Select a layer to add in!");
            return;
        }
        QApplication::setOverrideCursor(Qt::CrossCursor);
        if (currentMode != nullptr)
        {
            delete currentMode;
        }
        currentMode = new DrawSectorMode;
    }
};





class CommandBorderColor :public Command
{
    Q_OBJECT
public:
    CommandBorderColor() {}
public slots:
    void Invoke()
    {
        QColorDialog dialog;
        QColor bordercolor= dialog.getColor(QColor(), nullptr, "Selecting Border Color", QColorDialog::ShowAlphaChannel);
        if (bordercolor.isValid())
        {
            for (auto p : selectSet)
            {
                p->get_borderColor() = bordercolor;
            }
        }
        ChooseMode::style();
    }
};

class CommandBorderStyle :public Command
{
    Q_OBJECT
public:
    CommandBorderStyle () {}
public slots:
    void Invoke()
    {
        bool ok;
        int value = QInputDialog::getInt(nullptr, "Select Border Style", "Select Border Style", 1, 1, 10, 1, &ok);
        if (ok)
        {
            for (auto p : selectSet)
            {
                p->get_borderStyle() = value;
            }
        }
    }
};

class CommandFillColor :public Command
{
    Q_OBJECT
public:
    CommandFillColor() {}
public slots:
    void Invoke()
    {
        QColorDialog dialog;
        QColor fillcolor = dialog.getColor(QColor(), nullptr, "Selecting Fill Color", QColorDialog::ShowAlphaChannel);
        if (fillcolor.isValid())
        {
            for (auto p : selectSet)
            {
                p->get_fillColor() = fillcolor;
            }
        }
        ChooseMode::style();
    }
};

class CommandFillStyle :public Command
{
    Q_OBJECT
public:
    CommandFillStyle() {}
public slots:
    void Invoke()
    {
        bool ok;
        int value = QInputDialog::getInt(nullptr, "Select Fill Style", "Select Fill Style", 1, 1, 10, 1, &ok);
        if (ok)
        {
            for (auto p : selectSet)
            {
                p->get_fillStyle() = value;
            }
        }
    }
};

class CommandWidth :public Command
{
    Q_OBJECT
public:
    CommandWidth() {}
public slots:
    void Invoke()
    {
        bool ok;
        QString textValue = QInputDialog::getText(nullptr, "Enter Width", "Enter Width:", QLineEdit::Normal, "", &ok);
        if (ok && !textValue.isEmpty())
        {
            double value = textValue.toDouble();
            for (auto p : selectSet)
            {
                p->get_borderWidth() = value;
            }
        }
    }
};

class CommandEdit :public Command
{
    Q_OBJECT
public:
    CommandEdit() {}
public slots:
    void Invoke()
    {
        QApplication::setOverrideCursor(Qt::DragMoveCursor);
        if (currentMode != nullptr)
        {
            delete currentMode;
        }
        currentMode = new EditMode;
    }
};

class CommandTranslate :public Command
{
    Q_OBJECT
public:
    CommandTranslate() {}
public slots:
    void Invoke()
    {
        QApplication::setOverrideCursor(Qt::DragMoveCursor);
        if (currentMode != nullptr)
        {
            delete currentMode;
        }
        currentMode = new TranslateMode;
    }
};

class CommandRotate :public Command
{
    Q_OBJECT
public:
    CommandRotate() {}
public slots:
    void Invoke()
    {
        QApplication::setOverrideCursor(Qt::DragMoveCursor);
        if (currentMode != nullptr)
        {
            delete currentMode;
        }
        currentMode = new RotateMode;
    }
};

class CommandScale :public Command
{
    Q_OBJECT
public:
    CommandScale() {}
public slots:
    void Invoke()
    {
        QApplication::setOverrideCursor(Qt::DragMoveCursor);
        if (currentMode != nullptr)
        {
            delete currentMode;
        }
        currentMode = new ScaleMode;
    }
};
