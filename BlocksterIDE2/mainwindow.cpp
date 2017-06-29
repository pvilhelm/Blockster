#include "mainwindow.h"

#include <QLayout>

#include "blockster_session.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    this->setObjectName(QStringLiteral("Blockster"));
    this->resize(700, 600);
    menuBar = new QMenuBar(this);
    menuBar->setObjectName(QStringLiteral("menuBar"));
    this->setMenuBar(menuBar);
    mainToolBar = new QToolBar(this);
    mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
    this->addToolBar(mainToolBar);

    statusBar = new QStatusBar(this);
    statusBar->setObjectName(QStringLiteral("statusBar"));
    this->setStatusBar(statusBar);
    QMetaObject::connectSlotsByName(this);

    BlocksterSession* centralWidget = new BlocksterSession();
    this->setCentralWidget(centralWidget);

    centralWidget->addLibBrowser();
    centralWidget->addProgram();
}

MainWindow::~MainWindow()
{

}
