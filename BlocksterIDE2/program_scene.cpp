#include "program_scene.h"

#include <memory>
#include <stdexcept>

#include <QMessageBox>

#include "blockster_session.h"
#include "lib_container.h"
#include "lib_scene.h"

#include "b_lib_tree.h"
#include "b_xml.h"
#include "findparent.h"

ProgramScene::ProgramScene(QObject *parent) : QGraphicsScene(parent)
{

}

void ProgramScene::dragMoveEvent(QGraphicsSceneDragDropEvent *event)
{
    if (event->mimeData()->hasFormat("text/plain"))
            event->acceptProposedAction();
}

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
    }
}
