#include <tuple>

#include "b_lib_tree.h"
#include "b_node.h"
#include "b_xml.h"

#include "block_item.h"
#include "lib_scene.h"

LibScene::LibScene(std::string lib_path)
{
    this->lib.parseFolderTree(lib_path,true);
    addBlocksToScene();
}

void LibScene::reparseLib()
{
    this->lib.parseFolderTree(lib.root_dir);
    addBlocksToScene();
}

void LibScene::clearBlocksFromScene()
{
    this->clear();
}

void LibScene::addBlocksToScene()
{
    QPointF pos = {0,0};
    clearBlocksFromScene();
    for(auto& pair : lib.map_libpath_to_xml_str){
        std::string& xml_str = pair.second;
        addBlockToScene(pos,xml_str);
        pos+= QPointF(0,60);
    }
}

void LibScene::addBlockToScene(QPointF pos, std::string& xml_str)
{
    std::shared_ptr<bster::b_node> b = std::make_shared<bster::b_node>(
                bster::xml_str_to_node(xml_str));
    b->node_pos.x = pos.x();
    b->node_pos.y = pos.y();
    BlockItem* bl = new BlockItem(nullptr,std::move(b));
    this->addItem(bl);
}
