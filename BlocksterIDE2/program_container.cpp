#include "program_container.h"

#include <QHBoxLayout>

#include "b_program_tree.h"
#include "b_xml.h"

/**
 * @brief Constructs a ProgramContainer
 * @param[in] parent Address of parent widget
 *
 * The constructor takes the parent widget address as a parameter and:
 * - Constructs a ProgramScene.
 * - Constructs a QGraphicsView for that scene.
 * - Adds the view to this and shows it.
 * - Connects slot/signals.
 */

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

/**
 * @brief Constructs a ProgramContainer
 * @param[in] path_to_xml_file The path to a xml file that holds a Blockster program
 *
 * The constructor takes a string with the path to a Blockster program xml
 * file and populates the bster::b_program_tree #program_tree with it.
 *
 * Then it calls #ProgramContainer(QWidget*).
 */

ProgramContainer::ProgramContainer(std::string path_to_xml_file)
{
    this->program_tree = bster::xml_file_to_program_tree(path_to_xml_file);

    ProgramContainer::ProgramContainer(nullptr);
}

void ProgramContainer::nodeUpdateRequestSlot()
{
    qDebug("Print: nodeUpdateRequestSlot(); SLOT WORKED =)");
}
