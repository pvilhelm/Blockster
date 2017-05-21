#include "programscene.h"
#include "signalsegment.h"

ProgramScene::ProgramScene(QObject *parent)
    : QGraphicsScene(parent)
{
    this->setSceneRect(QRectF(0,0,500,500));
    this->addItem(test);
    test->addInport(1);
    test->addOutport(1);
    this->update();
}

void ProgramScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    QPointF pos = mouseEvent->pos();

    if(this->mode == ProgramScene::InsertLine){
        if(lastSignalSegment == 0){
            return;
        }
        else{
            if(mouseEvent->button() == Qt::RightButton){
                lastSignalSegment->type = SegmentType::END_SEGMENT;
                this->mode = ProgramScene::None;
            }
            else if(mouseEvent->button() == Qt::LeftButton){

            }
        }
    }
    else{
        QGraphicsScene::mousePressEvent(mouseEvent);
    }
}
