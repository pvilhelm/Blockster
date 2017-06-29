#include "program_container.h"

#include <QHBoxLayout>

#include "b_program_tree.h"
#include "b_xml.h"

ProgramContainer::ProgramContainer(QWidget *parent) : QWidget(parent)
{

    this->resize(700, 600);
    QHBoxLayout* ptr_layout = new QHBoxLayout(this);

    ProgramScene* root_scene = new ProgramScene(ptr_layout);
    root_view = new QGraphicsView(root_scene);
    root_view->show();
    ptr_layout->addWidget(root_view);

    //connects signals
    connect(root_scene,SIGNAL(nodeUpdateRequest()),this,SLOT(nodeUpdateRequestSlot()));
}

ProgramContainer::ProgramContainer(std::string path_to_xml_file)
{
    this->program_tree = bster::xml_file_to_program_tree(path_to_xml_file);

    ProgramContainer::ProgramContainer(nullptr);
}

void ProgramContainer::nodeUpdateRequestSlot()
{
    qDebug("Print: nodeUpdateRequestSlot(); SLOT WORKED =)");
}
