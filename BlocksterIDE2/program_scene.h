#ifndef PROGRAM_SCENE_H
#define PROGRAM_SCENE_H

#include <QGraphicsScene>
#include <QtWidgets>

#include <memory>
#include <vector>
#include <string>

#include "program_container.h"
#include "b_node.h"

class ProgramContainer;

class ProgramScene : public QGraphicsScene
{

    Q_OBJECT

public:
    ProgramScene(QObject *parent);
    ProgramContainer* program_container;
    std::vector<std::shared_ptr<bster::b_node>> v_bnodes; //Vector of bster nodes assosiated with this scene

    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent) override;


signals:
    void nodeUpdateRequest();
    void nodeAddRequest(std::string str_xml);

};


#endif // PROGRAM_SCENE_H
