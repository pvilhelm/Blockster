#include "block.h"

Block::Block(int w, int h): w(w), h(h)
{
    setFlags(ItemIsSelectable | ItemIsMovable);
}

QRectF Block::boundingRect() const
{
    return QRectF(0, 0, w, h);
}

QPainterPath Block::shape() const
{
    QPainterPath path;
    path.addRect(0, 0, w, h);
    return path;
}

void Block::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);
    painter->drawRect(QRectF(0, 0, w, h));

    return;
}

void Block::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mousePressEvent(event);
}

void Block::addInport(int n)
{

    for(int i = 0; i<n; i++){
        Inport* newInport = new Inport(0,h/(n+1));
        newInport->setParentItem(this);
    }
}

void Block::addOutport(int n)
{
    for(int i = 0; i<n; i++){
        Outport* newOutport = new Outport(w,h/(n+1));
        newOutport->setParentItem(this);
    }
}
