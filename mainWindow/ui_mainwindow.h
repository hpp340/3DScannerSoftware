/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.3.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>
#include "showmesh.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionNew_N;
    QAction *action_O;
    QAction *actionClose_C;
    QAction *actionSave_S;
    QAction *actionSave_as_A;
    QAction *actionExit_E;
    QAction *actionUndo_U;
    QAction *actionCut_X;
    QAction *actionCopy_C;
    QAction *actionPaste_V;
    QAction *actionSearch;
    QAction *action_6;
    QAction *actionRecon;
    QAction *actionHole_filling;
    QAction *actionRemeshing;
    QWidget *centralWidget;
    showMesh *widget;
    QMenuBar *menuBar;
    QMenu *menuFile_F;
    QMenu *menuEdit_E;
    QMenu *menuHelp_H;
    QMenu *menu_O;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(1446, 1123);
        actionNew_N = new QAction(MainWindow);
        actionNew_N->setObjectName(QStringLiteral("actionNew_N"));
        QIcon icon;
        icon.addFile(QStringLiteral(":/File/images/Files.ico"), QSize(), QIcon::Normal, QIcon::Off);
        actionNew_N->setIcon(icon);
        action_O = new QAction(MainWindow);
        action_O->setObjectName(QStringLiteral("action_O"));
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/File/images/Documents Folder.ico"), QSize(), QIcon::Normal, QIcon::Off);
        action_O->setIcon(icon1);
        actionClose_C = new QAction(MainWindow);
        actionClose_C->setObjectName(QStringLiteral("actionClose_C"));
        actionSave_S = new QAction(MainWindow);
        actionSave_S->setObjectName(QStringLiteral("actionSave_S"));
        QIcon icon2;
        icon2.addFile(QStringLiteral(":/File/images/Save.ico"), QSize(), QIcon::Normal, QIcon::Off);
        actionSave_S->setIcon(icon2);
        actionSave_as_A = new QAction(MainWindow);
        actionSave_as_A->setObjectName(QStringLiteral("actionSave_as_A"));
        actionExit_E = new QAction(MainWindow);
        actionExit_E->setObjectName(QStringLiteral("actionExit_E"));
        actionUndo_U = new QAction(MainWindow);
        actionUndo_U->setObjectName(QStringLiteral("actionUndo_U"));
        actionCut_X = new QAction(MainWindow);
        actionCut_X->setObjectName(QStringLiteral("actionCut_X"));
        actionCopy_C = new QAction(MainWindow);
        actionCopy_C->setObjectName(QStringLiteral("actionCopy_C"));
        actionPaste_V = new QAction(MainWindow);
        actionPaste_V->setObjectName(QStringLiteral("actionPaste_V"));
        actionSearch = new QAction(MainWindow);
        actionSearch->setObjectName(QStringLiteral("actionSearch"));
        action_6 = new QAction(MainWindow);
        action_6->setObjectName(QStringLiteral("action_6"));
        actionRecon = new QAction(MainWindow);
        actionRecon->setObjectName(QStringLiteral("actionRecon"));
        actionHole_filling = new QAction(MainWindow);
        actionHole_filling->setObjectName(QStringLiteral("actionHole_filling"));
        actionRemeshing = new QAction(MainWindow);
        actionRemeshing->setObjectName(QStringLiteral("actionRemeshing"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        widget = new showMesh(centralWidget);
        widget->setObjectName(QStringLiteral("widget"));
        widget->setGeometry(QRect(20, 20, 1406, 1011));
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1446, 38));
        menuFile_F = new QMenu(menuBar);
        menuFile_F->setObjectName(QStringLiteral("menuFile_F"));
        menuEdit_E = new QMenu(menuBar);
        menuEdit_E->setObjectName(QStringLiteral("menuEdit_E"));
        menuHelp_H = new QMenu(menuBar);
        menuHelp_H->setObjectName(QStringLiteral("menuHelp_H"));
        menu_O = new QMenu(menuBar);
        menu_O->setObjectName(QStringLiteral("menu_O"));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        menuBar->addAction(menuFile_F->menuAction());
        menuBar->addAction(menuEdit_E->menuAction());
        menuBar->addAction(menu_O->menuAction());
        menuBar->addAction(menuHelp_H->menuAction());
        menuFile_F->addAction(actionNew_N);
        menuFile_F->addAction(action_O);
        menuFile_F->addAction(actionClose_C);
        menuFile_F->addSeparator();
        menuFile_F->addAction(actionSave_S);
        menuFile_F->addAction(actionSave_as_A);
        menuFile_F->addSeparator();
        menuFile_F->addAction(actionExit_E);
        menuEdit_E->addAction(actionUndo_U);
        menuEdit_E->addSeparator();
        menuEdit_E->addAction(actionCut_X);
        menuEdit_E->addAction(actionCopy_C);
        menuEdit_E->addAction(actionPaste_V);
        menuEdit_E->addSeparator();
        menuEdit_E->addAction(actionSearch);
        menuHelp_H->addAction(action_6);
        menuHelp_H->addSeparator();
        menu_O->addAction(actionRecon);
        menu_O->addAction(actionHole_filling);
        menu_O->addAction(actionRemeshing);
        menu_O->addSeparator();
        mainToolBar->addAction(actionNew_N);
        mainToolBar->addAction(action_O);
        mainToolBar->addAction(actionSave_S);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0));
        actionNew_N->setText(QApplication::translate("MainWindow", "\346\226\260\345\273\272(&N)", 0));
        actionNew_N->setShortcut(QApplication::translate("MainWindow", "Ctrl+N", 0));
        action_O->setText(QApplication::translate("MainWindow", "\346\211\223\345\274\200(&O)", 0));
        action_O->setShortcut(QApplication::translate("MainWindow", "Ctrl+O", 0));
        actionClose_C->setText(QApplication::translate("MainWindow", "\345\205\263\351\227\255(&C)", 0));
        actionSave_S->setText(QApplication::translate("MainWindow", "\344\277\235\345\255\230(&S)", 0));
        actionSave_S->setShortcut(QApplication::translate("MainWindow", "Ctrl+S", 0));
        actionSave_as_A->setText(QApplication::translate("MainWindow", "\345\217\246\345\255\230\344\270\272...(&A)", 0));
        actionExit_E->setText(QApplication::translate("MainWindow", "\351\200\200\345\207\272(&E)", 0));
        actionExit_E->setShortcut(QApplication::translate("MainWindow", "Ctrl+E", 0));
        actionUndo_U->setText(QApplication::translate("MainWindow", "\346\222\244\351\224\200(&U)", 0));
        actionCut_X->setText(QApplication::translate("MainWindow", "\345\211\252\345\210\207(&X)", 0));
        actionCopy_C->setText(QApplication::translate("MainWindow", "\345\244\215\345\210\266(&C)", 0));
        actionPaste_V->setText(QApplication::translate("MainWindow", "\347\262\230\350\264\264(&V)", 0));
        actionSearch->setText(QApplication::translate("MainWindow", "\346\237\245\346\211\276(&F)", 0));
        action_6->setText(QApplication::translate("MainWindow", "\347\211\210\346\234\254\350\257\264\346\230\216", 0));
        actionRecon->setText(QApplication::translate("MainWindow", "Recon", 0));
        actionHole_filling->setText(QApplication::translate("MainWindow", "Hole filling", 0));
        actionRemeshing->setText(QApplication::translate("MainWindow", "Remeshing", 0));
        menuFile_F->setTitle(QApplication::translate("MainWindow", "\346\226\207\344\273\266(&F)", 0));
        menuEdit_E->setTitle(QApplication::translate("MainWindow", "\347\274\226\350\276\221(&E)", 0));
        menuHelp_H->setTitle(QApplication::translate("MainWindow", "\345\270\256\345\212\251(&H)", 0));
        menu_O->setTitle(QApplication::translate("MainWindow", "\346\223\215\344\275\234(&O)", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
