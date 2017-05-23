#include "block.h"
#include "signalline.h"

Block::Block(float w, float h): Block::Block(0,0,w,h)
{
}

Block::Block(float x, float y, float w, float h) : w(w), h(h)
{
    setPos(x,y);
    setFlags(ItemIsSelectable | ItemIsMovable |  ItemSendsGeometryChanges );
    nameTextItem.setTextInteractionFlags(Qt::TextEditable);
    nameTextItem.setParentItem(this);
    this->setName("");
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
                    ss->start = o->scenePos();
                    ss->update();
                }
                continue;
            }
            Inport* in = dynamic_cast<Inport*>(i);
            if(in){
                SignalLine* sl = in->inSignalLine;
                if(sl){
                    SignalSegment* ss = sl->vec_signalnodes.last();
                    ss->end = in->scenePos();
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
    for(float i = 1; i<=n; i++){
        Inport* newInport = new Inport(0,i*h/(n+1));
        newInport->setParentItem(this);
    }
}

void Block::addOutport(int n)
{
    for(float i = 1; i<=n; i++){
        Outport* newOutport = new Outport(w,i*h/(n+1));
        newOutport->setParentItem(this);
    }
}

void Block::setName(QString name)
{
    nameTextItem.setPlainText(name);
    nameTextItem.adjustSize();
    if(nameTextItem.textWidth()<40){
        nameTextItem.setTextWidth(40);
    }
    nameTextItem.setPos(this->w/2-nameTextItem.textWidth()/2,h+10);
}


