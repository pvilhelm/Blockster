#include "programscene.h"
#include "signalsegment.h"
#include <cassert>

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
    QPointF pos = mouseEvent->scenePos();

    QGraphicsItem* i = this->itemAt(pos, QTransform());

    if(!i){//check so no items are at mouse pointer
        //TODO: make so that items that shouldnt disrupt InsertLine are ignored
        //instead of propagated to .
        if(this->mode == ProgramScene::InsertLine){
            if(lastSignalSegment == 0){
                return;
            }
            else{
                if(mouseEvent->button() == Qt::RightButton){
                    lastSignalSegment->type = SegmentType::END_SEGMENT;
                    this->mode = ProgramScene::None;
                    lastSignalSegment = 0;
                }
                else if(mouseEvent->button() == Qt::LeftButton){
                    SignalLine* sl = (SignalLine*)lastSignalSegment->parentItem();
                    if(!sl)
                        throw std::runtime_error ("sl is null");
                    SignalSegment * ss = sl->vec_signalnodes.last();
                    sl->addSegment(ss->end,pos, SegmentType::MIDDLE_SEGMENT);
                    lastSignalSegment = sl->vec_signalnodes.last();
                }
            }
        }
    }
    else{//propagate click to base class
        QGraphicsScene::mousePressEvent(mouseEvent);
    }
}
