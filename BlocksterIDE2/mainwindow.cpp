#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "program_container.h"

#include <QLayout>

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
    centralWidget = new QWidget(this);
    centralWidget->setObjectName(QStringLiteral("centralWidget"));
    this->setCentralWidget(centralWidget);
    statusBar = new QStatusBar(this);
    statusBar->setObjectName(QStringLiteral("statusBar"));
    this->setStatusBar(statusBar);
    QMetaObject::connectSlotsByName(this);



    ProgramContainer* pc = new ProgramContainer();
    this->setCentralWidget(pc);
}

MainWindow::~MainWindow()
{

}
