#include "block.h"
#include "signalline.h"

Block::Block(int w, int h): w(w), h(h)
{
    setFlags(ItemIsSelectable | ItemIsMovable |  ItemSendsGeometryChanges );
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

QVariant Block::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
    if(change == QGraphicsItem::ItemPositionHasChanged){
        QPointF newPos = value.toPointF();
        for( QGraphicsItem* i : this->childItems()){
            Outport* o = dynamic_cast<Outport*>(i);
            if(o){
                SignalLine* sl = o->outSignalLine;
                if(sl){
                    SignalSegment* ss = sl->vec_signalnodes.first();
                    ss->start = o->scenePos()+QPointF(o->x,o->y);
                    ss->update();
                }
                continue;
            }
        }
    }
    return value;
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
