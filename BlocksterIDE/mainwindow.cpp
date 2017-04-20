#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "programcontainer.h"

#include <QInputDialog>
#include <QDir>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    libBrowser = new LibBrowser(this,"C:/Users/petter/Source/Repos/Blockster/Blockster/blocks");
    this->addDockWidget(Qt::RightDockWidgetArea,libBrowser);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionNew_program_triggered()
{
    bool ok;
    QString text = QInputDialog::getText(this, tr("Name your program"),
                                         tr("Program name:"), QLineEdit::Normal,
                                         "program1", &ok);
    if (!ok || text.isEmpty())
        return;


    ProgramContainer* prgC = new ProgramContainer(this,text);


    if(blocksterS.openPrograms.isEmpty()){
        this->addDockWidget(Qt::TopDockWidgetArea,prgC);
    }
    else{
        this->addDockWidget(Qt::TopDockWidgetArea,prgC);
        this->tabifyDockWidget(blocksterS.openPrograms.last(),prgC);
    }
    blocksterS.openPrograms.append(prgC);



}
