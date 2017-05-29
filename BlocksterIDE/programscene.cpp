#include "programscene.h"
#include "signalsegment.h"
#include "blockstersession.h"

extern BlocksterSession blocksterS;

ProgramScene::ProgramScene(QObject *parent)
    : QGraphicsScene(parent)
{
    this->setSceneRect(QRectF(0,0,500,500));
}

void ProgramScene::dragMoveEvent(QGraphicsSceneDragDropEvent *event)
{
    if (event->mimeData()->hasFormat("text/plain"))
            event->acceptProposedAction();
    //event->setAccepted(true);

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

void ProgramScene::dropEvent(QGraphicsSceneDragDropEvent *event)
{
    const QMimeData* data = event->mimeData();
    Qt::DropAction dropAction = event->dropAction();

    if(dropAction == Qt::MoveAction){
        if(!data->hasText())
            return;
        QString subpath = data->text();
        QString rootpath = blocksterS.config.confParHashtable->value("BLOCKSTER_BLOCKLIBS_PATH");
        QString fullpath = rootpath+"/"+subpath;
        Block* block = new Block(fullpath);
        this->addItem(block);
        block->setPos(event->scenePos());
    }
}
