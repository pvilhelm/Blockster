#include "programscene.h"

ProgramScene::ProgramScene(QObject *parent)
    : QGraphicsScene(parent)
{
    this->setSceneRect(QRectF(0,0,500,500));
    this->addItem(test);
    test->addInport(1);
    test->addOutport(1);
    this->update();
}
