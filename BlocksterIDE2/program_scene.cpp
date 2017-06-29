#include "program_scene.h"
#include <memory>
#include "b_xml.h"

ProgramScene::ProgramScene(QObject *parent) : QGraphicsScene(parent)
{
    {
        std::shared_ptr<bster::b_node> b = std::make_shared<bster::b_node>(
                    bster::xml_file_to_node(R"(C:\Repos\Blockster\Blockster\blocks\core\sources\constant\template_node_constant.xml)"));
        BlockItem* bl = new BlockItem(nullptr,b);
        this->addItem(bl);
    }
    {
        std::shared_ptr<bster::b_node> b = std::make_shared<bster::b_node>(
                    bster::xml_file_to_node(R"(C:\Repos\Blockster\Blockster\blocks\core\sources\constant\template_node_constant.xml)"));
        b->node_pos = bster::t_pos({50,50,0,0});
        BlockItem* bl = new BlockItem(nullptr,b);
        this->addItem(bl);

    }
}

void ProgramScene::dragMoveEvent(QGraphicsSceneDragDropEvent *event)
{
    if (event->mimeData()->hasFormat("text/plain"))
            event->acceptProposedAction();

    this->
}

void ProgramScene::dropEvent(QGraphicsSceneDragDropEvent *event)
{
    const QMimeData* data = event->mimeData();
    Qt::DropAction dropAction = event->dropAction();

    if(dropAction == Qt::MoveAction){
        if(!data->hasText())
            return;
        QString subpath = data->text();
        QString rootpath = R"(C:/repos/Blockster/Blockster/blocks)";//blocksterS.config.confParHashtable->value("BLOCKSTER_BLOCKLIBS_PATH");//TODO fix
        QString fullpath = rootpath+"/"+subpath;

        //this->program_container->program_tree. //todo lib tree xml str etc

        std::shared_ptr<bster::b_node> b = std::make_shared<bster::b_node>(
                    bster::xml_file_to_node(fullpath.toStdString()));//TODO use libtree
        QPointF pos = event->scenePos();
        b->node_pos.x = pos.x();
        b->node_pos.y = pos.y();
        b->node_pos.z = 0;
        BlockItem* bl = new BlockItem(nullptr,b);
        this->addItem(bl);
    }
}
