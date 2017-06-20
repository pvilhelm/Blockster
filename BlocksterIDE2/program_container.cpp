#include "program_container.h"

ProgramContainer::ProgramContainer(QWidget *parent) : QWidget(parent)
{
    this->resize(700, 600);
    root_scene = new ProgramScene(this);

    root_view = new QGraphicsView(root_scene,this);
    root_view->resize(700, 600);
    root_view->show();
}
