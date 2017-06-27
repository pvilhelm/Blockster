#include "mainwindow.h"

#include "program_container.h"
#include "lib_container.h"

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

    QWidget* centralWidget = new QWidget();
    this->setCentralWidget(centralWidget);

    this->addToolBar(mainToolBar);
    QHBoxLayout* hb = new QHBoxLayout(centralWidget);

    statusBar = new QStatusBar(this);
    statusBar->setObjectName(QStringLiteral("statusBar"));
    this->setStatusBar(statusBar);
    QMetaObject::connectSlotsByName(this);

    ProgramContainer* pc = new ProgramContainer();
    hb->addWidget(pc);
    LibContainer* lib_c = new LibContainer();
    hb->addWidget(lib_c);

}

MainWindow::~MainWindow()
{

}
