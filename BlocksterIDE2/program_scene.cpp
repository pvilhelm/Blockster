#include "program_scene.h"

ProgramScene::ProgramScene(QObject *parent) : QGraphicsScene(parent)
{
    this->setSceneRect(QRectF(0,0,700,600));
    //this->program_container = (ProgramContainer*)(parent->parent());
    this->addRect(QRectF(50,50,50,50));
}
