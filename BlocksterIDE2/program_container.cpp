#include "program_container.h"

#include <QHBoxLayout>

ProgramContainer::ProgramContainer(QWidget *parent) : QWidget(parent)
{
    this->resize(700, 600);

    QHBoxLayout* ptr_layout = new QHBoxLayout(this);

    root_scene = new ProgramScene(ptr_layout);

    root_view = new QGraphicsView(root_scene);
    //root_view->resize(this->width(), this->height());
    root_view->show();
    ptr_layout->addWidget(root_view);

    //connects signals
    connect(root_scene,SIGNAL(nodeUpdateRequest()),this,SLOT(nodeUpdateRequestSlot()));

}

void ProgramContainer::nodeUpdateRequestSlot()
{
    qDebug("Print: nodeUpdateRequestSlot(); SLOT WORKED =)");
}
