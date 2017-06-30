#include "program_scene.h"

#include <memory>
#include <stdexcept>

#include <QMessageBox>

#include "program_container.h"
#include "blockster_session.h"
#include "lib_container.h"
#include "lib_scene.h"

#include "b_lib_tree.h"
#include "b_xml.h"
#include "findparent.h"

/**
 * @brief ProgramScene::ProgramScene default constructor.
 * @param parent Address of parent widget.
 *
 * The constructor takes the parent widget as parameter and thus makes it it's parent.
 */

ProgramScene::ProgramScene(QObject *parent) : QGraphicsScene(parent)
{

}

/**
 * @brief Handles a drag-n-drop enter the scene event.
 * @param event The event handle.
 *
 * Accepts the event if it's a text/plain event and thus makes it possible for
 * the ProgramScene::dropEvent to trigger when the user drops the item.
 */

void ProgramScene::dragMoveEvent(QGraphicsSceneDragDropEvent *event)
{
    if (event->mimeData()->hasFormat("text/plain"))
            event->acceptProposedAction();
}

/**
 * @brief Handles a drop event in the scene.
 * @param event The event handle
 *
 * If the drag-drop-event is a Qt::MoveAction the string that comes with the
 * drop is processed as a path to a xml-file that holds a block. The block is added to the scene.
 */

void ProgramScene::dropEvent(QGraphicsSceneDragDropEvent *event)
{
    const QMimeData* data = event->mimeData();
    Qt::DropAction dropAction = event->dropAction();

    if(dropAction == Qt::MoveAction){
        if(!data->hasText())
            return;
        QString subpath = data->text();

        BlocksterSession* bs = find_parent<BlocksterSession>(this,QString("session"));
        if(!bs)
            throw std::runtime_error("No session");

        LibContainer* lc = bs->findChild<LibContainer*>("lib_browser");
        if(!lc)
            throw std::runtime_error("No library loaded.");


        std::string xml_str;
        try{
            xml_str = lc->root_scene->lib.map_libpath_to_xml_str.at(subpath.toStdString());
        }
        catch(...){
            QMessageBox::critical(nullptr, tr("Error"),tr("The block library is not loaded"),QMessageBox::Abort);
            return;
        }

        std::shared_ptr<bster::b_node> b = std::make_shared<bster::b_node>(
                    bster::xml_str_to_node(xml_str));//TODO use libtree

        QPointF pos = event->scenePos();
        b->node_pos.x = pos.x();
        b->node_pos.y = pos.y();
        b->node_pos.z = 0;
        BlockItem* bl = new BlockItem(nullptr,b);
        this->addItem(bl);
        ProgramContainer* pc = dynamic_cast<ProgramContainer*>(this->parent());
        if(!pc){
            throw std::runtime_error("No program container for this scene");
        }

        //pc->program_tree.addNode(b); TODO: b_program_tree addNode has to accept a shared_ptr to b_node so the same node as the one in the BlockItem is in the tree FIXFIXFIX

    }
}
