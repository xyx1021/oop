/********************************************************************************
** Form generated from reading UI file 'Mywindow.ui'
**
** Created by: Qt User Interface Compiler version 5.9.9
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MYWINDOW_H
#define UI_MYWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MywindowClass
{
public:
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QWidget *centralWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MywindowClass)
    {
        if (MywindowClass->objectName().isEmpty())
            MywindowClass->setObjectName(QStringLiteral("MywindowClass"));
        MywindowClass->resize(600, 400);
        menuBar = new QMenuBar(MywindowClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        MywindowClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MywindowClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MywindowClass->addToolBar(mainToolBar);
        centralWidget = new QWidget(MywindowClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        MywindowClass->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(MywindowClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MywindowClass->setStatusBar(statusBar);

        retranslateUi(MywindowClass);

        QMetaObject::connectSlotsByName(MywindowClass);
    } // setupUi

    void retranslateUi(QMainWindow *MywindowClass)
    {
        MywindowClass->setWindowTitle(QApplication::translate("MywindowClass", "Mywindow", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MywindowClass: public Ui_MywindowClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MYWINDOW_H
