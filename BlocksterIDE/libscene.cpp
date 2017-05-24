#include "libscene.h"

#include "block.h"

LibScene::LibScene(QObject *parent) : QGraphicsScene(parent)
{
    this->setSceneRect(QRectF(0,0,500,500));
}

void LibScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (mouseEvent->button() == Qt::LeftButton   &&
        !this->itemAt(mouseEvent->pos(),QTransform())) {

        Block* clickedBlock = dynamic_cast<Block*>(this->itemAt(mouseEvent->pos(),QTransform()));


        QDrag *drag = new QDrag(this);
        QMimeData *mimeData = new QMimeData;

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
    }
}
