#include "programcontainer.h"
#include "programscene.h"
#include "ui_programcontainer.h"

ProgramContainer::ProgramContainer(QWidget *parent, QString program_name) :
    QDockWidget(parent),
    ui(new Ui::ProgramContainer)
{
    ui->setupUi(this);
    setAllowedAreas(Qt::RightDockWidgetArea);
    setWindowTitle(program_name);
    root_scene = new ProgramScene(this);
    root_view = new QGraphicsView(root_scene,this);
    QHBoxLayout *layout = new QHBoxLayout();
    layout->addWidget(root_view);
    this->ui->root->setLayout(layout);

}

ProgramContainer::~ProgramContainer()
{
    delete ui;
}
