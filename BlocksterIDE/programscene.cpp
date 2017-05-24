#include "programscene.h"
#include "signalsegment.h"


ProgramScene::ProgramScene(QObject *parent)
    : QGraphicsScene(parent)
{
    this->setSceneRect(QRectF(0,0,500,500));
    Block* test = new Block(100,120);

    this->addItem(test);
    test->addInport(1);
    test->addOutport(1);
    this->update();

    Block* test2 = new Block(200,200,100,100);
    this->addItem(test2);
    test2->addInport(2);
    test2->addOutport(1);
    this->update();

    Block* test3 = new Block(200,300,120,120);
    this->addItem(test3);
    test3->addInport(2);
    test3->addOutport(1);
    this->update();
    test3->setName("test test ttttt");
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
                    QPointF startPoint = ss->scenePos()+ss->end;
                    QPointF posWhole(std::round(pos.x()/10.f)*10.f,std::round(pos.y()/10.f)*10.f);
                    sl->addSegment(startPoint,posWhole, SegmentType::MIDDLE_SEGMENT);
                    lastSignalSegment = sl->vec_signalnodes.last();
                }
            }
        }
        return;
    }

    QGraphicsScene::mousePressEvent(mouseEvent);

}
