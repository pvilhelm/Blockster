#include "programscene.h"
#include "signalsegment.h"
#include "blockstersession.h"

#include <iostream>

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
        addBlock(fullpath,event->scenePos());
    }
}

void ProgramScene::addBlock(QString template_path)
{
    this->addBlock(template_path,QPointF(0,0));
}

void ProgramScene::addBlock(QString template_path, QPointF scene_pos)
{
    Block* block = new Block(template_path);

    if(block->block_id == ""){
        if(this->block_totn_hashtable.contains(block->lib_path)){

            int n = this->block_totn_hashtable.value(block->lib_path);
            block->block_id = block->block_type+QString::fromStdString(std::to_string(n));
            this->block_totn_hashtable.insert(block->lib_path,++n);
        }
        else{

            int n = 0;
            block->block_id = QString::fromStdString(std::to_string(n));
            this->block_totn_hashtable.insert(block->lib_path,++n);

        }
    }

    this->addItem(block);
    block->setPos(scene_pos);

    //TODO remove
    QString tmp = block->getAsXML();
    QTextStream s(&tmp);

    while(!s.atEnd())
        qDebug() << s.readLine();
}
