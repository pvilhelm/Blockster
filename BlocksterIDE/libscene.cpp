#include "libscene.h"

#include "block.h"

LibScene::LibScene(QObject *parent) : QGraphicsScene(parent)
{
    this->setSceneRect(QRectF(0,0,500,500));
    //this->eventFilter()
}

void LibScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    //Qt::MouseButton a1 = mouseEvent->button();
    //QGraphicsItem* a2 = this->itemAt(mouseEvent->scenePos(),QTransform());

    if (mouseEvent->button() == Qt::LeftButton && this->itemAt(mouseEvent->scenePos(),QTransform())) {

        Block* clickedBlock = dynamic_cast<Block*>(this->itemAt(mouseEvent->scenePos(),QTransform()));
        if(clickedBlock){
            QDrag *drag = new QDrag(this);
            QMimeData *mimeData = new QMimeData;
            mimeData->setText("");
            drag->setMimeData(mimeData);
            drag->exec();
        }



    }
}

void LibScene::addBlock(QString path)
{
    //TODO: Make this accept xml instead for more granity

    if(path==""){
        Block* block = new Block(100,100);
        block->setPos(0,lastYPos);
        lastYPos+=100*1.1;
        this->addItem(block);
        block->setEnabled(false);
    }
}
