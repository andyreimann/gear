/********************************************************************************
** Form generated from reading UI file 'geareditor.ui'
**
** Created by: Qt User Interface Compiler version 5.1.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GEAREDITOR_H
#define UI_GEAREDITOR_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "ComponentListView.h"
#include "glcontext.h"

QT_BEGIN_NAMESPACE

class Ui_GEAREditorClass
{
public:
    QAction *actionHelp;
    QAction *actionContribute;
    QAction *actionProject;
    QAction *actionOpen_2;
    QWidget *centralWidget;
    GLContext *renderSurface;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuNew;
    QMenu *menuOptions;
    QMenu *menuAbout;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;
    QDockWidget *loggingWindow;
    QWidget *dockWidgetContents_3;
    QDockWidget *sceneWindow;
    QWidget *dockWidgetContents_15;
    QVBoxLayout *verticalLayout;
    ComponentListView *componentListView;
    QDockWidget *propertiesWindow;
    QWidget *dockWidgetContents_16;

    void setupUi(QMainWindow *GEAREditorClass)
    {
        if (GEAREditorClass->objectName().isEmpty())
            GEAREditorClass->setObjectName(QStringLiteral("GEAREditorClass"));
        GEAREditorClass->resize(1024, 768);
        actionHelp = new QAction(GEAREditorClass);
        actionHelp->setObjectName(QStringLiteral("actionHelp"));
        actionContribute = new QAction(GEAREditorClass);
        actionContribute->setObjectName(QStringLiteral("actionContribute"));
        actionProject = new QAction(GEAREditorClass);
        actionProject->setObjectName(QStringLiteral("actionProject"));
        actionOpen_2 = new QAction(GEAREditorClass);
        actionOpen_2->setObjectName(QStringLiteral("actionOpen_2"));
        centralWidget = new QWidget(GEAREditorClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        renderSurface = new GLContext(centralWidget);
        renderSurface->setObjectName(QStringLiteral("renderSurface"));
        renderSurface->setGeometry(QRect(-1, -1, 701, 581));
        GEAREditorClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(GEAREditorClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1024, 21));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QStringLiteral("menuFile"));
        menuNew = new QMenu(menuFile);
        menuNew->setObjectName(QStringLiteral("menuNew"));
        menuOptions = new QMenu(menuBar);
        menuOptions->setObjectName(QStringLiteral("menuOptions"));
        menuAbout = new QMenu(menuBar);
        menuAbout->setObjectName(QStringLiteral("menuAbout"));
        GEAREditorClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(GEAREditorClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        GEAREditorClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(GEAREditorClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        GEAREditorClass->setStatusBar(statusBar);
        loggingWindow = new QDockWidget(GEAREditorClass);
        loggingWindow->setObjectName(QStringLiteral("loggingWindow"));
        dockWidgetContents_3 = new QWidget();
        dockWidgetContents_3->setObjectName(QStringLiteral("dockWidgetContents_3"));
        loggingWindow->setWidget(dockWidgetContents_3);
        GEAREditorClass->addDockWidget(static_cast<Qt::DockWidgetArea>(8), loggingWindow);
        sceneWindow = new QDockWidget(GEAREditorClass);
        sceneWindow->setObjectName(QStringLiteral("sceneWindow"));
        dockWidgetContents_15 = new QWidget();
        dockWidgetContents_15->setObjectName(QStringLiteral("dockWidgetContents_15"));
        verticalLayout = new QVBoxLayout(dockWidgetContents_15);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        componentListView = new ComponentListView(dockWidgetContents_15);
        componentListView->setObjectName(QStringLiteral("componentListView"));

        verticalLayout->addWidget(componentListView);

        sceneWindow->setWidget(dockWidgetContents_15);
        GEAREditorClass->addDockWidget(static_cast<Qt::DockWidgetArea>(1), sceneWindow);
        propertiesWindow = new QDockWidget(GEAREditorClass);
        propertiesWindow->setObjectName(QStringLiteral("propertiesWindow"));
        dockWidgetContents_16 = new QWidget();
        dockWidgetContents_16->setObjectName(QStringLiteral("dockWidgetContents_16"));
        propertiesWindow->setWidget(dockWidgetContents_16);
        GEAREditorClass->addDockWidget(static_cast<Qt::DockWidgetArea>(2), propertiesWindow);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuOptions->menuAction());
        menuBar->addAction(menuAbout->menuAction());
        menuFile->addAction(menuNew->menuAction());
        menuFile->addAction(actionOpen_2);
        menuNew->addAction(actionProject);
        menuAbout->addAction(actionHelp);
        menuAbout->addAction(actionContribute);

        retranslateUi(GEAREditorClass);

        QMetaObject::connectSlotsByName(GEAREditorClass);
    } // setupUi

    void retranslateUi(QMainWindow *GEAREditorClass)
    {
        GEAREditorClass->setWindowTitle(QApplication::translate("GEAREditorClass", "GEAREditor", 0));
        actionHelp->setText(QApplication::translate("GEAREditorClass", "Help", 0));
        actionContribute->setText(QApplication::translate("GEAREditorClass", "Contribute", 0));
        actionProject->setText(QApplication::translate("GEAREditorClass", "Scene", 0));
        actionOpen_2->setText(QApplication::translate("GEAREditorClass", "Open", 0));
        menuFile->setTitle(QApplication::translate("GEAREditorClass", "File", 0));
        menuNew->setTitle(QApplication::translate("GEAREditorClass", "New...", 0));
        menuOptions->setTitle(QApplication::translate("GEAREditorClass", "Options", 0));
        menuAbout->setTitle(QApplication::translate("GEAREditorClass", "About", 0));
    } // retranslateUi

};

namespace Ui {
    class GEAREditorClass: public Ui_GEAREditorClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GEAREDITOR_H
