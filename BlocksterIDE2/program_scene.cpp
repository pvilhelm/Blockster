#include "program_scene.h"

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
        b->node_pos = bster::t_pos({50,50,0});
        BlockItem* bl = new BlockItem(nullptr,b);
        this->addItem(bl);
    }
}

/*void ProgramScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    emit nodeUpdateRequest();
}*/
