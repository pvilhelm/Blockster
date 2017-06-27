#ifndef LIB_SCENE_H
#define LIB_SCENE_H

#include <QObject>
#include <QGraphicsScene>

#include <string>
#include <memory>

#include "b_lib_tree.h"


class LibScene : public QGraphicsScene
{
    Q_OBJECT
public:
    LibScene(std::string lib_path);

    bster::b_lib_tree lib;
    //std::vector<std::shared_ptr<bster::b_node>> v_bnodes;

public slots:
    void reparseLib();

private:
    void clearBlocksFromScene();
    void addBlocksToScene();
    void addBlockToScene(QPointF pos, std::string& xml_str);
};

#endif // LIB_SCENE_H
