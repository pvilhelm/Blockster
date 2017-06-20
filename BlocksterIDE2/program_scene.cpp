#include "program_scene.h"

ProgramScene::ProgramScene(QObject *parent) : QGraphicsScene(parent)
{
    this->addRect(QRectF(50,50,50,50));
}

void ProgramScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    emit nodeUpdateRequest();
}
